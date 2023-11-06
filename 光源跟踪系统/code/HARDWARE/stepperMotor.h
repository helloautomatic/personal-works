#ifndef _STEPPERMOTOR_H
#define _STEPPERMOTOR_H
#include "stm32f10x.h"
#include "main.h"


// TODO 系统摆放位置说明
/*
 * step1,上面的电机,正转,右,转轴正对自己
 * step2,下面的电机,正转,顺时针,线段朝向外面
 * /


/**********************************代码移植****************************************/
/*
    模式为推挽输出，需要四个引脚，这里默认的是E7~E10
*/
// 步进电机1,上面的步进电机
#define STEPPER1_IN1_GPIO C
#define STEPPER1_IN1_PIN 0
#define STEPPER1_IN2_GPIO C
#define STEPPER1_IN2_PIN 1
#define STEPPER1_IN3_GPIO C
#define STEPPER1_IN3_PIN 2
#define STEPPER1_IN4_GPIO C
#define STEPPER1_IN4_PIN 3
// 步进电机2,下面的步进电机
#define STEPPER2_IN1_GPIO C
#define STEPPER2_IN1_PIN 6
#define STEPPER2_IN2_GPIO C
#define STEPPER2_IN2_PIN 7
#define STEPPER2_IN3_GPIO C
#define STEPPER2_IN3_PIN 8
#define STEPPER2_IN4_GPIO C
#define STEPPER2_IN4_PIN 9
/*************************************代码移植**********************************/


#define IN1_GPIO STEPPER1_IN1_GPIO
#define IN1_PIN STEPPER1_IN1_PIN
#define IN2_GPIO STEPPER1_IN2_GPIO
#define IN2_PIN STEPPER1_IN2_PIN
#define IN3_GPIO STEPPER1_IN3_GPIO
#define IN3_PIN STEPPER1_IN3_PIN
#define IN4_GPIO STEPPER1_IN4_GPIO
#define IN4_PIN STEPPER1_IN4_PIN

#define SPEED  2  // 转动速度
#define STEPPER_UP 1  // 驱动上面的电机
#define STEPPER_DOWN 0  // 驱动下面的电机
#define STEPPER_FORWARD 1  // 电机正转
#define STEPPER_REVERSAL 0  // 电机反转
#define ONE_PULSE 5.625  // 一个脉冲转5.625°
#define DEGREE 10  // 每次转动的角度
#define WUCHA 25  // 定义误差范围




#define STOP() GPIO_ResetBits(__POART__(IN1_GPIO),__PIN__(IN1_PIN));\
               GPIO_ResetBits(__POART__(IN2_GPIO),__PIN__(IN2_PIN));\
               GPIO_ResetBits(__POART__(IN3_GPIO),__PIN__(IN3_PIN));\
               GPIO_ResetBits(__POART__(IN4_GPIO),__PIN__(IN4_PIN));

// 步进电机1停转
// 步进电机2停转
#define STOP_STEPPER1() STOP()  
#define STOP_STEPPER2() GPIO_ResetBits(__POART__(STEPPER2_IN1_GPIO),__PIN__(STEPPER2_IN1_PIN));\
                        GPIO_ResetBits(__POART__(STEPPER2_IN2_GPIO),__PIN__(STEPPER2_IN2_PIN));\
                        GPIO_ResetBits(__POART__(STEPPER2_IN3_GPIO),__PIN__(STEPPER2_IN3_PIN));\
                        GPIO_ResetBits(__POART__(STEPPER2_IN4_GPIO),__PIN__(STEPPER2_IN4_PIN));

//TODO 步进电机1正转,一定要注意在使用下面的函数,i是未被定义的
#define STEPPER1_ZHEN() for(i = 0;i < 4; i++)                                \
{                                                                            \
GPIO_WriteBit(__POART__(IN1_GPIO),__PIN__(IN1_PIN),ab_bc_cd_da_abShun[i][0]);\
GPIO_WriteBit(__POART__(IN2_GPIO),__PIN__(IN2_PIN),ab_bc_cd_da_abShun[i][1]);\
GPIO_WriteBit(__POART__(IN3_GPIO),__PIN__(IN3_PIN),ab_bc_cd_da_abShun[i][2]);\
GPIO_WriteBit(__POART__(IN4_GPIO),__PIN__(IN4_PIN),ab_bc_cd_da_abShun[i][3]);\
delay_ms(SPEED);                                                             \
}
//TODO 步进电机1反转,一定要注意在使用这个地方的时候,i是未被定义的,所以一定要注意 
#define STEPPER1_FAN() for(i = 0;i < 4; i++)                               \
{                                                                          \
GPIO_WriteBit(__POART__(IN1_GPIO),__PIN__(IN1_PIN),ab_bc_cd_da_abNi[i][0]);\
GPIO_WriteBit(__POART__(IN2_GPIO),__PIN__(IN2_PIN),ab_bc_cd_da_abNi[i][1]);\
GPIO_WriteBit(__POART__(IN3_GPIO),__PIN__(IN3_PIN),ab_bc_cd_da_abNi[i][2]);\
GPIO_WriteBit(__POART__(IN4_GPIO),__PIN__(IN4_PIN),ab_bc_cd_da_abNi[i][3]);\
delay_ms(SPEED);                                                           \
}
//TODO 步进电机2正转,一定要注意在使用这个地方的时候,i是未被定义的,所以一定要注意 
#define STEPPER2_ZHEN() for(i = 0;i < 4; i++)\
{\
GPIO_WriteBit(__POART__(STEPPER2_IN1_GPIO),__PIN__(STEPPER2_IN1_PIN),ab_bc_cd_da_abShun[i][0]);\
GPIO_WriteBit(__POART__(STEPPER2_IN2_GPIO),__PIN__(STEPPER2_IN2_PIN),ab_bc_cd_da_abShun[i][1]);\
GPIO_WriteBit(__POART__(STEPPER2_IN3_GPIO),__PIN__(STEPPER2_IN3_PIN),ab_bc_cd_da_abShun[i][2]);\
GPIO_WriteBit(__POART__(STEPPER2_IN4_GPIO),__PIN__(STEPPER2_IN4_PIN),ab_bc_cd_da_abShun[i][3]);\
delay_ms(SPEED);\
}
//TODO 步进电机2反转,一定要注意在使用这个地方的时候,i是未被定义的,所以一定要注意 
#define STEPPER2_FAN() for(i = 0;i < 4; i++)\
{\
GPIO_WriteBit(__POART__(STEPPER2_IN1_GPIO),__PIN__(STEPPER2_IN1_PIN),ab_bc_cd_da_abNi[i][0]);\
GPIO_WriteBit(__POART__(STEPPER2_IN2_GPIO),__PIN__(STEPPER2_IN2_PIN),ab_bc_cd_da_abNi[i][1]);\
GPIO_WriteBit(__POART__(STEPPER2_IN3_GPIO),__PIN__(STEPPER2_IN3_PIN),ab_bc_cd_da_abNi[i][2]);\
GPIO_WriteBit(__POART__(STEPPER2_IN4_GPIO),__PIN__(STEPPER2_IN4_PIN),ab_bc_cd_da_abNi[i][3]);\
delay_ms(SPEED);\
}


#define WU_CHA_JINGDU_Z 10  // 
#define WU_CHA_JINGDU_F -10  // 


extern u32 a_b_c_d_aZheng[4];
extern u32 a_b_c_d_aFan[4];
extern u32 ab_bc_cd_da_abZheng[4];
extern u32 ab_bc_cd_da_abFan[4];
extern u32 a_ab_b_bc_c_cd_d_da_aZheng[8];
extern u32 a_ab_b_bc_c_cd_d_da_aFan[8];
extern u8 ab_bc_cd_da_abShun[4][4];
extern u8 ab_bc_cd_da_abNi[4][4];
void stepperRun1(u8 slectReverseForward, float stepperDegree);
void stepperRun2(u8 slectReverseForward, float stepperDegree);
void initStepperMotor(void);
#endif

