#ifndef __MY_MALLOC_H
#define __MY_MALLOC_H
#include "cmsis_os.h"

/*
	注意：

	1、在内存池中采取“自顶向下”的内存分配方式
	2、目前可管理的内存块数量为 uin16_t 个，最多65535个
	3、my_malloc申请到内存之后，应将申请到的内存手动全部清0（其实从本内存池内分配到的内存都已全被清0）
	4、my_free释放了内存之后，应将原指针指向NULL
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

// 内存管理参数
#define MEM1_TOTAL_SIZE  (16*1024)                         //内存池大小，Byte
#define MEM1_BLOCK_SIZE  (32)                              //内存块大小，Byte
#define MEM1_BLOCK_NUM   (MEM1_TOTAL_SIZE/MEM1_BLOCK_SIZE) //内存块数量

extern float mem1_occupancy;

// 用户调用函数
void my_memcpy(void *des, void *src, uint32_t n);//复制内存
void my_memset(void *s, uint8_t c, uint32_t count);//设置内存

void my_mem_init(void);                     //内存管理初始化函数
void *my_malloc(uint32_t size);             //内存分配
void *my_realloc(void *ptr, uint32_t size); //重新分配内存
void my_free(void *ptr);                 //内存释放
float my_mem_used(void);                    //获得内存使用率

#endif
