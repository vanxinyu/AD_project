#ifndef __ZIGBEE_H
#define	__ZIGBEE_H

#include "stm32f37x.h"
#include "systick.h"
#include "uart.h"

#define zigbee_ready  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)
#define FAILE   0
#define SUCCESS 0
#define head_flag  				0
#define time_flag  				1
#define time_begin_flag  	2
#define time_end_flag  		3
#define mac_flag          4


void zigbee_init(void);
uint16_t zigbee_reset(void);
uint16_t zigbee_wakeup(void);
uint16_t zigbee_send_hook(uint8_t *buf,uint16_t len);
uint8_t zigbee_recv_hook(uint8_t command_buf[],uint8_t len);
uint16_t zigbee_wakeup_hook(void);



#endif /* __ZIGBEE_H */
