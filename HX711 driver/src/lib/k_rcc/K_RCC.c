#include <stm32f030x8.h>
#include "K_RCC.h"

uint32_t volatile K_EXT_OSC_FREQ = K_STURTUP_EXT_OSC_FREQ; //10MHz
uint32_t volatile K_INT_OSC_FREQ = K_STURTUP_INT_OSC_FREQ; //8MHz
K_CLOC_CONTROL volatile K_CLOCK_CONTROL_STRUCT;

void K_updatePeriphFreq(void){

	K_CLOCK_CONTROL_STRUCT.K_AHB_FREQ = K_CLOCK_CONTROL_STRUCT.K_SYSCLK_FREQ;

	if(!(RCC->CFGR & RCC_CFGR_HPRE_3)){
		K_CLOCK_CONTROL_STRUCT.K_AHB_FREQ /= 1;
	}else if(!(RCC->CFGR & RCC_CFGR_HPRE_DIV512)){
		K_CLOCK_CONTROL_STRUCT.K_AHB_FREQ /= 512;
	}else if(!(RCC->CFGR & RCC_CFGR_HPRE_DIV256)){
		K_CLOCK_CONTROL_STRUCT.K_AHB_FREQ /= 256;
	}else if(!(RCC->CFGR & RCC_CFGR_HPRE_DIV128)){
		K_CLOCK_CONTROL_STRUCT.K_AHB_FREQ /= 128;
	}else if(!(RCC->CFGR & RCC_CFGR_HPRE_DIV64)){
		K_CLOCK_CONTROL_STRUCT.K_AHB_FREQ /= 64;
	}else if(!(RCC->CFGR & RCC_CFGR_HPRE_DIV16)){
		K_CLOCK_CONTROL_STRUCT.K_AHB_FREQ /= 16;
	}else if(!(RCC->CFGR & RCC_CFGR_HPRE_DIV8)){
		K_CLOCK_CONTROL_STRUCT.K_AHB_FREQ /= 8;
	}else if(!(RCC->CFGR & RCC_CFGR_HPRE_DIV4)){
		K_CLOCK_CONTROL_STRUCT.K_AHB_FREQ /= 4;
	}else if(!(RCC->CFGR & RCC_CFGR_HPRE_DIV2)){
		K_CLOCK_CONTROL_STRUCT.K_AHB_FREQ /= 2;
	}
	
	K_CLOCK_CONTROL_STRUCT.K_APB_FREQ = K_CLOCK_CONTROL_STRUCT.K_AHB_FREQ;
	
	if(!(RCC->CFGR & RCC_CFGR_PPRE_2)){
		K_CLOCK_CONTROL_STRUCT.K_APB_FREQ /= 1;
	}else if(!(RCC->CFGR & RCC_CFGR_PPRE_DIV16)){
		K_CLOCK_CONTROL_STRUCT.K_APB_FREQ /= 16;
	}else if(!(RCC->CFGR & RCC_CFGR_PPRE_DIV8)){
		K_CLOCK_CONTROL_STRUCT.K_APB_FREQ /= 8;
	}else if(!(RCC->CFGR & RCC_CFGR_PPRE_DIV4)){
		K_CLOCK_CONTROL_STRUCT.K_APB_FREQ /= 4;
	}else if(!(RCC->CFGR & RCC_CFGR_PPRE_DIV2)){
		K_CLOCK_CONTROL_STRUCT.K_APB_FREQ /= 2;
	}

}



void K_SetExtClockPLL(char mult,	//mult - 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16. 
	char div)										//div 1-16 [rcc_cfgr2_prediv]
{ 
	RCC->CR |= RCC_CR_HSEON; //включаем внешний генератор
	while (!(RCC->CR & RCC_CR_HSERDY));// ждем, пока внешний генератор запустится и будет готов
	
	RCC->CR |= RCC_CR_HSION; //включаем внутренний генератор
	while (!(RCC->CR & RCC_CR_HSIRDY));// ждем, пока внутренний генератор запустится и будет готов

	RCC->CFGR &= ~RCC_CFGR_SW;//обнуляем регистор выбора источника тактирования (выставляется внутренний напрямую), чтобы можно было конфигурировать внешний и умножитель
	
	RCC->CR &= ~RCC_CR_PLLON;//выключаем умножитель для того, чтобы его можно было сконфигурировать
	
	RCC->CFGR2 &= ~RCC_CFGR2_PREDIV;//выключаем предделитель HSE
	if (div == 1){
		RCC->CFGR2 |= RCC_CFGR2_PREDIV_DIV1;//включаем предделитель HSE
	}else if (div == 2){
		RCC->CFGR2 |= RCC_CFGR2_PREDIV_DIV2;
	}else if (div == 3){
		RCC->CFGR2 |= RCC_CFGR2_PREDIV_DIV3;
	}else if (div == 4){
		RCC->CFGR2 |= RCC_CFGR2_PREDIV_DIV4;
	}else if (div == 5){
		RCC->CFGR2 |= RCC_CFGR2_PREDIV_DIV5;
	}else if (div == 6){
		RCC->CFGR2 |= RCC_CFGR2_PREDIV_DIV6;
	}else if (div == 7){
		RCC->CFGR2 |= RCC_CFGR2_PREDIV_DIV7;
	}else if (div == 8){
		RCC->CFGR2 |= RCC_CFGR2_PREDIV_DIV8;
	}else if (div == 9){
		RCC->CFGR2 |= RCC_CFGR2_PREDIV_DIV9;
	}else if (div == 10){
		RCC->CFGR2 |= RCC_CFGR2_PREDIV_DIV10;
	}else if (div == 11){
		RCC->CFGR2 |= RCC_CFGR2_PREDIV_DIV11;
	}else if (div == 12){
		RCC->CFGR2 |= RCC_CFGR2_PREDIV_DIV12;
	}else if (div == 13){
		RCC->CFGR2 |= RCC_CFGR2_PREDIV_DIV13;
	}else if (div == 14){
		RCC->CFGR2 |= RCC_CFGR2_PREDIV_DIV14;
	}else if (div == 15){
		RCC->CFGR2 |= RCC_CFGR2_PREDIV_DIV15;
	}else if (div == 16){
		RCC->CFGR2 |= RCC_CFGR2_PREDIV_DIV16;
	}
	
	RCC->CFGR &= ~RCC_CFGR_PLLSRC;
	RCC->CFGR |= RCC_CFGR_PLLSRC_HSE_PREDIV; //переключаем вход умножителя на предделенный внешний


	RCC->CFGR &= ~RCC_CFGR_PLLMUL;//обнуляем множитель
	if(mult == 2){
		RCC->CFGR |= RCC_CFGR_PLLMUL2;
	}else if(mult == 3){
		RCC->CFGR |= RCC_CFGR_PLLMUL3;
	}else if(mult == 4){
		RCC->CFGR |= RCC_CFGR_PLLMUL4;
	}else if(mult == 5){
		RCC->CFGR |= RCC_CFGR_PLLMUL5;
	}else if(mult == 6){
		RCC->CFGR |= RCC_CFGR_PLLMUL6;
	}else if(mult == 7){
		RCC->CFGR |= RCC_CFGR_PLLMUL7;
	}else if(mult == 8){
		RCC->CFGR |= RCC_CFGR_PLLMUL8;
	}else if(mult == 9){
		RCC->CFGR |= RCC_CFGR_PLLMUL9;
	}else if(mult == 10){
		RCC->CFGR |= RCC_CFGR_PLLMUL10;
	}else if(mult == 11){
		RCC->CFGR |= RCC_CFGR_PLLMUL11;
	}else if(mult == 12){
		RCC->CFGR |= RCC_CFGR_PLLMUL12;
	}else if(mult == 13){
		RCC->CFGR |= RCC_CFGR_PLLMUL13;
	}else if(mult == 14){
		RCC->CFGR |= RCC_CFGR_PLLMUL14;
	}else if(mult == 15){
		RCC->CFGR |= RCC_CFGR_PLLMUL15;
	}else if(mult == 16){
		RCC->CFGR |= RCC_CFGR_PLLMUL16;
	}
	
	RCC->CR |= RCC_CR_PLLON;//включаем умнжитель
	while(!(RCC->CR & RCC_CR_PLLRDY));//ждем пока он будет готов
	RCC->CFGR |= RCC_CFGR_SW_PLL;// переключаем вход источника тактирования на умножитель

	K_CLOCK_CONTROL_STRUCT.K_SYSCLK_FREQ = K_EXT_OSC_FREQ * mult / div;

}


void K_SetExtClockDirect(void)
{ 
	RCC->CR |= RCC_CR_HSEON; //включаем внешний генератор
	while (!(RCC->CR & RCC_CR_HSERDY));// ждем, пока внешний генератор запустится и будет готов
	
	RCC->CFGR &= ~RCC_CFGR_SW;//обнуляем регистор выбора источника тактирования (выставляется внутренний напрямую)
	RCC->CFGR |= RCC_CFGR_SW_HSE;//выставляем внешний напрямую

	K_CLOCK_CONTROL_STRUCT.K_SYSCLK_FREQ = K_EXT_OSC_FREQ;

}


void K_SetIntClockDirect(void){
	RCC->CR |= RCC_CR_HSION; //включаем внутренний генератор
	while (!(RCC->CR & RCC_CR_HSIRDY));// ждем, пока внутренний генератор запустится и будет готов
	
	RCC->CFGR &= ~RCC_CFGR_SW;//обнуляем регистор выбора источника тактирования (выставляется внутренний напрямую)
	RCC->CFGR |= RCC_CFGR_SWS_HSI;//выставляем внутренний напрямую

	K_CLOCK_CONTROL_STRUCT.K_SYSCLK_FREQ = K_INT_OSC_FREQ;

}


void K_SetIntClockPLL(char freq)	//freq - 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64 MHz
{ 
	RCC->CR |= RCC_CR_HSION; //включаем внутренний генератор
	while (!(RCC->CR & RCC_CR_HSIRDY));// ждем, пока внутренний генератор запустится и будет готов
	
	RCC->CFGR &= ~RCC_CFGR_SW;//обнуляем регистор выбора источника тактирования (выставляется внутренний напрямую), чтобы можно было конфигурировать внешний и умножитель
	
	RCC->CR &= ~RCC_CR_PLLON;//выключаем умножитель для того, чтобы его можно было сконфигурировать
	
	RCC->CFGR &= ~RCC_CFGR_PLLSRC;
	RCC->CFGR |= RCC_CFGR_PLLSRC_HSI_DIV2; //переключаем вход умножителя предделенный внутренний


	RCC->CFGR &= ~RCC_CFGR_PLLMUL;//обнуляем множитель
	if(freq == 8){
		RCC->CFGR |= RCC_CFGR_PLLMUL2;
	}else if(freq == 12){
		RCC->CFGR |= RCC_CFGR_PLLMUL3;
	}else if(freq == 16){
		RCC->CFGR |= RCC_CFGR_PLLMUL4;
	}else if(freq == 20){
		RCC->CFGR |= RCC_CFGR_PLLMUL5;
	}else if(freq == 24){
		RCC->CFGR |= RCC_CFGR_PLLMUL6;
	}else if(freq == 28){
		RCC->CFGR |= RCC_CFGR_PLLMUL7;
	}else if(freq == 32){
		RCC->CFGR |= RCC_CFGR_PLLMUL8;
	}else if(freq == 36){
		RCC->CFGR |= RCC_CFGR_PLLMUL9;
	}else if(freq == 40){
		RCC->CFGR |= RCC_CFGR_PLLMUL10;
	}else if(freq == 44){
		RCC->CFGR |= RCC_CFGR_PLLMUL11;
	}else if(freq == 48){
		RCC->CFGR |= RCC_CFGR_PLLMUL12;
	}else if(freq == 52){
		RCC->CFGR |= RCC_CFGR_PLLMUL13;
	}else if(freq == 56){
		RCC->CFGR |= RCC_CFGR_PLLMUL14;
	}else if(freq == 60){
		RCC->CFGR |= RCC_CFGR_PLLMUL15;
	}else if(freq == 64){
		RCC->CFGR |= RCC_CFGR_PLLMUL16;
	}
	
	RCC->CR |= RCC_CR_PLLON;//включаем умнжитель
	while(!(RCC->CR & RCC_CR_PLLRDY));//ждем пока он будет готов
	RCC->CFGR |= RCC_CFGR_SW_PLL;// переключаем вход источника тактирования на умножитель
	
	K_CLOCK_CONTROL_STRUCT.K_SYSCLK_FREQ = freq * 1000000;

}




void K_initMCOoutput(K_MCOSource source){ //MCOPRE not available on STM32F030x8 devices

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; //включаем тактирование для gpioa, где находится наш MCO (PA8)

	GPIOA->MODER &= ~GPIO_MODER_MODER8; //Обнуляем регистры конфигурации вывода PA8
	GPIOA->MODER |= GPIO_MODER_MODER8_1; //alternate function

	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_8; //Output push-pull (reset state)
	
	GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR8;
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR8; //High speed
	
	RCC->CFGR &= ~RCC_CFGR_MCO;
	if(source == mcoPLLCLK_DIV2){
		RCC->CFGR |= RCC_CFGR_MCO_PLL;// divided by 2
	}else if(source == mcoHSI){
		RCC->CFGR |= RCC_CFGR_MCO_HSI;//0b100
	}else if(source == mcoHSE){
		RCC->CFGR |= RCC_CFGR_MCO_HSE;//0b100
	}else if(source == mcoSYS){
		RCC->CFGR |= RCC_CFGR_MCO_SYSCLK;//0b100
	}

}	



void K_setClock(K_SOURCE source,
	char presc, //1-16; если mult == 1, то presc=1, иначе но если source == cHSI, то presc=2
	char kPLL, //1-16 (если mult==1, то умножитель не используется)
	char ahbpresc, // 1, 2, 4, 8, 16, (not 32) 64, 128, 256, 512
	char apbpresc, // 1, 2, 4, 8, 16
	char enable_mco) //0 - disable, !0 - enable (sysclock)
{
	if(kPLL == 1){
		if(source == cHSI)
			K_SetIntClockDirect();
		else
			K_SetExtClockDirect();
	}else	if(source == cHSI){
		K_SetIntClockPLL(4*kPLL);
	}else if(source == cHSE){
		K_SetExtClockPLL(kPLL, presc);
	}
	
	RCC->CFGR &= ~RCC_CFGR_HPRE;
	if(ahbpresc == 1){
		RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
	}else if(ahbpresc == 2){
		RCC->CFGR |= RCC_CFGR_HPRE_DIV2;
	}else if(ahbpresc == 4){
		RCC->CFGR |= RCC_CFGR_HPRE_DIV4;
	}else if(ahbpresc == 8){
		RCC->CFGR |= RCC_CFGR_HPRE_DIV8;
	}else if(ahbpresc == 16){
		RCC->CFGR |= RCC_CFGR_HPRE_DIV16;
	}else if(ahbpresc == 64){
		RCC->CFGR |= RCC_CFGR_HPRE_DIV64;
	}else if(ahbpresc == 128){
		RCC->CFGR |= RCC_CFGR_HPRE_DIV128;
	}else if(ahbpresc == 256){
		RCC->CFGR |= RCC_CFGR_HPRE_DIV256;
	}else if(ahbpresc == 512){
		RCC->CFGR |= RCC_CFGR_HPRE_DIV512;
	}	

	RCC->CFGR &= ~RCC_CFGR_PPRE;
	if(apbpresc == 1){
		RCC->CFGR |= RCC_CFGR_PPRE_DIV1;
	}else if(apbpresc == 2){
		RCC->CFGR |= RCC_CFGR_PPRE_DIV2;
	}else if(apbpresc == 4){
		RCC->CFGR |= RCC_CFGR_PPRE_DIV4;
	}else if(apbpresc == 8){
		RCC->CFGR |= RCC_CFGR_PPRE_DIV8;
	}else if(apbpresc == 16){
		RCC->CFGR |= RCC_CFGR_PPRE_DIV16;
	}
	
	
	K_updatePeriphFreq();
	
	if(enable_mco){
		K_initMCOoutput(mcoSYS);
	}
	
}


