#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include  "cmsis_os.h"

#define handle_success  0
#define unexpected_command  1
#define handle_failure  1

#define greater   0
#define smaller   1
#define equal     2


#define command_buffer_Queue_LEN  20

extern  QueueHandle_t command_buffer_Queue;

typedef struct{
uint16_t year;
uint8_t month;
uint8_t day;
uint8_t hour;
uint8_t minte;	
}time_t;

typedef struct{
uint16_t time;
char head[24];
time_t start_time;
time_t end_time;	
}command_t;

uint8_t command_msg_analysis(uint8_t command_buf[],uint8_t len);
uint8_t Command_msg_handler( void );
uint8_t timet_checking(time_t time);
uint8_t timet_compare(time_t time1,time_t time2);
uint8_t MY_AD_start(uint16_t time);
#endif
