#include "stm32f10x.h"
#include "PWM.h"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// Область для объявления переменных 

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// Инициализация ШИМ
void PWM_Init(void)
{
	// Подключаем перефирийную систему GPIO к шине тактирования
	// Подключаем систему альтернативных функций для GPIO к шине тактирования
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
	
	// Подключаем Таймер-3 к шине тактирования
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	// Таймер 3 формируют ШИМ-сигналы на своих каналах Ch1, Ch2, Ch3, Ch4
	TIM2->CR1 = 0;
	TIM2->CR2 = 0;
	TIM2->SR = 0;
	TIM2->CCER = 0;
	TIM2->BDTR = 0;
	TIM2->EGR = 0;
	TIM2->CCMR1 = 0;
	TIM2->CCMR2 = 0;
	TIM2->DIER = 0;
	
	TIM2->PSC = (SystemCoreClock/1000000L) - 1;
	// Таймер 3 генерирует ШИМ таким образом чтобы при 1000 значение, на аналоговых выходах было напряжение =10В
	TIM2->ARR = 1181;
	
	// CH1 - PWM Mode 1 
	// CH2 - PWM Mode 1 
	// CH3 - PWM Mode 1 
	// CH4 - PWM Mode 1 
	TIM2->CCMR1 |= ( TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 );
	TIM2->CCMR2 |= ( TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 );

//ПОЛЯРНОСТЬ ШИМ НА ВЫХОДЕ ДЛЯ ПЕРВОГО ЧАСТОТНИКА	(когда ставил дополнительные цепи для стабилизации)
	TIM2->CCER |= TIM_CCER_CC1P;

	TIM2->CCER |= ( TIM_CCER_CC4E | TIM_CCER_CC3E | TIM_CCER_CC2E | TIM_CCER_CC1E );
	TIM2->BDTR |= TIM_BDTR_MOE;
	TIM2->DIER |= TIM_DIER_UIE;
	TIM2->EGR |= TIM_EGR_UG;
	
	TIM2->CCR1 = 0;
	TIM2->CCR2 = 0;
	TIM2->CCR3 = 0;
	TIM2->CCR4 = 0;
	
	GPIOA->CRL &= ~ (GPIO_CRL_CNF0 | GPIO_CRL_MODE0);
	GPIOA->CRL |= GPIO_CRL_MODE0 | GPIO_CRL_CNF0; //AF, output PP, 50MHz PA0
	
	// Разрешаем использование прерываний от Таймера-2
//	NVIC_EnableIRQ(TIM2_IRQn);
}
//

// Включение PWM
void PWM_Enable(void)
{
  TIM2->SR &= ~TIM_SR_UIF;
	TIM2->DIER |= TIM_DIER_UIE;
	TIM2->CR1 |= TIM_CR1_CEN;


}
//

// Выключение PWM
void PWM_Disable(void)
{
  TIM2->SR &= ~TIM_SR_UIF;
	TIM2->DIER &= ~TIM_DIER_UIE;
	TIM2->CCR1 = 0;
	TIM2->CCR2 = 0;
	TIM2->CCR3 = 0;
	TIM2->CCR4 = 0;
	TIM2->CR1 &= ~TIM_CR1_CEN;
}
//

// Обновление пар-ов PWM
void PWM_Update(uint8_t Ch, uint16_t CCR)//CCR - %
{
	if(CCR > 100) CCR = 100;
	CCR = ( (TIM2->ARR ) * CCR ) / 100;
	switch(Ch)
  {
	 case 1: { TIM2->CCR1 = CCR; break; }
	 case 2: { TIM2->CCR2 = CCR; break; }
	 case 3: { TIM2->CCR3 = CCR; break; }
	 case 4: { TIM2->CCR4 = CCR; break; }
	 default: break;
  }
//	TIM2->SR &= ~TIM_SR_UIF;
}
//
