#ifndef __LED_H
#define	__LED_H

#include "stm32f37x.h"
#define LED1_PIN      GPIO_Pin_7
#define LED2_PIN      GPIO_Pin_6
#define LED3_PIN      GPIO_Pin_5
#define LED4_PIN      GPIO_Pin_4

#define LED1_PORT       GPIOB
#define LED2_PORT       GPIOB
#define LED3_PORT       GPIOB
#define LED4_PORT       GPIOB

void LED_Init(void);
void LED_Open(void);
void LED_Close(void);
void LED1_Toggle(void);
#endif /* __LED_H */

