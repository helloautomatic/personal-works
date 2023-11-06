// #include "exti.h"
// #include "stm32f10x.h"
// #include "stm32f10x_rcc.h"
// #include "stm32f10x_exti.h"
// #include "stm32f10x_gpio.h"
// #include "misc.h"
// #include "led.h"
// #include "key.h"
// #include "delay.h"
// #include "time.h"
// #include "stm32f10x_tim.h"

// void extiInit(void)
// {
//     EXTI_InitTypeDef extiStruct;
//     NVIC_InitTypeDef nvicStruct;

//     // 开启时钟
//     RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

//     // 中断线连接,GPIOE.2
//     GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource2);

//     // 中断模式设置
//     extiStruct.EXTI_Line = EXTI_Line2;
//     extiStruct.EXTI_Trigger = EXTI_Trigger_Falling;  // 下降沿触发
//     extiStruct.EXTI_Mode = EXTI_Mode_Interrupt;
//     extiStruct.EXTI_LineCmd = ENABLE;
//     EXTI_Init(&extiStruct);

//     // 中断优先级设置
//     nvicStruct.NVIC_IRQChannel = EXTI2_IRQn; //使能按键外部中断通道
//     nvicStruct.NVIC_IRQChannelPreemptionPriority = 0x02; //抢占优先级 2，
//     nvicStruct.NVIC_IRQChannelSubPriority = 0x02; //子优先级 2
//     nvicStruct.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
//     NVIC_Init(&nvicStruct); //中断优先级分组初始化

//     // 中断线3和E.3连接，中断线4和E.4连接
//     GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
//     GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
    
//     extiStruct.EXTI_Line = EXTI_Line3;
//     nvicStruct.NVIC_IRQChannel = EXTI3_IRQn;
//     EXTI_Init(&extiStruct);
//     NVIC_Init(&nvicStruct);

//     extiStruct.EXTI_Line = EXTI_Line4;
//     nvicStruct.NVIC_IRQChannel = EXTI4_IRQn;
//     EXTI_Init(&extiStruct);
//     NVIC_Init(&nvicStruct);
// }


// void EXTI0_IRQHandler(void)
// {
//     if(EXTI_GetITStatus(EXTI_Line0)!=RESET)  // 判断某个线上的中断是否发生
//     {
//         EXTI_ClearITPendingBit(EXTI_Line0);  // 清除 LINE 上的中断标志位
//     }
// }


// void EXTI1_IRQHandler(void)
// {
//     delay_ms(10);
//     if(EXTI_GetITStatus(EXTI_Line1)!=RESET)  // 判断某个线上的中断是否发生
//     {
//         EXTI_ClearITPendingBit(EXTI_Line1);  // 清除 LINE 上的中断标志位
//     }
// }


// void EXTI2_IRQHandler(void)
// {
//     delay_ms(10);
//     if(EXTI_GetITStatus(EXTI_Line2)!=RESET)  // 判断某个线上的中断是否发生
//     {
//         runLed();
//         EXTI_ClearITPendingBit(EXTI_Line2);  // 清除 LINE 上的中断标志位
//     }
// }


// void EXTI3_IRQHandler(void)
// {
//     delay_ms(10);
//     if(EXTI_GetITStatus(EXTI_Line3)!=RESET)  // 判断某个线上的中断是否发生
//     {
//         runLed();
//         EXTI_ClearITPendingBit(EXTI_Line3);  // 清除 LINE 上的中断标志位
//     }
// }


// void EXTI4_IRQHandler(void)
// {
//     if(EXTI_GetITStatus(EXTI_Line4)!=RESET)  // 判断某个线上的中断是否发生
//     {
//         runLed();
//         EXTI_ClearITPendingBit(EXTI_Line4);  // 清除 LINE 上的中断标志位
//     }
// }


// void EXTI9_5_IRQHandler(void)
// {
//     if(EXTI_GetITStatus(EXTI_Line5)!=RESET)  // 判断某个线上的中断是否发生
//     {
//         /* code */
//         EXTI_ClearITPendingBit(EXTI_Line5);  // 清除 LINE 上的中断标志位
//     }else if(EXTI_GetITStatus(EXTI_Line6)!=RESET)
//     {
//         /* code */
//         EXTI_ClearITPendingBit(EXTI_Line6);
//     }else if(EXTI_GetITStatus(EXTI_Line7)!=RESET)
//     {
//         /* code */
//         EXTI_ClearITPendingBit(EXTI_Line7);
//     }else if(EXTI_GetITStatus(EXTI_Line8)!=RESET)
//     {
//         /* code */
//         EXTI_ClearITPendingBit(EXTI_Line8);
//     }else
//     {
//         /* code */
//         EXTI_ClearITPendingBit(EXTI_Line9);
//     }
// }


// void EXTI15_10_IRQHandler(void)
// {
//     if(EXTI_GetITStatus(EXTI_Line10)!=RESET)  // 判断某个线上的中断是否发生
//     {
//         /* code */
//         EXTI_ClearITPendingBit(EXTI_Line10);  // 清除 LINE 上的中断标志位
//     }else if(EXTI_GetITStatus(EXTI_Line11)!=RESET)
//     {
//         /* code */
//         EXTI_ClearITPendingBit(EXTI_Line11);
//     }else if(EXTI_GetITStatus(EXTI_Line12)!=RESET)
//     {
//         /* code */
//         EXTI_ClearITPendingBit(EXTI_Line12);
//     }else if(EXTI_GetITStatus(EXTI_Line13)!=RESET)
//     {
//         /* code */
//         EXTI_ClearITPendingBit(EXTI_Line13);
//     }else if(EXTI_GetITStatus(EXTI_Line14)!=RESET)
//     {
//         /* code */
//         EXTI_ClearITPendingBit(EXTI_Line14);
//     }else
//     {
//         /* code */
//         EXTI_ClearITPendingBit(EXTI_Line15);
//     }
    
// }


// // 定时器中断服务函数
// void TIM3_IRQHandler(void) //TIM3 中断
// {
//     if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查 TIM3 更新中断发生与否
//     {
        
//         TIM_ClearITPendingBit(TIM3, TIM_IT_Update ); //清除 TIM3 更新中断标志
//     }
// }


