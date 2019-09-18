#include "uart.h"
#include <stdarg.h>
#include <stdio.h>

/* Private function prototypes -----------------------------------------------*/

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
  
  
  
/* Private functions ---------------------------------------------------------*/

u16 USART_RX_STA=0;   	
u8 USART_RX_BUF[USART_REC_LEN]; 
 
void USART_Configuration(void)//���ڳ�ʼ������
  {  

        GPIO_InitTypeDef  GPIO_InitStructure;
				NVIC_InitTypeDef   NVIC_InitStructure;
        USART_InitTypeDef USART_InitStructure;
				USART_InitTypeDef USART_InitStructure2;
                
        RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE );
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE );       
        
				GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_7);
        GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_7);

				GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_7);
        GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_7);
				
        /*
        *  USART1_TX -> PA9 , USART1_RX ->        PA10
					 USART2_TX -> PA2 , USART2_RX ->        PA3
						
        */                                
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_2|GPIO_Pin_3;                 
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
        GPIO_Init(GPIOA, &GPIO_InitStructure);        
        
        USART_InitStructure.USART_BaudRate = 115200;//���ô��ڲ�����
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;//��������λ
        USART_InitStructure.USART_StopBits = USART_StopBits_1;//����ֹͣλ
        USART_InitStructure.USART_Parity = USART_Parity_No;//����Ч��λ
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//����������
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//���ù���ģʽ
        USART_Init(USART1, &USART_InitStructure); //������ṹ��
				
				USART_InitStructure2.USART_BaudRate = 115200;//���ô��ڲ�����
        USART_InitStructure2.USART_WordLength = USART_WordLength_8b;//��������λ
        USART_InitStructure2.USART_StopBits = USART_StopBits_1;//����ֹͣλ
        USART_InitStructure2.USART_Parity = USART_Parity_No;//����Ч��λ
        USART_InitStructure2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//����������
        USART_InitStructure2.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//���ù���ģʽ
        USART_Init(USART2, &USART_InitStructure2); //������ṹ��

        USART_Cmd(USART1, ENABLE);//ʹ�ܴ���1
				USART_Cmd(USART2, ENABLE);//ʹ�ܴ���2
				
				USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�
				USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�

				//Usart1 NVIC ����
				NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
				NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
				NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
				NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
				NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
				
				//Usart1 NVIC ����
				NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����1�ж�ͨ��
				NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
				NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
				NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
				NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

		}			

void UART_send_byte(uint8_t byte) //����1�ֽ�����
{
 while(!((USART1->ISR)&(1<<7)));
 USART1->TDR=byte;	
}		

void UART_Send(uint8_t *Buffer, uint32_t Length)
{
	while(Length != 0)
	{
		while(!((USART1->ISR)&(1<<7)));//�ȴ�������
		USART1->TDR= *Buffer;
		Buffer++;
		Length--;
	}
}

void UART2_send_byte(uint8_t byte) //����1�ֽ�����
{
 while(!((USART2->ISR)&(1<<7)));
 USART2->TDR=byte;	
}		

void UART2_Send(uint8_t *Buffer, uint32_t Length)
{
	while(Length != 0)
	{
		while(!((USART2->ISR)&(1<<7)));//�ȴ�������
		USART2->TDR= *Buffer;
		Buffer++;
		Length--;
	}
}

uint8_t UART_Recive(void)
{	
	while(!(USART1->ISR & (1<<5)));//�ȴ����յ�����
	return(USART1->RDR);			 //��������
}

//void USART1_IRQHandler(void)                	//??1??????
//{
//	u8 Res;
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //????(?????????0x0d 0x0a??)
//	{
//		Res =USART_ReceiveData(USART1);//(USART1->DR);	//????????
//		
//		if((USART_RX_STA&0x8000)==0)//?????
//		{
//			if(USART_RX_STA&0x4000)//????0x0d
//			{
//				if(Res!=0x0a)USART_RX_STA=0;//????,????
//				else USART_RX_STA|=0x8000;	//????? 
//			}
//			else //????0X0D
//			{	
//				if(Res==0x0d)USART_RX_STA|=0x4000;
//				else
//				{
//					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res;
//					USART_RX_STA++;
//					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//??????,??????	  
//				}		 
//			}
//		}   		 
//  } 
//} 

void USART2_IRQHandler(void)                	//??1??????
{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //????(?????????0x0d 0x0a??)
	{
		Res =USART_ReceiveData(USART2);//(USART1->DR);	//????????
		
		if((USART_RX_STA&0x8000)==0)//?????
		{
			if(USART_RX_STA&0x4000)//????0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//????,????
				else USART_RX_STA|=0x8000;	//????? 
			}
			else //????0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//??????,??????	  
				}		 
			}
		}   		 
  } 
} 
#if 1
#pragma import(__use_no_semihosting)             
                
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
    
int _sys_exit(int x) 
{ 
	x = x; 
} 

int fputc(int ch, FILE *f)
{      
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
    USART_SendData(USART1,(uint8_t)ch);   
	return ch;
}
#endif 	
