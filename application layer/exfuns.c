#include "string.h"
#include "exfuns.h"
#include "my_malloc.h"

extern char USERPath[4];

 //文件类型列表
const uint8_t *FILE_TYPE_TBL[6][13]=
{
{"BIN"},			//BIN文件
{"LRC"},			//LRC文件
{"NES"},			//NES文件
{"TXT","C","H"},	//文本文件
{"MP1","MP2","MP3","MP4","M4A","3GP","3G2","OGG","ACC","WMA","WAV","MID","FLAC"},//音乐文件
{"BMP","JPG","JPEG","GIF"},//图片文件
};
///////////////////////////////公共文件区,使用malloc的时候////////////////////////////////////////////
FATFS *fs[1];  		//逻辑磁盘工作区.	 
FIL *file;	  		//文件1
FIL *ftemp;	  		//文件2.
UINT br,bw;			//读写变量
FILINFO fileinfo;	//文件信息
DIR dir;  			//目录

uint8_t *fatbuf;			//SD卡数据缓存区
///////////////////////////////////////////////////////////////////////////////////////
//为exfuns申请内存
//返回值:0,成功
//1,失败
uint8_t exfuns_init(void)
{
	fs[0]=(FATFS*)my_malloc(sizeof(FATFS));	//为磁盘0工作区申请内存	
	file=(FIL*)my_malloc(sizeof(FIL));		//为file申请内存
	ftemp=(FIL*)my_malloc(sizeof(FIL));		//为ftemp申请内存
	fatbuf=(uint8_t*)my_malloc(512);				//为fatbuf申请内存
	if(fs[0]&&file&&ftemp&&fatbuf)return 0;  //申请有一个失败,即失败.
	else return 1;	
}

//将小写字母转为大写字母,如果是数字,则保持不变.
uint8_t char_upper(uint8_t c)
{
	if(c<'A')return c;//数字,保持不变.
	if(c>='a')return c-0x20;//变为大写.
	else return c;//大写,保持不变
}	      
//报告文件的类型
//fname:文件名
//返回值:0XFF,表示无法识别的文件类型编号.
//		 其他,高四位表示所属大类,低四位表示所属小类.
uint8_t f_typetell(uint8_t *fname)
{
	uint8_t tbuf[5];
	uint8_t *attr='\0';//后缀名
	uint8_t i=0,j;
	while(i<250)
	{
		i++;
		if(*fname=='\0')break;//偏移到了最后了.
		fname++;
	}
	if(i==250)return 0XFF;//错误的字符串.
 	for(i=0;i<5;i++)//得到后缀名
	{
		fname--;
		if(*fname=='.')
		{
			fname++;
			attr=fname;
			break;
		}
  	}
	strcpy((char *)tbuf,(const char*)attr);//copy
 	for(i=0;i<4;i++)tbuf[i]=char_upper(tbuf[i]);//全部变为大写 
	for(i=0;i<6;i++)
	{
		for(j=0;j<13;j++)
		{
			if(*FILE_TYPE_TBL[i][j]==0)break;//此组已经没有可对比的成员了.
			if(strcmp((const char *)FILE_TYPE_TBL[i][j],(const char *)tbuf)==0)//找到了
			{
				return (i<<4)|j;
			}
		}
	}
	return 0XFF;//没找到		 			   
}	 

//得到磁盘剩余容量
//drv:磁盘编号("0:"/"1:")
//total:总容量	 （单位KB）
//free:剩余容量	 （单位KB）
//返回值:0,正常.其他,错误代码
uint8_t exf_getfree(uint8_t *drv,uint32_t *total,uint32_t *free)
{
	FATFS *fs1;
	uint8_t res;
    DWORD fre_clust=0, fre_sect=0, tot_sect=0;
    //得到磁盘信息及空闲簇数量
    res = f_getfree((const TCHAR*)drv, &fre_clust, &fs1);
    if(res==0)
	{											   
	    tot_sect=(fs1->n_fatent-2)*fs1->csize;	//得到总扇区数
	    fre_sect=fre_clust*fs1->csize;			//得到空闲扇区数	   
#if _MAX_SS!=512				  				//扇区大小不是512字节,则转换为512字节
		tot_sect*=fs1->ssize/512;
		fre_sect*=fs1->ssize/512;
#endif	  
		*total=tot_sect>>1;	//单位为KB
		*free=fre_sect>>1;	//单位为KB 
 	}
	return res;
}
void mount_disk(void)
{
  uint8_t res = f_mount(fs[0], USERPath, 0);
  if (res != FR_OK)
  {
    printf("FAILED: %d\r\n",res);
  return;
 }
  printf("MOUNT OK\r\n");
}
 
void format_disk(void)
{
 uint8_t res = 0;

 printf("PROCESSING...\n");
 res = f_mkfs(USERPath, 1, 512);
 if (res == FR_OK)
 {
 printf("OK!\n");
 }
 else
 {
 printf("failed with: %d\n",res);
 }
}
 
void create_file(void)
{
  FIL file;
  FIL *pf = &file;
  uint8_t res;
	
  res = f_open(pf, "0:/test.txt", FA_OPEN_ALWAYS | FA_WRITE);
  if (res == FR_OK)
  {
    printf("creat ok\n"); 				
  }
  else
  {
    printf("creat failed\n");
    printf("error code: %d\n",res);	
  }
  
  f_printf(pf, "hello fatfs!\n");
  
  res = f_close(pf);
    if (res != FR_OK)
    {
      printf("close file error\n");
      printf("error code: %d\n",res);				
    }
}
 
void get_disk_info(void)
{
	FATFS fs;
	FATFS *fls = &fs;
	FRESULT res;
	DWORD fre_clust;	
 
	res = f_getfree("/",&fre_clust,&fls);         /* Get Number of Free Clusters */
	if (res == FR_OK) 
	{
	                                             /* Print free space in unit of MB (assuming 4096 bytes/sector) */
        printf("%d KB Total Drive Space.\n"
               "%d KB Available Space.\n",
               ((fls->n_fatent-2)*fls->csize)*4,(fre_clust*fls->csize)*4);
	}
	else
	{
		printf("get disk info error\n");
		printf("error code: %d\n",res);
	}																									   
}
 
void read_file(void)
{
  FIL file;
  FRESULT res;
  UINT bw;
  uint8_t rbuf[100] = {0};
  
  res = f_open(&file, "0:/test.txt", FA_READ);
  if (res != FR_OK)
  {
    printf("open error: %d\n",res);
    return;
  }
  f_read(&file, rbuf, 20, &bw);
  printf("%s\n", rbuf);
  
  res = f_close(&file);
  if (res != FR_OK)
  {
     printf("close file error\n");
     printf("error code: %d\n",res);				
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////




















