/*
 * @Author: 王营州
 * @Date: 2021-04-09 01:09:18
 * @LastEditTime: 2021-04-27 11:18:20
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \ZET6\HARDWARE\sunflower.h
 */
#ifndef _SUNFLOWER_H_
#define _SUNFLOWER_H_
#include "main.h"
#include "stm32f10x.h"
/***********************代码移植部分,用来检测四个光敏二极管的值****************************/
/*
采光盘正对自己的时候,上面的那个光敏二极管为第0号光敏二极管
模块在无光条件或者光强达不到设定阈值时，DO口输出高电平，当外界环境光强超过设定阈值时，模块D0输出低电平；
因此IO口应该设置为输入上拉模式
*/ 

// 一下内容的值是待定的
// 光敏二极管0
#define SUNFLOWER0_PORT B
#define SUNFLOWER0_PIN 5

// 光敏二极管1
#define SUNFLOWER1_PORT B
#define SUNFLOWER1_PIN 6

// 光敏二极管2
#define SUNFLOWER2_PORT B
#define SUNFLOWER2_PIN 7

// 光敏二极管3
#define SUNFLOWER3_PORT B
#define SUNFLOWER3_PIN 8
/***********************代码移植部分,用来检测四个光敏二极管的值****************************/

extern u8 pinBit[4];
void initSunflower();
void readSunfloerBit(void);

#endif
























