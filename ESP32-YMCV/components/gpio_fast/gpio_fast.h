#ifndef __GPIO_FAST_H
#define __GPIO_FAST_H			  	 

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sys/unistd.h"

#include "stdlib.h"
#include "stdint.h"	   
//********************************************************************************	 
//本例程只供学习使用，未经许可，不得用于其它任何用途
//YMKV开发板
//LCD 驱动代码	      
//项目：妖米视觉
//创建日期:2019/11/5
//版本：V1.0
//版权所有，盗版必究。
//沈阳妖米科技责任有限公司
//********************************************************************************

				  
//-----------------ESP32 GPIO快速操作----------------    
#define GPIO_REG_VALUE(addr) *((volatile uint32_t *)(addr))
#undef GPIO_DIR_REG
#undef GPIO_IN1_REG
#undef GPIO_IN_REG

#undef GPIO_OUT_REG
#undef GPIO_OUT_W1TS_REG
#undef GPIO_OUT_W1TC_REG
#undef GPIO_OUT1_REG
#undef GPIO_OUT1_W1TS_REG
#undef GPIO_OUT1_W1TC_REG

#define GPIO_DIR_REG        0x3ff44000  //GPIO 基地址
#define GPIO_OUT_REG        0x3ff44004  //GPIO 0-31 输出值
#define GPIO_OUT_W1TS_REG   0x3ff44008  //GPIO 0-31 输出置位寄存器
#define GPIO_OUT_W1TC_REG   0x3ff4400C  //GPIO 0-31 输出清零寄存器
#define GPIO_IN_REG         0x3ff4403C  //GPIO 0-31 输入寄存器

#ifndef BIT
#define BIT(x) (1 << (x))
#endif

#define GPIO_0_31_OUTPUT_HIGH(x)          (GPIO_REG_VALUE(GPIO_OUT_W1TS_REG) |= BIT(x))  //拉高
#define GPIO_0_31_OUTPUT_LOW(x)           (GPIO_REG_VALUE(GPIO_OUT_W1TC_REG) |= BIT(x))  //拉低
#define GPIO_0_31_OUTPUT_TOGGLE(x)        (GPIO_REG_VALUE(GPIO_OUT_REG) = (GPIO_REG_VALUE(GPIO_OUT_REG) & BIT(x))? (GPIO_REG_VALUE(GPIO_OUT_REG)& (~BIT(x))) : (GPIO_REG_VALUE(GPIO_OUT_REG)| BIT(x)) ) //翻转
#define GPIO_0_31_READ(x)                 ((GPIO_REG_VALUE(GPIO_IN_REG)>> (x)) & 0x01)

#define GPIO_OUT1_REG        0x3FF44010  //GPIO 32-39 输出值
#define GPIO_OUT1_W1TS_REG   0x3FF44014  //GPIO 32-39 输出置位寄存器
#define GPIO_OUT1_W1TC_REG   0x3FF44018  //GPIO 32-39 输出清零寄存器
#define GPIO_IN1_REG         0x3ff44040  //GPIO 32-39 输入寄存器

#define GPIO_32_39_OUTPUT_HIGH(x)          (GPIO_REG_VALUE(GPIO_OUT1_W1TS_REG) |= BIT(x-32))  //拉高
#define GPIO_32_39_OUTPUT_LOW(x)           (GPIO_REG_VALUE(GPIO_OUT1_W1TC_REG) |= BIT(x-32))  //拉低
#define GPIO_32_39_OUTPUT_TOGGLE(x)        (GPIO_REG_VALUE(GPIO_OUT1_REG) = (GPIO_REG_VALUE(GPIO_OUT1_REG) & BIT(x-32))? (GPIO_REG_VALUE(GPIO_OUT1_REG)& (~BIT(x-32))) : (GPIO_REG_VALUE(GPIO_OUT1_REG)| BIT(x-32)) ) //翻转
#define GPIO_32_39_READ(x)                 ((GPIO_REG_VALUE(GPIO_IN1_REG) >> (x-32)) & 0x01) 

#define Delay_ms(xms) vTaskDelay(xms / portTICK_PERIOD_MS)
#define Delay_us(xus) usleep(xus)



#endif  
	 



