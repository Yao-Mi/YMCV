/* ESPRESSIF MIT License
 * 
 * Copyright (c) 2018 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 * 
 * Permission is hereby granted for use on all ESPRESSIF SYSTEMS products, in which case,
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
 */
#include "esp_log.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "esp_camera.h"
#include "sdkconfig.h"
#include "./include/app_camera.h"


static const char *TAG = "app_camera";

void app_camera_init()
{
    //power up the camera if PWDN pin is defined
    if(PWDN_GPIO_NUM != -1){
        gpio_config_t conf = { 0 };
        conf.pin_bit_mask = 1LL << PWDN_GPIO_NUM;
        conf.mode = GPIO_MODE_OUTPUT;
        gpio_config(&conf);
        gpio_set_level(PWDN_GPIO_NUM, 0);
    }

    //config CAMERA Pin
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    //init with high specs to pre-allocate larger buffers
    config.frame_size = FRAMESIZE_QSXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;

    // camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Camera init failed with error 0x%x", err);
        return;
    }

    //获取摄像头参数
    sensor_t *s = esp_camera_sensor_get();
    s->set_vflip(s, 1); // flip it back
    // initial sensors are flipped vertically and colors are a bit saturated
    if (s->id.PID == OV3660_PID)
    {
        s->set_brightness(s, 1);  // up the blightness just a bit
        s->set_saturation(s, -2); // lower the saturation
    }
    //drop down frame size for higher initial frame rate
    //重置为QQVGA大小，即160x120
    s->set_framesize(s, FRAMESIZE_QQVGA);
}


void app_camera_led_init()
{
#ifdef CONFIG_LED_ILLUMINATOR_ENABLED
    gpio_set_direction(CONFIG_LED_LEDC_PIN,GPIO_MODE_OUTPUT);
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_8_BIT,            // resolution of PWM duty ,LEDC计时器分辨率
        .freq_hz         = 1000,                        // frequency of PWM signal,LEDC频率
        .speed_mode      = LEDC_LOW_SPEED_MODE,  // timer mode
        .timer_num       = CONFIG_LED_LEDC_TIMER        // timer index
    };
    ledc_channel_config_t ledc_channel = {
        .channel    = CONFIG_LED_LEDC_CHANNEL,
        .duty       = 0,
        .gpio_num   = CONFIG_LED_LEDC_PIN,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .hpoint     = 0,
        .timer_sel  = CONFIG_LED_LEDC_TIMER
    };
    switch (ledc_timer_config(&ledc_timer)) {
        case ESP_ERR_INVALID_ARG: ESP_LOGE(TAG, "ledc_timer_config() parameter error"); break;
        case ESP_FAIL: ESP_LOGE(TAG, "ledc_timer_config() Can not find a proper pre-divider number base on the given frequency and the current duty_resolution"); break;
        case ESP_OK: if (ledc_channel_config(&ledc_channel) == ESP_ERR_INVALID_ARG) {
            ESP_LOGE(TAG, "ledc_channel_config() parameter error");
          }
          break;
        default: break;
    }
#endif
}


/**
 * @brief 摄像头LED设置亮度
 */
void camera_led_setval(float val_0_1)
{
#ifdef CONFIG_LED_ILLUMINATOR_ENABLED
    if(val_0_1 > 1.0f)val_0_1=1.0f;
    if(val_0_1 < 0.0f)val_0_1=0.0f;
    
    ledc_set_duty(LEDC_LOW_SPEED_MODE, CONFIG_LED_LEDC_CHANNEL, val_0_1 * ((1 << LEDC_TIMER_8_BIT) - 1));
    ledc_update_duty(LEDC_LOW_SPEED_MODE, CONFIG_LED_LEDC_CHANNEL);
#endif
}

