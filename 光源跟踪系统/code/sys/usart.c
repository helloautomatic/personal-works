#include "sys.h"
#include "usart.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
#include "stm32f10x.h"

//使UASRT串口可用printf函数发送
//在usart.h文件里可更换使用printf函数的串口号	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE {
	int handle; 
}; 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x){ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f){      
	while((USART_n->SR&0X40)==0);//循环发送,直到发送完毕   
    USART_n->DR = (u8) ch;      
	return ch;
}
#endif 


/*
USART1串口相关程序
*/
 
#if EN_USART1   //USART1使用与屏蔽选择
u8 USART1_RX_BUF[USART1_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART1_RX_STA=0;       //接收状态标记	  

/*
USART1专用的printf函数
当同时开启2个以上串口时，printf函数只能用于其中之一，其他串口要自创独立的printf函数
调用方法：USART1_printf("123"); //向USART2发送字符123
*/
void USART1_printf (char *fmt, ...){ 
	char buffer[USART1_REC_LEN+1];  // 数据长度
	u8 i = 0;	
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, USART1_REC_LEN+1, fmt, arg_ptr);
	while ((i < USART1_REC_LEN) && (i < strlen(buffer))){
        USART_SendData(USART1, (u8) buffer[i++]);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 
	}
	va_end(arg_ptr);
}

void USART1_Init(u32 bound){ //串口1初始化并启动
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
     //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
   //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器 
   //USART 初始化设置
		USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
		USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	
		UART_DMA_Config(DMA1_Channel5,(u32)&USART1->DR,(u32)USART1_RX_BUF);//DMA1通道5,外设为串口1,存储器为USART2_RX_BUF 
	
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启接收中断
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);//开启空闲中断
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);//开启DMA接收
	USART_Cmd(USART1, ENABLE);                    //使能串口 
}


void UART_DMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar)
{
	DMA_InitTypeDef DMA_InitStructure;
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
  DMA_DeInit(DMA_CHx);//将DMA的通道1寄存器重设为缺省值
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA外设ADC基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，从内存读取发送到外设
	DMA_InitStructure.DMA_BufferSize = 200;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器	
} 



void USART1_IRQHandler(void){ //串口1中断服务程序（固定的函数名不能修改）	
//		u32 rc_len,i;
	
		if(USART_GetITStatus(USART1,USART_IT_IDLE)!=RESET)//判断接收中断是否触发
		{
			USART1->SR;
			USART1->DR;//清除IDLE标志位
			DMA_Cmd(DMA1_Channel5,DISABLE);//关闭DMA
			DMA_ClearITPendingBit(DMA1_IT_TC1);//清除传输完成标志位
			DMA_ClearITPendingBit(DMA1_IT_TE1);//清除传输错误标志位
//			rc_len=USART1_REC_LEN - DMA_GetCurrDataCounter(DMA1_Channel5);//得到待传输数据的长度
//			for(i=0;i<rc_len;i++)
//			{
//				USART1_RX_BUF[i]=USART1_RX_Buffer[i];
//			}
			DMA_Cmd(DMA1_Channel5,ENABLE);//开启下一次传输
			if(USART1_RX_STA>=(USART1_REC_LEN-1))
				USART1_RX_STA=0;
			USART1_RX_STA++;				
		}		
} 
#endif	

/*
USART2串口相关程序
*/
#if EN_USART2   //USART2使用与屏蔽选择
u8 USART2_RX_BUF[USART2_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART2_RX_STA=0;       //接收状态标记	  

/*
USART2专用的printf函数
当同时开启2个以上串口时，printf函数只能用于其中之一，其他串口要自创独立的printf函数
调用方法：USART2_printf("123"); //向USART2发送字符123
*/
void USART2_printf (char *fmt, ...){ 
	char buffer[USART2_REC_LEN+1];  // 数据长度
	u8 i = 0;	
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, USART2_REC_LEN+1, fmt, arg_ptr);
	while ((i < USART2_REC_LEN) && (i < strlen(buffer))){
        USART_SendData(USART2, (u8) buffer[i++]);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); 
	}
	va_end(arg_ptr);
}


void USART2_Init(u32 bound){ //串口1初始化并启动
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
		 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE); //使能UART2所在GPIOA的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //使能串口的RCC时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //设置USART2的RX接口是PA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //设置USART2的TX接口是PA2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

   //USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART2, &USART_InitStructure); //初始化串口
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启ENABLE/关闭DISABLE中断
    USART_Cmd(USART2, ENABLE);                    //使能串口 
   //Usart2 NVIC 配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器 
}

void USART2_IRQHandler(void){ //串口2中断服务程序（固定的函数名不能修改）	

} 
#endif	


#if EN_USART3   //如果使能了接收
u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART3_RX_STA=0;       //接收状态标记	  

/*
USART3专用的printf函数
当同时开启2个以上串口时，printf函数只能用于其中之一，其他串口要自创独立的printf函数
调用方法：USART3_printf("123"); //向USART3发送字符123
*/
void USART3_printf (char *fmt, ...){ 
	char buffer[USART3_REC_LEN+1];  // 数据长度
	u8 i = 0;	
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, USART3_REC_LEN+1, fmt, arg_ptr);
	while ((i < USART3_REC_LEN) && (i < strlen(buffer))){
        USART_SendData(USART3, (u8) buffer[i++]);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); 
	}
	va_end(arg_ptr);
}

void USART3_Init(u32 BaudRate){ //USART3初始化并启动
   GPIO_InitTypeDef GPIO_InitStructure;
   USART_InitTypeDef USART_InitStructure;
   NVIC_InitTypeDef NVIC_InitStructure; 

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能UART3所在GPIOB的时钟
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //使能串口的RCC时钟

   //串口使用的GPIO口配置
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//设置USART3的RX接口是PB11
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//接口模式 浮空输入
   GPIO_Init(GPIOB, &GPIO_InitStructure);

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//设置USART3的TX接口是PB10
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输出速度50MHz
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//接口模式 复用推挽输出
   GPIO_Init(GPIOB, &GPIO_InitStructure);

   //配置串口
   USART_InitStructure.USART_BaudRate = BaudRate;
   USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
   USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
   USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

   USART_Init(USART3, &USART_InitStructure);//配置串口3
   USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//使能串口接收中断  
   //USART_ITConfig(USART3, USART_IT_TXE, ENABLE);//串口发送中断在发送数据时开启
   USART_Cmd(USART3, ENABLE);//使能串口3

   //串口中断配置
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
   NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//允许USART3中断
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//中断等级
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
}

//串口3中断服务程序（固定的函数名不能修改）
void USART3_IRQHandler(void){ 	
		u8 Temp;
		if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)//判断接收中断是否触发
		{
			if(USART3_RX_STA>=(USART3_REC_LEN-1))
				USART3_RX_STA=0;
			Temp=USART_ReceiveData(USART3);
			USART3_RX_BUF[USART3_RX_STA]=Temp;
			USART3_RX_STA++;				
		}
} 
#endif	




/*********************************************************************************************
 * 杜洋工作室 www.DoYoung.net
 * 洋桃电子 www.DoYoung.net/YT 
*********************************************************************************************/

/*
a符号的作用：

%d 十进制有符号整数
%u 十进制无符号整数
%f 浮点数
%s 字符串
%c 单个字符
%p 指针的值
%e 指数形式的浮点数
%x, %X 无符号以十六进制表示的整数
%o 无符号以八进制表示的整数
%g 自动选择合适的表示法
%p 输出地址符

*/






