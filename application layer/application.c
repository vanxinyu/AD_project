/*
 * application.c
 *
 *  Created on: 2019??8??12??
 *  Author: FanXinYu
 */
#include "application.h"
#include "cmsis_os.h"
#include "string.h"
#include "BY_log.h"
#include "main.h"
#include "usart.h"
#include "stdlib.h"
#include "rtc.h"
#include "mmc_sd.h"		
#include "ff.h"			/* FatFs configurations and declarations */
#include "diskio.h"		/* Declarations of low level disk I/O functions */


#define head_flag  			0
#define time_flag  			1
#define time_begin_flag  	2
#define time_end_flag  		3

uint8_t tbuf[40];
RTC_TimeTypeDef RTC_Time;
RTC_DateTypeDef RTC_Date;

QueueHandle_t command_buffer_Queue;

uint8_t w;
/**
* \brief handle different usart command
* \param command_buf   the command wait for handle 
						ad_start,(time); (time) Refers to the ad sampling for the duration of time;eg  ad_start,15 (seconds)
						ad_read,(RTC TIME)-(RTC TIME) ;read the ad sample data in sd card from (RTC TIME1) to (RTC TIME2);eg ad_read,2019/8/13/10:13-2019/8/13/10:14
		len             the length of command
* \return 0 if successfull, otherwise failed
*/

uint8_t Command_msg_handler( void )
{
	//command_analysis()3
	static command_t* command_rcv = NULL;
	
	if(xQueueReceive(command_buffer_Queue, &command_rcv, 10*portTICK_PERIOD_MS))
	{
		
		if(strcmp(command_rcv->head,"ad_start")==0)
		{
			HAL_UART_Transmit(&huart2,(uint8_t*)"收到的命令是",sizeof("收到的命令是"),1000);
			HAL_UART_Transmit(&huart2,(uint8_t*)command_rcv->head,sizeof(command_rcv->head),1000);
			MY_AD_start(command_rcv->time);
			free(command_rcv);
		}
		if(strcmp(command_rcv->head,"ad_read")==0)
		{
			HAL_UART_Transmit(&huart2,(uint8_t*)"收到的命令是",sizeof("收到的命令是"),1000);
			HAL_UART_Transmit(&huart2,(uint8_t*)command_rcv->head,sizeof(command_rcv->head),1000);
			
			//AD_read(command_rcv->start_time,command_rcv->end_time);
			free(command_rcv);
		}
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
	uint8_t count=0;
	uint16_t year=0;
	uint8_t month=0;
	uint8_t day=0;
	uint8_t hour=0;
	uint8_t minte=0;
	
	command_t* command_p= (command_t *)malloc(sizeof(command_t)+len);
	memset(command_p,0,sizeof(command_t)+len);
	for(index=0;index<len;index++)
	{
		temp=command_buf[index];
		switch(state)
		{
			case head_flag:
				if(temp==',')
				{
					
					if(strcmp(command_p->head,"ad_start")==0)
					{
						state = time_flag;
					}
					else if(strcmp(command_p->head,"ad_read")==0)
					{
						state = time_begin_flag;
					}
					else
					{
						log_error("no such head :%s",command_p->head);
						free(command_p);
						state=head_flag;
						return unexpected_command;
					}
				}
				else
				{
					command_p->head[index]=temp;
					if((index>=12)||(index==len-1))
					{
						log_error("unexpected command: head is too long");
						free(command_p);
						state=head_flag;
						return unexpected_command;
					}
				}
				break;
			case time_flag:
				if((temp>='0')&&(temp<='9'))
				{
					command_p->time *=10;
					command_p->time +=temp-'0';
					if(command_p->time>300)
					{
						log_error("sample time too long :%d",command_p->time);
						free(command_p);
						state=head_flag;
						return unexpected_command;
					}
				}
				else
				{
					log_error("unexpected command: the format of time is wrong");
					free(command_p);
					state=head_flag;
					return unexpected_command;
				}
				break;
			case time_begin_flag:
				if(temp=='-')
				{
					if(index == len-1)
					{
						log_error("unexpected command: the format of time is wrong");
						free(command_p);
						state=head_flag;
						return unexpected_command;
					}
					if(count!=4)
					{
						log_error("unexpected command: the format of time is wrong");
						free(command_p);
						state=head_flag;
						return unexpected_command;
					}
					else
					{
						command_p->start_time.year=year;
						command_p->start_time.month=month;
						command_p->start_time.day=day;
						command_p->start_time.hour=hour;
						command_p->start_time.minte=minte;
						if(timet_checking(command_p->start_time)==0)
						{
							count=0;year=0;month=0;day=0;hour=0;minte=0;
							state = time_end_flag;
						}
						else
						{
							log_error("unexpected command: the format of time is wrong");
							state=head_flag;
							return unexpected_command;
						}
					}
				}
				else
				{
					if(index == len-1)
					{
						log_error("unexpected command: the format of time is wrong");
						free(command_p);
						state=head_flag;
						return unexpected_command;
					}
					if((temp=='/')||(temp ==':'))
					{
						count++;
					}
					else
					{
						if(count == 0)
						{
							year*=10;
							year+=temp-'0';
						}
						if(count == 1)
						{
							
							month*=10;
							month+=temp-'0';
						}
						if(count == 2)
						{	
							day*=10;
							day+=temp-'0';
						}
						if(count == 3)
						{										
							hour*=10;
							hour+=temp-'0';
						}
						if(count == 4)
						{
							minte*=10;
							minte+=temp-'0';
						}
					}
				}
				break;
			case time_end_flag:
					if((temp=='/')||(temp ==':'))
					{
						count++;
					}
					else
					{
						if(count == 0)
						{
							year*=10;
							year+=temp-'0';
						}
						if(count == 1)
						{
							month*=10;
							month+=temp-'0';
						}
						if(count == 2)
						{	
							day*=10;
							day+=temp-'0';
						}
						if(count == 3)
						{										
							hour*=10;
							hour+=temp-'0';
						}
						if(count == 4)
						{
							minte*=10;
							minte+=temp-'0';
						}
					}
					if(index == len-1)
					{
						if(count!=4)
						{
							log_error("unexpected command: the format of time is wrong");
							free(command_p);
							state=head_flag;
							return unexpected_command;
						}
						else
						{
							command_p->end_time.year=year;
							command_p->end_time.month=month;
							command_p->end_time.day=day;
							command_p->end_time.hour=hour;
							command_p->end_time.minte=minte;
							if(timet_checking(command_p->end_time)==0)
							{
								if(timet_compare(command_p->start_time,command_p->end_time))
								{
									log_debug("start_time:%d/%d/%d/%d:%d\r\n",command_p->start_time.year,command_p->start_time.month,command_p->start_time.day,command_p->start_time.hour,command_p->start_time.minte);
									log_debug("end_time:%d/%d/%d/%d:%d\r\n",command_p->end_time.year,command_p->end_time.month,command_p->end_time.day,command_p->end_time.hour,command_p->end_time.minte);
									count=0;
								}
								else
								{
									log_error("start_time can't greater than end_time");
									free(command_p);
									state=head_flag;
									return unexpected_command;
								}
							}
							else
							{
								log_error("unexpected command: the format of time is wrong");
								state=head_flag;
								return unexpected_command;
							}
						}
					}
					break;
		}
	}
	if(command_p)
	{
		log_info("command analyse ok  ");
		BaseType_t err;
		err = xQueueSend(command_buffer_Queue, &command_p, 0);
		if(err==pdPASS)
		{
			log_info("command  put  ok");
			free(command_p);
		}
		else
		{
			free(command_p);
			log_error("command put error");
		}
	}
	state=head_flag;
	return handle_success;
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
		log_error("the year is wrong :%d",time.year);
		return handle_failure;
	}
	if((time.month>12)||(time.month<=0))
	{
		log_error("the month is wrong :%d",time.month);
		return handle_failure;
	}
	if((time.month==1)||(time.month==3)||(time.month==5)||(time.month==7)||(time.month==8)||(time.month==10)||(time.month==12))
	{
		if(time.day>31)
		{
			log_error("no such day %d in month %d",time.day,time.month);
			return handle_failure;
		}
	}
	if((time.month==4)||(time.month==6)||(time.month==9)||(time.month==11))
	{
		if(time.day>30)
		{
			log_error("no such day %d in month %d",time.day,time.month);
			return handle_failure;
		}
	}
	if(time.month==2)
	{
		if(time.day>28)
		{
			log_error("no such day %d in month %d",time.day,time.month);
			return handle_failure;
		}
	}
	if(time.hour>23)
	{
		log_error("the hour is wrong :%d",time.hour);
		return handle_failure;
	}
	if(time.minte>59)
	{
		log_error("the minte is wrong :%d",time.minte);
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

uint8_t MY_AD_start(uint16_t time)
{
	HAL_RTC_GetTime(&hrtc,&RTC_Time,RTC_FORMAT_BIN);
	sprintf((char*)tbuf,"Time:%02d:%02d:%02d\r\n",RTC_Time.Hours,RTC_Time.Minutes,RTC_Time.Seconds);
	HAL_UART_Transmit(&huart2,(uint8_t*)tbuf,sizeof(tbuf),1000);
	HAL_RTC_GetDate(&hrtc,&RTC_Date,RTC_FORMAT_BIN);
	sprintf((char*)tbuf,"Date:20%02d-%02d-%02d\r\n",RTC_Date.Year,RTC_Date.Month,RTC_Date.Date);
	HAL_UART_Transmit(&huart2,(uint8_t*)tbuf,sizeof(tbuf),1000);
	return 0;
}
void FATFS_INIT(void)
{
	FATFS fs[2];  		//逻辑磁盘工作区.	 
	FIL fnew; //文件对象
	FRESULT res_sd;//文件操作结果
	UINT fnum; //文件成功读写数量
	res_sd = f_mount(0, &fs[0]);
	if(res_sd == FR_NO_FILESYSTEM)
	{
		log_debug("SD卡没有文件系统，即将进行格式化...\r\n");
		//格式化
		res_sd = f_mkfs(0, 0, 0);
		
		if(res_sd == FR_OK)
		{
			printf("SD卡成功格式化！\r\n");
			//格式化后先取消挂载
			res_sd = f_mount(0,NULL);
			//再重新挂载
			res_sd = f_mount(0,&fs[0]);
		}
		else
		{
			log_debug("文件格式化失败！错误代码：%d\r\n",res_sd);
			while(1);
		}
	}
	else if(res_sd != FR_OK)
	{
		log_error("挂载文件系统失败！可能是因为文件初始化失败！错误代码：%d\r\n", res_sd);
	}
	else
	{
		log_info("FATFS prepare ok！can write now\r\n");
	}
	
	
}

void 	project_init(void)
{
	RTC_init_set();
	SD_check_init();
	FATFS_INIT();
}
