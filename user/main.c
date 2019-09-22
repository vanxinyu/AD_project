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
#define countof(a) (sizeof(a) / sizeof(*(a)))//计算数组内的成员个数
///////////////////////////////////////////////////
	FATFS fs;// Work area (file system object) for logical drive
	FRESULT res;// FatFs function common result code
	FIL fsrc, fdst;      // file objects
	DIR dir;
	BYTE buffer[1024]; // file copy buffer
	u32 write_buf[1024];
	u32 write_buf2[1024];
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

  
	int time=0;
  int i=0;
	u8 tbuf[40];
	u8 t=0; 
	u8 len;
  /* SysTick end of count event each 1ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
	SystemInit();
	USART_Configuration();
	My_RTC_Init();
	TIM_INT_Config();
	TIM_OUT_Config();
	printf("/**************project start*****************/\r\n");

	 if(SDADC1_Config() != 0)
  {
		printf("sdadc config error");
  }
	  /*-------------------------- SD Init ----------------------------- */ 
	disk_initialize(0);
	printf("mmc/sd 演示\r\n");	
	res = f_mount(0, &fs);    
	if(res == FR_OK)
	printf("mmc/sd 初始化成功\r\n");		
	else	
	printf("mmc/sd 初始化失败\r\n");

//	res = f_mkdir("0:/19_09_14");
//	if(res == FR_OK)
//	{printf("目录创建成功\r\n");}
//	else
//	{printf("目录创建失败\r\n");}
//	res=f_open(&fsrc,(char *)filename,FA_CREATE_ALWAYS | FA_WRITE);
//	res=f_open(&fsrc,"0:/19_09_14/12_55.txt",FA_CREATE_ALWAYS | FA_WRITE);
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
	while(1)
	{
			if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			printf("\r\n您发送的消息为:\r\n");
			printf("%s\r\n",COMMAND_BUF);
			command_msg_analysis(COMMAND_BUF,len);
			printf("\r\n\r\n");//插入换行
			USART_RX_STA=0;
		}
			if(temp1>=100)//200ms  TIM3 CC3 F=500HZ (1s/500)*100=200ms
		{
			temp1=0;

		}
		if(creat_file)
		{
			if(adc_start_flag)
			{
				RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
				RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
				camera_new_pathname(pathname,filename,RTC_TimeStruct,RTC_DateStruct);
				res = f_opendir(&dir,(char *)pathname);
				if(res == FR_NO_PATH)
				{
					res = f_mkdir((char *)pathname);
					if(res == FR_OK)
					{printf("目录创建成功\r\n");}
					else
					{printf("目录创建失败\r\n");}
				}
				res=f_open(&fsrc,(char *)filename, FA_CREATE_NEW | FA_WRITE);
				if (res == FR_OK) 
				printf("文件创建成功\r\n");
				else	
				printf("文件创建失败\r\n");
				res=f_close(&fsrc);
			}
			creat_file=0;
		}
		if(write_file)
		{
			if(adc_start_flag)
			{
			res=f_open(&fsrc,(char *)filename, FA_WRITE);
			if(res==FR_OK)
			{
				printf("打开成功\r\n");
			}	
			for(i=0;i<=1000;i++)
			{
				if(writebuf1)
				{sprintf(w_buffer,"%2.0f ",(double)write_buf2[i]);}
				else
				{sprintf(w_buffer,"%2.0f ",(double)write_buf[i]);}
				f_lseek(&fsrc,fsrc.fsize);
				f_write(&fsrc, &w_buffer, strlen(w_buffer), &bw);
			}
			res=f_close(&fsrc);
			if(res==FR_OK)
			{
				printf("写成功\r\n");
			}
			}
			write_file=0;
		}

		if(sdadc_config)
		{
			if(t==0)
			{SDADC1_Config();t++;}
			else
			{}
		}
		else
		{
			t=0;
			SDADC_Cmd(POT_SDADC, DISABLE);
		}

	}
}
