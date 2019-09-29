#include "sdadc.h"
#include <stdio.h>
#include "led.h"
#include "led.h"
//#define ADC_DEBUG

extern u32 write_buf[1024];
extern u32 write_buf2[1024];
extern u8  writebuf1;
int w_index=0;
extern int adc_start_flag;
extern u8 write_file;

static void sensor_power_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
  GPIO_InitStruct.GPIO_Pin = PADC_PIN  ;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStruct.GPIO_Speed =GPIO_Speed_2MHz;
	
  GPIO_Init(PADC_PORT, &GPIO_InitStruct);
	GPIO_SetBits(PADC_PORT, PADC_PIN);		
}
static  void sensor_power_deinit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;	
	GPIO_ResetBits(PADC_PORT, PADC_PIN);
  GPIO_InitStruct.GPIO_Pin = PADC_PIN  ;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	
  GPIO_Init(PADC_PORT, &GPIO_InitStruct);	
}
static int16_t sdadc_init(void)
{
  SDADC_AINStructTypeDef  SDADC_AINStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  uint32_t SDADCTimeout = 0;

  /* SDADC����ʱ������ */
  RCC_APB2PeriphClockCmd(POT_SDADC_CLK, ENABLE);
	
  /* PWRʱ��ʹ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
  /* ʹ��ģ������*/
  PWR_SDADCAnalogCmd(POT_SDADC_PWR, ENABLE);

  /* ���� SDADC ��Ƶ: SDADC Ҫ���� @6MHz */
  /* ���ϵͳʱ�� 72MHz, SDADCӦ��12��Ƶ */
  RCC_SDADCCLKConfig(RCC_SDADCCLK_SYSCLK_Div12);

  /* GPIOʱ������ʹ�� */
  RCC_AHBPeriphClockCmd(POT_GPIO_CLK, ENABLE);

  /* SDADC io�˿�����5P (PE8,PE9) */
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = POT_GPIO_PIN;
  GPIO_Init(POT_GPIO_PORT, &GPIO_InitStructure);

  /* ѡ���ⲿ�ο��� The reference voltage selection is available
     only in SDADC1 and therefore to select the VREF for SDADC2/SDADC3, SDADC1
     clock must be already enabled */
  SDADC_VREFSelect(POT_SDADC_VREF);

  /* Insert delay equal to ~5 ms */
//  Delay_ms(5);
  
  /* ʹ�� POT_SDADC */
  SDADC_Cmd(POT_SDADC, ENABLE);

  /* �����ʼ��ģʽ  */
  SDADC_InitModeCmd(POT_SDADC, ENABLE);
	
  /*�ȴ�ֱ����ʼ����� wait for INITRDY flag to be set */	
  SDADCTimeout = SDADC_INIT_TIMEOUT;
  while((SDADC_GetFlagStatus(POT_SDADC, SDADC_FLAG_INITRDY) == RESET) && (--SDADCTimeout != 0));
  if(SDADCTimeout == 0)
  {
    /* INITRDY flag can not set */
		printf("adc init timeout!\r\n");		
    return -1;
  }

  /* ģ����������Analog Input configuration conf0: use single ended zero reference */
  SDADC_AINStructure.SDADC_InputMode = SDADC_InputMode_Diff;
  SDADC_AINStructure.SDADC_Gain = POT_SDADC_GAIN;
  SDADC_AINStructure.SDADC_CommonMode = SDADC_CommonMode_VDDA_2;
  SDADC_AINStructure.SDADC_Offset = 0;
  SDADC_AINInit(POT_SDADC, SDADC_Conf_0, &SDADC_AINStructure);

  /* select POT_SDADC channel 5 to use conf0 */
  SDADC_ChannelConfig(POT_SDADC, POT_SDADC_CHANNEL, SDADC_Conf_0);
	
  /* 
  SDADC_ChannelSelect(POT_SDADC, POT_SDADC_CHANNEL);
	SDADC_ContinuousModeCmd(POT_SDADC,ENABLE);
	SDADC_InjectedContinuousModeCmd(POT_SDADC,ENABLE);	
	SDADC_SoftwareStartConv(POT_SDADC);	
	*/
	
  /* select channel 5 */
  SDADC_InjectedChannelSelect(POT_SDADC, POT_SDADC_CHANNEL);
  /* Enable continuous mode */

	
  /* Exit initialization mode */
  SDADC_InitModeCmd(POT_SDADC, DISABLE);

  /* configure calibration to be performed on conf0 */
  SDADC_CalibrationSequenceConfig(POT_SDADC, SDADC_CalibrationSequence_1);
  /* start POT_SDADC Calibration */
  SDADC_StartCalibration(POT_SDADC);
  /* Set calibration timeout: 5.12 ms at 6 MHz in a single calibration sequence */
  SDADCTimeout = SDADC_CAL_TIMEOUT;
  /* �ȴ�ֱ��У�����wait for POT_SDADC Calibration process to end */
  while((SDADC_GetFlagStatus(POT_SDADC, SDADC_FLAG_EOCAL) == RESET) && (--SDADCTimeout != 0));
  
  if(SDADCTimeout == 0)
  {
    /* EOCAL flag can not set */
		printf("adc calibration timeout!\r\n");		
    return -2;
  }

  /* Start a software start conversion */	
  SDADC_SoftwareStartInjectedConv(POT_SDADC);

  return 0;
}
static void sdadc_deinit(void)
{
	SDADC_DeInit(POT_SDADC);

  RCC_APB2PeriphClockCmd(POT_SDADC_CLK, DISABLE);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, DISABLE);

  PWR_SDADCAnalogCmd(POT_SDADC_PWR, DISABLE);	
}
static void timer4_init(void)//��ʱ��7�жϳ�ʼ��
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//TIM7ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = 999; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 72; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision	= TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM_ClearFlag(TIM4, TIM_FLAG_Update);//����˶�ʱ����ʼ�������������жϵ�bug�������Ƿ�ʱ������������������������������������������������

	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM7�ж�,��������ж�

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00; //�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM4, ENABLE);	
}
void TIM4_IRQHandler(void)
{
	int16_t temp;
  if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		
		if(adc_start_flag)
		{
			if(writebuf1)
			{
				sensor_sample_one_time(&temp);
				write_buf[w_index]=temp;
//				w_index++;
				if(w_index++>=1000)
				{
					w_index=0;write_file=1;writebuf1=0;
				}
			}
			else
			{
				sensor_sample_one_time(&temp);
				write_buf2[w_index]=temp;
				if(w_index++>=1000)
				{
					w_index=0;write_file=1;writebuf1=1;
				}
			}
		}
		
		LED1_Toggle();
  }
}
//void TIM4_IRQHandler(void)
//{
//  if (TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET)
//  {
//    TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
//     temp1++;
//    /*  toggling with frequency = 500 Hz */

//    capture = TIM_GetCapture3(TIM4);
//    TIM_SetCompare3(TIM4, capture + CCR3_Val);
//  }
//  else
//  {
//    TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
////     temp2++;
//		if(adc_start_flag)
//		{
//			if(writebuf1)
//			{
//				InputVoltageMv = (((InjectedConvData + 32768) * SDADC_VREF) / (SDADC_GAIN * SDADC_RESOL))-SDADC_VREF/2;
//				write_buf[w_index]=InputVoltageMv;
//				if(w_index++>=1000)
//				{
//					w_index=0;write_file=1;writebuf1=0;
//				}
//			}
//			else
//			{
//				InputVoltageMv = (((InjectedConvData + 32768) * SDADC_VREF) / (SDADC_GAIN * SDADC_RESOL))-SDADC_VREF/2;
//				write_buf2[w_index]=InputVoltageMv;
//				if(w_index++>=1000)
//				{
//					w_index=0;write_file=1;writebuf1=1;
//				}
//			}
//		}
//    /* toggling with frequency = 1k Hz */
//    capture = TIM_GetCapture4(TIM4);
//    TIM_SetCompare4(TIM4, capture + CCR4_Val);
//  }
//}
static void sensor_sample_timer_init(void)
{
	timer4_init();
}
static void sensor_sample_timer_deinit(void)
{
	TIM_DeInit(TIM4);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, DISABLE);
}
int16_t sensor_sample_init(void)
{
	sensor_power_init();
	sensor_sample_timer_init();
	return sdadc_init();
}
void sensor_sample_deinit(void)
{
	sdadc_deinit();
	sensor_sample_timer_deinit();
	sensor_power_deinit();
}
int16_t sensor_sample_one_time(int16_t *value)
{
	  uint32_t SDADCTimeout = 0;
	  uint32_t channel = 0;
		int16_t sensor_value = 0;
		__IO float temp = 0;		
		// start conversion
		SDADC_SoftwareStartInjectedConv(POT_SDADC);
		//wait
	
		SDADCTimeout = SDADC_SAMPLE_TIMEOUT;
		while((SDADC_GetFlagStatus(POT_SDADC, SDADC_FLAG_JEOC) == RESET) && (--SDADCTimeout != 0));
		if(SDADCTimeout == 0)
		{
			printf("adc sample timeout!\r\n");		
			return -1;	
		}			
		//get value
    sensor_value = SDADC_GetInjectedConversionValue(SDADC1, &channel);	
		if(POT_SDADC_CHANNEL != channel)
		{
			printf("adc sample timeout!\r\n");		
			return -2;	
		}
		else
		{
			*value	=	sensor_value;			
			#ifdef ADC_DEBUG			
			printf("value:%x\r\n",(int)sensor_value);					
			temp = (((sensor_value + 32768) * SDADC_VREF) / (SDADC_GAIN * SDADC_RESOL))-SDADC_VREF/2;
			printf("voltage:%d\r\n",(int)temp);	
			#endif
			return 0;
		}		
}
