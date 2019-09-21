/*
 * application.c
 *
 *  Created on: 2019??8??12??
 *  Author: FanXinYu
 */
#include "application.h"
#include "string.h"
#include "uart.h"
#include "stdlib.h"
#include "rtc.h"
#include "sd.h"



uint8_t tbuf[40];
RTC_TimeTypeDef RTC_Time;
RTC_DateTypeDef RTC_Date;
extern RTC_TimeTypeDef RTC_TimeStruct;
extern RTC_DateTypeDef RTC_DateStruct;

/**
* \brief handle different usart command
* \param command_buf   the command wait for handle 
						ad_start,(time); (time) Refers to the ad sampling for the duration of time;eg  ad_start,15 (seconds)
						ad_read,(RTC TIME)-(RTC TIME) ;read the ad sample data in sd card from (RTC TIME1) to (RTC TIME2);eg ad_read,2019/8/13/10:13-2019/8/13/10:14
		len             the length of command
* \return 0 if successfull, otherwise failed
*/

uint8_t Command_msg_handler( command_t* command_rcv )
{
		if(strcmp(command_rcv->head,"AT+SETTIME")==0)
		{
			printf("收到的命令是%s\r\n",command_rcv->head);
			set_time(command_rcv);
			free(command_rcv);
		}
		if(strcmp(command_rcv->head,"AT+START")==0)
		{
			printf("收到的命令是%s\r\n",command_rcv->head);	
			set_alarm(command_rcv);
			free(command_rcv);
		}
		if(strcmp(command_rcv->head,"AT+READDATA")==0)
		{
			printf("收到的命令是%s\r\n",command_rcv->head);		
			set_alarm(command_rcv);
			free(command_rcv);
		}
	return handle_success;
}
/**
* \brief analysis command 
* \param command_buf   the command wait for handle 
						ad_start,(time); (time) Refers to the ad sampling for the duration of time;eg  ad_start,15 (seconds)
						ad_read,(RTC TIME)-(RTC TIME) ;read the ad sample data in sd card from (RTC TIME1) to (RTC TIME2);eg ad_read,2019/8/13/10:13-2019/8/13/10:14
		len             the length of command
* \return command's head and time
*/
uint8_t command_msg_analysis(uint8_t command_buf[],uint8_t len)
{
	static uint8_t state=head_flag;
	uint8_t index;
	uint8_t temp;

	u8 i=0;
	command_t* command_p= (command_t *)malloc(sizeof(command_t)+len);
	memset(command_p,0,sizeof(command_t)+len);
	for(index=0;index<len;index++)
	{
		temp=command_buf[index];
		switch(state)
		{
			case head_flag:
				if(temp==':')
				{
					state = mac_flag;
				}
				else
				{
					command_p->head[index]=temp;
					if(index==len-1)
					{
						printf("unexpected command: no ':'");
						UART2_Send((u8*)"ERROR:1",sizeof("ERROR:1"));
						free(command_p);
						state=head_flag;
						return unexpected_command;
					}
				}
				break;
			case mac_flag:
				if(temp==',')
				{
					if(strcmp(command_p->head,"AT+SETTIME")==0)
					{
						printf("head :%s\r\n",command_p->head);
						state = time_flag; i=index+1; 
					}
					else if(strcmp(command_p->head,"AT+START")==0)
					{
						printf("head :%s\r\n",command_p->head);
						state = time_begin_flag;i=index+1;
					}
					else if(strcmp(command_p->head,"AT+READDATA")==0)
					{
						printf("head :%s\r\n",command_p->head);
						state = time_begin_flag;i=index+1;
					}
					else
					{
						UART2_Send((u8*)"ERROR:0",sizeof("ERROR:0"));
						free(command_p);
						state=head_flag;
						return unexpected_command;
					}
				}
				break;
			case time_flag:
				if((temp>='0')&&(temp<='9'))
				{	
					if(index>=i&&index<=i+3)
					{
						command_p->time.year *=10;
						command_p->time.year +=temp-'0';
					}
						if(index>=i+4&&index<=i+5)
					{
						command_p->time.month *=10;
						command_p->time.month +=temp-'0';
					}
						if(index>=i+6&&index<=i+7)
					{
						command_p->time.day *=10;
						command_p->time.day +=temp-'0';
					}
						if(index>=i+8&&index<=i+9)
					{
						command_p->time.hour *=10;
						command_p->time.hour +=temp-'0';
					}
						if(index>=i+10&&index<=i+11)
					{
						command_p->time.minte *=10;
						command_p->time.minte +=temp-'0';
					}
						if(index>=i+12&&index<=i+13)
					{
						command_p->time.second *=10;
						command_p->time.second +=temp-'0';
					}
				}
				if(index==len-1)
				{
					if(timet_checking(command_p->time))
					{
						UART2_Send((u8*)"ERROR:2",sizeof("ERROR:2"));
						state=head_flag;
						return unexpected_command;
					}
					printf("year:%d\r\n",command_p->time.year);
					printf("month:%d\r\n",command_p->time.month);
					printf("day:%d\r\n",command_p->time.day);
					printf("hour:%d\r\n",command_p->time.hour);
					printf("minte:%d\r\n",command_p->time.minte);
					printf("second:%d\r\n",command_p->time.second);
					i=0;
					state=head_flag;
				}
				break;
				
				case time_begin_flag:
				 if(temp==',')
				 {
					 state=time_end_flag;i=index+1; 
					 printf("start_time year:%d\r\n",command_p->start_time.year);
					 printf("start_time month:%d\r\n",command_p->start_time.month);
					 printf("start_time day:%d\r\n",command_p->start_time.day);
					 printf("start_time hour:%d\r\n",command_p->start_time.hour);
					 printf("start_time minte:%d\r\n",command_p->start_time.minte);
					 printf("start_time second:%d\r\n",command_p->start_time.second);
				 }
				 if((temp>='0')&&(temp<='9'))
				 {	
						if(index>=i&&index<=i+3)
						{
							command_p->start_time.year *=10;
							command_p->start_time.year +=temp-'0';
						}
							if(index>=i+4&&index<=i+5)
						{
							command_p->start_time.month *=10;
							command_p->start_time.month +=temp-'0';
						}
							if(index>=i+6&&index<=i+7)
						{
							command_p->start_time.day *=10;
							command_p->start_time.day +=temp-'0';
						}
							if(index>=i+8&&index<=i+9)
						{
							command_p->start_time.hour *=10;
							command_p->start_time.hour +=temp-'0';
						}
							if(index>=i+10&&index<=i+11)
						{
							command_p->start_time.minte *=10;
							command_p->start_time.minte +=temp-'0';
						}
							if(index>=i+12&&index<=i+13)
						{
							command_p->start_time.second *=10;
							command_p->start_time.second +=temp-'0';
						}
				 }
				if(index==len-1)
				{
					UART2_Send((u8*)"ERROR:3",sizeof("ERROR:3"));
					state=head_flag;
					return unexpected_command;
				}
				break;
				
				case time_end_flag:
				 if((temp>='0')&&(temp<='9'))
				 {	
						if(index>=i&&index<=i+3)
						{
							command_p->end_time.year *=10;
							command_p->end_time.year +=temp-'0';
						}
							if(index>=i+4&&index<=i+5)
						{
							command_p->end_time.month *=10;
							command_p->end_time.month +=temp-'0';
						}
							if(index>=i+6&&index<=i+7)
						{
							command_p->end_time.day *=10;
							command_p->end_time.day +=temp-'0';
						}
							if(index>=i+8&&index<=i+9)
						{
							command_p->end_time.hour *=10;
							command_p->end_time.hour +=temp-'0';
						}
							if(index>=i+10&&index<=i+11)
						{
							command_p->end_time.minte *=10;
							command_p->end_time.minte +=temp-'0';
						}
							if(index>=i+12&&index<=i+13)
						{
							command_p->end_time.second *=10;
							command_p->end_time.second +=temp-'0';
						}
				 }
				if(index==len-1)
				{
					 state=head_flag;
					 printf("end_time year:%d\r\n",command_p->end_time.year);
					 printf("end_time month:%d\r\n",command_p->end_time.month);
					 printf("end_time day:%d\r\n",command_p->end_time.day);
					 printf("end_time hour:%d\r\n",command_p->end_time.hour);
					 printf("end_time minte:%d\r\n",command_p->end_time.minte);
					 printf("end_time second:%d\r\n",command_p->end_time.second);
				}
				break;
		}
	}
	if(command_p)
	{
		printf("command analyse ok  ");
		Command_msg_handler(command_p);
		free(command_p);
  }
		state=head_flag;
		return handle_success;
}

void set_time(command_t* command_rcv)
{
		RTC_Set_Time(command_rcv->time.hour,command_rcv->time.minte,command_rcv->time.second,RTC_H12_AM);	//设置时间
	  RTC_Set_Date(command_rcv->time.year%2000,command_rcv->time.month,command_rcv->time.day,7);		//设置日期
		RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
		sprintf((char*)tbuf,"Time:%02d:%02d:%02d",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds); 
		UART2_Send(tbuf,sizeof(tbuf));
		RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
		sprintf((char*)tbuf,"Date:20%02d-%02d-%02d",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date); 
		UART2_Send(tbuf,sizeof(tbuf));
}
void set_alarm(command_t* command_rcv)
{
		RTC_Set_AlarmA(1,command_rcv->start_time.hour,command_rcv->start_time.minte,command_rcv->start_time.second);
		RTC_Set_AlarmB(1,command_rcv->end_time.hour,command_rcv->end_time.minte,command_rcv->end_time.second);
}
/**
* \brief Check if the time format conforms to the specification  
* \param time   specification  2019/8/14/11:30
				year can't more than 2050
		
* \comment support month size check in different month;
* \comment don't support  leap year check;
* \return	 0  Compliance with specifications
			 1  Does not meet specifications
*/
uint8_t timet_checking(time_t time)
{
	if((time .year>=2050)||(time .year<=1976))
	{
		printf("the year is wrong :%d",time.year);
		return handle_failure;
	}
	if((time.month>12)||(time.month<=0))
	{
		printf("the month is wrong :%d",time.month);
		return handle_failure;
	}
	if((time.month==1)||(time.month==3)||(time.month==5)||(time.month==7)||(time.month==8)||(time.month==10)||(time.month==12))
	{
		if(time.day>31)
		{
			printf("no such day %d in month %d",time.day,time.month);
			return handle_failure;
		}
	}
	if((time.month==4)||(time.month==6)||(time.month==9)||(time.month==11))
	{
		if(time.day>30)
		{
			printf("no such day %d in month %d",time.day,time.month);
			return handle_failure;
		}
	}
	if(time.month==2)
	{
		if(time.day>28)
		{
			printf("no such day %d in month %d",time.day,time.month);
			return handle_failure;
		}
	}
	if(time.hour>23)
	{
		printf("the hour is wrong :%d",time.hour);
		return handle_failure;
	}
	if(time.minte>59)
	{
		printf("the minte is wrong :%d",time.minte);
		return handle_failure;
	}
	return handle_success;
		
}
/**
* \brief Compare time1 and time2  
* \param time1   specification  2019/8/14/11:30
		 time2   specification  2019/8/14/11:30
			
* \return	0  time1>time2
			1  time1<time2
			2  time1=time2
*/
uint8_t timet_compare(time_t time1,time_t time2)
{
	if(time1.year>time2.year){ return greater; }
	else if(time1.year < time2.year){ return smaller;}
	else
	{
		if(time1.month>time2.month){ return greater; }
		else if(time1.month < time2.month){ return smaller;}
		else
		{
			if(time1.day>time2.day){ return greater; }
			else if(time1.day < time2.day){ return smaller;}
			else
			{
				if(time1.hour>time2.hour){ return greater; }
				else if(time1.hour < time2.hour){ return smaller;}
				else
				{
					if(time1.hour>time2.hour){ return greater; }
					else if(time1.hour < time2.hour){ return smaller;}
					else
					{
						if(time1.minte>time2.minte){ return greater; }
						else if(time1.minte < time2.minte){ return smaller;}
						else
						{
							return equal;
						}
					}
				}
			}
		}
	}
}

//uint8_t MY_AD_start(uint16_t time)
//{
//	HAL_RTC_GetTime(&hrtc,&RTC_Time,RTC_FORMAT_BIN);
//	sprintf((char*)tbuf,"Time:%02d:%02d:%02d\r\n",RTC_Time.Hours,RTC_Time.Minutes,RTC_Time.Seconds);
//	HAL_UART_Transmit(&huart2,(uint8_t*)tbuf,sizeof(tbuf),1000);
//	HAL_RTC_GetDate(&hrtc,&RTC_Date,RTC_FORMAT_BIN);
//	sprintf((char*)tbuf,"Date:20%02d-%02d-%02d\r\n",RTC_Date.Year,RTC_Date.Month,RTC_Date.Date);
//	HAL_UART_Transmit(&huart2,(uint8_t*)tbuf,sizeof(tbuf),1000);
//	return 0;
//}

//void 	project_init(void)
//{
//	RTC_init_set();
//	SD_check_init();
//}
