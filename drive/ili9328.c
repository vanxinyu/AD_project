/******************************************************************/
/* ���ƣ�TFT����                                                   */
/* Ӳ�����ӣ�PB0-PB7---DB0-DB7   ��8λ                             */
/*           PC0-PC7---DB8-DB7   ��8λ                             */
/*           PD2---WR  PC12---RD PC11---CS  PC10---RS  PB11---CM     */            
/* ���ߣ����                                                     */
/*                  ����ַ��	qfv5.taobao.com��                   */
/* ��ϵ��ʽ��QQ:157736309                                         */
/******************************************************************/

#include"stm32f37x.h"
#include"ili9328.h"
#include "ascii.h"
#include "w25x16.h"
uint16_t  POINT_COLOR=BLACK;
uint16_t  BACK_COLOR=WHITE;
/**************************************************/
/* �������ܣ�������ʱ��ΪLCD��ʼ��ʱ�����        */
/**************************************************/	

static void delay (int cnt) {

  cnt <<= DELAY_2N;
  while (cnt--);
}
/***************************************************/
/* �������ܣ���ILI9325�ļĴ���д����               */
/* ��ڲ�����val��16λ����                         */
/* ˵����    д16λ���ݣ�                          */
/* ע�⣺ʹ��ʱ��Ҫ��ʹ��LCD_WR_REG(reg)ѡ��Ҫ���� */
/*       ��д���ĸ��Ĵ������档                    */
/***************************************************/
void LCD_WR_DATA(uint16_t val)
{  
	
 	Set_Rs;//���Ĵ���ѡ��
	Clr_Cs;	//Ƭѡ�õ�
  GPIOB->ODR &= 0xff00;
  GPIOC->ODR &= 0xff00;	//��PB,PC���ݶ˿�ȫ���õ�
	GPIOB->ODR|=((val)&(0x00ff));//д���8λ
	GPIOC->ODR|=(val>>8);//д��߰�λ
	Clr_nWr; //��дʹ��
	Set_nWr; //��ʹ��
  Set_Cs;	//��Ƭѡ
}

/****************************************************/
/* �������ܣ���ILI9238��д���������Ӧ�Ĵ���         */
/* ��ڲ�����д����                                 */
/* ˵����    �Ĵ�������16λ����8λ���ӷ�ʽ��Ҫд����*/
/****************************************************/
void LCD_WR_REG(uint16_t cmd)
{
	Clr_Rs;
	Clr_Cs;
	 GPIOB->ODR &= 0xff00;
   GPIOC->ODR &= 0xff00;	
	GPIOB->ODR |=((cmd)&(0x00ff));
	GPIOC->ODR|=(cmd>>8);
	Clr_nWr;
	Set_nWr;
	 Set_Cs;	
	}
/******************************************************/
/* �������ܣw��Ĵ�����д����                        */
/* ��ڲ�����reg���Ĵ�����  data������ֵ             */
/* ˵����    �ú�����ǰ���������ĺϳɣ�Ҳ������������ */
/*           ����һ����÷�����������ϡ�             */
/******************************************************/

void LCD_WR_REG_DATA(uint16_t reg, uint16_t data)
{ 
	 
   	LCD_WR_REG(reg);//ȷ��Ҫд��ļĴ���
   	LCD_WR_DATA(data);//ȷ��д��Ĵ���������
 	
	}	
	
	
/****************************************************/
/* �������ܣ���ILI9238�ж�����                      */
/* ��ڲ���������                               */
/* ˵����    �Ĵ�������16λ����8λ���ӷ�ʽ��Ҫд����*/
/****************************************************/
uint16_t LCD_RD_DATA(void)
{ 
	unsigned short val,val1,val2;
	Set_Rs;
	Set_nWr;
	Clr_nRd;
	GPIOB->MODER&=0xffff0000;		   
  GPIOC->MODER&=0xffff0000;
	val1 = GPIOB->IDR;
	val2 = GPIOC->IDR;
	val=((val1)&(0x00ff))+((val2<<8)&(0xff00));
	               //�������˺��������Ϊ���
		GPIOB->MODER|=0x00005555; 
		GPIOC->MODER|=0x00005555; 
	Set_nRd;
	return val;
}	

/******************************************************/
/* �������ܣw���Ĵ����������                        */
/* ��ڲ�����reg���Ĵ�����                           */
/* ˵����    �ú�����ǰ���������ĺϳɣ�Ҳ������������ */
/*           ����һ����÷�����������ϡ�             */
/******************************************************/
uint16_t  LCD_RD_REG_DATA (uint16_t  reg) 
{
  uint16_t value;
 	Clr_Cs;
	LCD_WR_REG(reg);    
	value = LCD_RD_DATA();      	
	Set_Cs;
	return value;
}

/*******************************************************/
/* �������ܣ���ʼ��LCD                                 */
/*******************************************************/
void LCD_init (void) 
{ 
	//////////////////////////////////////////////////////////////////////�ܽų�ʼ��
	 RCC->AHBENR&=0xff81ffff;
	 RCC->AHBENR|=0x007E0000;
	
	GPIOB->MODER&=0xffff0000;		   // ����PB�ڵ�8λ����Ϊ���������LCD16λ�������ݺͿ�������
	GPIOB->MODER|=0x00005555;  
  GPIOB->ODR &= 0xff00;	
	GPIOC->MODER&=0xffff0000;		   // ����PC�ڵ�8λ����Ϊ���������LCD16λ�������ݺͿ�������
	GPIOC->MODER|=0x00005555; 
	GPIOC->ODR &= 0xff00;	
  // PC�ڵ�8λ�����ø�	
	GPIOE->MODER&=0xfff3ffff;
	GPIOE->MODER|=0x00040000;  //PE9Ϊ���ģʽ
	GPIOE->BRR =0x00000200;    //���Ϊ�͵�ƽ
		
	 GPIOC->MODER&= 0xfc0fffff;  //PD2---WR  PC12---RD PC11---CS  PC10---RS  
   GPIOC->MODER|=0x01500000;
	 GPIOC->BSRR = 0x00001c00;
	 
	 GPIOD->MODER&= 0xffffffcf;
   GPIOD->MODER|=0x00000010;	 
	 GPIOD->BSRR = 0x00000004;
//////////////////////////////////////////////////////////////////	
	

		LCD_WR_DATA(0x0000); 		
   	delay(10); 
    LCD_WR_REG(0x0000);    	//transfer synchronization	
    LCD_WR_REG(0x0000);    	 
    LCD_WR_REG(0x0000);    	
    LCD_WR_REG(0x0000);    	
   	delay(1); 
    LCD_WR_REG(0x00a4);		//oscillation control
    LCD_WR_DATA(0x0001);
    delay(10);				//delay_ms 60 ms
    LCD_WR_REG(0x0001); 		//driver output control 
    LCD_WR_DATA(0x0100); 	//0x0100
    LCD_WR_REG(0x0002); 		// drive  waveform control
    LCD_WR_DATA(0x0700);    	//frame/line inversion  	  
    LCD_WR_REG(0x0003); 		//entry mode
    LCD_WR_DATA(0x1030);		//   0x1030
    LCD_WR_REG(0x0008); 		// display control
    LCD_WR_DATA(0x0404);		// set fp,bp  0x0404
    LCD_WR_REG(0x000a); 		//display control 4
    LCD_WR_DATA(0x0000);		//0x0000
    LCD_WR_REG(0x000c); 		//external display interface control2
    LCD_WR_DATA(0x0001);		//
    LCD_WR_REG(0x000d); 		//frame marker control 
    LCD_WR_DATA(0x0001);		//
	
    LCD_WR_REG(0x0060); 		// driver output control 
    LCD_WR_DATA(0x2700);		//320 line   0x2700  0xA700
    LCD_WR_REG(0x0061); 		// base image display control 
    LCD_WR_DATA(0x0001);		// 
    LCD_WR_REG(0x006A); 		// base image display control 
    LCD_WR_DATA(0x0000);		// 
    LCD_WR_REG(0x0090); 		// panel interface control 1 
    LCD_WR_DATA(0x0010);		// set div,rtn 
    LCD_WR_REG(0x0092); 		// panel interface control 2 
    LCD_WR_DATA(0x0000);		//   
    LCD_WR_REG(0x0093); 		// panel interface control 3 
    LCD_WR_DATA(0x0001);		//  

//set gamma value
    LCD_WR_REG(0x0030); 		// Gamma control (1)
    LCD_WR_DATA(0x0303); 
    LCD_WR_REG(0x0031); 		// Gamma control (2)
    LCD_WR_DATA(0x0303); 
    LCD_WR_REG(0x0032);   	// Gamma control (3)
    LCD_WR_DATA(0x0303);
    LCD_WR_REG(0x0033);  	// Gamma control (4)
    LCD_WR_DATA(0x0303);
    LCD_WR_REG(0x0034); 		// Gamma control (5)
    LCD_WR_DATA(0x0404); 
    LCD_WR_REG(0x0035); 		// Gamma control (6)
    LCD_WR_DATA(0x0404);
    LCD_WR_REG(0x0036); 		// Gamma control (7)
    LCD_WR_DATA(0x0404); 
    LCD_WR_REG(0x0037); 		// Gamma control (8)
    LCD_WR_DATA(0x0303);
    LCD_WR_REG(0x0038); 		// Gamma control (9)
    LCD_WR_DATA(0x0303);
    LCD_WR_REG(0x0039); 		// Gamma control (10)
    LCD_WR_DATA(0x0303);
    LCD_WR_REG(0x003a); 		// Gamma control (11)
    LCD_WR_DATA(0x0303);  
    LCD_WR_REG(0x003b); 		// Gamma control (12)
    LCD_WR_DATA(0x0303);
    LCD_WR_REG(0x003c); 		// Gamma control (13)
    LCD_WR_DATA(0x0404);
    LCD_WR_REG(0x003d); 		// Gamma control (14)
    LCD_WR_DATA(0x0303);
   	delay(1); 

  //power on     
    LCD_WR_REG(0x0007);		 //display control:GON=DTE=D1-0=0
    LCD_WR_DATA(0x0001);       
	LCD_WR_REG(0x0017); 		//power control 6:
    LCD_WR_DATA(0x0001);		//pse=1
	LCD_WR_REG(0x0019); 		//power control  
    LCD_WR_DATA(0x0000);		//new setting from renesas
  	LCD_WR_REG(0x0010);		//power control 1
    LCD_WR_DATA(0x17b0); 	//set bt ,new for renesas		      
	LCD_WR_REG(0x0011);		//power control 2
    LCD_WR_DATA(0x0006); 	//set vc2-0 ,new for renesas		  
    LCD_WR_REG(0x0012); 		//power control 3
    LCD_WR_DATA(0x0118);		//set pon vcmr=1,pson=0;set vrh3-0     
    LCD_WR_REG(0x0013);  //power control 4
    LCD_WR_DATA(0x0E00); //set  vdv3-0  0x1b00
	LCD_WR_REG(0x0029); 		//power control 7: 
    LCD_WR_DATA(0x0000);		//set vcomh,not use 
    LCD_WR_REG(0x002a); 		// set vcomh 
    LCD_WR_DATA(0x008c);		//vcmsel=1,use this register 
    LCD_WR_REG(0x0012); 		//power control 3
    LCD_WR_DATA(0x0138);		// pson=1; 
   	delay(20);				//delay_ms 20 ms   
    //end power on  
    LCD_WR_REG(0x0050); 		// window horizontal RAM address 
    LCD_WR_DATA(0x0000);		//start address
    LCD_WR_REG(0x0051); 		// window horizontal RAM address
    LCD_WR_DATA(0x00ef);		//end address, 
    LCD_WR_REG(0x0052); 		//window vertical RAM address 
    LCD_WR_DATA(0x0000);		//start address 
    LCD_WR_REG(0x0053); 		//window vertical RAM address
    LCD_WR_DATA(0x013f);		//end address,
    LCD_WR_REG(0x0020); 		// GRAM address set
    LCD_WR_DATA(0x0000);		// horizontal address set 
    LCD_WR_REG(0x0021); 		// GDDRAM address set
    LCD_WR_DATA(0x0000);		//vertical address set
    //display on     
    LCD_WR_REG(0x0007); 		// Display control on
    LCD_WR_DATA(0x0021);
    delay(10);
	LCD_WR_REG(0x0010);		//power control 1
    LCD_WR_DATA(0x10b0);     //set bt ,new for renesas		       
	LCD_WR_REG(0x0011);		//power control 2
    LCD_WR_DATA(0x0000); 	//set vc2-0 ,new from renesas		 
   	delay(10);
    LCD_WR_REG(0x0007); 		// Display control on
    LCD_WR_DATA(0x0061);
	delay(10);
    LCD_WR_REG(0x0007); 		//Display control on
    LCD_WR_DATA(0x0173);
    delay(20);  
  
    LCD_WR_REG(0x0022); 	//write data  to gram
}

/****************************************************************************
* ��    �ƣ�void GLCD_Test(void)
* ��    �ܣ�����Һ����
* ��ڲ�������
* ���ڲ�������
* ˵    ������ʾ����������Һ�����Ƿ���������
* ���÷�����GLCD_Test();
****************************************************************************/
void GLCD_Test(void)
{
  uint16_t i,j;
  LCD_WR_REG_DATA(0x20, 0);//ȷ��д���X����
  LCD_WR_REG_DATA(0x21, 0);//ȷ����д���Y����

  Clr_Cs;  //TFT����Ƭѡ
  LCD_WR_REG(0x22);//��ʼд��GRAM
  
  for(i=0;i<320;i++)
    for(j=0;j<240;j++)//ѭ��д��ɫ
    {
      if(i>279)LCD_WR_DATA(0x0000);
      else if(i>239)LCD_WR_DATA(0x001f);
      else if(i>199)LCD_WR_DATA(0x07e0);
      else if(i>159)LCD_WR_DATA(0x07ff);
      else if(i>119)LCD_WR_DATA(0xf800);
      else if(i>79)LCD_WR_DATA(0xf81f);
      else if(i>39)LCD_WR_DATA(0xffe0);
      else LCD_WR_DATA(0xffff);
    }
  Set_Cs;
}

/**********************************************/
/* �������ܣ�������ʾ                         */
/* ˵    ����һ�����ڹر���ʾ��Ŀ���         */
/**********************************************/
void LCD_DisplayOn(void)
{
 	LCD_WR_REG_DATA(0x0007, 0x0133); // ������ʾ
}
/**********************************************/
/* �������ܣ��ر���ʾ                         */
/* ˵    ����������ʹTFT����������ʹ��TFT�ϵ� */
/*           ���ݱ��ֲ��䣬һ������TFT��������*/
/*           ������ģ�����ӣ���������ģ���ʱ */
/*           ��ϣ��Ӱ��TFT����ʾ��          */
/**********************************************/
void LCD_DisplayOff(void)
{
	LCD_WR_REG_DATA(0x0007, 0x0); // �ر���ʾ
}

/***************************************************/
/* �������ܣ������Դ�����                          */
/* ˵    �������ý�Ҫ��ʾ���Դ�XY��ʼ�ͽ�������    */
/* ע    �⣺�����Ժ�Ҫ�ָ����(0,0)���յ�(239,319)*/
/***************************************************/
void LCD_XYRAM(uint16_t xstart ,uint16_t ystart ,uint16_t xend ,uint16_t yend)
{
	LCD_WR_REG_DATA(0x0050, xstart); // ���ú�����GRAM��ʼ��ַ
	LCD_WR_REG_DATA(0x0051, xend); // ���ú�����GRAM������ַ
	LCD_WR_REG_DATA(0x0052, ystart); // ����������GRAM��ʼ��ַ
	LCD_WR_REG_DATA(0x0053, yend); // ����������GRAM������ַ
}

/*******************************************************/
/* �������ܣ�����TFT����ʼ����                         */
/*******************************************************/
void LCD_SetC(uint16_t x, uint16_t y)
{
	LCD_WR_REG_DATA(0x0020,x);  //����X����λ��
	LCD_WR_REG_DATA(0x0021,y);  //����Y����λ��
}
/**********************************************/
/* �������ܣ�����TFT                          */
/* ��ڲ�����color ��ɫֵ                     */
/**********************************************/
void LCD_Clear(uint16_t color)
{
    uint32_t temp;
    LCD_WR_REG_DATA(0x0020,0);//����X����λ��
    LCD_WR_REG_DATA(0x0021,0);//����Y����λ��
    LCD_WR_REG(0x0022);				//ָ��RAM�Ĵ�����׼��д���ݵ�RAM
	for(temp=0;temp<76800;temp++)
	{
		LCD_WR_DATA(color);    
	}
}
/******************************************/
/* �������ܣ���һ�����صĵ�               */
/* ��ڲ�����x,y   �������               */
/******************************************/
void LCD_DrawPoint(uint16_t x,uint16_t y)
{
	LCD_WR_REG_DATA(0x0020,x);//����X����λ��
    LCD_WR_REG_DATA(0x0021,y);//����Y����λ��
	LCD_WR_REG(0x0022);       //��ʼд��GRAM
	LCD_WR_DATA(POINT_COLOR); 
}
/******************************************/
/* �������ܣ���TFTĳһ�����ɫ            */
/* ���ڲ�����color  ����ɫֵ              */
/******************************************/
uint16_t LCD_ReadPoint(uint16_t x,uint16_t y)
{
	uint16_t  color;

	LCD_WR_REG_DATA(0x0020,x);//����X����λ��
  LCD_WR_REG_DATA(0x0021,y);//����Y����λ��
	LCD_WR_REG(0x0022);       //��ʼд��GRAM
	
	GPIOB->MODER&=0xffff0000;		   
  GPIOC->MODER&=0xffff0000;        //��TFT������������Ϊ����
	
	color = LCD_RD_DATA();	  //����GRAMֵ��ע�⣺GRAMֵ�����ȡ���Σ�
	color = LCD_RD_DATA();	  //����GRAMֵ�����ILI932x�����ֲᣩ
	
	GPIOB->MODER|=0x00005555; 
	GPIOC->MODER|=0x00005555; 		  //�ָ���������Ϊ���

	return color;
} 

/**********************************************/
/* �������ܣ���ֱ��                           */
/**********************************************/
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}
/**********************************************/
/* �������ܣ�������                           */
/**********************************************/
void LCD_DrawRectage(uint16_t xstart,uint16_t ystart,uint16_t xend,uint16_t yend,uint16_t color)
{
	POINT_COLOR=color;
	LCD_DrawLine(xstart, ystart, xend, ystart);
	LCD_DrawLine(xstart, yend, xend, yend);
	LCD_DrawLine(xstart, ystart, xstart, yend);
	LCD_DrawLine(xend, ystart, xend, yend);
}
/**********************************************/
/* �������ܣ��������                         */
/**********************************************/
void LCD_Fill(uint16_t xstart ,uint16_t ystart ,uint16_t xend ,uint16_t yend ,uint16_t color)
{                    
    uint32_t max;
	LCD_XYRAM(xstart ,ystart ,xend ,yend); // ����GRAM����
	LCD_WR_REG_DATA(0x0020,xstart);//����X����λ��
    LCD_WR_REG_DATA(0x0021,ystart);//����Y����λ��
    LCD_WR_REG(0x0022);				//ָ��RAM�Ĵ�����׼��д���ݵ�RAM
	max=(uint32_t)((xend-xstart+1)*(yend-ystart+1));    
	while(max--)
	{
		LCD_WR_DATA(color);
	}
	LCD_XYRAM(0x0000 ,0x0000 ,0x00EF ,0X013F); // �ָ�GRAM������ʾ
}
/*******************************************/
/* �������ܣ���Բ                          */
/* ��ڲ�����x0,y0  Բ������               */
/*           r      �뾶(��λ������)       */
/*******************************************/
void LCD_DrawCircle(uint8_t x0, uint16_t y0, uint8_t r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a);             //3           
		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0-a,y0+b);             //1       
		LCD_DrawPoint(x0-b,y0-a);             //7           
		LCD_DrawPoint(x0-a,y0-b);             //2             
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0-b);             //5
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-b,y0+a);             
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b);
	}
} 
/**********************************************/
/* �������ܣ���m��n�η�                       */
/**********************************************/
uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}			 
/**********************************************/
/* �������ܣ���ʾ����                         */
/* ��ڲ�����x,y :�������	 	              */
/*           len :���ֵ�λ��				  */
/*           num:��ֵ(0~4294967295);	   	  */
/**********************************************/
void LCD_ShowNum(uint8_t x,uint16_t y,uint32_t num,uint8_t len)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;		 // �˱�������ȥ�����λ��0	
				   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+8*t,y,' ');
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+8*t,y,temp+'0'); 
	}
} 
/**********************************************/
/* �������ܣ���ʾ8*16����Ӣ���ַ�             */
/* ��ڲ�����x,y :�������	 	              */
/*           num:��ĸ�����         	   	  */
/* ע    �⣺x,y��ȡֵҪ��240��320��Χ��      */
/**********************************************/
void LCD_ShowChar(uint16_t x,uint16_t y,uint16_t num)
{
    uint8_t temp;
    uint8_t pos,t;
	      
   	LCD_WR_REG_DATA(0x0020,x);//����X����λ��
    LCD_WR_REG_DATA(0x0021,y);//����Y����λ��
 	/*�����Դ�����*/
	LCD_XYRAM(x,y,x+7,y+15); // ����GRAM����
    LCD_WR_REG(0x0022);	  	 //ָ��RAM�Ĵ�����׼��д���ݵ�RAM
 	
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	for(pos=0;pos<16;pos++)
	{
	    temp=ascii_16[num][pos];
	    for(t=0;t<8;t++)
	    {                 
	        if(temp&0x80)LCD_WR_DATA(POINT_COLOR);
	        else LCD_WR_DATA(BACK_COLOR);    
	        temp<<=1; 
	    }
	}
	/* �ָ��Դ���ʾ����240*320 */
	LCD_XYRAM(0x0000 ,0x0000 ,0x00EF ,0X013F); // �ָ�GRAM������ʾ
	return;
}

uint8_t buf[32]; //���ڴ��16*16����������

/**********************************************/
/* �������ܣ���W25X16����ȡ������             */
/* ��ڲ�����code:GBK���һ���ֽ�             */
/*           dz_data:��ŵ����������         */
/**********************************************/
void Get_GBK_DZK(uint8_t *code, uint8_t *dz_data)
{
	uint8_t GBKH,GBKL;   // GBK���λ���λ					  
	uint32_t offset; 	   // ����ƫ����

	GBKH=*code;
	GBKL=*(code+1);	   // GBKL=*(code+1);
	if(GBKH>0XFE||GBKH<0X81)return;
	GBKH-=0x81;
	GBKL-=0x40;
	offset=((uint32_t)192*GBKH+GBKL)*32;//�õ��ֿ��е��ֽ�ƫ���� 
  SPI_FLASH_BufferRead(dz_data,offset+0x100,32);
	return;
}
/**********************************************/
/* �������ܣ���ʾ16*16��������                */
/* ��ڲ�����x,y :�������	 	              */
/*           *hz������               	   	  */
/**********************************************/
void LCD_Show_hz(uint16_t x,uint16_t y,uint8_t *hz)
{
	uint8_t i,j,temp;
	uint8_t dz_data[32];

	Get_GBK_DZK(hz, dz_data);

	LCD_WR_REG_DATA(0x0020,x);//����X����λ��
    LCD_WR_REG_DATA(0x0021,y);//����Y����λ��
 	/*�����Դ�����*/
	LCD_XYRAM(x,y,x+15,y+15); // ����GRAM����
    LCD_WR_REG(0x0022);	  	 //ָ��RAM�Ĵ�����׼��д���ݵ�RAM

 	for(i=0;i<32;i++)
	{
		temp=dz_data[i];
	 	for(j=0;j<8;j++)
		{
		 	if(temp&0x80)LCD_WR_DATA(POINT_COLOR);
	        else LCD_WR_DATA(BACK_COLOR);    
	        temp<<=1; 
		}
	}

	/* �ָ��Դ���ʾ����240*320 */
	LCD_XYRAM(0x0000 ,0x0000 ,0x00EF ,0X013F); // �ָ�GRAM������ʾ
	
	return;
}

/*********************************************/
/* �������ܣ���ʾ�ַ��������ĺ�Ӣ�ģ�        */
/* ��ڲ�����x,y: ����                       */
/*           *p:�ַ���                       */
/*********************************************/
void LCD_ShowString(uint16_t x,uint16_t y,uint8_t *p)
{         
    while(*p!='\0')	// ���û�н���
    {       
        if(*p>0x80)//���������
		{
			if((*p=='\n')||(x>224))  // ���κͻ���
			{
				y=y+19;   //�����16 �м��3
				x=2;      //2�Ǳ߾�
			}
 			LCD_Show_hz(x, y, p);
			x+=16;
			p+=2;
		}
		else	   //�����Ӣ��
		{
			if((*p=='\n')||(x>224))   // ���κͻ���
			{
				y=y+19;   //�����16 �м��3
				x=2;      //2�Ǳ߾�
			}
			LCD_ShowChar(x,y,*p);
			x+=8;
			p++;
		}
    }  
}

  


