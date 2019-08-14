/*
 * uart.h
 *
 *  Created on: 12 апр. 2019 г.
 *      Author: ivanov.k
 */

#ifndef UART_H_
#define UART_H_

void usart2_init(void);
uint8_t sendStrToUart2(char *str, char size);


#endif /* UART_H_ */
