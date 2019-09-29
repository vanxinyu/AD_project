/******************** (C) COPYRIGHT 2012 青风电子 ********************
 * 文件名  ：main
 * 描述    ：         
 * 实验平台：青风stm32f373开发板
 * 描述    ：文件系统读sd卡
 * 作者    ：青风
 * 店铺    ：qfv5.taobao.com
**********************************************************************/

#include "diskio.h"
#include "stm32f37x.h"
#include "uart.h"
#include "sdadc.h"
#include "ffconf.h"
#include "sd.h"
#include "ff.h"
#include "fatapp.h"
#include "ili9328.h"
#include "string.h"
#include "w25x16.h"
#include "systick.h"
#include "rtc.h"
#include "application.h"
#include <stdio.h>
#include "time.h"
#include "file.h"
#include "wkup.h"
#include "iwdg.h"
#include "adc.h"
#include "led.h"
#define countof(a) (sizeof(a) / sizeof(*(a)))//计算数组内的成员个数
///////////////////////////////////////////////////
FATFS fs;// Work area (file system object) for logical drive
FRESULT res;// FatFs function common result code
FIL fsrc, fdst;      // file objects
DIR dir;
u16 write_buf[2048];
u16 write_buf2[1024];

u8  writebuf1=1;
UINT br, bw;         // File R/W count
char w_buffer[4];//演示写入文件
extern u8  pathname[48];
extern u8  filename[24];
u8 creat_file=0;
u8 write_file=0;
////////////////////////////////////////////////////
int16_t InjectedConvData = 0;
__IO uint32_t TimingDelay = 0;
int adc_start_flag=0;
int sdadc_config=0;
extern __IO uint16_t temp1;
extern __IO uint16_t temp2;
RTC_TimeTypeDef RTC_TimeStruct;
RTC_DateTypeDef RTC_DateStruct;

int main(void)
{
	RCC_ClocksTypeDef RCC_Clocks;
  int i=0;
	u8 t=0; 
	u8 len;
  /* SysTick end of count event each 1ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
	SystemInit();
	USART_Configuration();
	My_RTC_Init();
//	TIM_INT_Config();
//	TIM_OUT_Config();
	WKUP_Init();
	adc_init();
	LED_Init();
//	IWDG_Init(4,500); 
	sensor_sample_init();
	file_init();
	printf("/**************project start*****************/\r\n");
	UART2_Send((u8*)"start",sizeof("start"));
	
//	RTC_Set_Time(20,30,55,RTC_H12_AM);	//????
//	RTC_Set_Date(19,9,15,7);		//????

//	RTC_Set_AlarmA(1,20,31,0);
//	RTC_Set_AlarmB(1,20,31,1);
	while(1)
	{
		IWDG_Feed();
			if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			command_msg_analysis(COMMAND_BUF,len);
			printf("\r\n\r\n");//插入换行
			USART_RX_STA=0;
		}
			
		if(creat_file)
		{
			if(adc_start_flag)
			{
				RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
				RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
				camera_new_pathname(pathname,filename,RTC_TimeStruct,RTC_DateStruct);
				file_creat((char*)pathname,(char*) filename);
			}
			creat_file=0;
		}
		if(write_file)
		{
			if(adc_start_flag)
			{
				if(writebuf1)
				{file_write((char*) filename, write_buf2);}
				else
				{file_write((char*) filename, write_buf); }
			}
			write_file=0;
		}
//		if(sdadc_config)
//		{
//			if(t==0)
//			{SDADC1_Config();t++;}
//			else
//			{}
//		}
//		else
//		{
//			t=0;
//			SDADC_Cmd(POT_SDADC, DISABLE);
//		}
	}
}
