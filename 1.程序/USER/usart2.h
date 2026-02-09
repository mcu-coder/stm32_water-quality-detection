#ifndef __USART2_H
#define __USART2_H	 
//#include "sys.h"  

//#define USART2_MAX_RECV_LEN		400					//最大接收缓存字节数
//#define USART2_MAX_SEND_LEN		400					//最大发送缓存字节数

#define     USART2_MAX_TX_LEN		    500   //最大发送缓存字节数，刨去\0 最大-1
#define     USART2_MAX_RX_LEN		    1000   //最大接收缓存字节数，刨去\0 最大-1
#define USART2_RX_EN 			1					//0,不接收;1,接收.

extern u8  USART2_RX_BUF[USART2_MAX_RX_LEN]; 		//接收缓冲,最大USART2_MAX_RECV_LEN字节
extern u8  USART2_TX_BUF[USART2_MAX_TX_LEN]; 		//发送缓冲,最大USART2_MAX_SEND_LEN字节
extern volatile u16 USART2_RX_STA;   						//接收数据状态

void usart2_init(u32 bound);				//串口2初始化 
void u2_printf(char* fmt,...);
void USART2_RX_STA_SET(void);
#endif













