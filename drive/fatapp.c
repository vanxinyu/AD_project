#include "fatapp.h"
#include "stdio.h"						 //printf函数库文件
#include "string.h"						 //字符串库文件
#include "ili9328.h"

BYTE Buffer[512];


/********************************************************/
/* 函数功能：写文件                                     */
/* 入口参数：fileName:要写入的文件名或要创建的文件名    */
/*           buffer: 要写入的数据存放数据               */
/* 出口参数：0：成功    1：失败                         */
/********************************************************/
FRESULT FileWrite(const TCHAR *fileName,const uint8_t *buffer)
{
	FATFS fs;           // 文件系统
    FIL file;           // 文件
	UINT  bw;           // 数据字节数

    f_mount(0, &fs);	// 挂载文件系统
	// 给文件中写入数据，如果没有该文件，创建一个名为*fileName的文件，并写入数据
    if(f_open(&file, fileName, FA_CREATE_ALWAYS|FA_WRITE)) 
    {
		return FR_NO_FILE;
    }
	else
	{
		do
		{
			if(f_write(&file, buffer, 512, &bw))
			{
				return FR_NO_FILE;
			}
		} while (bw < 512);  // 判断是否读完(bw = 512，表示写入完成)
   	 	f_close(&file);      // 关闭文件，必须和f_open函数成对出现
	}
    f_mount(0, 0);	  //卸载文件系统

    return FR_OK;
}

/********************************************************/
/* 函数功能：读文件                                     */
/* 入口参数：fileName:要读取的文件名                    */
/*           buf: 要读取的数据存放数组                  */
/* 出口参数：0：成功    1：失败                         */
/********************************************************/
FRESULT FileRead(const TCHAR *fileName, uint8_t *buf)
{
	FATFS fs;              // 建立一个文件系统
    FIL file;              // 暂存文件
	UINT  br;              // 字节计数器
	FRESULT res;		   // 存储函数执行结果

    f_mount(0,&fs);	   // 加载文件系统

	res = f_open(&file, fileName, FA_OPEN_EXISTING|FA_READ); // 打开文件
	
    if(res != FR_OK)  // 如果没有正确打开文件
    {
		return res;	  // 返回错误报告
    }
	else			  // 如果打开了文件
	{
		do
		{
		    f_read(&file, buf, 512, &br);// 读取文件内容，每次512个字节
		}while(br); // br = 0 表示读完了数据
	}
	
   	f_close(&file);      // 关闭文件，必须和f_open函数成对出现
// 	GPIO1->DATA &= ~(1<<10);
    f_mount(0,0);		 // 卸载文件系统
	
    return FR_OK;
}

/************************************************/
/* 函数名称：文件名和文件夹名扫描               */
/* 入口参数：*path：路径                        */
/* 出口参数：文件个数                           */
/* 说    明：支持长文件名
             除了的到总文件个数之外，还会把每一
			 个文件名和文件的类型存储在下面所示
			 的FileN和flag中，最多存储50个文件名
			 和50个文件属性，如果想增加，可以在
			 LPC1114内存允许之下修改。          */
/************************************************/
char FileN[50][50];	// 文件名存储
char type[8][20]={"bmp","txt","exe","pdf","doc","xls","zip","rar"};	// 文件类型定义
char flag[50]; // 文件类型存储标记 

BYTE FileScan(BYTE *path)
{
	FATFS fs;      // 建立一个文件系统         
    FRESULT res;   // 存储函数执行结果
    FILINFO finfo;  // 存储文件状态信息
    DIR dir;	   // 暂存路径
    TCHAR *fn;	   // 暂存文件名
	INT fileNum = 0;  // 初始化文件个数
	INT size;   // 文件名长度
#if _USE_LFN
  	static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
  	finfo.lfname=lfn;
  	finfo.lfsize=sizeof(lfn);
#endif
    f_mount(0, &fs);    // 加载文件系统
    res = f_opendir(&dir, (const TCHAR *)path);    // 打开路径目录
    if (res == FR_OK)	// 路径打开成功
    {
    	while (f_readdir(&dir, &finfo) == FR_OK)  //循环读取目录，直到读完 
    	{ 			
			#if _USE_LFN   // 长文件名
			  fn = *finfo.lfname ? finfo.lfname : finfo.fname;
			#else          // 短文件名
			  fn = finfo.fname;
			#endif			
			if((finfo.fattrib & AM_DIR)||(finfo.fattrib & AM_ARC))  //如果是文件夹(AM_DIR)、或是文件(AM_ARC) 
			{ 
				if(!fn[0])break;	//如果读完，跳出循环			    
				size=strlen(fn);	//获得文件名长度
				if(finfo.fattrib & AM_DIR)flag[fileNum]=0;	//判断如果是文件夹，给个标记				  	
				if(finfo.fattrib & AM_ARC)	 //判断如果是文件
				{
					if(strcasecmp(&fn[size-3],type[0])==0)flag[fileNum]=1;//再次判断如果是bmp图片																			 //当前文件标志问置1
					else if(strcasecmp(&fn[size-3],type[1])==0)flag[fileNum]=2;//如果是文本文档
					else if(strcasecmp(&fn[size-3],type[2])==0)flag[fileNum]=3;//如果是exe文件
					else if(strcasecmp(&fn[size-3],type[3])==0)flag[fileNum]=4;//如果是pdf文件
					else if(strcasecmp(&fn[size-3],type[4])==0)flag[fileNum]=5;//如果是word文本文档
					else if(strcasecmp(&fn[size-3],type[5])==0)flag[fileNum]=6;//如果是execl文件
					else if(strcasecmp(&fn[size-3],type[6])==0)flag[fileNum]=7;//如果是zip文件
					else if(strcasecmp(&fn[size-3],type[7])==0)flag[fileNum]=7;//如果是rar文件
					else flag[fileNum] = 88;																				 //当前文件标志问置2
				}  	   
				strcpy(FileN[fileNum],fn);	//文件名复制给FileN数组保存
				fileNum++;    //查找下一个，fileNum为总文件数
			}			 
    	}  
    }

    f_mount(0, 0);	 // 卸载文件系统
    return fileNum;	 // 返回文件个数
}



/**************************************************/
/* 函数功能：获取BMP图片的头文件信息              */
/* 入口参数：buf：暂存缓存                        */
/**************************************************/
BMP_HEADER TFTBmpGetHeadInfo(uint8_t *buf)
{
	BMP_HEADER bmpHead;

	bmpHead.bfType = (buf[0] << 8) + buf[1];		   // BM
	bmpHead.bfSize	= (buf[5]<<24) + (buf[4]<<16) + (buf[3]<<8) + buf[2];     // 文件大小
	bmpHead.biWidth = (buf[21]<<24) + (buf[20]<<16) + (buf[19]<<8) + buf[18]; // 图像宽度
	bmpHead.biHeight = (buf[25]<<24) + (buf[24]<<16) + (buf[23]<<8) + buf[22];// 图像高度
	bmpHead.biBitCount = (buf[29] << 8) + buf[28];	   // 每个像素的位数，单色位图为１，256色为8，16bit为16， 24bit为24

	return bmpHead;
}
/**************************************************/
/* 函数功能：显示BMP图片                          */
/* 入口参数：x,y: 坐标                            */
/*           *bmpName:bmp图片的名称，可以带路径   */
/**************************************************/
uint8_t TFTBmpDisplay(uint8_t *bmpName,uint16_t x,uint16_t y)
{
    FATFS fs;         // 建立文件系统
    FIL file;         // 建立文件
	UINT  br;         // 字节计数器
	FRESULT res;      // 返回值信息
	BMP_HEADER bmpHead;	    // 头信息
	uint16_t i;

    f_mount(0, &fs);  // 挂载文件系统
	res = f_open(&file, (const TCHAR *)bmpName, FA_OPEN_EXISTING|FA_READ); // 打开BMP文件并读取到file中
	if(res != FR_OK)
    {
		return res;
    }
	else
	{
	    res = f_read(&file, Buffer, 54, &br);	 // 读取头文件信息
		if(res != FR_OK)
	    {
			return res;	   // 返回错误表示
	    }
		else
		{
		    bmpHead = TFTBmpGetHeadInfo(Buffer); // 获取头信息

			if (bmpHead.bfType == 0x424D) // 判断是否为BMP图像
			{
				LCD_WR_REG_DATA(0x0003, 0x1010); // 由下而上显示
				LCD_XYRAM(x, y, x+bmpHead.biWidth-1, y+bmpHead.biHeight-1);
				LCD_WR_REG_DATA(0x0020,x);//设置X坐标位置
			    LCD_WR_REG_DATA(0x0021,y+bmpHead.biHeight-1);//设置Y坐标位置(注意：在由下而上显示的时候，这里y坐标应该是最下边的值)
			    LCD_WR_REG(0x0022);	//指向RAM寄存器，准备写数据到RAM
			    while(1)
				{
		            res = f_read(&file, Buffer, 240, &br);  //读取240个数据
					if(res||br==0)   																					//错误跳出
	                break;
		          	for(i=0;i<80;i++)
					{	
						// 在TFT上显示一个像素点的颜色			
						LCD_WR_DATA(((Buffer[i*3+2]/8)<<11 | (Buffer[i*3+1]/4)<<5 |(Buffer[i*3]/8)));
					}
				}
				LCD_WR_REG_DATA(0x0003, 0x1030); // 恢复正常显示
				LCD_XYRAM(0, 0, 239, 319);       // 恢复GRAM
			}
		}
	}

   	f_close(&file);      // 关闭文件，必须和f_open函数成对出现
    f_mount(0, 0);  	 // 卸载文件系统

	return FR_OK;	   // 返回成功标志
}

// /*********************************************/
// /* 函数功能：显示文件名（中文和英文）        */
// /* 入口参数：x,y: 坐标                       */
// /*           *p:字符串                       */
// /*********************************************/
// void FileNameShow(uint16_t x,uint16_t y,uint8_t *p)
// {         
//     while(*p!='\0')	// 如果没有结束
//     {       
//         if(*p>0x80)//如果是中文
// 		{
// 			LCD_Show_hz(x, y, p);
// 			x+=16;
// 			if(x>224)return;
// 			p+=2;
// 		}
// 		else	   //如果是英文
// 		{
// 			LCD_ShowChar(x,y,*p);
// 			if(x>224)return;
// 			x+=8;
// 			p++;
// 		}
//     }  
// }
