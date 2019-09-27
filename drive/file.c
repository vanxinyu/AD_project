#include "file.h"
#include "diskio.h"
#include "ffconf.h"
#include "ff.h"
#include "fatapp.h"
#include "ili9328.h"
#include "uart.h"

extern FATFS fs;// Work area (file system object) for logical drive
extern FRESULT res;// FatFs function common result code
extern FIL fsrc, fdst;      // file objects
extern DIR dir;
extern UINT br, bw;         // File R/W count
u8 space=0x20;
BYTE buffer[512]; // file copy buffer
uint16_t file_init(void)
{
	u8 res=FAILE;

	/*PSD ��ʼ��   PSD -> PF6*/
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOF, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOF,GPIO_Pin_6);
	
	disk_initialize(0);
	printf("sd ��ʼ��\r\n");
	
	res = f_mount(0, &fs);  
	if(res == FR_OK)
	printf("�ļ�ϵͳ ��ʼ���ɹ�\r\n");		
	else
	printf("�ļ�ϵͳ ��ʼ��ʧ��\r\n");
	
	res=SUCCESS;
	return res;
}
uint16_t file_creat(char*pathname,char* filename)
{
	res = f_opendir(&dir,(char *)pathname);
	if(res == FR_NO_PATH)
	{
		res = f_mkdir((char *)pathname);
		if(res == FR_OK)
		{printf("Ŀ¼�����ɹ�\r\n");}
		else
		{printf("Ŀ¼����ʧ��\r\n");}
	}
	res=f_open(&fsrc,(char *)filename, FA_CREATE_NEW | FA_WRITE);
	if (res == FR_OK) 
	printf("�ļ������ɹ�\r\n");
	else	
	printf("�ļ�����ʧ��\r\n");
	res=f_close(&fsrc);
}

uint16_t file_read(char* filename)
{
	res=f_open(&fsrc,(char *)filename,FA_READ);
	if (res == FR_OK) 
	{
		printf("���ı��ɹ�\r\n");
		do
		{
			res = f_read(&fsrc, &buffer, 512, &br);
			if (res == FR_OK) 
			{
			UART2_Send(buffer,sizeof(buffer));
			}
			else
			printf("���ļ�ʧ��\r\n");
		}while(br!=0);
		res=f_close(&fsrc);
	}
	else
	printf("�����ڴ��ļ�%s",filename);
}
uint16_t file_write(char* filename,u32 write_buf[1024])
{
	int i=0;
	res=f_open(&fsrc,(char *)filename, FA_WRITE);
	if(res==FR_OK)
	{
		printf("�򿪳ɹ�\r\n");
	}	
	for(i=0;i<=1000;i++)
	{
//		  write_buf[i]=0x22222222;
			f_lseek(&fsrc,fsrc.fsize);
			f_write(&fsrc, &write_buf[i], 4, &bw);
			f_lseek(&fsrc,fsrc.fsize);
			f_write(&fsrc, &space, 1, &bw);
	}
	res=f_close(&fsrc);
	if(res==FR_OK)
	{
		printf("д�ɹ�\r\n");
	}
}
uint16_t file_deinit(void)
{
	GPIO_ResetBits(GPIOF,GPIO_Pin_6);
}