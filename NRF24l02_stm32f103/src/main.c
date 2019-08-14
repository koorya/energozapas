//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// ��� ����������� ����������� ���������������� ��������. �� ������ ���������
// ��� �� ����� �����, �� �� �������� ������� ������ �� ��� YouTube-����� 
// "����������� � ���������" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// �����: �������� ������ / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#include "stm32f10x.h"
#include <stdio.h>
#include "delay/delay.h"
#include "nrf24/nrf24.h"
#include "uart.h"

#include "PWM.h"


#define LED_ON        GPIOC->BSRR = GPIO_BSRR_BR13
#define LED_OFF       GPIOC->BSRR = GPIO_BSRR_BS13


#define nRF_CHANNEL     70
#define nRF_isTX_DEMO   1       // ���� ������ ����-������� � �������� �����������
#define nRF_PaketLen    32      // ���� ������ ������ = 0, �� ������������ ���������� ����� ������


// ����� ���������� � ������ �����������������
uint8_t Addr[] = {0x01, 0x02, 0x03, 0x04, 0x05};
// ����� ������
uint8_t Buff[] = {
  0x02, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
  0x11, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
  0x21, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
  0x31, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
};

uint8_t Pipe = 0;       // ����� ����������
uint8_t Len = 0;        // ����� ������
uint32_t Counter = 0;   // ������� �������
uint32_t TryCounter = 0;// ������� ��������� ������� �������� ������
uint32_t Err = 0;       // ������� ������ ��� ��������
  
uint16_t ADC_work(void);
void ADC_Init(void);

int main(void)
{


//  gpio_PortClockStart(LED_Port);
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
  GPIOC->CRH |= GPIO_CRH_MODE13; //output 50MHz openDrain
//  gpio_SetGPIOmode_Out(LED_Port, LED_Pin);


  
  // ������������� nRF24
  nrf24_init(SPI1, nRF_CHANNEL);
  // ��������� ����������, ��������� ������
  nrf24_RxPipe_Setup(0, Addr, nRF_PaketLen);      // ���� ������ ������ = 0, �� ������������ ���������� ����� ������

  



#if (nRF_isTX_DEMO)
  char statistic_str[] = "\033\143Packages 1000000000\r\nRetries  1000000000\r\nErrors   1000000000\r\n---\r\n";
  Len = nRF_PaketLen;
  ADC_Init();
  usart2_init();
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
  GPIOB->CRH &= ~(GPIO_CRH_CNF11 | GPIO_CRH_MODE11);
  GPIOB->CRH |= GPIO_CRH_CNF11_1; //input pull-down
  GPIOB->BRR |= GPIO_BRR_BR11;
  uint8_t i = 0;
  while (1)
  {
	if(i++ % 2)
		LED_ON;
	else
		LED_OFF;
    int8_t TryNum = nrf24_Send(Addr, Buff, Len);        // �������� ������

    if (TryNum >= 0)            // ����� ������� ��� ���������
    {
//    	LED_OFF;
    	Counter++;                // ������� ���-�� ������������ �������
    	TryCounter += TryNum;     // ������� ����� ���-�� ������� ��������
    }    
    else{
//    	LED_ON;
    	Err++;                    // ������� ���-�� ������ ��� ��������
    	int16_t str_len = sprintf(statistic_str, "\033\143Packages %10d\r\nRetries  %10d\r\nErrors   %10d\r\n---\r\n", Counter, TryCounter, Err);
    	sendStrToUart2(statistic_str, str_len);
    }
    // ������� ���������� �� �������
/*    disp1color_printf(0, 0, FONTID_10X16F, "250 kbps");
    disp1color_printf(0, 16, FONTID_10X16F, "������� %d", Counter);
    disp1color_printf(0, 32, FONTID_10X16F, "�������� %d", TryCounter);
    disp1color_printf(0, 48, FONTID_10X16F, "������ %d", Err);
*/
    Buff[1] = (((uint32_t)ADC_work())*100)/4095; //in %
    if(GPIOB->IDR & GPIO_IDR_IDR11)
    	Buff[0] = 0x01;
    else
    	Buff[0] = 0x00;

//    delay_ms(50);
  }
#else


	PWM_Init();
	PWM_Update(1, 70);
	PWM_Enable();

	LED_ON;
	PWM_Update(1, Buff[1]);
	uint32_t wd = 0;
	while (1)
	{
		wd++;
		if(wd >= 72000000/1000){
			LED_OFF;
			PWM_Update(1, 0);
			wd = 72000000/10;
		}
		Len = nrf24_Recv(&Pipe, Buff);      // ���������, ���� �� �������� ������
		if (Len)    // ������ ����� �� ���������� � Pipe ������ Len
		{
		  // ��� ���� ��������� ������
			wd = 0;
			Counter++;// ������� ���-�� �������� �������
			if(Buff[0] == 0x01)
			  LED_ON;
			else
			  LED_OFF;

			PWM_Update(1, Buff[1]);

		}
	}
#endif
}
