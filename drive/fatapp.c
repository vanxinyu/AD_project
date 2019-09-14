#include "fatapp.h"
#include "stdio.h"						 //printf�������ļ�
#include "string.h"						 //�ַ������ļ�
#include "ili9328.h"

BYTE Buffer[512];


/********************************************************/
/* �������ܣ�д�ļ�                                     */
/* ��ڲ�����fileName:Ҫд����ļ�����Ҫ�������ļ���    */
/*           buffer: Ҫд������ݴ������               */
/* ���ڲ�����0���ɹ�    1��ʧ��                         */
/********************************************************/
FRESULT FileWrite(const TCHAR *fileName,const uint8_t *buffer)
{
	FATFS fs;           // �ļ�ϵͳ
    FIL file;           // �ļ�
	UINT  bw;           // �����ֽ���

    f_mount(0, &fs);	// �����ļ�ϵͳ
	// ���ļ���д�����ݣ����û�и��ļ�������һ����Ϊ*fileName���ļ�����д������
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
		} while (bw < 512);  // �ж��Ƿ����(bw = 512����ʾд�����)
   	 	f_close(&file);      // �ر��ļ��������f_open�����ɶԳ���
	}
    f_mount(0, 0);	  //ж���ļ�ϵͳ

    return FR_OK;
}

/********************************************************/
/* �������ܣ����ļ�                                     */
/* ��ڲ�����fileName:Ҫ��ȡ���ļ���                    */
/*           buf: Ҫ��ȡ�����ݴ������                  */
/* ���ڲ�����0���ɹ�    1��ʧ��                         */
/********************************************************/
FRESULT FileRead(const TCHAR *fileName, uint8_t *buf)
{
	FATFS fs;              // ����һ���ļ�ϵͳ
    FIL file;              // �ݴ��ļ�
	UINT  br;              // �ֽڼ�����
	FRESULT res;		   // �洢����ִ�н��

    f_mount(0,&fs);	   // �����ļ�ϵͳ

	res = f_open(&file, fileName, FA_OPEN_EXISTING|FA_READ); // ���ļ�
	
    if(res != FR_OK)  // ���û����ȷ���ļ�
    {
		return res;	  // ���ش��󱨸�
    }
	else			  // ��������ļ�
	{
		do
		{
		    f_read(&file, buf, 512, &br);// ��ȡ�ļ����ݣ�ÿ��512���ֽ�
		}while(br); // br = 0 ��ʾ����������
	}
	
   	f_close(&file);      // �ر��ļ��������f_open�����ɶԳ���
// 	GPIO1->DATA &= ~(1<<10);
    f_mount(0,0);		 // ж���ļ�ϵͳ
	
    return FR_OK;
}

/************************************************/
/* �������ƣ��ļ������ļ�����ɨ��               */
/* ��ڲ�����*path��·��                        */
/* ���ڲ������ļ�����                           */
/* ˵    ����֧�ֳ��ļ���
             ���˵ĵ����ļ�����֮�⣬�����ÿһ
			 ���ļ������ļ������ʹ洢��������ʾ
			 ��FileN��flag�У����洢50���ļ���
			 ��50���ļ����ԣ���������ӣ�������
			 LPC1114�ڴ�����֮���޸ġ�          */
/************************************************/
char FileN[50][50];	// �ļ����洢
char type[8][20]={"bmp","txt","exe","pdf","doc","xls","zip","rar"};	// �ļ����Ͷ���
char flag[50]; // �ļ����ʹ洢��� 

BYTE FileScan(BYTE *path)
{
	FATFS fs;      // ����һ���ļ�ϵͳ         
    FRESULT res;   // �洢����ִ�н��
    FILINFO finfo;  // �洢�ļ�״̬��Ϣ
    DIR dir;	   // �ݴ�·��
    TCHAR *fn;	   // �ݴ��ļ���
	INT fileNum = 0;  // ��ʼ���ļ�����
	INT size;   // �ļ�������
#if _USE_LFN
  	static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
  	finfo.lfname=lfn;
  	finfo.lfsize=sizeof(lfn);
#endif
    f_mount(0, &fs);    // �����ļ�ϵͳ
    res = f_opendir(&dir, (const TCHAR *)path);    // ��·��Ŀ¼
    if (res == FR_OK)	// ·���򿪳ɹ�
    {
    	while (f_readdir(&dir, &finfo) == FR_OK)  //ѭ����ȡĿ¼��ֱ������ 
    	{ 			
			#if _USE_LFN   // ���ļ���
			  fn = *finfo.lfname ? finfo.lfname : finfo.fname;
			#else          // ���ļ���
			  fn = finfo.fname;
			#endif			
			if((finfo.fattrib & AM_DIR)||(finfo.fattrib & AM_ARC))  //������ļ���(AM_DIR)�������ļ�(AM_ARC) 
			{ 
				if(!fn[0])break;	//������꣬����ѭ��			    
				size=strlen(fn);	//����ļ�������
				if(finfo.fattrib & AM_DIR)flag[fileNum]=0;	//�ж�������ļ��У��������				  	
				if(finfo.fattrib & AM_ARC)	 //�ж�������ļ�
				{
					if(strcasecmp(&fn[size-3],type[0])==0)flag[fileNum]=1;//�ٴ��ж������bmpͼƬ																			 //��ǰ�ļ���־����1
					else if(strcasecmp(&fn[size-3],type[1])==0)flag[fileNum]=2;//������ı��ĵ�
					else if(strcasecmp(&fn[size-3],type[2])==0)flag[fileNum]=3;//�����exe�ļ�
					else if(strcasecmp(&fn[size-3],type[3])==0)flag[fileNum]=4;//�����pdf�ļ�
					else if(strcasecmp(&fn[size-3],type[4])==0)flag[fileNum]=5;//�����word�ı��ĵ�
					else if(strcasecmp(&fn[size-3],type[5])==0)flag[fileNum]=6;//�����execl�ļ�
					else if(strcasecmp(&fn[size-3],type[6])==0)flag[fileNum]=7;//�����zip�ļ�
					else if(strcasecmp(&fn[size-3],type[7])==0)flag[fileNum]=7;//�����rar�ļ�
					else flag[fileNum] = 88;																				 //��ǰ�ļ���־����2
				}  	   
				strcpy(FileN[fileNum],fn);	//�ļ������Ƹ�FileN���鱣��
				fileNum++;    //������һ����fileNumΪ���ļ���
			}			 
    	}  
    }

    f_mount(0, 0);	 // ж���ļ�ϵͳ
    return fileNum;	 // �����ļ�����
}



/**************************************************/
/* �������ܣ���ȡBMPͼƬ��ͷ�ļ���Ϣ              */
/* ��ڲ�����buf���ݴ滺��                        */
/**************************************************/
BMP_HEADER TFTBmpGetHeadInfo(uint8_t *buf)
{
	BMP_HEADER bmpHead;

	bmpHead.bfType = (buf[0] << 8) + buf[1];		   // BM
	bmpHead.bfSize	= (buf[5]<<24) + (buf[4]<<16) + (buf[3]<<8) + buf[2];     // �ļ���С
	bmpHead.biWidth = (buf[21]<<24) + (buf[20]<<16) + (buf[19]<<8) + buf[18]; // ͼ����
	bmpHead.biHeight = (buf[25]<<24) + (buf[24]<<16) + (buf[23]<<8) + buf[22];// ͼ��߶�
	bmpHead.biBitCount = (buf[29] << 8) + buf[28];	   // ÿ�����ص�λ������ɫλͼΪ����256ɫΪ8��16bitΪ16�� 24bitΪ24

	return bmpHead;
}
/**************************************************/
/* �������ܣ���ʾBMPͼƬ                          */
/* ��ڲ�����x,y: ����                            */
/*           *bmpName:bmpͼƬ�����ƣ����Դ�·��   */
/**************************************************/
uint8_t TFTBmpDisplay(uint8_t *bmpName,uint16_t x,uint16_t y)
{
    FATFS fs;         // �����ļ�ϵͳ
    FIL file;         // �����ļ�
	UINT  br;         // �ֽڼ�����
	FRESULT res;      // ����ֵ��Ϣ
	BMP_HEADER bmpHead;	    // ͷ��Ϣ
	uint16_t i;

    f_mount(0, &fs);  // �����ļ�ϵͳ
	res = f_open(&file, (const TCHAR *)bmpName, FA_OPEN_EXISTING|FA_READ); // ��BMP�ļ�����ȡ��file��
	if(res != FR_OK)
    {
		return res;
    }
	else
	{
	    res = f_read(&file, Buffer, 54, &br);	 // ��ȡͷ�ļ���Ϣ
		if(res != FR_OK)
	    {
			return res;	   // ���ش����ʾ
	    }
		else
		{
		    bmpHead = TFTBmpGetHeadInfo(Buffer); // ��ȡͷ��Ϣ

			if (bmpHead.bfType == 0x424D) // �ж��Ƿ�ΪBMPͼ��
			{
				LCD_WR_REG_DATA(0x0003, 0x1010); // ���¶�����ʾ
				LCD_XYRAM(x, y, x+bmpHead.biWidth-1, y+bmpHead.biHeight-1);
				LCD_WR_REG_DATA(0x0020,x);//����X����λ��
			    LCD_WR_REG_DATA(0x0021,y+bmpHead.biHeight-1);//����Y����λ��(ע�⣺�����¶�����ʾ��ʱ������y����Ӧ�������±ߵ�ֵ)
			    LCD_WR_REG(0x0022);	//ָ��RAM�Ĵ�����׼��д���ݵ�RAM
			    while(1)
				{
		            res = f_read(&file, Buffer, 240, &br);  //��ȡ240������
					if(res||br==0)   																					//��������
	                break;
		          	for(i=0;i<80;i++)
					{	
						// ��TFT����ʾһ�����ص����ɫ			
						LCD_WR_DATA(((Buffer[i*3+2]/8)<<11 | (Buffer[i*3+1]/4)<<5 |(Buffer[i*3]/8)));
					}
				}
				LCD_WR_REG_DATA(0x0003, 0x1030); // �ָ�������ʾ
				LCD_XYRAM(0, 0, 239, 319);       // �ָ�GRAM
			}
		}
	}

   	f_close(&file);      // �ر��ļ��������f_open�����ɶԳ���
    f_mount(0, 0);  	 // ж���ļ�ϵͳ

	return FR_OK;	   // ���سɹ���־
}

// /*********************************************/
// /* �������ܣ���ʾ�ļ��������ĺ�Ӣ�ģ�        */
// /* ��ڲ�����x,y: ����                       */
// /*           *p:�ַ���                       */
// /*********************************************/
// void FileNameShow(uint16_t x,uint16_t y,uint8_t *p)
// {         
//     while(*p!='\0')	// ���û�н���
//     {       
//         if(*p>0x80)//���������
// 		{
// 			LCD_Show_hz(x, y, p);
// 			x+=16;
// 			if(x>224)return;
// 			p+=2;
// 		}
// 		else	   //�����Ӣ��
// 		{
// 			LCD_ShowChar(x,y,*p);
// 			if(x>224)return;
// 			x+=8;
// 			p++;
// 		}
//     }  
// }
