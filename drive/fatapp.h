#ifndef __FATAPP_H 
#define __FATAPP_H

#include "stm32f37x.h"
#include "integer.h"
#include "ffconf.h"
#include "diskio.h"
#include "ff.h"


// BMP��Ϣͷ 54Byte, ֻʹ�����ò���
typedef struct
{	
	uint16_t	bfType;				 // ˵���ļ����ͣ���windowsϵͳ��ΪBM
	uint32_t	bfSize;				 // ˵���ļ���С
	uint32_t	biWidth;			 //	˵��ͼ����  							 
	uint32_t	biHeight;			 //	˵��ͼ��߶�
	uint16_t	biBitCount;			 //	ÿ�����ص�λ������ɫλͼΪ����256ɫΪ8��16bitΪ16�� 24bitΪ24
}BMP_HEADER;

extern FRESULT FileWrite(const TCHAR *fileName,const BYTE *buffer); // д�ļ�
extern FRESULT FileRead(const TCHAR *fileName, BYTE *buf); // ���ļ�
extern BYTE FileScan(BYTE *path);	 // �ļ�ɨ��
extern BMP_HEADER TFTBmpGetHeadInfo(uint8_t *buf);  // ��ȡBMPͼ��ͷ�ļ���Ϣ
extern uint8_t TFTBmpDisplay(uint8_t *bmpName,uint16_t x,uint16_t y);  // bmpͼƬ��ʾ
extern void FileNameShow(uint16_t x,uint16_t y,uint8_t *p); // ��ʾ�ļ���

extern char FileN[50][50];	// �ļ����洢
extern char type[8][20];	// �ļ����Ͷ���
extern char flag[50]; // �ļ����ʹ洢��� (50��ʾ���Դ��50���ļ��ı��)




#endif





