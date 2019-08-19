#include "string.h"
#include "exfuns.h"
#include "my_malloc.h"

extern char USERPath[4];

 //�ļ������б�
const uint8_t *FILE_TYPE_TBL[6][13]=
{
{"BIN"},			//BIN�ļ�
{"LRC"},			//LRC�ļ�
{"NES"},			//NES�ļ�
{"TXT","C","H"},	//�ı��ļ�
{"MP1","MP2","MP3","MP4","M4A","3GP","3G2","OGG","ACC","WMA","WAV","MID","FLAC"},//�����ļ�
{"BMP","JPG","JPEG","GIF"},//ͼƬ�ļ�
};
///////////////////////////////�����ļ���,ʹ��malloc��ʱ��////////////////////////////////////////////
FATFS *fs[1];  		//�߼����̹�����.	 
FIL *file;	  		//�ļ�1
FIL *ftemp;	  		//�ļ�2.
UINT br,bw;			//��д����
FILINFO fileinfo;	//�ļ���Ϣ
DIR dir;  			//Ŀ¼

uint8_t *fatbuf;			//SD�����ݻ�����
///////////////////////////////////////////////////////////////////////////////////////
//Ϊexfuns�����ڴ�
//����ֵ:0,�ɹ�
//1,ʧ��
uint8_t exfuns_init(void)
{
	fs[0]=(FATFS*)my_malloc(sizeof(FATFS));	//Ϊ����0�����������ڴ�	
	file=(FIL*)my_malloc(sizeof(FIL));		//Ϊfile�����ڴ�
	ftemp=(FIL*)my_malloc(sizeof(FIL));		//Ϊftemp�����ڴ�
	fatbuf=(uint8_t*)my_malloc(512);				//Ϊfatbuf�����ڴ�
	if(fs[0]&&file&&ftemp&&fatbuf)return 0;  //������һ��ʧ��,��ʧ��.
	else return 1;	
}

//��Сд��ĸתΪ��д��ĸ,���������,�򱣳ֲ���.
uint8_t char_upper(uint8_t c)
{
	if(c<'A')return c;//����,���ֲ���.
	if(c>='a')return c-0x20;//��Ϊ��д.
	else return c;//��д,���ֲ���
}	      
//�����ļ�������
//fname:�ļ���
//����ֵ:0XFF,��ʾ�޷�ʶ����ļ����ͱ��.
//		 ����,����λ��ʾ��������,����λ��ʾ����С��.
uint8_t f_typetell(uint8_t *fname)
{
	uint8_t tbuf[5];
	uint8_t *attr='\0';//��׺��
	uint8_t i=0,j;
	while(i<250)
	{
		i++;
		if(*fname=='\0')break;//ƫ�Ƶ��������.
		fname++;
	}
	if(i==250)return 0XFF;//������ַ���.
 	for(i=0;i<5;i++)//�õ���׺��
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
 	for(i=0;i<4;i++)tbuf[i]=char_upper(tbuf[i]);//ȫ����Ϊ��д 
	for(i=0;i<6;i++)
	{
		for(j=0;j<13;j++)
		{
			if(*FILE_TYPE_TBL[i][j]==0)break;//�����Ѿ�û�пɶԱȵĳ�Ա��.
			if(strcmp((const char *)FILE_TYPE_TBL[i][j],(const char *)tbuf)==0)//�ҵ���
			{
				return (i<<4)|j;
			}
		}
	}
	return 0XFF;//û�ҵ�		 			   
}	 

//�õ�����ʣ������
//drv:���̱��("0:"/"1:")
//total:������	 ����λKB��
//free:ʣ������	 ����λKB��
//����ֵ:0,����.����,�������
uint8_t exf_getfree(uint8_t *drv,uint32_t *total,uint32_t *free)
{
	FATFS *fs1;
	uint8_t res;
    DWORD fre_clust=0, fre_sect=0, tot_sect=0;
    //�õ�������Ϣ�����д�����
    res = f_getfree((const TCHAR*)drv, &fre_clust, &fs1);
    if(res==0)
	{											   
	    tot_sect=(fs1->n_fatent-2)*fs1->csize;	//�õ���������
	    fre_sect=fre_clust*fs1->csize;			//�õ�����������	   
#if _MAX_SS!=512				  				//������С����512�ֽ�,��ת��Ϊ512�ֽ�
		tot_sect*=fs1->ssize/512;
		fre_sect*=fs1->ssize/512;
#endif	  
		*total=tot_sect>>1;	//��λΪKB
		*free=fre_sect>>1;	//��λΪKB 
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




















