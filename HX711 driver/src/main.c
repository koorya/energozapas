#include <stm32f030x8.h>
#include "./lib/k_rcc/K_RCC.h"

volatile int32_t RECEIVE_DATA = 0;

volatile char str[] = "kirya\n";

typedef enum {
	RECEIVING,
	WAITING
} RECEIVE_STATE_ENUM;


void receiveHX711Value(void){
	static uint8_t bit_numb = 0;
	static uint32_t reveive_data = 0;
	static RECEIVE_STATE_ENUM receive_state = WAITING;
	
	if(receive_state == WAITING){
		if(!(GPIOA->IDR & GPIO_IDR_2)){
			receive_state = RECEIVING;
			TIM6->ARR = 19; //досчитывать до 20
			TIM6->PSC = 15; //делит на 16, получается, что считает с периодом 1мкс
			TIM6->EGR |= TIM_EGR_UG; //на всякий случай
		}
	}else if(receive_state == RECEIVING){
		if(bit_numb < 50){//25
//		if(bit_numb < 52){//26
//		if(bit_numb < 54){//27

			
			if(bit_numb & 1){
				GPIOA->ODR |= GPIO_ODR_6;
			}else{
				GPIOA->ODR &= ~GPIO_ODR_6;
				if(bit_numb<=48){
					reveive_data <<=1;
					if(GPIOA->IDR & GPIO_IDR_2)
						reveive_data++;
				}
			}
			bit_numb++;
		}else{
			receive_state = WAITING;
			bit_numb = 0;
//			RECEIVE_DATA = (8388607 - (reveive_data ^ 0x800000));
			RECEIVE_DATA = (reveive_data ^ 0x800000);

			DMA1_Channel2->CCR &= ~DMA_CCR_EN;
			DMA1_Channel2->CNDTR = 3;
			DMA1_Channel2->CCR |= DMA_CCR_EN;

//			RECEIVE_DATA = 3*(RECEIVE_DATA+12000)/1000;
//			if(RECEIVE_DATA%1000>=500)
//				RECEIVE_DATA += 1000;
//			if(RECEIVE_DATA%1000<=-500)
//				RECEIVE_DATA -= 1000;
//			RECEIVE_DATA /= 1000;
			
			reveive_data = 0;
			GPIOA->ODR &= ~GPIO_ODR_6;
			
			TIM6->ARR = 100; //100
			TIM6->PSC = 999; //16 kHz
			TIM6->EGR |= TIM_EGR_UG; //на всякий случай
		}
	}
	TIM6->CR1 |= TIM_CR1_CEN;
}


void TIM6_IRQHandler(){
	TIM6->SR &= ~TIM_SR_UIF;
	receiveHX711Value();
}


int main (void) {


/*
void K_setClock(K_SOURCE source,
	char presc, //1-16; если mult == 1, то presc=1, иначе но если source == cHSI, то presc=2 (при 8MHz наибольший предделитель 14)
	char kPLL, //1-16 (если mult==1, то умножитель не используется и предделитель не работает)
	char ahbpresc, // 1, 2, 4, 8, 16, (not 32) 64, 128, 256, 512
	char apbpresc, // 1, 2, 4, 8, 16
	char enable_mco) //0 - disable, !0 - enable (sysclock)
*/
	
	K_setClock(cHSE, 1, 2, 1, 1, 1); //16MHz
	
//	K_initMCOoutput(K_HSI);
	
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER6_0; //general purpose output mode (push-pull is default state)
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6; //High speed

//	GPIOA->MODER |= GPIO_MODER_MODER10_1;
//	GPIOA->AFR[0] |= GPIO_AFRH_AFRH1;

	GPIOA->MODER |= GPIO_MODER_MODER9_1; //alternate function mode
//	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9; //high speed
//	GPIOA->AFR[1] |= (0x1 << GPIO_AFRH_AFSEL9_Pos); //USART1_TX alternate function
	GPIOA->AFR[1] |= GPIO_AFRH_AFRH1;//USART1_TX alternate function
	GPIOA->MODER |= GPIO_MODER_MODER12_0;
	GPIOA->ODR |= GPIO_ODR_12;

	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	USART1->CR3 |= USART_CR3_DMAT; //DMA mode is enabled for transmission
	USART1->CR2 |= USART_CR2_STOP_1;
	USART1->BRR = 16000000 / 9600;// ~9603 baud
	USART1->CR1 |= 	USART_CR1_TE		|
									USART_CR1_UE		|
																	0;
	while ((USART1->ISR & USART_ISR_TC) != USART_ISR_TC);
	USART1->ICR |= USART_ICR_TCCF; //clear TC flag
//	USART1->CR1 |= USART_CR1_TCIE; //enable TC interrupt
	
	
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	DMA1_Channel2->CPAR = (uint32_t)(&(USART1->TDR));
	DMA1_Channel2->CMAR = (uint32_t)(&RECEIVE_DATA);
//	DMA1_Channel2->CMAR = (uint32_t)(str);
//	DMA1_Channel2->CNDTR = 3;
	DMA1_Channel2->CCR |= DMA_CCR_MINC | DMA_CCR_DIR;// | DMA_CCR_CIRC;// | DMA_CCR_TCIE;
//	DMA1_Channel2->CCR |= DMA_CCR_EN;
	
//	NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
//	NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0);



	
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
//	RCC->APB1RSTR |= RCC_APB1RSTR_TIM6RST;
	
	
	//TIM6->ARR = 19; //досчитывать до 20
	//TIM6->PSC = 15; //делит на 16, получается, что считает с периодом 1мкс
	TIM6->ARR = 99; //100
	TIM6->PSC = 999; //16 kHz
//	TIM6->CR1 |= TIM_CR1_ARPE; //включить буфер arr, записывать после события

	TIM6->CR1 |= TIM_CR1_OPM; //one puls mode
	TIM6->CR1 |= TIM_CR1_URS; //событие (прерывание) только при обновлении счетчика
	
	TIM6->DIER |= TIM_DIER_UIE; //включаем прерывание

//	TIM6->CR1 |= TIM_CR1_CEN; //запуск счета
		
		

	NVIC_EnableIRQ(TIM6_IRQn); /* (1) */
	NVIC_SetPriority(TIM6_IRQn,0);
	
//	TIM6->EGR |= TIM_EGR_UG; //обновить значения после настройки
	
	
	
	receiveHX711Value();
	
  while (1)  {

  }
	
}

