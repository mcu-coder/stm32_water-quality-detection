#include "main.h"
 

//串口接收缓存区 	
u8 USART2_RX_BUF[USART2_MAX_RX_LEN]; 				//接收缓冲,最大USART2_MAX_RECV_LEN个字节.
u8 USART2_TX_BUF[USART2_MAX_TX_LEN]; 			  //发送缓冲,最大USART2_MAX_SEND_LEN字节

volatile u16 USART2_RX_STA=0;   	
u8	u2_cmd;
void usart2_init(u32 bound)
{  

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); //串口2时钟使能

 	USART_DeInit(USART2);  //复位串口2
		 //USART2_TX   PA2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA2
   
    //USART2_RX	  PA3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA3
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  
	
	USART_Init(USART2, &USART_InitStructure);     //初始化串口
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//开启串口空闲IDEL中断
	USART_Cmd(USART2, ENABLE);                    //使能串口
//	USART_Init(USART2, &USART_InitStructure); //初始化串口2
//	USART_Cmd(USART2, ENABLE);                    //使能串口 
//	
//	//使能接收中断
//  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断   
//	
//	//设置中断优先级

	
	u2_printf("USART2 OK...\r\n");
//	TIM2_Int_Init(99,7199);		//10ms中断
//	USART2_RX_STA=0;		//清零
//	TIM_Cmd(TIM2,DISABLE);			//关闭定时器4

}

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)               // 空闲中断
	{
		USART_ReceiveData(USART2);                                      //清除空闲中断标志
		USART2_RX_STA_SET();
		//添加处理函数,最好主函数查询处理

	}	
	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET)               // 检查 ORE 标志,防止开关总中断死机，放在接收中断前面
	{
		USART_ClearFlag(USART2,USART_FLAG_ORE);
		USART_ReceiveData(USART2);
	}
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)               //接收中断 
	{
	//	u8 res = USART_ReceiveData(USART3);	                            //读取接收到的数据	
		u2_cmd = USART_ReceiveData(USART2);
		gizPutData(&u2_cmd, 1);//数据写入到缓冲区
	}
}



//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率	  


void u2_printf(char* fmt,...)  
{  

	u16 i = 0;
	va_list arg_ptr;
	va_start(arg_ptr, fmt); 
	vsnprintf((char *)USART2_TX_BUF, USART2_MAX_TX_LEN+1, fmt, arg_ptr);
	va_end(arg_ptr);

	while ((i < USART2_MAX_TX_LEN) && USART2_TX_BUF[i])
	{
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); 
		USART_SendData(USART2, (u8) USART2_TX_BUF[i++]);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC)  == RESET);
	}
}

 
void USART2_RX_STA_SET(void)
{
	if (USART2_RX_BUF[(USART2_RX_STA&0X7FFF)-2]=='\r') 
	{
		USART2_RX_BUF[(USART2_RX_STA&0X7FFF)-2]=0;		                //收到\r\n就删掉\r\n，添加\0
		USART2_RX_STA=USART2_RX_STA-2;                                  //删掉\r\n计数2
	}
	else                                                            
	{ 
		USART2_RX_BUF[(USART2_RX_STA&0X7FFF)]='\0';		                //没收到\r\n，直接添加\0
	}
	USART2_RX_STA|=0x8000;	                                            //标记接收完成了
}
 


















