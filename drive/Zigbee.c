#include "Zigbee.h"
#include "application.h"
#include "stdlib.h"
#include "string.h"

void zigbee_init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		NVIC_InitTypeDef   NVIC_InitStructure;
		USART_InitTypeDef USART_InitStructure2;
						
		RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE );       

		GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_7);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_7);

		/*
			 USART2_TX -> PA2 , USART2_RX ->        PA3
			 ZIG_READY -> PA4 , ZIG_RST -> PA5 , WAKEUP_OUT  -> PA1
				
		*/                                
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;                 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_5;                 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(GPIOA, &GPIO_InitStructure);    
		

		USART_InitStructure2.USART_BaudRate = 115200;//设置串口波特率
		USART_InitStructure2.USART_WordLength = USART_WordLength_8b;//设置数据位
		USART_InitStructure2.USART_StopBits = USART_StopBits_1;//设置停止位
		USART_InitStructure2.USART_Parity = USART_Parity_No;//设置效验位
		USART_InitStructure2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//设置流控制
		USART_InitStructure2.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//设置工作模式
		USART_Init(USART2, &USART_InitStructure2); //配置入结构体

		USART_Cmd(USART2, ENABLE);//使能串口2

		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断

		//Usart1 NVIC 配置
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口1中断通道
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
		
}
uint16_t zigbee_reset(void)
{
	u8 res = FAILE;
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
	res = SUCCESS;
	return res;
}
uint16_t zigbee_wakeup(void)
{
	u8 res = FAILE;
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	Delay_ms(1);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	res = SUCCESS;
	return res;
}
uint16_t zigbee_send_hook(uint8_t * buf,uint16_t len)
{
	u8 res = FAILE;
	zigbee_wakeup();
	if(zigbee_ready)
	{
		UART2_Send(buf,len);
		res = SUCCESS;
	}
	return res;
}

	/**
* \brief analysis command 
* \param command_buf   the command wait for handle 
						ad_start,(time); (time) Refers to the ad sampling for the duration of time;eg  ad_start,15 (seconds)
						ad_read,(RTC TIME)-(RTC TIME) ;read the ad sample data in sd card from (RTC TIME1) to (RTC TIME2);eg ad_read,2019/8/13/10:13-2019/8/13/10:14
		len             the length of command
* \return command's head and time
*/
uint8_t zigbee_recv_hook(uint8_t command_buf[],uint8_t len)
{
	u8 res = FAILE;
	static uint8_t state=head_flag;
	uint8_t index;
	uint8_t temp;
	u8 i=0;
	command_t* command_p= (command_t *)malloc(sizeof(command_t)+len);
	memset(command_p,0,sizeof(command_t)+len);
	for(index=0;index<len;index++)
	{
		temp=command_buf[index];
		switch(state)
		{
			case head_flag:
				if(temp==':')
				{
					state = mac_flag;
				}
				else
				{
					command_p->head[index]=temp;
					if(index==len-1)
					{
						printf("unexpected command: no ':'");
						UART2_Send((u8*)"ERROR:1",sizeof("ERROR:1"));
						free(command_p);
						state=head_flag;
						return res;
					}
				}
				break;
			case mac_flag:
				if(temp==',')
				{
					if(strcmp(command_p->head,"AT+SETTIME")==0)
					{
						printf("head :%s\r\n",command_p->head);
						state = time_flag; i=index+1; 
					}
					else if(strcmp(command_p->head,"AT+START")==0)
					{
						printf("head :%s\r\n",command_p->head);
						state = time_begin_flag;i=index+1;
					}
					else if(strcmp(command_p->head,"AT+READDATA")==0)
					{
						printf("head :%s\r\n",command_p->head);
						state = time_flag;i=index+1;
					}
					else
					{
						UART2_Send((u8*)"ERROR:0",sizeof("ERROR:0"));
						free(command_p);
						state=head_flag;
						return res;
					}
				}
				break;
			case time_flag:
				if((temp>='0')&&(temp<='9'))
				{	
					if(index>=i&&index<=i+3)
					{
						command_p->time.year *=10;
						command_p->time.year +=temp-'0';
					}
						if(index>=i+4&&index<=i+5)
					{
						command_p->time.month *=10;
						command_p->time.month +=temp-'0';
					}
						if(index>=i+6&&index<=i+7)
					{
						command_p->time.day *=10;
						command_p->time.day +=temp-'0';
					}
						if(index>=i+8&&index<=i+9)
					{
						command_p->time.hour *=10;
						command_p->time.hour +=temp-'0';
					}
						if(index>=i+10&&index<=i+11)
					{
						command_p->time.minte *=10;
						command_p->time.minte +=temp-'0';
					}
						if(index>=i+12&&index<=i+13)
					{
						command_p->time.second *=10;
						command_p->time.second +=temp-'0';
					}
				}
				if(index==len-1)
				{
					if(timet_checking(command_p->time))
					{
						UART2_Send((u8*)"ERROR:2",sizeof("ERROR:2"));
						state=head_flag;
						return res;
					}
					printf("year:%d\r\n",command_p->time.year);
					printf("month:%d\r\n",command_p->time.month);
					printf("day:%d\r\n",command_p->time.day);
					printf("hour:%d\r\n",command_p->time.hour);
					printf("minte:%d\r\n",command_p->time.minte);
					printf("second:%d\r\n",command_p->time.second);
					i=0;
					state=head_flag;
				}
				break;
				
				case time_begin_flag:
				 if(temp==',')
				 {
					 state=time_end_flag;i=index+1; 
					 printf("start_time year:%d\r\n",command_p->start_time.year);
					 printf("start_time month:%d\r\n",command_p->start_time.month);
					 printf("start_time day:%d\r\n",command_p->start_time.day);
					 printf("start_time hour:%d\r\n",command_p->start_time.hour);
					 printf("start_time minte:%d\r\n",command_p->start_time.minte);
					 printf("start_time second:%d\r\n",command_p->start_time.second);
				 }
				 if((temp>='0')&&(temp<='9'))
				 {	
						if(index>=i&&index<=i+3)
						{
							command_p->start_time.year *=10;
							command_p->start_time.year +=temp-'0';
						}
							if(index>=i+4&&index<=i+5)
						{
							command_p->start_time.month *=10;
							command_p->start_time.month +=temp-'0';
						}
							if(index>=i+6&&index<=i+7)
						{
							command_p->start_time.day *=10;
							command_p->start_time.day +=temp-'0';
						}
							if(index>=i+8&&index<=i+9)
						{
							command_p->start_time.hour *=10;
							command_p->start_time.hour +=temp-'0';
						}
							if(index>=i+10&&index<=i+11)
						{
							command_p->start_time.minte *=10;
							command_p->start_time.minte +=temp-'0';
						}
							if(index>=i+12&&index<=i+13)
						{
							command_p->start_time.second *=10;
							command_p->start_time.second +=temp-'0';
						}
				 }
				if(index==len-1)
				{
					UART2_Send((u8*)"ERROR:3",sizeof("ERROR:3"));
					state=head_flag;
					return res;
				}
				break;
				
				case time_end_flag:
				 if((temp>='0')&&(temp<='9'))
				 {	
						if(index>=i&&index<=i+3)
						{
							command_p->end_time.year *=10;
							command_p->end_time.year +=temp-'0';
						}
							if(index>=i+4&&index<=i+5)
						{
							command_p->end_time.month *=10;
							command_p->end_time.month +=temp-'0';
						}
							if(index>=i+6&&index<=i+7)
						{
							command_p->end_time.day *=10;
							command_p->end_time.day +=temp-'0';
						}
							if(index>=i+8&&index<=i+9)
						{
							command_p->end_time.hour *=10;
							command_p->end_time.hour +=temp-'0';
						}
							if(index>=i+10&&index<=i+11)
						{
							command_p->end_time.minte *=10;
							command_p->end_time.minte +=temp-'0';
						}
							if(index>=i+12&&index<=i+13)
						{
							command_p->end_time.second *=10;
							command_p->end_time.second +=temp-'0';
						}
				 }
				if(index==len-1)
				{
					 state=head_flag;
					 printf("end_time year:%d\r\n",command_p->end_time.year);
					 printf("end_time month:%d\r\n",command_p->end_time.month);
					 printf("end_time day:%d\r\n",command_p->end_time.day);
					 printf("end_time hour:%d\r\n",command_p->end_time.hour);
					 printf("end_time minte:%d\r\n",command_p->end_time.minte);
					 printf("end_time second:%d\r\n",command_p->end_time.second);
				}
				break;
		}
	}
	if(command_p)
	{
		printf("command analyse ok ");
		Command_msg_handler(command_p);
		free(command_p);
  }
		state=head_flag;
		res = SUCCESS;
		return res;
}
uint16_t zigbee_wakeup_hook(void)
{
	//zigbee??????
}
