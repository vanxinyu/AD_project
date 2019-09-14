// ------------------------------------
// Ӳ�����ӣ�PE8---T-INT
//           PA4----T-CS 
//           PA5----T-CLK
//           PA6----T-OUT
// 					 PA7----T-DIN
//           SPI1-- AF5
//
// -------------------------------------

#include "xpt2046.h"

Pen_Holder Pen_Point;//�����
extern volatile unsigned char touch_flag;
	/******************************************************
* ��������XPT_Init();
* ����  ����ʼ��ADC784��spi�ӿ�
* ����  : ��������ʱ�ӣ�Ȼ������GPIO���������ADC�˿ڸ���
* ���  ����
* ע��  ��
*********************************************************/ 
void XPT_Init(void)
{ 
	//������Ҫ���õĲ���
	GPIO_InitTypeDef  GPIO_InitStruct;
  SPI_InitTypeDef   SPI_InitStruct;
	//����ʱ��
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	 RCC_AHBPeriphClockCmd(TOUCH_CS_PIN_SCK|TOUCH_SCK_PIN_SCK|TOUCH_MISO_PIN_SCK | TOUCH_MOSI_PIN_SCK|TOUCH_INT_PIN_SCK , ENABLE);
	 RCC_APB2PeriphClockCmd(TOUCH_SPI1, ENABLE); 
	//����spiʹ�õ���GPIO�˿�
	
// 	/< Configure TOUCH_SPI pins: SCK */
  GPIO_InitStruct.GPIO_Pin = TOUCH_SCK_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(TOUCH_SCK_PORT, &GPIO_InitStruct);

  /*!< Configure TOUCH_SPI pins: MISO */
  GPIO_InitStruct.GPIO_Pin = TOUCH_MISO_PIN;
  GPIO_Init(TOUCH_MISO_PORT, &GPIO_InitStruct);

  /*!< Configure TOUCH_SPI pins: MOSI */
  GPIO_InitStruct.GPIO_Pin =TOUCH_MOSI_PIN;
  GPIO_Init(TOUCH_MOSI_PORT, &GPIO_InitStruct);

  /*!< Configure TOUCH_CS_PIN pin */
  GPIO_InitStruct.GPIO_Pin =TOUCH_CS_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(TOUCH_CS_PORT, &GPIO_InitStruct);
	
   /*!< Configure TOUCH_TIN_PIN pin */
	GPIO_InitStruct.GPIO_Pin =TOUCH_INT_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_PuPd =GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(TOUCH_INT_PORT, &GPIO_InitStruct);
	//��������
  GPIO_PinAFConfig(TOUCH_SCK_PORT, TOUCH_SCK_SOURCE, TOUCH_SCK_AF);
  GPIO_PinAFConfig(TOUCH_MISO_PORT, TOUCH_MISO_SOURCE, TOUCH_MISO_AF); 
  GPIO_PinAFConfig(TOUCH_MOSI_PORT, TOUCH_MOSI_SOURCE, TOUCH_MOSI_AF);
	
	//����spi
	SPI_TOUCH_CS_HIGH();
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
  SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
  SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStruct.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStruct);
  
  SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);
  
  SPI_Cmd(SPI1, ENABLE); /*!< TOUCH_SPI enable */
	
	}
	/******************************************************
* ��������TOUCH_INT()��
* ����  ��AD7845�ж�Ƕ��
* ����  : 
* ���  ��
* ע��  ��
*********************************************************/    	
void TOUCH_Int(void)
	{ 
	
		NVIC_InitTypeDef NVIC_InitStruct;
		EXTI_InitTypeDef EXTI_InitStruct;
		
		NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStruct);
		
		
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource8);
		EXTI_InitStruct.EXTI_Line = EXTI_Line8;
		EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
		EXTI_InitStruct.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStruct); 
			
		}
	
	/******************************************************
* ��������WR_Cmd();
* ����  ��xpt spiд����
* ����  : cmd
* ���  ����
* ע��  ��
*********************************************************/  

uint8_t WR_Cmd(uint8_t cmd)
{   /* Wait for SPI1 Tx buffer empty */ 
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); 
    /* Send SPI1 data */ 
    SPI_SendData8(SPI1,cmd); 
    /* Wait for SPI1 data reception */ 
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); 
    /* Read SPI1 received data */ 
    return SPI_ReceiveData8(SPI1); 
	}	
	
	
void Touch_Init(void)
{
	XPT_Init();
	// SPIͨ����������
	TOUCH_Int();
	// Penirq �ж���������	
  touch_flag=0;	
	Pen_Point.Pen_Sign=Pen_Up;
	ADS_Read_AD(CMD_RDX);
	ADS_Read_AD(CMD_RDY);
}

	/******************************************************
* ��������DelayUS
* ����  ��������ʱcnt ΢��
* ����  : cnt
* ���  ����
* ע��  ���ú���������Ŀ���Ǿֲ�ʹ��,�Ӷ���������ȥʹ�ö�ʱ������ʱ
*********************************************************/  
void delay_us(uint32_t cnt)
{
    uint16_t i;
    for(i = 0;i<cnt;i++)
    {
        uint8_t us = 22; /* ����ֵΪ12����Լ��1΢�� */    
        while (us--)     /* ��1΢��	*/
        {
            ;   
        }
    }
}	

/*************************************************/
/* ��    �ܣ���ȡX���Y���ADCֵ				 */
/* ��ڲ�����CMD:����                            */ 
/*************************************************/
uint16_t ADS_Read_AD(uint8_t CMD)
{	  
	uint16_t NUMH,NUML;
	uint16_t Num; 

	SPI_TOUCH_CS_LOW(); // CS=0 ��ʼSPIͨ��	 
	delay_us(1);
	WR_Cmd(CMD);
	delay_us(6);            // ��ʱ�ȴ�ת�����
	NUMH=WR_Cmd(0x00);
	NUML=WR_Cmd(0x00);
	Num=((NUMH)<<8)+NUML; 	
	Num>>=4;                // ֻ�и�12λ��Ч.
	SPI_TOUCH_CS_HIGH();  // CS=1 ����SPIͨ��	 
	return(Num);   
}
	
#define READ_TIMES 10 //��ȡ����
#define LOST_VAL 4	  //����ֵ
/*************************************************/
/* ��    �ܣ���ȡX���Y���ADCֵ				 */
/* ��ڲ�����CMD_RDX:��ȡX��ADCֵ                */ 
/*           CMD_RDY:��ȡY��ADCֵ                */
/* ˵    ��������һ��������ȣ���������˲�      */
/*************************************************/
uint16_t ADS_Read_XY(uint8_t xy)
{
	uint16_t i, j;
	uint16_t buf[READ_TIMES];
	uint16_t sum=0;
	uint16_t temp;
	for(i=0;i<READ_TIMES;i++)
	{				 
		buf[i]=ADS_Read_AD(xy);	    
	}				    
	for(i=0;i<READ_TIMES-1; i++)//����
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//��������
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 
/*************************************************/
/* ��    �ܣ���ȡX������ADCֵ                  */
/* ��ڲ�����&Pen_Point.X_ADC,&Pen_Point.Y_ADC   */
/* ���ڲ�����0���ɹ������ص�X,Y_ADCֵ��Ч��      */
/*           1: ʧ�ܣ����ص�X,Y_ADCֵ��Ч��      */ 
/*************************************************/
uint8_t Read_ADS(uint16_t *x,uint16_t *y)
{
	uint16_t xtemp,ytemp;			 	 		  
	xtemp=ADS_Read_XY(CMD_RDX);
	ytemp=ADS_Read_XY(CMD_RDY);	  												   
	if(xtemp<100||ytemp<100)return 1;//����ʧ��
	*x=xtemp;
	*y=ytemp;
	return 0;//�����ɹ�
}
/*************************************************/
/* ���ܣ��������ζ�ȡADCֵ						 */
/* ԭ�������ζ�ȡ��ֵ���Ƚϣ�����Χ�ڿ�ȡ  */ 
/* ��ڲ�����&Pen_Point.X_ADC,&Pen_Point.Y_ADC   */
/* ���ڲ�����0���ɹ������ص�X,Y_ADCֵ��Ч��      */
/*           1: ʧ�ܣ����ص�X,Y_ADCֵ��Ч��      */ 
/*************************************************/
#define ERR_RANGE 50 //��Χ 
uint8_t Read_ADS2(uint16_t*x,uint16_t *y) 
{
	uint16_t x1,y1;
 	uint16_t x2,y2;
 	uint8_t res; 
	   
    res=Read_ADS(&x1,&y1);  // ��һ�ζ�ȡADCֵ 
    if(res==1)return(1);	// �������ʧ�ܣ�����1
    res=Read_ADS(&x2,&y2);	// �ڶ��ζ�ȡADCֵ   
    if(res==1)return(1);   	// �������ʧ�ܣ�����1
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//ǰ�����β�����+-50��
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 0;	 // ��ȷ��ȡ������0
    }else return 1;	 // ǰ����+-50�ڣ��������� 
} 

/*************************************************/
/* ���ܣ��Ѷ�����ADCֵת��������ֵ               */
/*************************************************/	  
void Change_XY(void)
{
	Pen_Point.X_Coord=(240-(Pen_Point.X_ADC-129)/7.454); // �Ѷ�����X_ADCֵת����TFT X����ֵ
	Pen_Point.Y_Coord=(320-(Pen_Point.Y_ADC-286)/5.093); // �Ѷ�����Y_ADCֵת����TFT Y����ֵ 
}

/*************************************************/
/* ���ܣ���ȡһ��XY����ֵ                        */
/*************************************************/	
uint8_t Read_Once(void)
{
	  touch_flag=0;	
	Pen_Point.Pen_Sign=Pen_Up;
	if(Read_ADS2(&Pen_Point.X_ADC,&Pen_Point.Y_ADC)==0)	// �����ȡ���ݳɹ�
	{
		while(SPI_TOUHC_INT==0);   // �����ǲ��ǻ�������
		Change_XY();   // �Ѷ�����ADCֵת���TFT����ֵ
		return 0;	// ����0����ʾ�ɹ�
	}
	else return 1;	// �����ȡ����ʧ�ܣ�����1��ʾʧ��
}
/*************************************************/
/* ���ܣ�������ȡXY����ֵ                        */
/*************************************************/
uint8_t Read_Continue(void)
{
	  touch_flag=0;	
	Pen_Point.Pen_Sign=Pen_Up;	  
	if(Read_ADS2(&Pen_Point.X_ADC,&Pen_Point.Y_ADC)==0)	 // �����ȡ���ݳɹ�
	{
		Change_XY();   // �Ѷ�����ADCֵת���TFT����ֵ
		return 0;	   // ����0����ʾ�ɹ�
	}
	else return 1;	   // �����ȡ����ʧ�ܣ�����1��ʾʧ��
}
