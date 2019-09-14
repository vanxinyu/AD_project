#include "gui.h"
#include "ili9328.h"

 void delay (int cnt) 
{
  int i;
	i=5000*cnt;
  while (i--);

}
/*********************************/
/* 函数功能：显示圆点5*5         */
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
/* 函数功能：显示圆点9*9         */
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
/* 函数功能：显示标准按钮      */
/*******************************/
void Draw_Button(uint16_t xstart,uint16_t ystart,uint16_t xend,uint16_t yend)
{
	EscButton(xstart, ystart, xend, yend);
	LCD_Fill(xstart+2, ystart+2, xend-2, yend-2,LGRAY);//填充中间颜色
}

/***********************************/
/* 函数功能：显示按钮选中状态      */
/***********************************/
void SetButton(uint8_t xstart,uint16_t ystart,uint8_t xend,uint16_t yend)
{
	POINT_COLOR=BLACK;
	LCD_DrawLine(xstart, ystart, xend, ystart);//画顶部横线1
	LCD_DrawLine(xstart, ystart, xstart, yend);//画左边竖线1
	POINT_COLOR=DARKGRAY;
	LCD_DrawLine(xstart+1, ystart+1, xend-1, ystart+1);//画顶部横线2
	LCD_DrawLine(xstart+1, ystart+1, xstart+1, yend-1);//画左边竖线2
	POINT_COLOR=LGRAY;
	LCD_DrawLine(xstart+1, yend-1, xend-1, yend-1);//画底部横线1
	LCD_DrawLine(xend-1, ystart+1, xend-1, yend-1);//画右边竖线1	
	POINT_COLOR=WHITE;
	LCD_DrawLine(xstart, yend, xend, yend);//画底部横线2
	LCD_DrawLine(xend, ystart, xend, yend);//画右边竖线2
}

/***********************************/
/* 函数功能：显示按钮取消状态      */
/***********************************/
void EscButton(uint16_t xstart,uint16_t ystart,uint8_t xend,uint16_t yend)
{
	POINT_COLOR=LGRAY;
	LCD_DrawLine(xstart, ystart, xend, ystart);//画顶部横线1
	LCD_DrawLine(xstart, ystart, xstart, yend);//画左边竖线1
	POINT_COLOR=WHITE;
	LCD_DrawLine(xstart+1, ystart+1, xend-1, ystart+1);//画顶部横线2
	LCD_DrawLine(xstart+1, ystart+1, xstart+1, yend-1);//画左边竖线2
	POINT_COLOR=BLACK;
	LCD_DrawLine(xstart, yend, xend, yend);//画底部横线1
	LCD_DrawLine(xend, ystart, xend, yend);//画右边竖线1
	POINT_COLOR=DARKGRAY;
	LCD_DrawLine(xstart+1, yend-1, xend-1, yend-1);//画底部横线2
	LCD_DrawLine(xend-1, ystart+1, xend-1, yend-1);//画右边竖线2
}


/***********************************/
/* 函数功能：显示一个文字输入框    */
/***********************************/
void Draw_TextBox(uint16_t xstart, uint16_t ystart, uint16_t xend, uint16_t yend)
{
	POINT_COLOR=DARKGRAY;
	LCD_DrawLine(xstart, ystart, xend, ystart);//画顶部横线1
	LCD_DrawLine(xstart, ystart+1, xstart, yend);//画左面竖线1
	POINT_COLOR=BLACK;
	LCD_DrawLine(xstart+1, ystart+1, xend-1, ystart+1);//画顶部横线2
	LCD_DrawLine(xstart+1, ystart+2, xstart+1, yend-1);//画左面竖线2
	POINT_COLOR=WHITE;
	LCD_DrawLine(xstart, yend, xend, yend);//画底部横线1
	LCD_DrawLine(xend, ystart, xend, yend);//画右面竖线1
	POINT_COLOR=LGRAY;
	LCD_DrawLine(xstart+1, yend-1, xend-1, yend-1);//画底部横线2
	LCD_DrawLine(xend-1, ystart+1, xend-1, yend-1);//画右面竖线2
	LCD_Fill(xstart+2, ystart+2, xend-2, yend-2,WHITE);	
}
/***********************************/
/* 函数功能：显示一个窗口，        */
/* 说    明：caption:标题名称      */
/***********************************/
void Draw_Window(uint16_t xstart,uint16_t ystart,uint8_t xend,uint16_t yend,uint8_t* caption)
{
	Draw_Button(xstart, ystart, xend, yend);   // 显示主体窗口
	LCD_Fill(xstart+3, ystart+3, xend-3, ystart+25, DARKBLUE);	 // 显示标题栏
	Draw_TextBox(xstart+3, ystart+29, xend-3, yend-3); // 显示文本输入区
	POINT_COLOR = WHITE;
	BACK_COLOR = DARKBLUE;
	LCD_ShowString(xstart+5, ystart+6, caption);
}

/*******************************************/	  
/* 函数功能：显示一个Frame                 */
/* 说    明：在调用此函数时，需要先把需要  */
/*           设置的范围内背景色设置为LGRAY */
/*******************************************/
void Draw_Frame(uint16_t xstart,uint16_t ystart,uint8_t xend,uint16_t yend,uint8_t *FrameName)
{
 	POINT_COLOR = DARKGRAY;
	LCD_DrawLine(xstart, ystart, xend, ystart);	  // 上边框
	LCD_DrawLine(xstart, yend, xend, yend);	// 下边框
	LCD_DrawLine(xstart, ystart, xstart, yend);	// 左边框
	LCD_DrawLine(xend, ystart, xend, yend);	   // 右边框

	POINT_COLOR = WHITE;
	LCD_DrawLine(xstart+1, ystart+1, xend-1, ystart+1); // 上边框灯光
	LCD_DrawLine(xstart, yend+1, xend+1, yend+1);  // 下边框灯光
	LCD_DrawLine(xstart+1, ystart+1, xstart+1, yend-1);	  // 左边框灯光
	LCD_DrawLine(xend+1, ystart, xend+1, yend+1);		 // 右边框灯光

	POINT_COLOR = BLACK;
	BACK_COLOR = LGRAY;
	LCD_ShowString(xstart+5, ystart-6, FrameName);	 // 显示Frame名称
}

/***********************************/
/* 函数功能：LOADING.....          */
/***********************************/
void POINT_Demo(void)
{
	uint8_t x=120,y=160,r=20,t,m,i;

	t = 7*r/10;
	m =50;	//显示速度毫秒值
	
	LCD_Clear(BLACK);
	POINT_COLOR = WHITE;
	BACK_COLOR = BLACK;
	LCD_ShowString(90, 190, "Loading.....");
	for(i=0;i<2;i++)
	{
		LCD_Draw9Point(x, y-r, WHITE); //画第一个点
		delay(m);
		LCD_Draw9Point(x+t, y-t, WHITE); //画第二个点
		delay(m);
		LCD_Draw9Point(x, y-r, BLACK); //删除第一个点
		delay(m);
		LCD_Draw9Point(x+r, y, WHITE); //画第三个点
		delay(m);
		LCD_Draw9Point(x+t, y-t, BLACK);  //删除第二个点
		delay(m);
		LCD_Draw9Point(x+t, y+t, WHITE);  //画第四个点
		delay(m);
		LCD_Draw9Point(x+r, y, BLACK); //删除第三个点
		delay(m);
		LCD_Draw9Point(x, y+r, WHITE);  //画第五个点
		delay(m);
		LCD_Draw9Point(x+t, y+t, BLACK);  //删除第四个点
		delay(m);
		LCD_Draw9Point(x-t, y+t, WHITE);  //画第六个点
		delay(m);
		LCD_Draw9Point(x, y+r, BLACK);  //删除第五个点
		delay(m);
		LCD_Draw9Point(x-r, y, WHITE);  //画第七个点
		delay(m);
		LCD_Draw9Point(x-t, y+t, BLACK);  //删除第六个点
		delay(m);
		LCD_Draw9Point(x-t, y-t, WHITE);  //画第八个点
		delay(m);
		LCD_Draw9Point(x-r, y, BLACK);  //删除第七个点
		delay(m);
		LCD_Draw9Point(x-t, y-t, BLACK);  //删除第八个点
		delay(m);
	}
}


/******************************************/
/* 函数功能：颜色显示，观看刷屏速度       */
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
/* 函数功能：动感条形报表                 */
/******************************************/ 
void BarReport_Demo(void)
{
	uint16_t i;	

	LCD_Clear(BLACK);
	POINT_COLOR = WHITE;
	//画纵坐标
	LCD_DrawLine(20, 140, 20, 300);
	LCD_DrawLine(10, 150, 20, 140);
	LCD_DrawLine(30, 150, 20, 140);
	//画横坐标
	LCD_DrawLine(20, 300, 220, 300);
	LCD_DrawLine(210, 290, 220, 300);
	LCD_DrawLine(210, 310, 220, 300);
	//画条形
	LCD_Fill(35, 170, 55, 299,RED);
	LCD_Fill(75, 220, 95, 299,YELLOW);
	LCD_Fill(115, 150, 135, 299,BLUE);
	LCD_Fill(155, 180, 175, 299,GREEN);
	//条形渐变
	delay(50);
	for(i=171;i<299;i++) //红色条降低
	{
		LCD_Fill(35, 170, 55, i,BLACK);
		delay(10);
	}
	for(i=298;i>190;i--) //红色条升高
	{
		LCD_Fill(35, i, 55, 299,RED);
		delay(10);
	}
	for(i=219;i>170;i--) //黄色条升高
	{
		LCD_Fill(75, i, 95, 220,YELLOW);
		delay(10);
	}
	for(i=115;i<250;i++) //蓝色条降低
	{
		LCD_Fill(115, 114, 135, i,BLACK);
		delay(10);
	}						
}


/******************************************/
/* 函数功能：进度条演示                   */
/******************************************/ 
void ProgresBar_Demo(void)
{
	uint8_t i,num=1;
		
	LCD_Clear(BLUE);	   // 整屏显示红色
	Draw_Button(100, 210, 230, 310);   // 显示主体窗口
	LCD_Fill(103, 213, 227, 235, DARKBLUE);	 // 显示标题栏 
	Draw_Button(105, 280, 155, 305);  // 显示第一个按钮
	Draw_Button(175, 280, 225, 305);  // 显示第二个按钮
	POINT_COLOR=BLACK;	
	BACK_COLOR=LGRAY;
	LCD_ShowString( 114, 284, "确定"); // 按钮上写字
	LCD_ShowString( 184, 284, "退出");
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
/* 函数功能：画黑箭头方向图标(向下)       */
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
/* 函数功能：combo效果演示                */
/******************************************/ 
void ComboDemo(void)
{
	LCD_Clear(GRAY);
	//画一个条形输入框
	Draw_TextBox(50, 50, 200, 73);
	//画下拉列表按钮(19*19)像素
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
	//拉出下拉列表
	LCD_DrawRectage(50, 74, 200, 143, BLACK);
	LCD_Fill(51, 75, 199, 142, WHITE);
	//写列表中的内容
	LCD_Fill(51, 75, 199, 97, DARKBLUE); //第一个默认为选中状态
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
	//选中第二个
	LCD_Fill(51, 75, 199, 97, WHITE);	   //先取消第一个
	POINT_COLOR=BLACK;
	BACK_COLOR=WHITE;
	LCD_ShowString(53, 79, "NXP ICP Bridge");

	LCD_Fill(51, 98, 199, 120, DARKBLUE);  //选中第二个
	POINT_COLOR=WHITE;
	BACK_COLOR=DARKBLUE;
	LCD_ShowString(53, 101, "NXP PP Bridge");
	delay(500);
	delay(500);
	delay(500);
	//选中第三个
	LCD_Fill(51, 98, 199, 120, WHITE);//先取消选中的第二个
	POINT_COLOR=BLACK;
	BACK_COLOR=WHITE;
	LCD_ShowString(53, 101, "NXP PP Bridge");

	LCD_Fill(51, 121, 199, 142, DARKBLUE);//选中第三个
	POINT_COLOR=WHITE;
	BACK_COLOR=DARKBLUE;
	LCD_ShowString(53, 124, "None ISP");
	delay(500);
	delay(500);
	delay(500);
	delay(500);
	//清除
	LCD_Fill(50, 74, 200, 143, LGRAY);
}

/******************************************/
/* 函数功能：Window效果演示               */
/******************************************/ 
void Window_Demo(void)
{
	uint16_t xstart=0,ystart=0,xend=239,yend=319;
	uint8_t i=5;

	do
	{
		Draw_Window(xstart,ystart,xend,yend,"标题栏");
		delay(500);
		delay(500);
		xstart+=15;ystart+=15;xend-=30;yend-=30;
	}while(--i);
}

/******************************************/
/* 函数功能：Button效果演示               */
/******************************************/ 
void Button_Demo(void)
{		
	LCD_Clear(WHITE);
	Draw_TextBox(30, 60, 170, 90);	 // 显示一个文字输入框
	Draw_Button(180, 60, 230, 90);			 // 显示1个按钮
	POINT_COLOR = BLACK;
	BACK_COLOR = LGRAY;
	LCD_ShowString(187,67,"搜索");		 // 按钮上写字
	
	POINT_COLOR = BLUE;
	BACK_COLOR = WHITE;
	LCD_ShowString(30,38,"新闻");
	LCD_DrawLine(30,55,62,55);
	LCD_ShowString(110,38,"图片");
	LCD_DrawLine(110,55,142,55);
	POINT_COLOR = BLACK;
	LCD_ShowString(70,38,"标签");
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

	SetButton(180, 60, 230, 90);			 // 按下“搜索”按钮
	POINT_COLOR = BLACK;
	BACK_COLOR = LGRAY;
	LCD_ShowString(188,68,"搜索");		 
	delay(500);

	EscButton(180, 60, 230, 90);			 // 放开“搜索”按钮
	POINT_COLOR = BLACK;
	BACK_COLOR = LGRAY;
	LCD_ShowString(187,67,"搜索");
	POINT_COLOR = BLACK;
	BACK_COLOR = WHITE;		 
	LCD_ShowString(12,100,"ARM Cortex-M0处理器是现有的最小、能耗最低和能效最高的ARM处理器。该处理器硅面积极小、能耗极低并且所需的代码量极少，这使得开发人员能够以8位的设备实现32位设备的性能，从而省略16位设备的研发步骤。Cortex-M0 处理器超低的门数也使得它可以部署在模拟和混合信号设备中。");
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










