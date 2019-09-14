#include "gui.h"
#include "ili9328.h"

 void delay (int cnt) 
{
  int i;
	i=5000*cnt;
  while (i--);

}
/*********************************/
/* �������ܣ���ʾԲ��5*5         */
/*********************************/
void LCD_Draw5Point(uint16_t x, uint16_t y, uint16_t color)
{
	POINT_COLOR=color;
	LCD_DrawPoint(x-1,y-2);
	LCD_WR_DATA(POINT_COLOR);
	LCD_WR_DATA(POINT_COLOR);
	LCD_DrawPoint(x-2,y-1);
	LCD_WR_DATA(POINT_COLOR);
	LCD_WR_DATA(POINT_COLOR);
	LCD_WR_DATA(POINT_COLOR);
	LCD_WR_DATA(POINT_COLOR);
	LCD_DrawPoint(x-2,y);
	LCD_WR_DATA(POINT_COLOR);
	LCD_WR_DATA(POINT_COLOR);
	LCD_WR_DATA(POINT_COLOR);
	LCD_WR_DATA(POINT_COLOR);
	LCD_DrawPoint(x-2,y+1);
	LCD_WR_DATA(POINT_COLOR);
	LCD_WR_DATA(POINT_COLOR);
	LCD_WR_DATA(POINT_COLOR);
	LCD_WR_DATA(POINT_COLOR);
	LCD_DrawPoint(x-1,y+2);
	LCD_WR_DATA(POINT_COLOR);
	LCD_WR_DATA(POINT_COLOR);

}


/*********************************/
/* �������ܣ���ʾԲ��9*9         */
/*********************************/
void LCD_Draw9Point(uint16_t x, uint16_t y, uint16_t color)
{
	POINT_COLOR=color;
	LCD_DrawPoint(x-1,y-4);
	LCD_WR_DATA(POINT_COLOR);
	LCD_WR_DATA(POINT_COLOR);
	LCD_DrawPoint(x-4,y-1);
	LCD_DrawPoint(x-4,y);
	LCD_DrawPoint(x-4,y+1);
	LCD_DrawPoint(x+4,y-1);
	LCD_DrawPoint(x+4,y);
	LCD_DrawPoint(x+4,y+1);
	LCD_DrawPoint(x-1,y+4);
	LCD_WR_DATA(POINT_COLOR);
	LCD_WR_DATA(POINT_COLOR);
	LCD_Fill(x-3,y-3,x+3,y+3, color);
}

/*******************************/
/* �������ܣ���ʾ��׼��ť      */
/*******************************/
void Draw_Button(uint16_t xstart,uint16_t ystart,uint16_t xend,uint16_t yend)
{
	EscButton(xstart, ystart, xend, yend);
	LCD_Fill(xstart+2, ystart+2, xend-2, yend-2,LGRAY);//����м���ɫ
}

/***********************************/
/* �������ܣ���ʾ��ťѡ��״̬      */
/***********************************/
void SetButton(uint8_t xstart,uint16_t ystart,uint8_t xend,uint16_t yend)
{
	POINT_COLOR=BLACK;
	LCD_DrawLine(xstart, ystart, xend, ystart);//����������1
	LCD_DrawLine(xstart, ystart, xstart, yend);//���������1
	POINT_COLOR=DARKGRAY;
	LCD_DrawLine(xstart+1, ystart+1, xend-1, ystart+1);//����������2
	LCD_DrawLine(xstart+1, ystart+1, xstart+1, yend-1);//���������2
	POINT_COLOR=LGRAY;
	LCD_DrawLine(xstart+1, yend-1, xend-1, yend-1);//���ײ�����1
	LCD_DrawLine(xend-1, ystart+1, xend-1, yend-1);//���ұ�����1	
	POINT_COLOR=WHITE;
	LCD_DrawLine(xstart, yend, xend, yend);//���ײ�����2
	LCD_DrawLine(xend, ystart, xend, yend);//���ұ�����2
}

/***********************************/
/* �������ܣ���ʾ��ťȡ��״̬      */
/***********************************/
void EscButton(uint16_t xstart,uint16_t ystart,uint8_t xend,uint16_t yend)
{
	POINT_COLOR=LGRAY;
	LCD_DrawLine(xstart, ystart, xend, ystart);//����������1
	LCD_DrawLine(xstart, ystart, xstart, yend);//���������1
	POINT_COLOR=WHITE;
	LCD_DrawLine(xstart+1, ystart+1, xend-1, ystart+1);//����������2
	LCD_DrawLine(xstart+1, ystart+1, xstart+1, yend-1);//���������2
	POINT_COLOR=BLACK;
	LCD_DrawLine(xstart, yend, xend, yend);//���ײ�����1
	LCD_DrawLine(xend, ystart, xend, yend);//���ұ�����1
	POINT_COLOR=DARKGRAY;
	LCD_DrawLine(xstart+1, yend-1, xend-1, yend-1);//���ײ�����2
	LCD_DrawLine(xend-1, ystart+1, xend-1, yend-1);//���ұ�����2
}


/***********************************/
/* �������ܣ���ʾһ�����������    */
/***********************************/
void Draw_TextBox(uint16_t xstart, uint16_t ystart, uint16_t xend, uint16_t yend)
{
	POINT_COLOR=DARKGRAY;
	LCD_DrawLine(xstart, ystart, xend, ystart);//����������1
	LCD_DrawLine(xstart, ystart+1, xstart, yend);//����������1
	POINT_COLOR=BLACK;
	LCD_DrawLine(xstart+1, ystart+1, xend-1, ystart+1);//����������2
	LCD_DrawLine(xstart+1, ystart+2, xstart+1, yend-1);//����������2
	POINT_COLOR=WHITE;
	LCD_DrawLine(xstart, yend, xend, yend);//���ײ�����1
	LCD_DrawLine(xend, ystart, xend, yend);//����������1
	POINT_COLOR=LGRAY;
	LCD_DrawLine(xstart+1, yend-1, xend-1, yend-1);//���ײ�����2
	LCD_DrawLine(xend-1, ystart+1, xend-1, yend-1);//����������2
	LCD_Fill(xstart+2, ystart+2, xend-2, yend-2,WHITE);	
}
/***********************************/
/* �������ܣ���ʾһ�����ڣ�        */
/* ˵    ����caption:��������      */
/***********************************/
void Draw_Window(uint16_t xstart,uint16_t ystart,uint8_t xend,uint16_t yend,uint8_t* caption)
{
	Draw_Button(xstart, ystart, xend, yend);   // ��ʾ���崰��
	LCD_Fill(xstart+3, ystart+3, xend-3, ystart+25, DARKBLUE);	 // ��ʾ������
	Draw_TextBox(xstart+3, ystart+29, xend-3, yend-3); // ��ʾ�ı�������
	POINT_COLOR = WHITE;
	BACK_COLOR = DARKBLUE;
	LCD_ShowString(xstart+5, ystart+6, caption);
}

/*******************************************/	  
/* �������ܣ���ʾһ��Frame                 */
/* ˵    �����ڵ��ô˺���ʱ����Ҫ�Ȱ���Ҫ  */
/*           ���õķ�Χ�ڱ���ɫ����ΪLGRAY */
/*******************************************/
void Draw_Frame(uint16_t xstart,uint16_t ystart,uint8_t xend,uint16_t yend,uint8_t *FrameName)
{
 	POINT_COLOR = DARKGRAY;
	LCD_DrawLine(xstart, ystart, xend, ystart);	  // �ϱ߿�
	LCD_DrawLine(xstart, yend, xend, yend);	// �±߿�
	LCD_DrawLine(xstart, ystart, xstart, yend);	// ��߿�
	LCD_DrawLine(xend, ystart, xend, yend);	   // �ұ߿�

	POINT_COLOR = WHITE;
	LCD_DrawLine(xstart+1, ystart+1, xend-1, ystart+1); // �ϱ߿�ƹ�
	LCD_DrawLine(xstart, yend+1, xend+1, yend+1);  // �±߿�ƹ�
	LCD_DrawLine(xstart+1, ystart+1, xstart+1, yend-1);	  // ��߿�ƹ�
	LCD_DrawLine(xend+1, ystart, xend+1, yend+1);		 // �ұ߿�ƹ�

	POINT_COLOR = BLACK;
	BACK_COLOR = LGRAY;
	LCD_ShowString(xstart+5, ystart-6, FrameName);	 // ��ʾFrame����
}

/***********************************/
/* �������ܣ�LOADING.....          */
/***********************************/
void POINT_Demo(void)
{
	uint8_t x=120,y=160,r=20,t,m,i;

	t = 7*r/10;
	m =50;	//��ʾ�ٶȺ���ֵ
	
	LCD_Clear(BLACK);
	POINT_COLOR = WHITE;
	BACK_COLOR = BLACK;
	LCD_ShowString(90, 190, "Loading.....");
	for(i=0;i<2;i++)
	{
		LCD_Draw9Point(x, y-r, WHITE); //����һ����
		delay(m);
		LCD_Draw9Point(x+t, y-t, WHITE); //���ڶ�����
		delay(m);
		LCD_Draw9Point(x, y-r, BLACK); //ɾ����һ����
		delay(m);
		LCD_Draw9Point(x+r, y, WHITE); //����������
		delay(m);
		LCD_Draw9Point(x+t, y-t, BLACK);  //ɾ���ڶ�����
		delay(m);
		LCD_Draw9Point(x+t, y+t, WHITE);  //�����ĸ���
		delay(m);
		LCD_Draw9Point(x+r, y, BLACK); //ɾ����������
		delay(m);
		LCD_Draw9Point(x, y+r, WHITE);  //���������
		delay(m);
		LCD_Draw9Point(x+t, y+t, BLACK);  //ɾ�����ĸ���
		delay(m);
		LCD_Draw9Point(x-t, y+t, WHITE);  //����������
		delay(m);
		LCD_Draw9Point(x, y+r, BLACK);  //ɾ���������
		delay(m);
		LCD_Draw9Point(x-r, y, WHITE);  //�����߸���
		delay(m);
		LCD_Draw9Point(x-t, y+t, BLACK);  //ɾ����������
		delay(m);
		LCD_Draw9Point(x-t, y-t, WHITE);  //���ڰ˸���
		delay(m);
		LCD_Draw9Point(x-r, y, BLACK);  //ɾ�����߸���
		delay(m);
		LCD_Draw9Point(x-t, y-t, BLACK);  //ɾ���ڰ˸���
		delay(m);
	}
}


/******************************************/
/* �������ܣ���ɫ��ʾ���ۿ�ˢ���ٶ�       */
/******************************************/ 
void RGB_Demo(void)
{
	uint8_t i;

	LCD_Clear(BLUE);
	for(i=0;i<3;i++)
	{
	LCD_Fill(0, 0,100,100,YELLOW);
	LCD_Fill(0, 0,120,120,RED);
	LCD_Fill(0, 0,140,140,GREEN);
	LCD_Fill(0, 0,160,160,PINK);
	LCD_Fill(0, 0,180,180,GRAY);
	LCD_Fill(0, 0,200,200,ORANGE);
	LCD_Fill(0, 0,200,200,PORPO);
	LCD_Fill(0, 0,200,200,LGRAYBLUE);
	LCD_Fill(0, 0,200,200,BLUE);
	}
	LCD_Clear(BLUE);
}

/******************************************/
/* �������ܣ��������α���                 */
/******************************************/ 
void BarReport_Demo(void)
{
	uint16_t i;	

	LCD_Clear(BLACK);
	POINT_COLOR = WHITE;
	//��������
	LCD_DrawLine(20, 140, 20, 300);
	LCD_DrawLine(10, 150, 20, 140);
	LCD_DrawLine(30, 150, 20, 140);
	//��������
	LCD_DrawLine(20, 300, 220, 300);
	LCD_DrawLine(210, 290, 220, 300);
	LCD_DrawLine(210, 310, 220, 300);
	//������
	LCD_Fill(35, 170, 55, 299,RED);
	LCD_Fill(75, 220, 95, 299,YELLOW);
	LCD_Fill(115, 150, 135, 299,BLUE);
	LCD_Fill(155, 180, 175, 299,GREEN);
	//���ν���
	delay(50);
	for(i=171;i<299;i++) //��ɫ������
	{
		LCD_Fill(35, 170, 55, i,BLACK);
		delay(10);
	}
	for(i=298;i>190;i--) //��ɫ������
	{
		LCD_Fill(35, i, 55, 299,RED);
		delay(10);
	}
	for(i=219;i>170;i--) //��ɫ������
	{
		LCD_Fill(75, i, 95, 220,YELLOW);
		delay(10);
	}
	for(i=115;i<250;i++) //��ɫ������
	{
		LCD_Fill(115, 114, 135, i,BLACK);
		delay(10);
	}						
}


/******************************************/
/* �������ܣ���������ʾ                   */
/******************************************/ 
void ProgresBar_Demo(void)
{
	uint8_t i,num=1;
		
	LCD_Clear(BLUE);	   // ������ʾ��ɫ
	Draw_Button(100, 210, 230, 310);   // ��ʾ���崰��
	LCD_Fill(103, 213, 227, 235, DARKBLUE);	 // ��ʾ������ 
	Draw_Button(105, 280, 155, 305);  // ��ʾ��һ����ť
	Draw_Button(175, 280, 225, 305);  // ��ʾ�ڶ�����ť
	POINT_COLOR=BLACK;	
	BACK_COLOR=LGRAY;
	LCD_ShowString( 114, 284, "ȷ��"); // ��ť��д��
	LCD_ShowString( 184, 284, "�˳�");
	LCD_ShowString(180, 245, "%");	  
	for(i=126;i<225;i++)
	{
		LCD_Fill(105, 263, i, 278, RED);
		delay(40);
		LCD_ShowNum(162, 245, num,2);
		num++;
	}
}


/******************************************/
/* �������ܣ����ڼ�ͷ����ͼ��(����)       */
/******************************************/ 
void Draw_DirectButton(uint16_t xstart, uint16_t ystart)
{
	POINT_COLOR=BLACK;
	LCD_DrawLine(xstart+6, ystart+8, xstart+14, ystart+8);
	LCD_DrawLine(xstart+7, ystart+9, xstart+13, ystart+9);
	LCD_DrawLine(xstart+8, ystart+10, xstart+12, ystart+10);
	LCD_DrawPoint(xstart+9,ystart+11);LCD_DrawPoint(xstart+10,ystart+11);LCD_DrawPoint(xstart+11,ystart+11);
	LCD_DrawPoint(xstart+10,ystart+12);
}

/******************************************/
/* �������ܣ�comboЧ����ʾ                */
/******************************************/ 
void ComboDemo(void)
{
	LCD_Clear(GRAY);
	//��һ�����������
	Draw_TextBox(50, 50, 200, 73);
	//�������б�ť(19*19)����
	Draw_Button(179, 52, 198, 71);
	Draw_DirectButton(179,52);
	delay(500);
	delay(500);
	delay(500);
	SetButton(179, 52, 198, 71);
	LCD_Fill(183, 56, 194, 67, LGRAY);
   	Draw_DirectButton(180,53);
	delay(500);
	delay(500);
	delay(500);
	EscButton(179, 52, 198, 71);
	LCD_Fill(183, 56, 194, 67, LGRAY);
	Draw_DirectButton(179,52);
	//���������б�
	LCD_DrawRectage(50, 74, 200, 143, BLACK);
	LCD_Fill(51, 75, 199, 142, WHITE);
	//д�б��е�����
	LCD_Fill(51, 75, 199, 97, DARKBLUE); //��һ��Ĭ��Ϊѡ��״̬
	POINT_COLOR=WHITE;
	BACK_COLOR=DARKBLUE;
	LCD_ShowString(53, 79, "NXP ICP Bridge");

	POINT_COLOR=BLACK;
	BACK_COLOR=WHITE;
	LCD_ShowString(53, 101, "NXP PP Bridge");
	LCD_ShowString(53, 124, "None ISP");
	delay(500);
	delay(500);
	delay(500);
	//ѡ�еڶ���
	LCD_Fill(51, 75, 199, 97, WHITE);	   //��ȡ����һ��
	POINT_COLOR=BLACK;
	BACK_COLOR=WHITE;
	LCD_ShowString(53, 79, "NXP ICP Bridge");

	LCD_Fill(51, 98, 199, 120, DARKBLUE);  //ѡ�еڶ���
	POINT_COLOR=WHITE;
	BACK_COLOR=DARKBLUE;
	LCD_ShowString(53, 101, "NXP PP Bridge");
	delay(500);
	delay(500);
	delay(500);
	//ѡ�е�����
	LCD_Fill(51, 98, 199, 120, WHITE);//��ȡ��ѡ�еĵڶ���
	POINT_COLOR=BLACK;
	BACK_COLOR=WHITE;
	LCD_ShowString(53, 101, "NXP PP Bridge");

	LCD_Fill(51, 121, 199, 142, DARKBLUE);//ѡ�е�����
	POINT_COLOR=WHITE;
	BACK_COLOR=DARKBLUE;
	LCD_ShowString(53, 124, "None ISP");
	delay(500);
	delay(500);
	delay(500);
	delay(500);
	//���
	LCD_Fill(50, 74, 200, 143, LGRAY);
}

/******************************************/
/* �������ܣ�WindowЧ����ʾ               */
/******************************************/ 
void Window_Demo(void)
{
	uint16_t xstart=0,ystart=0,xend=239,yend=319;
	uint8_t i=5;

	do
	{
		Draw_Window(xstart,ystart,xend,yend,"������");
		delay(500);
		delay(500);
		xstart+=15;ystart+=15;xend-=30;yend-=30;
	}while(--i);
}

/******************************************/
/* �������ܣ�ButtonЧ����ʾ               */
/******************************************/ 
void Button_Demo(void)
{		
	LCD_Clear(WHITE);
	Draw_TextBox(30, 60, 170, 90);	 // ��ʾһ�����������
	Draw_Button(180, 60, 230, 90);			 // ��ʾ1����ť
	POINT_COLOR = BLACK;
	BACK_COLOR = LGRAY;
	LCD_ShowString(187,67,"����");		 // ��ť��д��
	
	POINT_COLOR = BLUE;
	BACK_COLOR = WHITE;
	LCD_ShowString(30,38,"����");
	LCD_DrawLine(30,55,62,55);
	LCD_ShowString(110,38,"ͼƬ");
	LCD_DrawLine(110,55,142,55);
	POINT_COLOR = BLACK;
	LCD_ShowString(70,38,"��ǩ");
	delay(500);
	delay(500);
	delay(500);
	
	LCD_ShowChar(35, 67, 'C');
	delay(500);
	LCD_ShowChar(43, 67, 'o');
	delay(500);
	LCD_ShowChar(51, 67, 'r');
	delay(500);
	LCD_ShowChar(58, 67, 't');
	delay(500);
	LCD_ShowChar(66, 67, 'e');
	delay(500);
	LCD_ShowChar(74, 67, 'x');
	delay(500);
	LCD_ShowChar(82, 67, '-');
	delay(500);
	LCD_ShowChar(88, 67, 'M');
	delay(500);
	LCD_ShowChar(96, 67, '0');
	delay(500);

	SetButton(180, 60, 230, 90);			 // ���¡���������ť
	POINT_COLOR = BLACK;
	BACK_COLOR = LGRAY;
	LCD_ShowString(188,68,"����");		 
	delay(500);

	EscButton(180, 60, 230, 90);			 // �ſ�����������ť
	POINT_COLOR = BLACK;
	BACK_COLOR = LGRAY;
	LCD_ShowString(187,67,"����");
	POINT_COLOR = BLACK;
	BACK_COLOR = WHITE;		 
	LCD_ShowString(12,100,"ARM Cortex-M0�����������е���С���ܺ���ͺ���Ч��ߵ�ARM���������ô������������С���ܺļ��Ͳ�������Ĵ��������٣���ʹ�ÿ�����Ա�ܹ���8λ���豸ʵ��32λ�豸�����ܣ��Ӷ�ʡ��16λ�豸���з����衣Cortex-M0 ���������͵�����Ҳʹ�������Բ�����ģ��ͻ���ź��豸�С�");
	delay(500);
	delay(500);
	delay(500);
	delay(500);
	delay(500);
	delay(500);
	delay(500);
	delay(500);
	delay(500);
	delay(500);
	delay(500);
	delay(500);
}










