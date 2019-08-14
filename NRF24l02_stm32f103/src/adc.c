#include <stdint.h>
#include <stm32f10x.h>


uint16_t ADC_work(void)
{
	static uint16_t A0_raw = 0;
	static uint8_t state = 0;

	if(state == 0){
		// ����� ���������� �������������� (1 ����� � ���������� ������)
		ADC1->SQR1 = 0;
		ADC1->SQR2 = 0;
		ADC1->SQR3 = 0;

		// �������� ����� ��� ������ � ��� (10,11,12,13) => 1 ����� � ���������� ������ (���������� ����� ������ ��� ��������������)
		ADC1->SQR3 = 1; //1� �����
		ADC1->CR2 |= ADC_CR2_SWSTART;
		state = 1;
	}else{

		if(ADC1->SR & ADC_SR_EOC){
			 A0_raw = ADC1->DR;
			 state = 0;
		}
	}

	return A0_raw;
}


// ������������� ��������� ��� ( �������-��������� ��������������� )
void ADC_Init(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOA->CRL &= ~( GPIO_CRL_MODE0 | GPIO_CRL_CNF0 );

	// ����������� ������� ������������ ������ ���
	RCC->CFGR &= ~RCC_CFGR_ADCPRE;
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV2;

	// �������� ������������ ��������� ���
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	// ����� ���� �������� ���
	ADC1->SR = 0;

	ADC1->CR1 = 0;
	ADC1->CR2 = 0;

	ADC1->SMPR1 = 0;
	ADC1->SMPR2 = 0;

	ADC1->JOFR1 = 0;
	ADC1->JOFR2 = 0;
	ADC1->JOFR3 = 0;
	ADC1->JOFR4 = 0;

	ADC1->HTR = 0;
	ADC1->LTR = 0;

	ADC1->SQR1 = 0;
	ADC1->SQR2 = 0;
	ADC1->SQR3 = 0;

	ADC1->JSQR = 0;

	ADC1->JDR1 = 0;
	ADC1->JDR2 = 0;
	ADC1->JDR3 = 0;
	ADC1->JDR4 = 0;

	ADC1->DR = 0;

	// �������� ������ ��� (��� ���������� �������� ��������������)
	ADC1->CR2 |= ADC_CR2_ADON;

	// ����� �������� �������������� ���
	ADC1->CR2 |= ADC_CR2_RSTCAL;

	// ���� ��������� ������ �������� �������������� ���
	while (ADC1->CR2 & ADC_CR2_RSTCAL) {}  // ���� �� �������� ��� ADC_CR2_RSTCAL

	// ������ �������������� ��� (��������� ������ ��� ��� ������������� ��� �� ���� ������������)
	ADC1->CR2 |= ADC_CR2_CAL;

	// ���� ��������� ���������� ���
	while (ADC1->CR2 & ADC_CR2_CAL) {}  // ���� �� �������� ��� ADC_CR2_CAL

	// ������������� ������������ �������� �� ������� ����, ������ �������������� �������� SWSTART � �������� ADC1->CR2 ADC_CR2_ALIGN |
	ADC1->CR2 |= ( ADC_CR2_EXTSEL_2 | ADC_CR2_EXTSEL_1 | ADC_CR2_EXTSEL_0 | ADC_CR2_EXTTRIG );

	// ������ ���������� ����� ��������� ������ ��� � [ 41.5 + 12.5 = 53 ADC ] ��� 10, 11, 12, 13 �������
	ADC1->SMPR2 |=  ADC_SMPR2_SMP0_2 | ADC_SMPR2_SMP1_2 | ADC_SMPR2_SMP2_2 | ADC_SMPR2_SMP3_2 ;

}
//



