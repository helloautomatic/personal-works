/*
	在使用延时函数的时候应当注意，使用不同晶振频率的单片机的延时间是不一样的
	就拿_nop_()函数而言，执行一次该函数就是一个机器周期，如果单片机是12分频的话，一个机器周期就是12个时钟周期，而时钟周期取决于单片机的主晶振频率	
*/

#include <intrins.h>
#include "delay.h"


void delay_second(unsigned char n)  // 秒延迟
{
	int i;
	while(n--)  // 执行完该循环，延迟n秒
	{
		for(i = 0; i <= 1000; i++)
		{
			delay_millisecond(1);  // 执行完该语句是一秒
		}
	}
}


void delay_millisecond(int n)  // 毫秒延迟
{
	int i;
	while(n--)  // 执行完该循环是n毫秒
	{
		for(i = 0; i <= 1000; i++)  //执行完该循环是一毫秒
		{
			_nop_();  // 执行完该语句是一微秒
		}
	}
}


void delay_5us()
{
	/*
		该函数的功能是延时5个us，因为，进入该函数需要两个机器周期，退出该函数需要两个机器周期
		执行一次_nop_()函数大概是一个机器周期，所以该函数的功能是延时5us
	*/
	_nop_();
}

