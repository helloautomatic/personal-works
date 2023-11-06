/*
 * @Author: your name
 * @Date: 2021-04-07 21:11:14
 * @LastEditTime: 2021-04-25 14:24:34
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \ZET6\HARDWARE\oled.h
 */
#ifndef __OLED_H
#define __OLED_H 

#include "sys.h"
#include "stdlib.h"	
#include "main.h"
//-----------------OLED�˿ڶ���---------------- 
#define OLED_SCL_PORT C // OLED�˿ڶ���
#define OLED_SCL_PIN 10 // OLED���Ŷ���
#define OLED_SDA_PORT C // OLED�˿ڶ���
#define OLED_SDA_PIN  11 // OLED���Ŷ���


#define OLED_SCLK_Clr() GPIO_ResetBits(__POART__(OLED_SCL_PORT),__PIN__(OLED_SCL_PIN))//SCL
#define OLED_SCLK_Set() GPIO_SetBits(__POART__(OLED_SCL_PORT),__PIN__(OLED_SCL_PIN))

#define OLED_SDIN_Clr() GPIO_ResetBits(__POART__(OLED_SDA_PORT),__PIN__(OLED_SDA_PIN))//DIN
#define OLED_SDIN_Set() GPIO_SetBits(__POART__(OLED_SDA_PORT),__PIN__(OLED_SDA_PIN))


#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

#define FONT_SIZE 16  // �����С�̶�Ϊ16
#define X(x) x*7 // ��Ҫ��ӡ�ַ���x����
#define Y(y) y*7   // ��Ҫ��ӡ�ַ���y����
#define u8 unsigned char
#define u32 unsigned int

void OLED_ClearPoint(u8 x,u8 y);
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_WaitAck(void);
void Send_Byte(u8 dat);
void OLED_WR_Byte(u8 dat,u8 mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y);
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2);
void OLED_DrawCircle(u8 x,u8 y,u8 r);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1);
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1);
void OLED_ScrollDisplay(u8 num,u8 space);
void OLED_WR_BP(u8 x,u8 y);
void OLED_ShowPicture(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[]);
void OLED_Init(void);

#endif


