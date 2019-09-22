#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "stm32f37x.h"

/*
ERROR CODE
0   no such head
1   command don't have ':'
2   time Does not meet specifications
3   don't find ',' between starttime and endtime
*/
#define handle_success  0
#define unexpected_command  1
#define handle_failure  1

#define greater   0
#define smaller   1
#define equal     2

#define head_flag  				0
#define time_flag  				1
#define time_begin_flag  	2
#define time_end_flag  		3
#define mac_flag          4


#define command_buffer_Queue_LEN  20


typedef struct{
u16 year;
u8 month;
u8 day;
u8 hour;
u8 minte;
u8 second;	
}time_t;

typedef struct{
char head[24];
time_t start_time;
time_t end_time;
time_t time;	
}command_t;

uint8_t command_msg_analysis(uint8_t command_buf[],uint8_t len);
uint8_t Command_msg_handler( command_t* command_rcv );
uint8_t timet_checking(time_t time);
uint8_t timet_compare(time_t time1,time_t time2);
uint8_t MY_AD_start(uint16_t time);
void set_time(command_t* command_rcv);
void set_alarm(command_t* command_rcv);
void time_inject(uint8_t index,uint8_t i,time_t time,uint8_t temp);
void 	project_init(void);
uint8_t Date_read(command_t* command_rcv);
uint8_t timet_increment(time_t time1);
#endif
