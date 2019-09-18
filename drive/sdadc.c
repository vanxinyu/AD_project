#include "sdadc.h"

 uint32_t SDADC1_Config(void)
{
  SDADC_AINStructTypeDef  SDADC_AINStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;
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

  /* SDADC io�˿�����5P (PB1) */
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = POT_GPIO_PIN;
  GPIO_Init(POT_GPIO_PORT, &GPIO_InitStructure);

  /* ѡ���ⲿ�ο��� The reference voltage selection is available
     only in SDADC1 and therefore to select the VREF for SDADC2/SDADC3, SDADC1
     clock must be already enabled */
  SDADC_VREFSelect(POT_SDADC_VREF);

  /* Insert delay equal to ~5 ms */
  Delay_ms(5);
  
  /* ʹ�� POT_SDADC */
  SDADC_Cmd(POT_SDADC, ENABLE);

  /* �����ʼ��ģʽ  */
  SDADC_InitModeCmd(POT_SDADC, ENABLE);
  SDADCTimeout = SDADC_INIT_TIMEOUT;
  /*�ȴ�ֱ����ʼ����� wait for INITRDY flag to be set */
  while((SDADC_GetFlagStatus(POT_SDADC, SDADC_FLAG_INITRDY) == RESET) && (--SDADCTimeout != 0));

  if(SDADCTimeout == 0)
  {
    /* INITRDY flag can not set */
    return 1;
  }

  /* ģ����������Analog Input configuration conf0: use single ended zero reference */
  SDADC_AINStructure.SDADC_InputMode = SDADC_InputMode_SEZeroReference;
  SDADC_AINStructure.SDADC_Gain = POT_SDADC_GAIN;
  SDADC_AINStructure.SDADC_CommonMode = SDADC_CommonMode_VSSA;
  SDADC_AINStructure.SDADC_Offset = 0;
  SDADC_AINInit(POT_SDADC, SDADC_Conf_0, &SDADC_AINStructure);

  /* select POT_SDADC channel 5 to use conf0 */
  SDADC_ChannelConfig(POT_SDADC, POT_SDADC_CHANNEL, SDADC_Conf_0);

  /* select channel 5 */
  SDADC_InjectedChannelSelect(POT_SDADC, POT_SDADC_CHANNEL);
  /* Enable continuous mode */
  SDADC_InjectedContinuousModeCmd(POT_SDADC, ENABLE);

  /* Exit initialization mode */
  SDADC_InitModeCmd(POT_SDADC, DISABLE);

  /* NVIC Configuration */
  NVIC_InitStructure.NVIC_IRQChannel = SDADC1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

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
    return 2;
  }

  /* Enable end of injected conversion interrupt */
  SDADC_ITConfig(POT_SDADC, SDADC_IT_JEOC, ENABLE);
  /* Start a software start conversion */
  SDADC_SoftwareStartInjectedConv(POT_SDADC);
  
  return 0;
}
