/*
  * ESPRESSIF MIT License
  *
  * Copyright (c) 2017 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
  *
  * Permission is hereby granted for use on ESPRESSIF SYSTEMS products only, in which case,
  * it is free of charge, to any person obtaining a copy of this software and associated
  * documentation files (the "Software"), to deal in the Software without restriction, including
  * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
  * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
  * to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in all copies or
  * substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  *
  */

#ifndef _APP_CAMERA_H_
#define _APP_CAMERA_H_

#define CONFIG_CAMERA_MODEL_AI_THINKER (1)
#define CONFIG_LED_ILLUMINATOR_ENABLED (1)

// ESP32Cam (AiThinker) PIN Map
#if CONFIG_CAMERA_MODEL_AI_THINKER
#define CAM_BOARD         "AI-THINKER"
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1//software reset will be performed
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22
#endif

#ifdef CONFIG_LED_ILLUMINATOR_ENABLED

#define CONFIG_LED_LEDC_PIN     GPIO_NUM_4 // LED控制引脚
#define CONFIG_LED_LEDC_CHANNEL LEDC_CHANNEL_1 // LEDC通道
#define CONFIG_LED_LEDC_TIMER   LEDC_TIMER_1 // LEDC计时器

#endif 

#ifdef __cplusplus
extern "C" {
#endif

void app_camera_init();
void app_camera_led_init();
void camera_led_setval(float val_0_1);

#ifdef __cplusplus
}
#endif

#endif
