# 介绍
这里在开发stm32的时候，直接使用的是固件库而非寄存器库。

# 开发工具
keil5——主要用做编译器

vscode——主要用作编辑器

sublime Text3——主要用作编辑器

# keil5中的代码结构
- RCT6
  - usr
    - main.c
    - stm32f10x_it.c
    - system_stm32f10x.c
  - core
    - core_cm3.c
    - startup_stm32f10x_hd.s
  - fwlib
    - stm32f10x_gpio.c
    - stm32f10x_rcc.c
    - misc.c
    - stm32f10x_usart.c
    - stm32f10x_exti.c
    - stm32f10x_tim.c
    - stm32f10x_dma.c
    - stm32f10x_i2c.c
    - stm32f10x_flash.c
  - hardware
    - key.c
    - stepperMotor.c
    - gps.c
    - oled.c
    - sunflower.c
    - timeCtrl.c
  - sys
    - usart.c
    - delay.c
    - sys.c
    - exti.c
    - myiic.c

# 编辑和编译选项
## 打开方式
方式1：Project--->Options for Target'工程名字'

方式2：Alt + F7

## 芯片选型
![image](https://github.com/helloautomatic/personal-works/assets/88640443/38f063b8-f6ac-456d-89dd-d8029561e61c)

## 晶振频率设置
![image](https://github.com/helloautomatic/personal-works/assets/88640443/bab58d38-bea8-4fb3-baed-17a2bc7067e2)

## C/C++选项
![image](https://github.com/helloautomatic/personal-works/assets/88640443/7ae63c17-10b0-49e8-a19d-523da3eed492)

## Debug选项
![image](https://github.com/helloautomatic/personal-works/assets/88640443/c718c2f4-22c0-48d4-9655-f657ba6aa9bf)

## 软件烧录选项
![image](https://github.com/helloautomatic/personal-works/assets/88640443/7feae7e5-62b8-4794-9484-88364305f640)

## 乱码问题的可能解决方法

Edit--->Configuration

![image](https://github.com/helloautomatic/personal-works/assets/88640443/ce0dc2dc-dc04-4959-a0ee-e08b2146d7cd)

















