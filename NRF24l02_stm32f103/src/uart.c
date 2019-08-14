#include <stdint.h>
#include <stm32f10x.h>

void DMA1_Channel7_IRQHandler(void){ //прерывание DMA по событию окончани€ передачи (в канал от usart2)
	if(DMA1->ISR & DMA_ISR_TCIF7){ // A transfer complete (TC) event occurred on channel 7
		DMA1->IFCR |= DMA_IFCR_CTCIF7; //Clears the corresponding TCIF flag in the DMA_ISR register
	}
//	DMA1->IFCR |= 0x0fffffff; //сбрасываем флаги всех событий (на вс€кий случай, хз)
}

/*
volatile char i = 0;
void USART2_IRQHandler(void) //прерыывание по отправке и по приему байта
{
	if (USART2->SR & USART_SR_TC){ //если регистр отправки пустой (transmit complite)
		i++;
		USART2->SR &= ~USART_SR_TC; //сбрасываем флаг пустого регистра отправки
		if(DMA1->ISR & DMA_ISR_TCIF7){ //если закончена передача через DMA,
			DMA1->IFCR |= DMA_IFCR_CTCIF7; //сбрасываем флаг
//			GPIOA->BSRR = GPIO_BSRR_BR1; //включаем на прием микросхему max485
		}
	}
}
*/

void usart2_init(void)
{
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN; // PA2-Tx PA3-Rx (gpioa к шине тактировани€, альтернативные функции к шине тактировани€)
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN; //USART2 - PA2-Tx PA3-Rx , тактирование от APB1 (36MHz дл€ этого проекта)

//Tx PA2 (no remap, usart2)
		GPIOA->CRL	&= ~GPIO_CRL_CNF2 & ~GPIO_CRL_MODE2;
		GPIOA->CRL |= GPIO_CRL_CNF2_1 | GPIO_CRL_MODE2; //CNF2 0b10 PP alt, MODE2 0b11 Output mode max speed 50 MHz
/**
//Rx PA3
		GPIOA->CRL &= ~GPIO_CRL_CNF3 & ~GPIO_CRL_MODE3;
		GPIOA->CRL |= GPIO_CRL_CNF3_0 ; //input
**/
//		USART2->BRR =  0x753; //(36 000 000/19200)/16 = 117.18 ; //тут надо пересвчитать по формуле, целую и дробную часть (на самом деле, мантиссу и сдвиг)
		USART2->BRR =  0x138; //(36 000 000/115200)/16 = 19.53 ; //тут надо пересвчитать по формуле, целую и дробную часть (на самом деле, мантиссу и сдвиг)

		USART2->CR1 = 0;
		USART2->CR1 |= 	//USART_CR1_TCIE	| //tr_compl interr
//								USART_CR1_M 			| // 9 бит
//								USART_CR1_PCE 		| // parity
								//USART_CR1_PS			| // odd parity нечетный
								USART_CR1_TE 			|	//Transmitter enable
								//USART_CR1_RE 			; //Receiver enable
									0;

		USART2->CR2 = 0;

		USART2->CR3 |= USART_CR3_DMAT; //подключаем DMA

		USART2->CR1 |= USART_CR1_UE; //включаем UART2
		USART2->SR &= ~USART_SR_TC; //сбрасываем флаг конца передачи, дл€ того, чтобы сформировать запрос к DMA

		RCC->AHBENR |= RCC_AHBENR_DMA1EN; //включаем тактирование DMA
		DMA1_Channel7->CCR |= DMA_CCR1_MINC | //memory increment
								DMA_CCR1_DIR | //направление ѕјћя“№->ѕерефери€
								DMA_CCR1_TCIE;//| //прерывание по окончании отправки
								//| DMA_CCR1_PL_1;// приоретет канала
		DMA1_Channel7->CPAR = (uint32_t)(&(USART2->DR));//адрес регистра дл€ пересылки данных в переферии
		DMA1_Channel7->CNDTR = 0;
//		DMA1_Channel7->CMAR = (uint32_t)(arr_to_transfer); //адрес пам€ти
//		DMA1_Channel7->CNDTR = 4*4 ; // 4 times by 4 byte
//		DMA1_Channel7->CCR |= DMA_CCR1_EN ;

//		NVIC_EnableIRQ (DMA1_Channel7_IRQn);

//		NVIC_EnableIRQ (USART2_IRQn);
}

uint8_t sendStrToUart2(char *str, char size){
	if(DMA1_Channel7->CNDTR) // ожидаем, пока закончитс€ предыдуща€ передача данных
		return 1; //busy
	DMA1_Channel7->CCR &= ~DMA_CCR1_EN ; //выключаем 7 канал DMA дл€ того, чтобы можно было измен€ть его настройки
	DMA1_Channel7->CMAR = (uint32_t)(str); //memory address
	DMA1_Channel7->CNDTR = size ; // memory size (bytes)
	DMA1_Channel7->CCR |= DMA_CCR1_EN ; //enable Channel7 in DMA for transmit
	return 0; //transfer begin
}


