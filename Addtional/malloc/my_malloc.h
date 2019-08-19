#ifndef __MY_MALLOC_H
#define __MY_MALLOC_H
#include "cmsis_os.h"

/*
	ע�⣺

	1�����ڴ���в�ȡ���Զ����¡����ڴ���䷽ʽ
	2��Ŀǰ�ɹ�����ڴ������Ϊ uin16_t �������65535��
	3��my_malloc���뵽�ڴ�֮��Ӧ�����뵽���ڴ��ֶ�ȫ����0����ʵ�ӱ��ڴ���ڷ��䵽���ڴ涼��ȫ����0��
	4��my_free�ͷ����ڴ�֮��Ӧ��ԭָ��ָ��NULL
*/

#ifndef NULL
#define NULL 0
#endif

// internal result of my_free
#define MEM_SUCCESS_FREE         0x00
#define MEM_ERROR_WRONG_OFFSET   0x01
#define MEM_ERROR_EMPTY_POINTER  0x02

// internal result of my_malloc
#define MEM_ERROR_INVALID_SIZE   0XFFFFFFFE
#define MEM_ERROR_NO_VALID_BLOCK 0XFFFFFFFF

// �ڴ�������
#define MEM1_TOTAL_SIZE  (16*1024)                         //�ڴ�ش�С��Byte
#define MEM1_BLOCK_SIZE  (32)                              //�ڴ���С��Byte
#define MEM1_BLOCK_NUM   (MEM1_TOTAL_SIZE/MEM1_BLOCK_SIZE) //�ڴ������

extern float mem1_occupancy;

// �û����ú���
void my_memcpy(void *des, void *src, uint32_t n);//�����ڴ�
void my_memset(void *s, uint8_t c, uint32_t count);//�����ڴ�

void my_mem_init(void);                     //�ڴ�����ʼ������
void *my_malloc(uint32_t size);             //�ڴ����
void *my_realloc(void *ptr, uint32_t size); //���·����ڴ�
void my_free(void *ptr);                 //�ڴ��ͷ�
float my_mem_used(void);                    //����ڴ�ʹ����

#endif
