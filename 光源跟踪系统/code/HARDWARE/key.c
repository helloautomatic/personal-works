/*
 * @Author:
 * @Date: 2021-01-17 20:13:26
 * @LastEditTime: 2021-05-17 22:36:39
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \ZET6\HARDWARE\key.c
 */
#include "key.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "delay.h"
#include "exti.h"
#include "stm32f10x_exti.h"
#include "misc.h"
#include "oled.h"
// 以下三个变量用来标识按键是否被按下的标志位
unsigned char FLAG__WK_UP__ = 0;  // 当该按键按下的时候显示一下经纬度信息，就是胡的原始代码
unsigned char FLAG__KEY0__  = 0;  // 用来唤醒时控模式，在开机后，默认是时控模式，所以得是1
unsigned char FLAG__KEY1__  = 1;  // 用来唤醒光控模式，按下该按键后，就进入光控模式了，并显示相应的字符串


// PC1，下降沿触发
void initKey0(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);
    
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


// 按键key0，PC1
void EXTI1_IRQHandler(void)
{
	delay_ms(10);  // 消除抖动
	if(EXTI_GetITStatus(EXTI_Line1)!=RESET)  // 读取GPIOC1端口
	{
        FLAG__WK_UP__ = 0;
        FLAG__KEY0__ = 1;
        FLAG__KEY1__ = 0;
		EXTI_ClearITPendingBit(EXTI_Line1); //清除LINE0上的中断标志位
	}
}


