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

## 芯片选型
![image](https://github.com/helloautomatic/personal-works/assets/88640443/6a36e81b-389d-4cfc-81b6-55165d98e5e7)

## 晶振频率设置
![image](https://github.com/helloautomatic/personal-works/assets/88640443/5bf07706-e4a2-4757-ad72-7520fd2e1205)














