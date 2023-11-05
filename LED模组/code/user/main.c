#include "reg52.h"
#include "intrins.h"  // 延时函数__nop__()所在的头文件
#include "delay.h"
#define T = 100;  // 定义PWM的周期为100个微秒


// RGB引脚控制输出端口定义
sbit LED_R = P2^5;
sbit LED_G = P2^6;
sbit LED_B = P2^7;


// 控制引脚定义
sbit rgbSelect = P2^3;  // 红绿蓝灯的颜色选择
sbit seWen = P2^4;  // 色温选择


unsigned int n = 0;  // 用于控制PWM占空比的一个变量
unsigned char lightLever = 1;  // 用来控制灯亮度的等级，该变量越大，亮度越大，由两个外部中断来控制，最大为10
void PWM_ctrl_RGB(unsigned char RGB_chan);  // PWM控制RGB亮度的算法
void PWM_ctrl_SeWen(unsigned char seWenChan);  // PWM控制色温的算法
unsigned char rgbFlag = 0;  // 0->R,1->G,2->B，默认的是为0
unsigned char seWenFlag = 0;  // 0->暖色，1->一般色温，2->冷色


void main(void)
{   
	// 用于初始化的代码
  LED_R = 1;  // 关灯，蓝
  LED_G = 1;  // 关灯，红
  LED_B = 1;  // 关灯，绿
	EA = 1;  // 允许中断
	EX0 = 1;  // 允许外部中断0中断
	IT0 = 1;  // 允许外部中断0为跳沿触发方式
	EX1 = 1;  // 允许外部中断1的中断
	IT1 = 1;  // 允许外部中断1为跳沿触发方式

//	LED_B = 0;
//	//LED_R = 0;
//	//LED_G = 0;
//	while(1);
	// 通过按键扫描来选择灯的颜色，还有亮度的调节，以及色温的调节
	while(1)
	{
		if((rgbSelect == 0) && (seWen == 1))  // 颜色选择按键被按下，而且色温选择按键没有被按下
		{
			delay_5us();
			delay_5us();
			delay_5us();
			while(rgbSelect == 0);  // 判断按键是否松开
			delay_5us();
			delay_5us();
			delay_5us();
			switch (rgbFlag)
			{
				case 0:
						PWM_ctrl_RGB(0);
				// 这里可能需要加上一个if语句来确定rgbFlag是否需要自增
						rgbFlag ++;
				break;
				
				
				case 1:
						PWM_ctrl_RGB(1);
						rgbFlag ++;
				break;
				
				
				case 2:
						PWM_ctrl_RGB(2);
						rgbFlag ++;
				break;
				
			}
			if(rgbFlag == 3)  // 完成一次循环后，需要重新置为0
			{
				rgbFlag = 0;
			}
		}
		
		
		if((rgbSelect == 1) && (seWen == 0))  // 颜色选择按键没有被按下，色温选择按键被按下
		{
			delay_5us();
			delay_5us();
			delay_5us();
			while(seWen == 0);  // 判断按键是否松开
			delay_5us();
			delay_5us();
			delay_5us();
			switch (seWenFlag)
			{
				case 0:
							PWM_ctrl_SeWen(0);
							seWenFlag ++;
				break;
				
				
				case 1:
							PWM_ctrl_SeWen(1);
							seWenFlag ++;
				break;
				
				
				case 2:
							PWM_ctrl_SeWen(2);
							seWenFlag ++;
				break;
				
			}
			if(seWenFlag == 3)  // 完成一次循环后，需要重新置为0
			{
				seWenFlag = 0;
			}
		}
	}
}


// 该函数可以控制三种色温
void PWM_ctrl_SeWen(unsigned char seWenChan)
{
	switch(seWenChan)
	{	
		case 0:  // 发出暖光
			while((seWen == 1) && (seWenFlag == 0) && (rgbSelect == 1))  // 颜色选择按键没有被按下的时候是被上拉电阻拉高的
			{
				LED_R = 1;LED_G = 1;LED_B = 1;  // 关闭所有的灯
				n = lightLever*10;
				while(n --){_nop_();}
				LED_R = 0;  // 打开红色灯
				
				n = lightLever*10; 
				while(n--){_nop_();}
				LED_G = 0;  // 打开绿色灯
				
				n = lightLever*10;
				while(n--){_nop_();}
				LED_B = 0;  // 打开蓝色灯
				
				n = lightLever*10;
				while(n--){_nop_();}
				// 转到该循环的开始
			}
		break;
	
		
		
		case 1:  // 发出冷光
			while((seWen == 1) && (seWenFlag == 1) && (rgbSelect == 1))
			{
				LED_R = 1;LED_G = 1;LED_B = 1;  // 关闭所有的灯
				n = lightLever*10;
				while(n --){_nop_();}
				LED_B = 0;  // 打开蓝色灯
				
				n = lightLever*10; 
				while(n--){_nop_();}
				LED_G = 0;  // 打开绿色灯
				
				n = lightLever*10;
				while(n--){_nop_();}
				LED_R = 0;  // 打开红色灯
				
				n = lightLever*10;
				while(n--){_nop_();}
				// 转到该循环的开始
			}
		break;
	
		
		case 2:  // 发出白光，这个时候的RGB在理论上是一样的。
			while((seWen == 1) && (seWenFlag == 2) && (rgbSelect == 1))
			{
				LED_R = 1;LED_G = 1;LED_B = 1;  // 关闭所有的灯
				n = lightLever*10;
				while(n--){_nop_();}
				LED_R = 0;LED_G = 0;LED_B = 0;  // 打开所有的灯
				n = 100 - lightLever*10;
				while(n--){_nop_();}
			}
		break;
		}
	
}

void PWM_ctrl_RGB(unsigned char RGB_chan)
{
	/*
	参数解释：
	RGB_chan是选择需要调节LED通道
	*/
	switch(RGB_chan)
	{
		case 0:
				while((rgbFlag == 0) && (rgbSelect == 1) && (seWen == 1))  // rgb选择按键没有被按下，而且色温选择按键没有被按下
				{
					
					LED_R = 0;  // 亮
					n = lightLever*10;
					while(n--)
					{
						_nop_();
					}
					LED_R = 1;  // 灭
					n = 100 - lightLever*10;
					while(n--)
					{
						_nop_();
					}
				}
				// 按键消除抖动
				delay_5us();
				delay_5us();
				delay_5us();
				LED_R = 1;  // 关灯
		break;
		
		
		case 1:
				while((rgbFlag == 1) && (rgbSelect == 1) && (seWen == 1))  // rgb选择按键没有被按下，而且色温选择按键没有被按下
				{
					LED_G = 0;  // 亮
					n = lightLever*10;
					while(n--)
					{
						_nop_();
					}
					LED_G = 1;  // 灭
					n = 100 - lightLever*10;
					while(n--)
					{
						_nop_();
					}
				}	
				// 按键消除抖动
				delay_5us();
				delay_5us();
				delay_5us();
				LED_G = 1;  // 关灯
		break;
		
		case 2:
				while((rgbFlag == 2) && (rgbSelect == 1) && (seWen == 1))  // rgb选择按键没有被按下，而且色温选择按键没有被按下
				{
					LED_B = 0;  // 亮
					n = lightLever*10;
					while(n--)
					{
						_nop_();
					}
					LED_B = 1;  // 灭
					n = 100 - lightLever*10;
					while(n--)
					{
						_nop_();
					}
				}
				// 按键消除抖动
				delay_5us();
				delay_5us();
				delay_5us();
				LED_B = 1;  // 关灯
		break;
	}
}


UP () interrupt 0  using 0  // 外部中断0
{
	// 按键消除抖动
	delay_5us();
	delay_5us();
	delay_5us();
	
	// 判断按键是抬起
	while(IE0 == 1);  // 当按键一直按下时，相当于一直处于中断触发状态，不会跳出该循环，只有松手的时候才能跳出该循环
	
	// 如果亮度增加到了10，就不能再增加了
	if(lightLever <= 9)  // 这里应该是9，不然会出现bug，如果取到10，会导致占空比为0，这不符合逻辑
	{
		lightLever ++;
	}
}


DOWN () interrupt 2  using 2  // 外部中断1
{
	delay_5us();
	delay_5us();
	delay_5us();
	while(IE1 == 1);  // 当按键一直按下时，相当于一直处于中断触发状态，不会跳出该循环，只有松手的时候才能跳出该循环
	if(lightLever > 1)
	{
		lightLever --;
	}
}



