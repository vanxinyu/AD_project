#ifndef _ILI9328_H
#define _ILI9328_H
 

/*Ӳ����صĺ궨��*/
/********************************************************************************/
#define DELAY_2N    18
#define Set_Cs  GPIOC->BSRR  = 0x00000800;
#define Clr_Cs  GPIOC->BRR   = 0x00000800;

#define Set_Rs  GPIOC->BSRR  = 0x00000400;
#define Clr_Rs  GPIOC->BRR   = 0x00000400;

#define Set_nWr GPIOD->BSRR  = 0x00000004;
#define Clr_nWr GPIOD->BRR   = 0x00000004;

#define Set_nRd GPIOC->BSRR  = 0x00001000;
#define Clr_nRd GPIOC->BRR   = 0x00001000;

//////////////////////////////////////////////////////////////////////
//������ɫ

#define     RED          0XF800	  //��ɫ
#define     GREEN        0X07E0	  //��ɫ
#define     BLUE         0X001F	  //��ɫ
#define     WHITE        0XFFFF	  //��ɫ
#define     BLACK        0X0000	  //��ɫ
#define     YELLOW       0XFFE0	  //��ɫ
#define     ORANGE       0XFC08	  //��ɫ
#define     GRAY  	     0X8430   //��ɫ
#define     LGRAY        0XC618	  //ǳ��ɫ
#define     DARKGRAY     0X8410	  //���ɫ
#define     PORPO        0X801F	  //��ɫ
#define     PINK         0XF81F	  //�ۺ�ɫ
#define     GRAYBLUE     0X5458   //����ɫ
#define     LGRAYBLUE    0XA651   //ǳ����ɫ
#define     DARKBLUE     0X01CF	  //�����ɫ
#define 	LIGHTBLUE    0X7D7C	  //ǳ��ɫ
	    															  
#define Line0                0
#define Line1               24
#define Line2               48
#define Line3               72
#define Line4               96
#define Line5              120
#define Line6              144
#define Line7              168
#define Line8              192
#define Line9              216


extern uint16_t  POINT_COLOR;
extern uint16_t  BACK_COLOR;

void delay (int cnt);
void LCD_WR_DATA(uint16_t val);
void LCD_WR_REG(uint16_t cmd);
void LCD_WR_REG_DATA(uint16_t reg, uint16_t data);
uint16_t LCD_RD_DATA(void);
uint16_t  LCD_RD_REG_DATA (uint16_t  reg);
void LCD_XYRAM(uint16_t xstart ,uint16_t ystart ,uint16_t xend ,uint16_t yend);
extern void LCD_init (void) ;
extern void LCD_Init(void);	   // ��ʼ��Һ����ʾ��
extern void LCD_DisplayOn(void); // ���Դ�
extern void LCD_DisplayOff(void);  // ���Դ�
extern void LCD_Clear(uint16_t color);  // ������ʾ
extern void LCD_DrawPoint(uint16_t x,uint16_t y); // �����ϻ�һ�����صĵ�
extern uint16_t LCD_ReadPoint(uint16_t x,uint16_t y);	 // ������һ�����صĵ�
extern void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2); // ��һ��ֱ��
extern void LCD_DrawRectage(uint16_t xstart,uint16_t ystart,uint16_t xend,uint16_t yend,uint16_t color);	// ������
extern void LCD_Fill(uint16_t xstart ,uint16_t ystart ,uint16_t xend ,uint16_t yend ,uint16_t color);	// �������ľ���
extern void LCD_DrawCircle(uint8_t x0, uint16_t y0, uint8_t r); // ��һ��Բ
extern void LCD_ShowNum(uint8_t x,uint16_t y,uint32_t num,uint8_t len);	// ��ʾ����
extern void LCD_ShowChar(uint16_t x,uint16_t y,uint16_t num);	// ��ʾһ��ascii��Ϊ0~94���ַ�
 extern void LCD_Show_hz(uint16_t x,uint16_t y,uint8_t *hz); // ��ʾһ��GBK����
 extern void LCD_ShowString(uint16_t x,uint16_t y,uint8_t *p);	// ��ʾ�ַ�����Ӣ�ĺ����Ķ����ԣ�
extern void GLCD_Test           (void);

#endif /* _ILI9328_H */
