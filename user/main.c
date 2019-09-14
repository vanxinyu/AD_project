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
#include "ffconf.h"
#include "sd.h"
#include "ff.h"
#include "fatapp.h"
#include "ili9328.h"
#include "string.h"
#include "w25x16.h"
#include "uart.h"
#include <stdio.h>
#define countof(a) (sizeof(a) / sizeof(*(a)))//���������ڵĳ�Ա����
///////////////////////////////////////////////////
	FATFS fs;// Work area (file system object) for logical drive
	FRESULT res;// FatFs function common result code
	FIL fsrc, fdst;      // file objects
	BYTE buffer[1024]; // file copy buffer
	UINT br, bw;         // File R/W count
	unsigned char w_buffer[]={"hello!"};//��ʾд���ļ�
////////////////////////////////////////////////////

int main(void)
{  	
	SystemInit();
	USART_Configuration();
	printf("start\r\n");
//	LCD_init();		 // Һ����ʾ����ʼ��
//	SPI_FLASH_Init();
//	LCD_Clear(RED);	 // ȫ����ʾ��ɫ
//	POINT_COLOR =BLACK; // ����ʵ���ɫΪ��ɫ
//	BACK_COLOR = WHITE  ;	 // ����ʵı���ɫΪ��ɫ
	  /*-------------------------- SD Init ----------------------------- */ 
	 //SPI_FLASH_Init();

	  /*-------------------------- SD Init ----------------------------- */ 
	disk_initialize(0);
	printf("mmc/sd ��ʾ\r\n");	
	res = f_mount(0, &fs);    
	if(res == FR_OK)
	printf("mmc/sd ��ʼ���ɹ�\r\n");		
	else	
	printf("mmc/sd ��ʼ��ʧ��\r\n");
	
	res=f_open(&fsrc,"12-30.txt",FA_CREATE_ALWAYS | FA_WRITE);
	if (res == FR_OK) 
	printf("�ļ������ɹ�\r\n");
	else	
	printf("�ļ�����ʧ��\r\n");
	res = f_write(&fsrc, &w_buffer, countof(w_buffer), &bw);

	if (res == FR_OK) 
	printf("SD��д�ɹ�\r\n");	
	else
	printf("SD��дʧ��\r\n");
	res=f_close(&fsrc);
	if (res == FR_OK) 
	printf("�ı��رճɹ�\r\n");
	else
	printf("�ı��ر�ʧ��\r\n");

	res=f_open(&fsrc,"12-29.TXT",FA_READ);
	if (res == FR_OK) 
	printf("���ı��ɹ�\r\n");
	else
	printf("���ı�ʧ��\r\n");
	res = f_read(&fsrc, &buffer, 1024, &br);
	if (res == FR_OK) 
	{
	printf("�ļ���ȡ�ɹ�\r\n");
	printf("%s\r\n",buffer);
	}
	else
	printf("���ļ�ʧ��\r\n");
	while(1)
	{
	}
	}
