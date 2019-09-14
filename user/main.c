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
#include "w25x16.h"
#include "uart.h"
#include <stdio.h>
#define countof(a) (sizeof(a) / sizeof(*(a)))//计算数组内的成员个数
///////////////////////////////////////////////////
	FATFS fs;// Work area (file system object) for logical drive
	FRESULT res;// FatFs function common result code
	FIL fsrc, fdst;      // file objects
	BYTE buffer[1024]; // file copy buffer
	UINT br, bw;         // File R/W count
	unsigned char w_buffer[]={"hello!"};//演示写入文件
////////////////////////////////////////////////////

int main(void)
{  	
	SystemInit();
	USART_Configuration();
	printf("start\r\n");
//	LCD_init();		 // 液晶显示器初始化
//	SPI_FLASH_Init();
//	LCD_Clear(RED);	 // 全屏显示白色
//	POINT_COLOR =BLACK; // 定义笔的颜色为黑色
//	BACK_COLOR = WHITE  ;	 // 定义笔的背景色为白色
	  /*-------------------------- SD Init ----------------------------- */ 
	 //SPI_FLASH_Init();

	  /*-------------------------- SD Init ----------------------------- */ 
	disk_initialize(0);
	printf("mmc/sd 演示\r\n");	
	res = f_mount(0, &fs);    
	if(res == FR_OK)
	printf("mmc/sd 初始化成功\r\n");		
	else	
	printf("mmc/sd 初始化失败\r\n");
	
	res=f_open(&fsrc,"12-30.txt",FA_CREATE_ALWAYS | FA_WRITE);
	if (res == FR_OK) 
	printf("文件创建成功\r\n");
	else	
	printf("文件创建失败\r\n");
	res = f_write(&fsrc, &w_buffer, countof(w_buffer), &bw);

	if (res == FR_OK) 
	printf("SD卡写成功\r\n");	
	else
	printf("SD卡写失败\r\n");
	res=f_close(&fsrc);
	if (res == FR_OK) 
	printf("文本关闭成功\r\n");
	else
	printf("文本关闭失败\r\n");

	res=f_open(&fsrc,"12-29.TXT",FA_READ);
	if (res == FR_OK) 
	printf("打开文本成功\r\n");
	else
	printf("打开文本失败\r\n");
	res = f_read(&fsrc, &buffer, 1024, &br);
	if (res == FR_OK) 
	{
	printf("文件读取成功\r\n");
	printf("%s\r\n",buffer);
	}
	else
	printf("读文件失败\r\n");
	while(1)
	{
	}
	}
