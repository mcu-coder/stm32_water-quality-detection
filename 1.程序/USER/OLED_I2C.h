#ifndef __OLED_I2C_H
#define	__OLED_I2C_H

//#include "stm32f10x.h"

#define OLED_ADDRESS	0x78 //通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78


//定义IIC总线连接的GPIO端口, 用户只需要修改下面代码即可任意改变SCL和SDA的引脚
//修改引脚时修改  四个地方    
#define OLED_SCL PBout(6)  //SCL引脚              引脚修改
#define OLED_SDA PBout(7)  //SDA引脚							引脚修改

#define OLED_SCL_GPIO_PORT	GPIOB			/* GPIO端口 */
#define OLED_SCL_RCC 	      RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define OLED_SCL_PIN		    GPIO_Pin_6			/* 连接到SCL时钟线的GPIO */				//         引脚修改

#define OLED_SDA_GPIO_PORT	GPIOB			/* GPIO端口 */
#define OLED_SDA_RCC 	      RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define OLED_SDA_PIN		    GPIO_Pin_7			/* 连接到SDA数据线的GPIO */       //         引脚修改


#define OLED_IIC_SDA_READ()  GPIO_ReadInputDataBit(OLED_SDA_GPIO_PORT, OLED_SDA_PIN)	/* 读SDA口线状态 */



void IIC_GPIO_Config(void);
void IIC_Start(void);
void IIC_Stop(void);
uint8_t IIC_WaitAck(void);
void Write_IIC_Byte(uint8_t _ucByte);
void I2C_WriteByte(uint8_t addr,uint8_t data);
void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data);
void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
//基于OLED_ShowStr函数做的反白设置操作  XY为显示坐标  ch显示数据  POS反白起始位置第几个就写几   len反白字符长度  size字体大小
void OLED_ShowStr_S(unsigned char x, unsigned char y, unsigned char ch[], unsigned char POS,unsigned char len,unsigned char TextSize);
void OLED_ShowCC_S(unsigned char x, unsigned char y, unsigned char *s ,unsigned char POS,unsigned char len);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_ShowCC(unsigned char x, unsigned char y, unsigned char *s);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void OLED_Set_Pos(u8 x, u8 y);//设置光标位置
#endif


