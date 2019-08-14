#include "my_malloc.h"
#include "BY_log.h"

float mem1_occupancy = 0.0;

__align(32) uint8_t mem1_base[MEM1_TOTAL_SIZE]; //�ڲ�SRAM�ڴ��, 32�ֽڶ���

uint16_t mem1_flag[MEM1_BLOCK_NUM]; //�ڲ�SRAM�ڴ�ع����

//*des:Ŀ�ĵ�ַ
//*src:Դ��ַ
//n:��Ҫ���Ƶ��ڴ泤��(�ֽ�Ϊ��λ)
void my_memcpy(void *des, void *src, uint32_t n)//�����ڴ�
{
	uint8_t *xdes = des;
	uint8_t *xsrc = src;

	while(n--)
	{
		*xdes++ = *xsrc++;
	}
}

//*s:�ڴ��׵�ַ
//c :Ҫ���õ�ֵ
//count:��Ҫ���õ��ڴ��С(�ֽ�Ϊ��λ)
void my_memset(void *s, uint8_t c, uint32_t count)//�����ڴ�
{
	uint8_t *xs = s;

	while(count--)
	{
		*xs++ = c;
	}
}

//size:Ҫ������ڴ��С(�ֽ�)
//����ֵ:0XFFFFFFFF,�������;����,�ڴ�ƫ�Ƶ�ַ
uint32_t my_mem_malloc(uint32_t size)//�ڴ����
{
	uint16_t index = 0;
	uint32_t offset = 0;

	uint16_t mem_b_need = 0;//��Ҫ���ڴ����
	uint16_t mem_b_ok = 0;//�������ڴ����

	uint16_t i = 0;

	if((size==0)||(size>MEM1_TOTAL_SIZE))
	{
		return MEM_ERROR_INVALID_SIZE;
	}

	mem_b_need = size/MEM1_BLOCK_SIZE;  	//��ȡ��Ҫ����������ڴ����
	if(size%MEM1_BLOCK_SIZE) mem_b_need++;

	for(index=0; index<MEM1_BLOCK_NUM; index++)//���������ڴ������
	{
		if(mem1_flag[index]==0) mem_b_ok++;//�������ڴ��������
		else                    mem_b_ok = 0;//�����ڴ������

		if(mem_b_ok==mem_b_need)//�ҵ�������mem_b_need�����ڴ��
		{
			offset = index+1-mem_b_need;
			for(i=0; i<mem_b_need; i++)//��ע�ڴ��ǿ�
			{
				mem1_flag[offset+i] = mem_b_need;
			}
			my_memset((void*)(offset*MEM1_BLOCK_SIZE+(uint32_t)mem1_base), 0x00, mem_b_need*MEM1_BLOCK_SIZE);// ensure that memory allocated by my_malloc is all 0x00.
			mem1_occupancy = my_mem_used();
			return (offset*MEM1_BLOCK_SIZE);//����ƫ�Ƶ�ַ
		}
	}
	return MEM_ERROR_NO_VALID_BLOCK;//δ�ҵ����Ϸ����������ڴ��
}

//offset:�ڴ��ַƫ��
void my_mem_free(uint32_t offset)//�ͷ��ڴ�
{
	uint16_t index = offset/MEM1_BLOCK_SIZE; //ƫ�������ڴ�����
	uint16_t nmemb = mem1_flag[index];	//�ڴ������
	uint32_t i = 0;

	for(i=0; i<nmemb; i++) //�ڴ������
	{
		mem1_flag[index+i] = 0;
	}

	mem1_occupancy = my_mem_used();
}

//memx:�����ڴ��
void my_mem_init(void)//�ڴ�����ʼ��
{
	my_memset(mem1_base, 0x00, MEM1_TOTAL_SIZE); //�ڴ��������������
	my_memset(mem1_flag, 0x00, MEM1_BLOCK_NUM*2);//�ڴ�״̬����������
}

//size:�ڴ��С(�ֽ�)
//����ֵ:���䵽���ڴ��׵�ַ
void *my_malloc(uint32_t size)//�����ڴ�
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

//*ptr:���ڴ��׵�ַ
//size:Ҫ������ڴ��С(�ֽ�)
//����ֵ:�·��䵽���ڴ��׵�ַ.
void *my_realloc(void *ptr, uint32_t size)//���·����ڴ�
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
		index = offset/MEM1_BLOCK_SIZE; //ƫ�������ڴ�����
		mem_b_old = mem1_flag[index];	//�ڴ������

		mem_b_new = size/MEM1_BLOCK_SIZE;  	//��ȡ��Ҫ����������ڴ����
		if(size%MEM1_BLOCK_SIZE) mem_b_new++;

		if(mem_b_old<mem_b_new)
			my_memcpy(newptr, ptr, mem_b_old*MEM1_BLOCK_SIZE); //�������ڴ����ݵ����ڴ�
		else
			my_memcpy(newptr, ptr, size); //�������ڴ����ݵ����ڴ�

		my_free(ptr); //�ͷž��ڴ�
		return newptr; //�������ڴ��׵�ַ
	}
}

//ptr:�ڴ��׵�ַ
void my_free(void *ptr)//�ͷ��ڴ�
{
	uint32_t offset = 0;

	if(((uint32_t)ptr>=(uint32_t)mem1_base)&&((uint32_t)ptr<=((uint32_t)mem1_base+MEM1_TOTAL_SIZE-1)))//ָ��ָ���ڴ����
	{
		offset = (uint32_t)ptr-(uint32_t)mem1_base;
		my_mem_free(offset);	//�ͷ��ڴ�
	}
	else
	{
		log_error("my_free error, wrong pointer address !!!");
	}
}

//����ֵ:ʹ����(0~100)
float my_mem_used(void)//��ȡ�ڴ�ʹ����
{
	uint16_t used = 0;
	uint16_t i = 0;

	for(i=0; i<MEM1_BLOCK_NUM; i++)
	{
		if(mem1_flag[i]) used++;
	}
	return (float)((used*100.0)/MEM1_BLOCK_NUM);
}
