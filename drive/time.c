#include "time.h"
#include "led.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint16_t CCR3_Val = 13654;
__IO uint16_t CCR4_Val = 6826;
__IO uint16_t temp1;
__IO uint16_t temp2;
uint16_t capture = 0;
uint16_t PrescalerValue = 0;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;


void TIM_INT_Config(void)
{
   NVIC_InitTypeDef NVIC_InitStructure;

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /*  TIM3 �ж�Ƕ�����*/
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void TIM_OUT_Config(void)
{
	/* -----------------------------------------------------------------------
    TIM3 ����: ����Ƚ�ʱ��ģʽ:
    
    �������� TIM3����ʱ��(TIM3CLK) ����Ϊ2*APB1 ʱ�� (HCLK),   
      TIM3CLK =HCLK       
      => TIM3CLK = HCLK = SystemCoreClock  = 72 MHz
          
    �� TIM3 ����ʱ�� ��Ϊ6 MHz, Ԥ��Ƶ�����԰������湫ʽ���㣺
       Prescaler = (TIM3CLK / TIM3 counter clock) - 1
       Prescaler = (PCLK1 /6 MHz) - 1
                                                  
    CC3 ��ת�� = TIM3 counter clock / CCR3_Val = 439.4 Hz
    ==> Toggling frequency = 219.7 Hz
    
    CC4 ��ת��= TIM3 counter clock / CCR4_Val = 878.9 Hz
    ==> Toggling frequency = 439.4 Hz
  ----------------------------------------------------------------------- */   

  /* ����Ԥ��Ƶֵ */
  PrescalerValue = (uint16_t) (SystemCoreClock  / 6000000) - 1;

  /* Time ��ʱ���������� */
  TIM_TimeBaseStructure.TIM_Period = 65535;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* Ԥ��Ƶ������ */
  TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);

  /* ����Ƚ�ʱ��ģʽ�������� */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  /* ����Ƚ�ʱ��ģʽ����: Ƶ��3*/
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
  TIM_OC3Init(TIM3, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Disable);

  /* ����Ƚ�ʱ��ģʽ����: Ƶ��4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;
  TIM_OC4Init(TIM3, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Disable);
   
  /* TIM �ж�ʹ�� */
  TIM_ITConfig(TIM3, TIM_IT_CC3 | TIM_IT_CC4, ENABLE);

  /* TIM3 ʹ�� */
  TIM_Cmd(TIM3, ENABLE);
	
	}

void TIM3_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
     temp1++;
    /* LED3 toggling with frequency = 219.7 Hz */
 //   LED1_Toggle();
    capture = TIM_GetCapture3(TIM3);
    TIM_SetCompare3(TIM3, capture + CCR3_Val);
  }
  else
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
     temp2++;
    /* LED4 toggling with frequency = 439.4 Hz */
 //   LED2_Toggle();
    capture = TIM_GetCapture4(TIM3);
    TIM_SetCompare4(TIM3, capture + CCR4_Val);
  }
}