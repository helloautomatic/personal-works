/*
 * @Author: 
 * @Date: 2021-01-08 17:38:18
 * @LastEditTime: 2021-05-18 11:04:55
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \ZET6\USER\main.c
 */
#include "stm32f10x.h"
#include "delay.h"
#include "stm32f10x_conf.h"
#include "exti.h"
#include "stm32f10x_usart.h"
#include "sys.h"
#include "time.h"
#include "stepperMotor.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
#include "stm32f10x_tim.h"
#include "main.h"
#include "usart.h"
#include "gps.h"
#include "key.h" 
#include "oled.h"
#include "bmp.h"
#include "myiic.h"
#include "stepperMotor.h"
#include "sunflower.h"
#include "timeCtrl.h"


nmea_msg gpsx;  //GPS信息(纬度经度,高度,速度)
__align(4) u8 dtbuf[50];  //打印缓存器


void freshTime()
{
    float tp;		
	// 得到经度字符串Longitude
	tp=gpsx.longitude;
	sprintf((char *)dtbuf,"%.5f %1c   ",tp/=100000,gpsx.ewhemi);
    jingdu = (int)tp;  // 获得经度字符串
	//OLED_ShowString(X(0),Y(0),dtbuf,FONT_SIZE);

    OLED_ShowNum(X(0),Y(0),jingdu,3,FONT_SIZE);  // 显示经度,数字
	OLED_ShowChar(X(4),Y(0),'E',16);

	// 得到纬度字符串Latitude
	tp=gpsx.latitude;
	sprintf((char *)dtbuf,"%.5f %1c   ",tp/=100000,gpsx.nshemi);
    dangqianweidu = (int)tp;  // 获得纬度字符串
	//OLED_ShowString(X(0),Y(2),dtbuf,FONT_SIZE);
    OLED_ShowNum(X(6),Y(0),dangqianweidu,3,FONT_SIZE);
	OLED_ShowChar(X(10),Y(0),'N',16);

	//显示UTC日期,年   月   日
	sprintf((char *)dtbuf,"UTC Date:%04d/%02d/%02d   ",gpsx.utc.year,gpsx.utc.month,gpsx.utc.date);	
    monthGPS = (dtbuf[14] - 48)*10 + dtbuf[15] - 48;  // 获取月份
    dayGPS = (dtbuf[17] - 48)*10 + dtbuf[18] - 48;  // 获取日子
	//OLED_ShowString(X(0),Y(4),&dtbuf[9],FONT_SIZE);
    OLED_ShowNum(X(0),Y(2),monthGPS,2,FONT_SIZE);  // 显示月份
	OLED_ShowChar(X(3),Y(2),'/',16);
    OLED_ShowNum(X(5),Y(2),dayGPS,2,FONT_SIZE);  // 显示日子

	//显示UTC时间,小时  分钟  秒
	sprintf((char *)dtbuf,"UTC Time:%02d:%02d:%02d   ",gpsx.utc.hour,gpsx.utc.min,gpsx.utc.sec);	
    hourGPS = (dtbuf[9] - 48)*10 + dtbuf[10] - 48;  // 获取小时
    minGPS = (dtbuf[12] - 48)*10 + dtbuf[13] - 48;  // 获取分钟
	// OLED_ShowString(X(0),Y(6)+4,&dtbuf[9],FONT_SIZE);
	
	//OLED_ShowString(X(0),Y(4),"UTC:",16);
    OLED_ShowNum(X(0),Y(4),hourGPS,2,FONT_SIZE);  // 显示小时
	OLED_ShowChar(X(3),Y(4),':',16);
    OLED_ShowNum(X(5),Y(4),minGPS,2,FONT_SIZE);  // 显示分钟
	OLED_ShowString(X(8),Y(4),"->",16);

    OLED_Refresh();  // 刷新OLED
}






// // TODO 时控模式算法设计
// int main(void)
// {
// 	u8 t;
// 	int i = 0;
// 	unsigned char j = 0;
// 	unsigned char k = 0;
// 	OLED_Init();
// 	OLED_ColorTurn(0);   // 0正常显示,1 反色显示
//   	OLED_DisplayTurn(0);  // 0正常显示 1 屏幕翻转显示
// 	USART1_Init(9600);  // 初始化串口,用于GPS的通信
// 	IIC_Init();  // 初始化IIC,用于OLED的通信
// 	initStepperMotor();  // 初始化步进电机
// 	delay_ms(100);  // 个人感觉需要延时一下,给各器件初始化腾出来时间
// 	while(1)
// 	{
// 		if(i == 10000)
// 		{
// 			// 显示GPS定位信息
// 			GPS_Analysis(&gpsx,(u8*)USART1_RX_BUF);  // 分析字符串,提取数据
// 			//Gps_Msg_Show();  // 显示信息	
// 			freshTime();  // 刷新显示
// 			getZhiSheWeidu();  // 得到直射纬度
// 			i=0;
// 		}
// 		i++;
// 	}
// }




// 主函数						
int main()
{
	u8 t;
	int i = 0;
	unsigned char j = 0;
	unsigned char k = 0;
	OLED_Init();
	OLED_ColorTurn(0);   //0正常显示，1 反色显示
  	OLED_DisplayTurn(0);  //0正常显示 1 屏幕翻转显示
	USART1_Init(9600);  // 初始化串口
	initStepperMotor();  // 初始化步进电机

	delay_ms(100);  // 个人感觉需要延时一下,给各器件初始化腾出来时间
	OLED_Refresh();
	delay_ms(100);  // 等待一下其它软件
	initSunflower();  // 初始化向日葵

	while(1)  // 该循环中，有两个空式模型
	{
		readSunfloerBit();  // 读取向日葵

		//判断上面的电机是否要转动
		if(pinBit[1] != pinBit[3])
		{
			if(pinBit[1]== 0) // 上面的电机正转
			{
				STEPPER1_ZHEN();
			}
			else  // 上面的电机反转
			{
				STEPPER1_FAN();
			}
		}
		// 判断下面的电机是否需要转动
		if(pinBit[0] != pinBit[2])
		{
			if(pinBit[0] == 0)
			{
				STEPPER2_ZHEN();  // 正转是顺时针
			}
			else
			{
				STEPPER2_FAN();  // 反转是逆时针
			}
		}
		
		for(k = 0;k <4;k++){pinBit[k] = 0;}

		if(i == 10000)
		{
			// 显示GPS定位信息
			GPS_Analysis(&gpsx,(u8*)USART1_RX_BUF);  // 分析字符串,提取数据
			//Gps_Msg_Show();  // 显示信息	
			freshTime();  // 刷新显示
			getZhiSheWeidu();  // 得到直射纬度，这是在最后一行显示的
			getSunlightGaodu();  // 得到其余纬度的太阳高度角
			lastGetJiaodu();
			i=0;
		}
		i++;
	}
	while(1);
	return 0;
}



//步进电机测试代码

//int main()
//{
//	int i = 0;
//	initStepperMotor();  // 初始化步进电机
//	STEPPER2_ZHEN();  // TODO 在使用这个东西的时候,一定要声明一下i,这也是非常容易出bug的地方.
//	while(1);
//}



// LED测试程序
// int main()
// {
// 	initLed();
// 	runLed();
// }




//  //步进电机测试程序
//  int main(void)
//  {
//  	initStepperMotor();  // 初始化步进电机
//  	while(1)
//  	{
//  		stepperRun1(STEPPER_REVERSAL,90);
//  		stepperRun2(STEPPER_FORWARD,90);
//  		STOP_STEPPER2();
//  		STOP_STEPPER1();
//  		stepperRun1(STEPPER_FORWARD,90);
//  		stepperRun2(STEPPER_REVERSAL,90);
//  		STOP_STEPPER2();
//  		STOP_STEPPER1();
//  		delay_ms(10);
//  	}
//  	while(1);
//  }



