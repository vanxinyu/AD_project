#include "my_malloc.h"
#include "BY_log.h"

float mem1_occupancy = 0.0;

__align(32) uint8_t mem1_base[MEM1_TOTAL_SIZE]; //内部SRAM内存池, 32字节对齐

uint16_t mem1_flag[MEM1_BLOCK_NUM]; //内部SRAM内存池管理表

//*des:目的地址
//*src:源地址
//n:需要复制的内存长度(字节为单位)
void my_memcpy(void *des, void *src, uint32_t n)//复制内存
{
	uint8_t *xdes = des;
	uint8_t *xsrc = src;

	while(n--)
	{
		*xdes++ = *xsrc++;
	}
}

//*s:内存首地址
//c :要设置的值
//count:需要设置的内存大小(字节为单位)
void my_memset(void *s, uint8_t c, uint32_t count)//设置内存
{
	uint8_t *xs = s;

	while(count--)
	{
		*xs++ = c;
	}
}

//size:要分配的内存大小(字节)
//返回值:0XFFFFFFFF,代表错误;其他,内存偏移地址
uint32_t my_mem_malloc(uint32_t size)//内存分配
{
	uint16_t index = 0;
	uint32_t offset = 0;

	uint16_t mem_b_need = 0;//需要的内存块数
	uint16_t mem_b_ok = 0;//连续空内存块数

	uint16_t i = 0;

	if((size==0)||(size>MEM1_TOTAL_SIZE))
	{
		return MEM_ERROR_INVALID_SIZE;
	}

	mem_b_need = size/MEM1_BLOCK_SIZE;  	//获取需要分配的连续内存块数
	if(size%MEM1_BLOCK_SIZE) mem_b_need++;

	for(index=0; index<MEM1_BLOCK_NUM; index++)//搜索整个内存控制区
	{
		if(mem1_flag[index]==0) mem_b_ok++;//连续空内存块数增加
		else                    mem_b_ok = 0;//连续内存块清零

		if(mem_b_ok==mem_b_need)//找到了连续mem_b_need个空内存块
		{
			offset = index+1-mem_b_need;
			for(i=0; i<mem_b_need; i++)//标注内存块非空
			{
				mem1_flag[offset+i] = mem_b_need;
			}
			my_memset((void*)(offset*MEM1_BLOCK_SIZE+(uint32_t)mem1_base), 0x00, mem_b_need*MEM1_BLOCK_SIZE);// ensure that memory allocated by my_malloc is all 0x00.
			mem1_occupancy = my_mem_used();
			return (offset*MEM1_BLOCK_SIZE);//返回偏移地址
		}
	}
	return MEM_ERROR_NO_VALID_BLOCK;//未找到符合分配条件的内存块
}

//offset:内存地址偏移
void my_mem_free(uint32_t offset)//释放内存
{
	uint16_t index = offset/MEM1_BLOCK_SIZE; //偏移所在内存块号码
	uint16_t nmemb = mem1_flag[index];	//内存块数量
	uint32_t i = 0;

	for(i=0; i<nmemb; i++) //内存块清零
	{
		mem1_flag[index+i] = 0;
	}

	mem1_occupancy = my_mem_used();
}

//memx:所属内存块
void my_mem_init(void)//内存管理初始化
{
	my_memset(mem1_base, 0x00, MEM1_TOTAL_SIZE); //内存池所有数据清零
	my_memset(mem1_flag, 0x00, MEM1_BLOCK_NUM*2);//内存状态表数据清零
}

//size:内存大小(字节)
//返回值:分配到的内存首地址
void *my_malloc(uint32_t size)//分配内存
{
	uint32_t offset = 0;

	offset = my_mem_malloc(size);

	if((offset==MEM_ERROR_INVALID_SIZE)||(offset==MEM_ERROR_NO_VALID_BLOCK))
	{
		log_error("my_malloc error, code is 0x%08X, mem used %6.2f%%", offset, my_mem_used());
		return NULL;
	}
	else
	{
		return (void*)((uint32_t)mem1_base+offset);
	}
}

//*ptr:旧内存首地址
//size:要分配的内存大小(字节)
//返回值:新分配到的内存首地址.
void *my_realloc(void *ptr, uint32_t size)//重新分配内存
{
	void *newptr = NULL;
	uint32_t offset = 0;
	uint16_t index = 0;
	uint16_t mem_b_old = 0;
	uint16_t mem_b_new = 0;

	newptr = my_malloc(size);

	if(newptr==NULL)
	{
		return NULL;
	}
	else
	{
		offset = (uint32_t)ptr-(uint32_t)mem1_base;
		index = offset/MEM1_BLOCK_SIZE; //偏移所在内存块号码
		mem_b_old = mem1_flag[index];	//内存块数量

		mem_b_new = size/MEM1_BLOCK_SIZE;  	//获取需要分配的连续内存块数
		if(size%MEM1_BLOCK_SIZE) mem_b_new++;

		if(mem_b_old<mem_b_new)
			my_memcpy(newptr, ptr, mem_b_old*MEM1_BLOCK_SIZE); //拷贝旧内存内容到新内存
		else
			my_memcpy(newptr, ptr, size); //拷贝旧内存内容到新内存

		my_free(ptr); //释放旧内存
		return newptr; //返回新内存首地址
	}
}

//ptr:内存首地址
void my_free(void *ptr)//释放内存
{
	uint32_t offset = 0;

	if(((uint32_t)ptr>=(uint32_t)mem1_base)&&((uint32_t)ptr<=((uint32_t)mem1_base+MEM1_TOTAL_SIZE-1)))//指针指向内存池内
	{
		offset = (uint32_t)ptr-(uint32_t)mem1_base;
		my_mem_free(offset);	//释放内存
	}
	else
	{
		log_error("my_free error, wrong pointer address !!!");
	}
}

//返回值:使用率(0~100)
float my_mem_used(void)//获取内存使用率
{
	uint16_t used = 0;
	uint16_t i = 0;

	for(i=0; i<MEM1_BLOCK_NUM; i++)
	{
		if(mem1_flag[i]) used++;
	}
	return (float)((used*100.0)/MEM1_BLOCK_NUM);
}
