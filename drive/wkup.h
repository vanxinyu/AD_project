#ifndef __WKUP_H
#define __WKUP_H	 
#include "stm32f37x.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//�������� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
					    
#define WKUP_KD   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)

void WKUP_Init(void); 			//PA0 WKUP���ѳ�ʼ��
void Sys_Enter_Standby(void);	//ϵͳ�������ģʽ
#endif


