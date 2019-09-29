#include "led.h"

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_InitStruct.GPIO_Pin = LED1_PIN |LED2_PIN |LED3_PIN  |LED4_PIN   ;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
// 	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed =GPIO_Speed_50MHz;
  GPIO_Init(LED1_PORT, &GPIO_InitStruct);
	GPIO_SetBits(LED1_PORT, LED1_PIN  | LED2_PIN | LED3_PIN| LED4_PIN);
}

void LED_Open(void)
{
	 GPIO_ResetBits(LED1_PORT, LED1_PIN );
}

void LED_Close(void)
{
		GPIO_SetBits(LED1_PORT, LED1_PIN );
}

void LED1_Toggle(void)
{
GPIO_WriteBit(GPIOB, GPIO_Pin_7, 
		               (BitAction)((1-GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_7))));
}
