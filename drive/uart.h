#ifndef __UART_H
#define	__UART_H

#include "stm32f37x.h"
#include <stdio.h>

#define USART_REC_LEN  			200  	//�����������ֽ��� 200

extern u8  COMMAND_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	

void USART_Configuration(void);
int fputc(int ch, FILE *f);
void UART_send_byte(uint8_t byte);
void UART_Send(uint8_t *Buffer, uint32_t Length);
void UART2_Send(uint8_t *Buffer, uint32_t Length);
uint8_t UART_Recive(void);

#endif /* __UART_H */
