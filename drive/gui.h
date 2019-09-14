#ifndef  __GUI_H_
#define  __GUI_H_

#include "stm32f37x.h"
#include "ili9328.h"

extern void LCD_Draw5Point(uint16_t x, uint16_t y, uint16_t color); // ��5������ֱ����Բ��
extern void LCD_Draw9Point(uint16_t x, uint16_t y, uint16_t color); // ��9������ֱ����Բ��
extern void Draw_Button(uint16_t xstart,uint16_t ystart,uint16_t xend,uint16_t yend);  // ��ʾһ����ť
extern void SetButton(uint8_t xstart,uint16_t ystart,uint8_t xend,uint16_t yend);  // ��ʾ��ť����״̬
extern void EscButton(uint16_t xstart,uint16_t ystart,uint8_t xend,uint16_t yend); // ��ʾ��ť�ɿ�״̬
extern void Draw_TextBox(uint16_t xstart, uint16_t ystart, uint16_t xend, uint16_t yend);	// ��ʾһ�������
extern void Draw_Window(uint16_t xstart,uint16_t ystart,uint8_t xend,uint16_t yend,uint8_t* caption);	// ��ʾһ��WINDOWS����
extern void Draw_Frame(uint16_t xstart,uint16_t ystart,uint8_t xend,uint16_t yend,uint8_t *FrameName); // ��ʾһ��Frame

extern void ComboDemo(void);		// �����б���ʾ
extern void ProgresBar_Demo(void);	// ��������ʾ
extern void POINT_Demo(void);		// Loading ��ʾ
extern void RGB_Demo(void);			// ˢ����ʾ
extern void BarReport_Demo(void);	// ���α�����ʾ
extern void Window_Demo(void);		// ������ʾ
extern void Button_Demo(void);		// ��ť��ʾ


#endif  /* _GUI_H_ */

