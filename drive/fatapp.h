#ifndef __FATAPP_H 
#define __FATAPP_H

#include "stm32f37x.h"
#include "integer.h"
#include "ffconf.h"
#include "diskio.h"
#include "ff.h"


// BMP信息头 54Byte, 只使用有用部分
typedef struct
{	
	uint16_t	bfType;				 // 说明文件类型，在windows系统中为BM
	uint32_t	bfSize;				 // 说明文件大小
	uint32_t	biWidth;			 //	说明图像宽度  							 
	uint32_t	biHeight;			 //	说明图像高度
	uint16_t	biBitCount;			 //	每个像素的位数，单色位图为１，256色为8，16bit为16， 24bit为24
}BMP_HEADER;

extern FRESULT FileWrite(const TCHAR *fileName,const BYTE *buffer); // 写文件
extern FRESULT FileRead(const TCHAR *fileName, BYTE *buf); // 读文件
extern BYTE FileScan(BYTE *path);	 // 文件扫描
extern BMP_HEADER TFTBmpGetHeadInfo(uint8_t *buf);  // 获取BMP图像头文件信息
extern uint8_t TFTBmpDisplay(uint8_t *bmpName,uint16_t x,uint16_t y);  // bmp图片显示
extern void FileNameShow(uint16_t x,uint16_t y,uint8_t *p); // 显示文件名

extern char FileN[50][50];	// 文件名存储
extern char type[8][20];	// 文件类型定义
extern char flag[50]; // 文件类型存储标记 (50表示可以存放50个文件的标记)




#endif





