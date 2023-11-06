/*
 * @Author: your name
 * @Date: 2021-01-17 20:13:33
 * @LastEditTime: 2021-05-17 19:13:17
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \ZET6\HARDWARE\key.h
 */
#ifndef _KEY_H_
#define _KEY_H_
#include "stm32f10x.h"
#include "main.h"
#define UP U
#define DOWN D

/**************代码移植，这里定义四个按键******************/
// 设置按键C1和C13，因为是外部电路接的是低电平，所以内部应该设置成为上拉输入
#define KEY0_GPIO C
#define KEY0_PIN 1
#define KEY0_MODE UP  // DOWN代表的是输入下拉
#define KEY1_GPIO C
#define KEY1_PIN 13
#define KEY1_MODE UP  // DOWN代表的是输入下拉

// A0按键按下
#define KEY2_GPIO A
#define KEY2_PIN 0
#define KEY2_MODE DOWN  // DOWN代表的是输入下拉


#define KEY3_GPIO A
#define KEY3_PIN 0
#define KEY3_MODE UP  // DOWN代表的是输入下拉
/**************代码移植，这里定义四个按键******************/

#define KEY_MODE_0 0  // 不支持连续按
#define KEY_MODE_1 1  // 支持连续按下

// 10个按键
#define KEY_NULL 0  // 没有按键被按下
#define KEY0 1  // 按键0被按下
#define KEY1 2
#define KEY2 3
#define KEY3 4
#define KEY4 5
#define KEY5 6
#define KEY6 7
#define KEY7 8
#define KEY8 9
#define KEY9 10

void initKey(void);
u8 scanKey(u8 mode);
void runKey(void);
//void initKeyInterrupt(void);
void initKey0(void);
void initKey1(void);
void initWakeUp(void);
extern unsigned char FLAG__WK_UP__;
extern unsigned char FLAG__KEY0__;
extern unsigned char FLAG__KEY1__;


#endif

