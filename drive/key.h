#ifndef __KEY_H
#define	__KEY_H

#include "stm32f37x.h"

void KEY_Init(void);
uint8_t KEY_Down(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);

#endif /* __KEY_H */

