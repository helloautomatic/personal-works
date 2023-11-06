# 介绍
这里在开发stm32的时候，直接使用的是固件库而非寄存器库。

## 开发工具
keil5——主要用做编译器
vscode——主要用作编辑器
sublime Text3——主要用作编辑器

## keil5中的代码结构
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
