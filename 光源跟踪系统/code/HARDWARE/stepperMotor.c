#include "stm32f10x.h"
#include "delay.h"
#include "key.h"
#include "exti.h"
#include "exti.h"
#include "usart.h"
#include "sys.h"
#include "time.h"
#include "stepperMotor.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"


/*  使用寄存器操作  */
// ABCDA相序 这个相序的工作情况不太好，没有下面的好，可能和驱动电流关系
u32 a_b_c_d_aZheng[4] = {0X0080,0X0100,0X0200,0x0400};
u32 a_b_c_d_aFan[4] = {0x0400,0X0200,0X0100,0X0080};

// AB BC CD DA AB相序 这个相序的工作情况最好
u32 ab_bc_cd_da_abZheng[4] = {0X0180,0X0300,0X0600,0X0480};
u32 ab_bc_cd_da_abFan[4] = {0X0480,0X0600,0X0300,0X0180};

// A AB B BC C CD D DA A相序 这个相序不能正常工作，不知道为什么
u32 a_ab_b_bc_c_cd_d_da_aZheng[8] = 
{0X0080,0X0180,0X0100,0X0300,0X0200,0X0600,0X0400,0X0410};
u32 a_ab_b_bc_c_cd_d_da_aFan[8] = 
{0X0410,0X0400,0X0600,0X0200,0X0300,0X0100,0X0180,0X0080};

/*  使用库函数操作  */
u8 ab_bc_cd_da_abShun[4][4] = 
{
    {1,1,0,0},
    {0,1,1,0},
    {0,0,1,1},
    {1,0,0,1}
};

u8 ab_bc_cd_da_abNi[4][4] = 
{
    {1,0,0,1},
    {0,0,1,1},
    {0,1,1,0},
    {1,1,0,0}
};


void stepperRun1(u8 slectReverseForward, float stepperDegree)
{
    u8 i = 0;
    u32 j = 0;
    u32 pulseNumber = (stepperDegree/ONE_PULSE)*8;

    // 正转
    if(slectReverseForward == STEPPER_FORWARD)
    {
        for(j = 0;j < pulseNumber;j ++)
        {
            for(i = 0;i < 4; i++)
            {
                GPIO_WriteBit(__POART__(IN1_GPIO),__PIN__(IN1_PIN),ab_bc_cd_da_abShun[i][0]);
                GPIO_WriteBit(__POART__(IN2_GPIO),__PIN__(IN2_PIN),ab_bc_cd_da_abShun[i][1]);
                GPIO_WriteBit(__POART__(IN3_GPIO),__PIN__(IN3_PIN),ab_bc_cd_da_abShun[i][2]);
                GPIO_WriteBit(__POART__(IN4_GPIO),__PIN__(IN4_PIN),ab_bc_cd_da_abShun[i][3]);
				delay_ms(SPEED);
            }
        }
    }
    i = 0;
    // 反转
    if(slectReverseForward == STEPPER_REVERSAL)
    {
        for(j = 0;j < pulseNumber;j ++)
        {
            for(i = 0;i < 4; i++)
            {
                GPIO_WriteBit(__POART__(IN1_GPIO),__PIN__(IN1_PIN),ab_bc_cd_da_abNi[i][0]);
                GPIO_WriteBit(__POART__(IN2_GPIO),__PIN__(IN2_PIN),ab_bc_cd_da_abNi[i][1]);
                GPIO_WriteBit(__POART__(IN3_GPIO),__PIN__(IN3_PIN),ab_bc_cd_da_abNi[i][2]);
                GPIO_WriteBit(__POART__(IN4_GPIO),__PIN__(IN4_PIN),ab_bc_cd_da_abNi[i][3]);
				delay_ms(SPEED);
            }
        }
    }
}

void stepperRun2(u8 slectReverseForward, float stepperDegree)
{
    u8 i = 0;
    u32 j = 0;
    u32 pulseNumber = (stepperDegree/ONE_PULSE)*8;

    // 正转
    if(slectReverseForward == STEPPER_FORWARD)
    {
        for(j = 0;j < pulseNumber;j ++)
        {
            for(i = 0;i < 4; i++)
            {
                GPIO_WriteBit(__POART__(STEPPER2_IN1_GPIO),__PIN__(STEPPER2_IN1_PIN),ab_bc_cd_da_abShun[i][0]);
                GPIO_WriteBit(__POART__(STEPPER2_IN2_GPIO),__PIN__(STEPPER2_IN2_PIN),ab_bc_cd_da_abShun[i][1]);
                GPIO_WriteBit(__POART__(STEPPER2_IN3_GPIO),__PIN__(STEPPER2_IN3_PIN),ab_bc_cd_da_abShun[i][2]);
                GPIO_WriteBit(__POART__(STEPPER2_IN4_GPIO),__PIN__(STEPPER2_IN4_PIN),ab_bc_cd_da_abShun[i][3]);
				delay_ms(SPEED);
            }
        }
    }
    i = 0;
    // 反转
    if(slectReverseForward == STEPPER_REVERSAL)
    {
        for(j = 0;j < pulseNumber;j ++)
        {
            for(i = 0;i < 4; i++)
            {
                GPIO_WriteBit(__POART__(STEPPER2_IN1_GPIO),__PIN__(STEPPER2_IN1_PIN),ab_bc_cd_da_abNi[i][0]);
                GPIO_WriteBit(__POART__(STEPPER2_IN2_GPIO),__PIN__(STEPPER2_IN2_PIN),ab_bc_cd_da_abNi[i][1]);
                GPIO_WriteBit(__POART__(STEPPER2_IN3_GPIO),__PIN__(STEPPER2_IN3_PIN),ab_bc_cd_da_abNi[i][2]);
                GPIO_WriteBit(__POART__(STEPPER2_IN4_GPIO),__PIN__(STEPPER2_IN4_PIN),ab_bc_cd_da_abNi[i][3]);
				delay_ms(SPEED);
            }
        }
    }
}


// GPIOE.7~GPIOE.10
void initStepperMotor(void)
{
	GPIO_InitTypeDef StepperInitStruct;
	RCC_APB2PeriphClockCmd(
        __RCC_APB2__(IN1_GPIO)|
        __RCC_APB2__(IN2_GPIO)|
        __RCC_APB2__(IN3_GPIO)|
        __RCC_APB2__(IN4_GPIO)|
        __RCC_APB2__(STEPPER2_IN1_GPIO)|
        __RCC_APB2__(STEPPER2_IN2_GPIO)|
        __RCC_APB2__(STEPPER2_IN3_GPIO)|
        __RCC_APB2__(STEPPER2_IN4_GPIO),ENABLE);
	StepperInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
	StepperInitStruct.GPIO_Speed = GPIO_Speed_50MHz;  // 
    StepperInitStruct.GPIO_Pin = __PIN__(IN1_PIN);
	GPIO_Init(__POART__(IN1_GPIO),&StepperInitStruct);
    StepperInitStruct.GPIO_Pin = __PIN__(IN2_PIN);
	GPIO_Init(__POART__(IN2_GPIO),&StepperInitStruct);
    StepperInitStruct.GPIO_Pin = __PIN__(IN3_PIN);
	GPIO_Init(__POART__(IN3_GPIO),&StepperInitStruct);
    StepperInitStruct.GPIO_Pin = __PIN__(IN4_PIN);
	GPIO_Init(__POART__(IN4_GPIO),&StepperInitStruct);
    STOP_STEPPER1();  // 关闭步进电机1
    delay_ms(5);

    StepperInitStruct.GPIO_Pin = __PIN__(STEPPER2_IN1_PIN);
	GPIO_Init(__POART__(STEPPER2_IN1_GPIO),&StepperInitStruct);
    StepperInitStruct.GPIO_Pin = __PIN__(STEPPER2_IN2_PIN);
	GPIO_Init(__POART__(STEPPER2_IN2_GPIO),&StepperInitStruct);
    StepperInitStruct.GPIO_Pin = __PIN__(STEPPER2_IN3_PIN);
	GPIO_Init(__POART__(STEPPER2_IN3_GPIO),&StepperInitStruct);
    StepperInitStruct.GPIO_Pin = __PIN__(STEPPER2_IN4_PIN);
	GPIO_Init(__POART__(STEPPER2_IN4_GPIO),&StepperInitStruct);
    STOP_STEPPER2();  // 关闭步进电机2
    delay_ms(5);
}



