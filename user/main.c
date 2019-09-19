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
#include "ffconf.h"
#include "sd.h"
#include "ff.h"
#include "fatapp.h"
#include "ili9328.h"
#include "string.h"
#include "uart.h"
#include "w25x16.h"
#include "sdadc.h"
#include "systick.h"
#include "rtc.h"
#include "application.h"
#include <stdio.h>

#define countof(a) (sizeof(a) / sizeof(*(a)))//计算数组内的成员个数
///////////////////////////////////////////////////
	FATFS fs;// Work area (file system object) for logical drive
	FRESULT res;// FatFs function common result code
	FIL fsrc, fdst;      // file objects
	BYTE buffer[1024]; // file copy buffer
	UINT br, bw;         // File R/W count
  char w_buffer[24];//演示写入文件
////////////////////////////////////////////////////

	int16_t InjectedConvData = 0;
	__IO uint32_t TimingDelay = 0;
	int adc_start_flag=0;
	int read_flag=0;
	int read_start_flag=0;
int main(void)
{  	
	  RCC_ClocksTypeDef RCC_Clocks;
		RTC_TimeTypeDef RTC_TimeStruct;
		RTC_DateTypeDef RTC_DateStruct;
  __IO float InputVoltageMv = 0;
	int time=0;
	u8 tbuf[40];
	u8 t=0; 
	u8 len;
  /* SysTick end of count event each 1ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
	SystemInit();
	USART_Configuration();
	My_RTC_Init();
	printf("/**************project start*****************/\r\n");
	

//	 if(SDADC1_Config() != 0)
//  {
//		printf("sdadc config error");
//  }
	  /*-------------------------- SD Init ----------------------------- */ 
//	disk_initialize(0);
//	printf("mmc/sd 演示\r\n");	
//	res = f_mount(0, &fs);    
//	if(res == FR_OK)
//	printf("mmc/sd 初始化成功\r\n");		
//	else	
//	printf("mmc/sd 初始化失败\r\n");
	
//	
//	res=f_open(&fsrc,"12-30.txt",FA_CREATE_ALWAYS | FA_WRITE);
//	if (res == FR_OK) 
//	printf("文件创建成功\r\n");
//	else	
//	printf("文件创建失败\r\n");
//	res = f_write(&fsrc, &w_buffer, countof(w_buffer), &bw);

//	if (res == FR_OK) 
//	printf("SD卡写成功\r\n");	
//	else
//	printf("SD卡写失败\r\n");
//	res=f_close(&fsrc);
//	if (res == FR_OK) 
//	printf("文本关闭成功\r\n");
//	else
//	printf("文本关闭失败\r\n");
//	
//	res=f_open(&fsrc,"12-30.TXT", FA_WRITE);
//	if (res == FR_OK) 
//	printf("打开文本成功\r\n");
//	else
//	printf("打开文本失败\r\n");
//	
//	res=f_lseek(&fsrc,fsrc.fsize);
//	if (res == FR_OK) 
//	printf("打开文本成功\r\n");
//	else
//	printf("打开文本失败\r\n");
//	res = f_write(&fsrc, &w_buffer, countof(w_buffer), &bw);
//	if (res == FR_OK) 
//	printf("SD卡写成功\r\n");	
//	else
//	printf("SD卡写失败\r\n");
//	res=f_close(&fsrc);
//	if (res == FR_OK) 
//	printf("文本关闭成功\r\n");
//	else
//	printf("文本关闭失败\r\n");
//	
//	
//	

//	res=f_open(&fsrc,"12-30.TXT",FA_READ);
//	if (res == FR_OK) 
//	printf("打开文本成功\r\n");
//	else
//	printf("打开文本失败\r\n");
//	res = f_read(&fsrc, &buffer, 1024, &br);
//	if (res == FR_OK) 
//	{
//	printf("文件读取成功\r\n");
//	printf("%s\r\n",buffer);
//	}
//	else
//	printf("读文件失败\r\n");
//	res=f_close(&fsrc);
//	RTC_Set_Time(20,30,50,RTC_H12_AM);	//设置时间
//	RTC_Set_Date(19,9,15,7);		//设置日期
//		
//	RTC_Set_AlarmA(1,20,31,0);
//	RTC_Set_AlarmB(1,20,31,10);
		
	while(1)
	{
			if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
//			printf("\r\n您发送的消息为:\r\n");
			printf("%s\r\n",COMMAND_BUF);
			command_msg_analysis(COMMAND_BUF,len);
			printf("\r\n\r\n");//插入换行
			USART_RX_STA=0;
		}
//		t++;
//		if(adc_start_flag)
//		{
//			
//			SDADC1_Config();
//			res=f_open(&fsrc,"12-30.txt",FA_WRITE);
//			if (res == FR_OK) 
//			printf("文件创建成功\r\n");
//			else	
//			printf("文件创建失败\r\n");
//			InputVoltageMv = (((InjectedConvData + 32768) * SDADC_VREF) / (SDADC_GAIN * SDADC_RESOL));
//			sprintf(w_buffer,"%2.0f mV  ",InputVoltageMv);
//			f_lseek(&fsrc,fsrc.fsize);
//			f_write(&fsrc, &w_buffer, strlen(w_buffer), &bw);
//			res=f_close(&fsrc);
//      /* write result to LCD */
//      printf(" value = %2.0f mV  \r\n", InputVoltageMv);
//		}
//		else
//		{	
//		
//		SDADC_Cmd(POT_SDADC, DISABLE);
//		if((t%10)==0)	//?100ms????????
//		{
//			
//			RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
//			sprintf((char*)tbuf,"Time:%02d:%02d:%02d",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds); 
//			printf("time%s\r\n",tbuf);
//			RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
//			sprintf((char*)tbuf,"Date:20%02d-%02d-%02d",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date); 
//			printf("data%s\r\n",tbuf);
//		} 
//		}
////		Delay_ms(1);
	}
	}
