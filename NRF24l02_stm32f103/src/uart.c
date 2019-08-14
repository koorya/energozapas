#include <stdint.h>
#include <stm32f10x.h>

void DMA1_Channel7_IRQHandler(void){ //���������� DMA �� ������� ��������� �������� (� ����� �� usart2)
	if(DMA1->ISR & DMA_ISR_TCIF7){ // A transfer complete (TC) event occurred on channel 7
		DMA1->IFCR |= DMA_IFCR_CTCIF7; //Clears the corresponding TCIF flag in the DMA_ISR register
	}
//	DMA1->IFCR |= 0x0fffffff; //���������� ����� ���� ������� (�� ������ ������, ��)
}

/*
volatile char i = 0;
void USART2_IRQHandler(void) //����������� �� �������� � �� ������ �����
{
	if (USART2->SR & USART_SR_TC){ //���� ������� �������� ������ (transmit complite)
		i++;
		USART2->SR &= ~USART_SR_TC; //���������� ���� ������� �������� ��������
		if(DMA1->ISR & DMA_ISR_TCIF7){ //���� ��������� �������� ����� DMA,
			DMA1->IFCR |= DMA_IFCR_CTCIF7; //���������� ����
//			GPIOA->BSRR = GPIO_BSRR_BR1; //�������� �� ����� ���������� max485
		}
	}
}
*/

void usart2_init(void)
{
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN; // PA2-Tx PA3-Rx (gpioa � ���� ������������, �������������� ������� � ���� ������������)
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN; //USART2 - PA2-Tx PA3-Rx , ������������ �� APB1 (36MHz ��� ����� �������)

//Tx PA2 (no remap, usart2)
		GPIOA->CRL	&= ~GPIO_CRL_CNF2 & ~GPIO_CRL_MODE2;
		GPIOA->CRL |= GPIO_CRL_CNF2_1 | GPIO_CRL_MODE2; //CNF2 0b10 PP alt, MODE2 0b11 Output mode max speed 50 MHz
/**
//Rx PA3
		GPIOA->CRL &= ~GPIO_CRL_CNF3 & ~GPIO_CRL_MODE3;
		GPIOA->CRL |= GPIO_CRL_CNF3_0 ; //input
**/
//		USART2->BRR =  0x753; //(36 000 000/19200)/16 = 117.18 ; //��� ���� ������������ �� �������, ����� � ������� ����� (�� ����� ����, �������� � �����)
		USART2->BRR =  0x138; //(36 000 000/115200)/16 = 19.53 ; //��� ���� ������������ �� �������, ����� � ������� ����� (�� ����� ����, �������� � �����)

		USART2->CR1 = 0;
		USART2->CR1 |= 	//USART_CR1_TCIE	| //tr_compl interr
//								USART_CR1_M 			| // 9 ���
//								USART_CR1_PCE 		| // parity
								//USART_CR1_PS			| // odd parity ��������
								USART_CR1_TE 			|	//Transmitter enable
								//USART_CR1_RE 			; //Receiver enable
									0;

		USART2->CR2 = 0;

		USART2->CR3 |= USART_CR3_DMAT; //���������� DMA

		USART2->CR1 |= USART_CR1_UE; //�������� UART2
		USART2->SR &= ~USART_SR_TC; //���������� ���� ����� ��������, ��� ����, ����� ������������ ������ � DMA

		RCC->AHBENR |= RCC_AHBENR_DMA1EN; //�������� ������������ DMA
		DMA1_Channel7->CCR |= DMA_CCR1_MINC | //memory increment
								DMA_CCR1_DIR | //����������� ������->���������
								DMA_CCR1_TCIE;//| //���������� �� ��������� ��������
								//| DMA_CCR1_PL_1;// ��������� ������
		DMA1_Channel7->CPAR = (uint32_t)(&(USART2->DR));//����� �������� ��� ��������� ������ � ���������
		DMA1_Channel7->CNDTR = 0;
//		DMA1_Channel7->CMAR = (uint32_t)(arr_to_transfer); //����� ������
//		DMA1_Channel7->CNDTR = 4*4 ; // 4 times by 4 byte
//		DMA1_Channel7->CCR |= DMA_CCR1_EN ;

//		NVIC_EnableIRQ (DMA1_Channel7_IRQn);

//		NVIC_EnableIRQ (USART2_IRQn);
}

uint8_t sendStrToUart2(char *str, char size){
	if(DMA1_Channel7->CNDTR) // �������, ���� ���������� ���������� �������� ������
		return 1; //busy
	DMA1_Channel7->CCR &= ~DMA_CCR1_EN ; //��������� 7 ����� DMA ��� ����, ����� ����� ���� �������� ��� ���������
	DMA1_Channel7->CMAR = (uint32_t)(str); //memory address
	DMA1_Channel7->CNDTR = size ; // memory size (bytes)
	DMA1_Channel7->CCR |= DMA_CCR1_EN ; //enable Channel7 in DMA for transmit
	return 0; //transfer begin
}


