// Copyright 2023-2033 YaoMi Tech
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include "./LED.h"
#include "driver/gpio.h"

/**
 * @brief LED初始化
 */
void LED_Init(void)
{
    // 板载红色LED
    gpio_pad_select_gpio(GPIO_NUM_33); // 选择一个GPIO33
    // gpio_set_direction(GPIO_NUM_33, GPIO_MODE_OUTPUT);//把这个GPIO作为输出
    gpio_set_direction(GPIO_NUM_33, GPIO_MODE_INPUT_OUTPUT_OD); // 把这个GPIO作为输出，也可以作输入
}

/**
 * @brief LED设置亮灭
 */
void Sys_LED_SetVal(bool val)
{
    // 输出高电平时 LED熄灭
    gpio_set_level(GPIO_NUM_33, (val == false) ? 1 : 0);
}

/**
 * @brief LED翻转
 */
void Sys_LED_Toggle()
{
    // 获取引脚电平
    int val = gpio_get_level(GPIO_NUM_33);
    // 电平翻转
    gpio_set_level(GPIO_NUM_33, (val == 0) ? 1 : 0);
}
