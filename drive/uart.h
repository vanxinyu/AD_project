#ifndef __UART_H
#define	__UART_H

#include "stm32f37x.h"
#include <stdio.h>




void USART_Configuration(void);
int fputc(int ch, FILE *f);
void UART_send_byte(uint8_t byte);
void UART_Send(uint8_t *Buffer, uint32_t Length);
uint8_t UART_Recive(void);

#endif /* __UART_H */
