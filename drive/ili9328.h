#ifndef _ILI9328_H
#define _ILI9328_H
 

/*硬件相关的宏定义*/
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
//画笔颜色

#define     RED          0XF800	  //红色
#define     GREEN        0X07E0	  //绿色
#define     BLUE         0X001F	  //蓝色
#define     WHITE        0XFFFF	  //白色
#define     BLACK        0X0000	  //黑色
#define     YELLOW       0XFFE0	  //黄色
#define     ORANGE       0XFC08	  //橙色
#define     GRAY  	     0X8430   //灰色
#define     LGRAY        0XC618	  //浅灰色
#define     DARKGRAY     0X8410	  //深灰色
#define     PORPO        0X801F	  //紫色
#define     PINK         0XF81F	  //粉红色
#define     GRAYBLUE     0X5458   //灰蓝色
#define     LGRAYBLUE    0XA651   //浅灰蓝色
#define     DARKBLUE     0X01CF	  //深灰蓝色
#define 	LIGHTBLUE    0X7D7C	  //浅蓝色
	    															  
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
extern void LCD_Init(void);	   // 初始化液晶显示器
extern void LCD_DisplayOn(void); // 开显存
extern void LCD_DisplayOff(void);  // 关显存
extern void LCD_Clear(uint16_t color);  // 整屏显示
extern void LCD_DrawPoint(uint16_t x,uint16_t y); // 在屏上画一个像素的点
extern uint16_t LCD_ReadPoint(uint16_t x,uint16_t y);	 // 读屏上一个像素的点
extern void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2); // 画一条直线
extern void LCD_DrawRectage(uint16_t xstart,uint16_t ystart,uint16_t xend,uint16_t yend,uint16_t color);	// 画矩形
extern void LCD_Fill(uint16_t xstart ,uint16_t ystart ,uint16_t xend ,uint16_t yend ,uint16_t color);	// 画带填充的矩形
extern void LCD_DrawCircle(uint8_t x0, uint16_t y0, uint8_t r); // 画一个圆
extern void LCD_ShowNum(uint8_t x,uint16_t y,uint32_t num,uint8_t len);	// 显示数字
extern void LCD_ShowChar(uint16_t x,uint16_t y,uint16_t num);	// 显示一个ascii码为0~94的字符
 extern void LCD_Show_hz(uint16_t x,uint16_t y,uint8_t *hz); // 显示一个GBK汉字
 extern void LCD_ShowString(uint16_t x,uint16_t y,uint8_t *p);	// 显示字符串（英文和中文都可以）
extern void GLCD_Test           (void);

#endif /* _ILI9328_H */
