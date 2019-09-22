#include "time.h"
#include "led.h"
#include "sdadc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t CCR3_Val = 6000;
__IO uint16_t CCR4_Val = 4660;
__IO uint16_t temp1;
__IO uint16_t temp2;
uint16_t capture = 0;
uint16_t PrescalerValue = 0;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

extern u32 write_buf[1024];
extern u32 write_buf2[1024];
extern u8  writebuf1;
int w_index=0;
__IO float InputVoltageMv = 0;
extern int16_t InjectedConvData;
extern int adc_start_flag;
extern u8 write_file;
void TIM_INT_Config(void)
{
   NVIC_InitTypeDef NVIC_InitStructure;

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  /*  TIM3 中断嵌套设计*/
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void TIM_OUT_Config(void)
{
	/* -----------------------------------------------------------------------
    TIM3 配置: 输出比较时序模式:
    
    本试验中 TIM3输入时钟(TIM3CLK) 被设为2*APB1 时钟 (HCLK),   
      TIM3CLK =HCLK       
      => TIM3CLK = HCLK = SystemCoreClock  = 72 MHz
          
    当 TIM3 计数时钟 设为6 MHz, 预分频器可以按照下面公式计算：
       Prescaler = (TIM3CLK / TIM3 counter clock) - 1
       Prescaler = (PCLK1 /6 MHz) - 1
                                                  
    CC3 翻转率 = TIM3 counter clock / CCR3_Val = 1kHz
    ==> Toggling frequency = 500 Hz
    
    CC4 翻转率= TIM3 counter clock / CCR4_Val = 2k Hz
    ==> Toggling frequency = 1k Hz
  ----------------------------------------------------------------------- */   

  /* 计算预分频值 */
  PrescalerValue = (uint16_t) (SystemCoreClock  / 6000000) - 1;

  /* Time 定时器基础设置 */
  TIM_TimeBaseStructure.TIM_Period = 65535;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  /* 预分频器配置 */
  TIM_PrescalerConfig(TIM4, PrescalerValue, TIM_PSCReloadMode_Immediate);

  /* 输出比较时序模式配置设置 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  /* 输出比较时序模式配置: 频道3*/
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Disable);

  /* 输出比较时序模式配置: 频道4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;
  TIM_OC4Init(TIM4, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Disable);
   
  /* TIM 中断使能 */
  TIM_ITConfig(TIM4, TIM_IT_CC3 | TIM_IT_CC4, ENABLE);

  /* TIM3 使能 */
  TIM_Cmd(TIM4, ENABLE);
	
	}

void TIM4_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET)
  {
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
     temp1++;
    /*  toggling with frequency = 500 Hz */

    capture = TIM_GetCapture3(TIM4);
    TIM_SetCompare3(TIM4, capture + CCR3_Val);
  }
  else
  {
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
//     temp2++;
		if(adc_start_flag)
		{
			if(writebuf1)
			{
				InputVoltageMv = (((InjectedConvData + 32768) * SDADC_VREF) / (SDADC_GAIN * SDADC_RESOL));
				write_buf[w_index]=InputVoltageMv;
				if(w_index++>=1000)
				{
					w_index=0;write_file=1;writebuf1=0;
				}
			}
			else
			{
				InputVoltageMv = (((InjectedConvData + 32768) * SDADC_VREF) / (SDADC_GAIN * SDADC_RESOL));
				write_buf2[w_index]=InputVoltageMv;
				if(w_index++>=1000)
				{
					w_index=0;write_file=1;writebuf1=1;
				}
			}
		}
    /* toggling with frequency = 1k Hz */
    capture = TIM_GetCapture4(TIM4);
    TIM_SetCompare4(TIM4, capture + CCR4_Val);
  }
}
