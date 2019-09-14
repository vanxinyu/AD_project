#include "key.h"


void KEY_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6 ;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	GPIO_SetBits(GPIOF, GPIO_Pin_7| GPIO_Pin_6 );
	
	}
	
 void Delay(uint32_t temp)
{
  for(; temp!= 0; temp--);
} 


uint8_t KEY_Down(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
		  /*检测是否有按键按下 */
   	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0 ) 
	  {	   
	 	   /*延时消抖*/
	  	 Delay(10000);		
	   		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0 )  
					{	 
						/*等待按键释放 */
						while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0);   
			 			return 	0	; 
					}
			else
					return 1;
		}
	else
		return 1;
}

