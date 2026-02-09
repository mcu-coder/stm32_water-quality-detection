//头文件
#include "main.h"//所有头文件都放在里面方便调用管理
#define turbidity_K 2047.19
//函数声明
void Key_Init(void);//按键函数声明
void display(void);
u8 b_1s,eer_f,smart_config;
u8 mod=0;//显示模式
short set_f; //一级目录编号 设置数据位标记
u32 begin_code[21] = {0, 30,500,4,10,
											500,0,0,0,0,0,0,0,0,0,
											0,0,0,0,66};//备用数组   set_code[19]验证位	
u32 set_code[21];//设置参数
						char str[32];//显示缓存
short beep_io_flag; //蜂鸣器标志位
u32 set_code[21];//设置参数存储数组
u8 run_mod;//工作模式	
float PH,PH_temp;//PH 变量
u16 temperature;//温度存储变量
float TDS_DAT;
float temperature_temp;	//温度存储临时变量
float turbidity,turbidity_temp;//PH值浊度值存储变量
											
float T;											
											
short set_f; //一级目录编号 设置数据位标记

																	
//=============================================================================
//文件名称：KEY
//功能概要：按键服务
//参数说明：无
//函数返回：VOID
//=============================================================================
void key(void)
{
//	int i;
//////////////////////////////////////////开启按键按下

	if(KEY1==0) 
	{
		DelayMs(5);
		if(KEY1==0)//
		{
			while(KEY1==0);			
			switch(mod)
			{
				case 0://待机
							mod = 1 ;//切换到菜单设置
							set_f = 0;
							OLED_CLS();//清屏		
				break;
				case 1://参数设置
							if(set_f>5) set_f = 0;
							else 	set_f++;							
				break;
			}
		}	
	}
	//////////////////////////////////////////++
	if(KEY2==0) 
	{
		DelayMs(5);
		if(KEY2==0)//
		{
			while(KEY2==0);
			
			switch(mod)
			{
				case 0://待机
				//fan_pwm = 100;
				break;
				case 1://设置参数
						switch(set_f)
						{
							case 0:	   

							break;
							case 1:	
								set_code[set_f]++;
								if(set_code[set_f]>80) set_code[set_f] = 0;
							break;
							case 2:	  
								set_code[set_f] = set_code[set_f] + 10;
								if(set_code[set_f]>=2000) set_code[set_f] = 0;
							break;
							case 3:
								set_code[set_f]++;
								if(set_code[set_f]>14) set_code[set_f] = 1;
							break;
							case 4:	
								   set_code[set_f]++;
								   if(set_code[set_f]>14) set_code[set_f] = 1;
							break;
							case 5:	
								   set_code[set_f] = set_code[set_f]+10;
								   if(set_code[set_f]>999) set_code[set_f] = 10;
							break;
						}
				break;

			}					
		}
	}	
	//////////////////////////////////////////--
	if(KEY3==0) 
	{
		DelayMs(5);
		if(KEY3==0)//
		{
			while(KEY3==0);
			switch(mod)
			{
				case 0://待机
						
					//		fan_pwm = 50;
				break;
				case 1://设置参数
						switch(set_f)
						{
							case 0:	  

							break;
							case 1:
								if(set_code[set_f]==0) set_code[set_f] = 80;
									else set_code[set_f] = set_code[set_f] - 1;
							break;
							case 2:	  
								  if(set_code[set_f]<=0) set_code[set_f] = 2000;
									else set_code[set_f] = set_code[set_f]-10;
							break;
							case 3:	
								if(set_code[set_f]==0) set_code[set_f] = 14;
									else set_code[set_f] = set_code[set_f]-1;
							break;
							case 4:	 
								   if(set_code[set_f]==0) set_code[set_f] = 14;
									else set_code[set_f] = set_code[set_f]-1;
							break;
							case 5:	
								   if(set_code[set_f]==0) set_code[set_f] = 1000;
									else set_code[set_f] = set_code[set_f]-10;
							break;
						}								
				break;
			}			
		}
		
	}
	//////////////////////////////////////////返回
	if(KEY4==0) 
	{
		DelayMs(5);
		if(KEY4==0)//
		{
			while(KEY4==0);
			switch(mod)
			{
				case 0://待机
		
				break;
				case 1://参数设置
							Writeflash(0,set_code,0,20);//写入初始化数据
							mod = 0 ;//切换到菜单设置
							OLED_CLS();//清屏						
				break;

			}			
		}
		
	}
}

void display(void)
{
	int i;
	switch(mod)
	{
	 
		///////////////////////////////////////////////////////////////////////////
		case 0://参数设置界面
			
					OLED_ShowCC(0,0,"====参数设置====");//显示标题				
					sprintf(str,"水温:%02d℃  TDS  ",(int)set_code[1]) ;	//显示水温参数
					if(set_f==1) i = 3; else  i = 0;
					OLED_ShowCC_S(0,2,(unsigned char *)str,6,i);//根据设置参数的位置反白
					sprintf(str,"浊度:%04d %04d  ",(int)set_code[2],(int)set_code[5]) ;//显示浊度参数
					if(set_f==2) {i = 4;} 
					else if(set_f==5) i = 4; 
					else  i = 0;		
					OLED_ShowCC_S(0,4,(unsigned char *)str,6+(set_f-2)*1.7,i);//根据设置参数的位置反白
					sprintf(str,"PH_L:%02d PH_H:%02d",(int)set_code[3],(int)set_code[4]) ;//显示PH值下限 与上限
					if((set_f>=3)&(set_f<=4)) i = 2; else  i = 0;
					OLED_ShowStr_S(0,6,(unsigned char *)str,6+(set_f-3)*8,i,2);//根据设置参数的位置反白
							
			break;
	
		}
}

void run(void)
{ 
	 if(turbidity>set_code[2])
	 {
			eer_f = 10;//高于设定值响10声
	 }
	 

}
	
 
 

//=============================================================================
//文件名称：main
//功能概要：主函数
//参数说明：无
//函数返回：int
//=============================================================================
//	RCC_ClocksTypeDef get_rcc_clock;//定义一个结构体检查系统时钟

int main(void)
{	
		u8 i;
	  DelayInit();//系统延时函数初始化
		DelayMs(500);
		usart2_init(9600);//串口2初始化	
		ADC1_DMA_Config();
	 
		Readflash(set_code,0,20);  //读取内部保存数据
	  DelayMs(10);
		 
		smart_config = 1;
		OLED_ShowStr(0,0,"===Device Starting===",1);
		OLED_ShowStr(0,1,"Wait for link Interne",1);//等待网络连接
		OLED_ShowStr(0,2,"=====================",1);		
		OLED_ShowStr(0,3,"KE2   Skip networking",1);//跳过配网
		OLED_ShowStr(0,4,"KE3  WIFI_SOFTAP_MODE",1);//热点配网模式
		OLED_ShowStr(0,5,"KE4 WIFI_AIRLINK_MODE",1);//一键配网模式		
		
		while(smart_config)//配网标志
		{
			gizwitsHandle((dataPoint_t *)&currentDataPoint);//机智云协议处理
			if(KEY2 == 0) smart_config = 0;
			if(KEY3 == 0) 
			{
				OLED_CLS();//清屏
				OLED_ShowStr(0,0,"===Device Starting===",1);
				OLED_ShowStr(0,1,"==WIFI smart_config==",1);
				OLED_ShowStr(0,2,"WIFI_SOFTAP_MODE",1);
				OLED_ShowStr(0,3,"SSID:XPG-GAgne-   ",1);//热点名称
				OLED_ShowStr(0,4,"password:123456789",1);//连接的热点WiFi密码
				OLED_ShowStr(0,5,"Operation on app",1);
				gizwitsSetMode(WIFI_SOFTAP_MODE);//WIFI_SOFTAP_MODE模式接入
			}
			if(KEY4 == 0) 
			{
				OLED_CLS();//清屏
				OLED_ShowStr(0,0,"===Device Starting===",1);
				OLED_ShowStr(0,1,"==WIFI smart_config==",1);
				OLED_ShowStr(0,2,"WIFI_AIRLINK_MODE",1);
				OLED_ShowStr(0,3,"Distribution network",1);
				OLED_ShowStr(0,4," by broadcasting",1);
				OLED_ShowStr(0,5,"Operation on app",1);
				gizwitsSetMode(WIFI_AIRLINK_MODE);//WIFI_SOFTAP_MODE模式接入
			}
			
			OLED_ShowStr(i*8,6,">",1);
			i++;	
			if(i>=15){OLED_ShowStr(0,6,"                     ",1);i = 0;}
			DelayMs(500);
		}
		userInit();
		OLED_CLS();//清屏	
    while (1)
		{	
		 
		  userHandle();		//用户数据采集
		  gizwitsHandle((dataPoint_t *)&currentDataPoint);//机智云协议处理
		}
}
//=============================================================================
//文件名称：Key_Init
//功能概要：GPIO初始化
//参数说明：无
//函数返回：无
//=============================================================================
void Key_Init(void)  //按键引脚定义
{
//将按键接口设置为上拉电阻输入因为我们使用低电平信号触发
	GPIO_InitTypeDef gpio_initstruct;
	
//按键引脚定义		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOC , ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	gpio_initstruct.GPIO_Mode = GPIO_Mode_IPU;							
	gpio_initstruct.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;			
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;							
	GPIO_Init(GPIOC, &gpio_initstruct);
//按键引脚定义 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		
	gpio_initstruct.GPIO_Mode = GPIO_Mode_IPU;							
	gpio_initstruct.GPIO_Pin = GPIO_Pin_0;			
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;							
	GPIO_Init(GPIOA, &gpio_initstruct);	

//蜂鸣器引脚定义	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB , ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	gpio_initstruct.GPIO_Mode = GPIO_Mode_Out_PP;							
	gpio_initstruct.GPIO_Pin = GPIO_Pin_12;			
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;							
	GPIO_Init(GPIOB, &gpio_initstruct);	
	bee_out	 = 0;//蜂鸣器开机置1  不开启
}



