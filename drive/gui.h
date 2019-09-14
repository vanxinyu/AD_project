#ifndef  __GUI_H_
#define  __GUI_H_

#include "stm32f37x.h"
#include "ili9328.h"

extern void LCD_Draw5Point(uint16_t x, uint16_t y, uint16_t color); // 画5个像素直径的圆点
extern void LCD_Draw9Point(uint16_t x, uint16_t y, uint16_t color); // 画9个像素直径的圆点
extern void Draw_Button(uint16_t xstart,uint16_t ystart,uint16_t xend,uint16_t yend);  // 显示一个按钮
extern void SetButton(uint8_t xstart,uint16_t ystart,uint8_t xend,uint16_t yend);  // 显示按钮按下状态
extern void EscButton(uint16_t xstart,uint16_t ystart,uint8_t xend,uint16_t yend); // 显示按钮松开状态
extern void Draw_TextBox(uint16_t xstart, uint16_t ystart, uint16_t xend, uint16_t yend);	// 显示一个输入框
extern void Draw_Window(uint16_t xstart,uint16_t ystart,uint8_t xend,uint16_t yend,uint8_t* caption);	// 显示一个WINDOWS窗口
extern void Draw_Frame(uint16_t xstart,uint16_t ystart,uint8_t xend,uint16_t yend,uint8_t *FrameName); // 显示一个Frame

extern void ComboDemo(void);		// 下拉列表演示
extern void ProgresBar_Demo(void);	// 进度条演示
extern void POINT_Demo(void);		// Loading 演示
extern void RGB_Demo(void);			// 刷屏演示
extern void BarReport_Demo(void);	// 条形报表演示
extern void Window_Demo(void);		// 窗口演示
extern void Button_Demo(void);		// 按钮演示


#endif  /* _GUI_H_ */

