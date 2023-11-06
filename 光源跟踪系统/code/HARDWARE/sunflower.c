/*
 * @Author: 
 * @Date: 2021-04-09 01:09:12
 * @LastEditTime: 2021-04-09 02:22:43
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \ZET6\HARDWARE\sunflower.c
 */

// 该部分的代码可以使用LED来测试,有可以通过OLED来测试
#include "sunflower.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "main.h"
#include "stm32f10x.h"
u8 pinBit[4] = {0,0,0,0};  // 用来存放读取到的开关量


void initSunflower(void)
{
    GPIO_InitTypeDef sunflowerInitstructure;  
	RCC_APB2PeriphClockCmd(
		__RCC_APB2__(SUNFLOWER0_PORT)|
		__RCC_APB2__(SUNFLOWER1_PORT)|
		__RCC_APB2__(SUNFLOWER2_PORT)|
		__RCC_APB2__(SUNFLOWER3_PORT)
	,ENABLE);
	sunflowerInitstructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入模式

    // 光敏二极管0
	sunflowerInitstructure.GPIO_Pin = __PIN__(SUNFLOWER0_PIN);
	GPIO_Init(__POART__(SUNFLOWER0_PORT),&sunflowerInitstructure);

    // 光敏二极管1
	sunflowerInitstructure.GPIO_Pin = __PIN__(SUNFLOWER1_PIN);
	GPIO_Init(__POART__(SUNFLOWER1_PORT),&sunflowerInitstructure);

    // 光敏二极管2
    sunflowerInitstructure.GPIO_Pin = __PIN__(SUNFLOWER2_PIN);
	GPIO_Init(__POART__(SUNFLOWER2_PORT),&sunflowerInitstructure);

    // 光敏二极管3
    sunflowerInitstructure.GPIO_Pin = __PIN__(SUNFLOWER3_PIN);
	GPIO_Init(__POART__(SUNFLOWER3_PORT),&sunflowerInitstructure);
}

void readSunfloerBit(void)
{
    /*
        该函数一旦被调用,就会对开关量进行采集.
    */
    pinBit[0] = GPIO_ReadInputDataBit(__POART__(SUNFLOWER0_PORT),__PIN__(SUNFLOWER0_PIN));
    pinBit[1] = GPIO_ReadInputDataBit(__POART__(SUNFLOWER1_PORT),__PIN__(SUNFLOWER1_PIN));
    pinBit[2] = GPIO_ReadInputDataBit(__POART__(SUNFLOWER2_PORT),__PIN__(SUNFLOWER2_PIN));
    pinBit[3] = GPIO_ReadInputDataBit(__POART__(SUNFLOWER3_PORT),__PIN__(SUNFLOWER3_PIN));
}











