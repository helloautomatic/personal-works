#ifndef _MAIN_H_
#define _MAIN_H_


// 用作移植代码
#define __CODE_MAGRATION_
#define RCC_APB2Periph_GPIO(X) RCC_APB2Periph_GPIO##X
#define GPIO_Pin_(X)  GPIO_Pin_##X
#define GPIO(X)  GPIO##X
#define __POART__(X)  GPIO(X)
#define __PIN__(X)  GPIO_Pin_(X)
#define __RCC_APB2__(X)  RCC_APB2Periph_GPIO(X)


#define GPIO_Mode_IP(X) GPIO_Mode_IP##X
#define __PIN_MODE__(X) GPIO_Mode_IP(X)
#endif

