#ifndef _XPT2046_H_
#define _XPT2046_H_

#include "stm32f37x.h"
#include  "ili9328.h"

#define TOUCH_INT_PIN        GPIO_Pin_8
#define TOUCH_INT_PORT       GPIOE
#define TOUCH_INT_PIN_SCK    RCC_AHBPeriph_GPIOE

#define TOUCH_CS_PIN        GPIO_Pin_4
#define TOUCH_CS_PORT       GPIOA
#define TOUCH_CS_PIN_SCK    RCC_AHBPeriph_GPIOA

#define TOUCH_SCK_PIN        GPIO_Pin_5
#define TOUCH_SCK_PORT       GPIOA
#define TOUCH_SCK_PIN_SCK    RCC_AHBPeriph_GPIOA
#define TOUCH_SCK_SOURCE     GPIO_PinSource5
#define TOUCH_SCK_AF         GPIO_AF_5

#define TOUCH_MISO_PIN       GPIO_Pin_6
#define TOUCH_MISO_PORT      GPIOA
#define TOUCH_MISO_PIN_SCK   RCC_AHBPeriph_GPIOA
#define TOUCH_MISO_SOURCE    GPIO_PinSource6
#define TOUCH_MISO_AF        GPIO_AF_5

#define TOUCH_MOSI_PIN       GPIO_Pin_7
#define TOUCH_MOSI_PORT      GPIOA
#define TOUCH_MOSI_PIN_SCK   RCC_AHBPeriph_GPIOA
#define TOUCH_MOSI_SOURCE    GPIO_PinSource7
#define TOUCH_MOSI_AF        GPIO_AF_5


#define TOUCH_SPI1               RCC_APB2Periph_SPI1
#define SPI_TOUCH_CS_LOW()       GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define SPI_TOUCH_CS_HIGH()      GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define SPI_TOUHC_INT          GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_8)

#define CMD_RDY 0X90  //0B10010000即用差分方式读X坐标
#define CMD_RDX	0XD0  //0B11010000即用差分方式读Y坐标  
#define Pen_Down 1
#define Pen_Up   0 



typedef struct 
{
	uint16_t X_Coord;//LCD坐标
	uint16_t Y_Coord;
	uint16_t X_ADC; //ADC值
	uint16_t Y_ADC;						   	    
	uint8_t  Pen_Sign;//笔的状态			  
}Pen_Holder;
void delay_us(uint32_t cnt);
extern Pen_Holder Pen_Point;
extern void Touch_Init(void);
// extern void PIOINT2_IRQHandler(void);
extern uint16_t ADS_Read_AD(uint8_t CMD);
extern uint8_t Read_ADS(uint16_t*x,uint16_t *y);
extern uint8_t Read_ADS2(uint16_t *x,uint16_t *y);
extern uint8_t Read_Once(void);
extern uint8_t Read_Continue(void);
extern void Change_XY(void);

#endif
