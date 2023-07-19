#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sys/unistd.h"
#include "esp_heap_caps.h"
#include "esp_camera.h"
#include "lcd_cfg.h"
#include "LED.h"
#include "LCD.h"
#include "app_camera.h"
#include "read_esp32_mac.h"
#include "YMCV.h"

// //使用微秒阻塞延时
// void delay_ms(int ttt)
// {
//     for(int i=0;i<ttt;i++)
//     usleep(1000);
// }

// 这里包含要运行的demo文件
#if 0
#include "../Demo/0--LCD_Test.c"
////或者通过下面方式进行Demo选择
#else
#define DemoChose 1
#include "../Demo/DemoChose.txt"
#endif

// 图片数据
#include "imageSource.h"

#define pic_w 160
#define pic_h 120
#define picbuf_len (pic_h * pic_w + 1) // 定义数据缓存camera_buf的大小(*2字节)，由于使用uint16,使用时需除2，这里故要+1
uint16_t camera_buf[picbuf_len];       // 摄像头数据缓存buf

void waitting_a_frame(CVIMAGE myimg);

void app_main(void)
{
    // 设备初始化
    app_camera_led_init();
    app_camera_init();//这里面默认配置了图像大小为QQVGA = 160x120
    //app_camera_init这个一定要置顶，否则一旦初始化了LCD的SPI设备，他就会报错，暂时不知道为什么

    LED_Init();
    LCD_Init();

    // //系统初始化
    // camera_led_setval(0.2);//开启LED

    // 打印一定要加换行符，否则不会显示出来
    // printf("YMCV Init OK\n");

    uint8_t efuse_mac[6] = {0};
    uint8_t readok = read_esp32_mac_for_ymcv(efuse_mac);

    printf("hello readok = %d \n", readok);
    if (readok)
    {
        printf("MAC :%02X%02X%02X%02X%02X%02X\n", efuse_mac[0], efuse_mac[1], efuse_mac[2], efuse_mac[3], efuse_mac[4], efuse_mac[5]);
    }
    // 内存使用剩余量
    printf("Free memory: heap total %d bytes ,sram %d bytes, psram %d bytes\n", esp_get_free_heap_size(), heap_caps_get_free_size(MALLOC_CAP_INTERNAL), heap_caps_get_free_size(MALLOC_CAP_SPIRAM));

    // 将摄像头数据封装为图片
    CVimage camImg = {
        .width = pic_w,
        .height = pic_h,
        .type = CVRgb16Type,
        .sizet = CVRgb16Size,
        .pmlc = 0,
        .data = (uint8 *)camera_buf,
    };
    // 取地址
    CVIMAGE myImage0 = &camImg;

    while (1)
    {
        // //采集摄像头的图像
        // waitting_a_frame(myImage0);

        // 采用的是内存中的数据，而不是摄像头的数据
        uint16 *data3p = (uint16 *)myImage0->data;
        uint16 *data3p2 = (uint16 *)mm02;
        for (int i = 0; i < 160 * 120; i++)
        {
            data3p[i] = data3p2[i]; // 0x202
        }

        // 运行demo
        runYMCV_Demo(myImage0);

        vTaskDelay(500 / portTICK_PERIOD_MS);
        // delay_ms(500);使用微秒阻塞延时，会触发看门狗

        // printf("LED闪烁...\n");
        Sys_LED_Toggle();

        // 内存使用剩余量，检测内存是否泄漏
        printf("Free memory: heap total %d bytes ,sram %d bytes, psram %d bytes\n", esp_get_free_heap_size(), heap_caps_get_free_size(MALLOC_CAP_INTERNAL), heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
    }
}



void waitting_a_frame(CVIMAGE myimg)
{
    camera_fb_t * fb = NULL;
    //读取一帧数据
    fb = esp_camera_fb_get();
    if (!fb) {
        printf("Camera capture failed\n");
        return ;
    }

    //加载到解码缓存区
    YMCV_AVI_Decode_JpegBuff_LoadFromData(fb->buf,fb->len);
    //转为rgb565格式
    YMCV_AVI_Decode_JpegBuff_ToImage(myimg);
    
    //释放帧缓存
    esp_camera_fb_return(fb);
}
