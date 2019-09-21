#ifndef __RTC_H
#define __RTC_H	 
#include "stm32f37x.h"
 //////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//RTC 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/5
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
//********************************************************************************
//修改说明
//V1.1 20140726
//新增:RTC_Get_Week函数,用于根据年月日信息,得到星期信息.
////////////////////////////////////////////////////////////////////////////////// 

uint8_t My_RTC_Init(void);						//RTC初始化
ErrorStatus RTC_Set_Time(uint8_t hour,uint8_t min,uint8_t sec,uint8_t ampm);			//RTC时间设置
ErrorStatus RTC_Set_Date(uint8_t year,uint8_t month,uint8_t date,uint8_t week); 		//RTC日期设置
void RTC_Set_AlarmA(uint8_t week,uint8_t hour,uint8_t min,uint8_t sec);		//设置闹钟时间(按星期闹铃,24小时制)
void RTC_Set_AlarmB(u8 week,u8 hour,u8 min,u8 sec);
void RTC_Set_WakeUp(u32 wksel,u16 cnt);					//周期性唤醒定时器设置
void camera_new_pathname(u8 *pname,RTC_TimeTypeDef RTC_TimeStruct,RTC_DateTypeDef RTC_DateStruct);
#endif

















