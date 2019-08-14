//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------

#include "spim.h"
#include <stm32f10x_spi.h>

//==============================================================================
// Процедура инициализации spi (SPI1 или SPI2) в режиме master
//==============================================================================
void spim_init(SPI_TypeDef* SPIx, uint8_t WordLen)
{
  // Стартуем тактирование GPIO и SPI
  if (SPIx == SPI1)
  {
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN | RCC_APB2ENR_IOPAEN;
  }
  else
  {
	 RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
	 RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
  }
  
  // Настраиваем SPI
  SPI_Cmd(SPIx, DISABLE); 
  SPI_InitTypeDef SPI_InitStruct;
  SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;// SPI_Direction_1Line_Tx;
  SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
  SPI_InitStruct.SPI_DataSize = (WordLen == 16) ? SPI_DataSize_16b : SPI_DataSize_8b;
  SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
  SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStruct.SPI_CRCPolynomial = 0x7;
  SPI_Init(SPIx, &SPI_InitStruct);
  SPI_Cmd(SPIx, ENABLE);
    

  // Настраиваем ноги GPIO для сигналов SCK и MOSI
  if (SPIx == SPI1)
  {
    GPIOA->CRL &= ~ (GPIO_CRL_CNF5 | GPIO_CRL_CNF7) ;
    GPIOA->CRL |= GPIO_CRL_MODE5_1 | GPIO_CRL_MODE5_0 | GPIO_CRL_CNF5_1 |
    				GPIO_CRL_MODE7_0 | GPIO_CRL_MODE7_1 | GPIO_CRL_CNF7_1 ; //Output, AF, PP, 50MHz
  }
  else
  {
    GPIOB->CRL &= ~ (GPIO_CRH_CNF13 | GPIO_CRH_CNF15) ;
    GPIOB->CRH |= GPIO_CRH_MODE13_1 | GPIO_CRH_MODE13_0 | GPIO_CRH_CNF13_1 |
    				GPIO_CRH_MODE15_0 | GPIO_CRH_MODE15_1 | GPIO_CRH_CNF15_1 ; //Output, AF, PP, 50MHz

  }

  // Настраиваем ножку GPIO для сигнала MISO
//floating input (reset state)
  if (SPIx == SPI1)
  {
	  //PA6
  }
  else
  {
	  //PB14
  }
}
//==============================================================================


//==============================================================================
// Процедура отправляет массив 16-битных слов
//==============================================================================
void SPI_send16b(SPI_TypeDef* SPIx, uint16_t *pBuff, uint16_t Len)
{
  for (uint16_t i = 0; i < Len; i++)
  {
    SPI_I2S_SendData(SPIx, *(pBuff++));
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET) ;
  }
}
//==============================================================================


//==============================================================================
// Процедура отправляет массив 8-битных слов
//==============================================================================
void SPI_send8b(SPI_TypeDef* SPIx, uint8_t *pBuff, uint16_t Len)
{
  for (uint16_t i = 0; i < Len; i++)
  {
    SPI_I2S_SendData(SPIx, *(pBuff++));
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET) ;
  }
}
//==============================================================================


//==============================================================================
// Процедура отправляет массив 16-битных слов
//==============================================================================
void SPI_recv16b(SPI_TypeDef* SPIx, uint16_t *pBuff, uint16_t Len)
{
  for (uint16_t i = 0; i < Len; i++)
  {
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPIx, 0xFFFF);
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET) ;
    //while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
    *(pBuff++) = SPI_I2S_ReceiveData(SPIx);
  }
}//==============================================================================


//==============================================================================
// Процедура отправляет массив 8-битных слов
//==============================================================================
void SPI_recv8b(SPI_TypeDef* SPIx, uint8_t *pBuff, uint16_t Len)
{
  for (uint16_t i = 0; i < Len; i++)
  {
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPIx, 0xFF);
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET) ;
    //while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
    *(pBuff++) = SPI_I2S_ReceiveData(SPIx);
  }
}
//==============================================================================


//==============================================================================
// Процедура отправляет 1 байт и возвращает принятый байт
//==============================================================================
uint8_t SPI_SendRecvByte(SPI_TypeDef* SPIx, uint8_t TxByte)
{
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
  SPI_I2S_SendData(SPIx, TxByte);
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET) ;
  //while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
  return SPI_I2S_ReceiveData(SPIx);
}
//==============================================================================
