/******************** (C) COPYRIGHT 2012 ������ ********************
 * �ļ���  ��main
 * ����    ��         
 * ʵ��ƽ̨�����stm32f373������
 * ����    ���ļ�ϵͳ��sd��
 * ����    �����
 * ����    ��qfv5.taobao.com
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
#define countof(a) (sizeof(a) / sizeof(*(a)))//���������ڵĳ�Ա����
///////////////////////////////////////////////////
FATFS fs;// Work area (file system object) for logical drive
FRESULT res;// FatFs function common result code
FIL fsrc, fdst;      // file objects
DIR dir;
u32 write_buf[1024];
u32 write_buf2[1024];

u8  writebuf1=1;
UINT br, bw;         // File R/W count
char w_buffer[4];//��ʾд���ļ�
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
	TIM_INT_Config();
	TIM_OUT_Config();
	WKUP_Init();
	adc_init();
	IWDG_Init(4,500); 
	printf("/**************project start*****************/\r\n");

	 if(SDADC1_Config()!= 0)
  {
		printf("sdadc config error");
  }
	file_init();
	UART2_Send((u8*)"start",sizeof("start"));
	while(1)
	{

			if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			command_msg_analysis(COMMAND_BUF,len);
			printf("\r\n\r\n");//���뻻��
			USART_RX_STA=0;
		}
			if(temp1>=100)//200ms  TIM3 CC3 F=500HZ (1s/500)*100=200ms
		{
			temp1=0;
			IWDG_Feed();
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
