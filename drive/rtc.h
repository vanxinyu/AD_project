#ifndef __RTC_H
#define __RTC_H	 
#include "stm32f37x.h"
 //////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//RTC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/5
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//********************************************************************************
//�޸�˵��
//V1.1 20140726
//����:RTC_Get_Week����,���ڸ�����������Ϣ,�õ�������Ϣ.
////////////////////////////////////////////////////////////////////////////////// 

uint8_t My_RTC_Init(void);						//RTC��ʼ��
ErrorStatus RTC_Set_Time(uint8_t hour,uint8_t min,uint8_t sec,uint8_t ampm);			//RTCʱ������
ErrorStatus RTC_Set_Date(uint8_t year,uint8_t month,uint8_t date,uint8_t week); 		//RTC��������
void RTC_Set_AlarmA(uint8_t week,uint8_t hour,uint8_t min,uint8_t sec);		//��������ʱ��(����������,24Сʱ��)
void RTC_Set_AlarmB(u8 week,u8 hour,u8 min,u8 sec);
void RTC_Set_WakeUp(u32 wksel,u16 cnt);					//�����Ի��Ѷ�ʱ������
void camera_new_pathname(u8 *pname,RTC_TimeTypeDef RTC_TimeStruct,RTC_DateTypeDef RTC_DateStruct);
#endif

















