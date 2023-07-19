#ifndef _CAM_H
#define _CAM_H
#include "Puse.h"

//********************************************************************************	 
//本例程只供学习使用，未经许可，不得用于其它任何用途
//YMKV开发板
//摄像头接口配置代码   
//项目：妖米视觉
//创建日期:2019/11/5
//版本：V1.0
//版权所有，盗版必究。
//沈阳妖米科技责任有限公司
//********************************************************************************

//JPEG尺寸支持列表
typedef enum
{
	Jpeg_QQVGA_W = 160, Jpeg_QQVGA_H = 120,	//QQVGA 
	Jpeg_QVGA_W = 320,	Jpeg_QVGA_H = 240,	//QVGA  
	Jpeg_VGA_W = 640,		Jpeg_VGA_H = 480,		//VGA
	Jpeg_SVGA_W = 800,	Jpeg_SVGA_H = 600,	//SVGA
	Jpeg_XGA_W = 1024,		Jpeg_XGA_H = 768,	//XGA
	Jpeg_WXGA_W = 1280,	Jpeg_WXGA_H = 800,	//WXGA 
	Jpeg_WXGA2_W = 1440,	Jpeg_WXGA2_H = 900,//WXGA+
	Jpeg_SXGA_W = 1280,	Jpeg_SXGA_H = 1024,	//SXGA
	Jpeg_UXGA_W = 1600,	Jpeg_UXGA_H = 1200,	//UXGA	
	Jpeg_1080P_W = 1920,Jpeg_1080P_H = 1080,//1080P
	Jpeg_QXGA_W = 2048,	Jpeg_QXGA_H = 1536,	//QXGA
	Jpeg_500W_W = 2592,	Jpeg_500W_H = 1944,	//500W 
}Jpeg_Size;

extern DCMI_HandleTypeDef hdcmi;
extern DMA_HandleTypeDef hdma_dcmi;
//摄像头配置

#define pic_w Jpeg_QQVGA_W
#define pic_h Jpeg_QQVGA_H
//#define pic_w 220
//#define pic_h 165

#define picbuf_len (pic_h*pic_w+1)//定义数据缓存camera_buf的大小(*4字节)，由于使用uint16,使用时需除2，这里故要+1
extern uint16_t camera_buf[];	//摄像头数据缓存buf
extern volatile u32 camera_data_len;//buf中的有效数据长度 


//摄像头处理
void CameraInit(void);//初始化摄像头
uint8_t getNextCameraImage(void);//获取下一帧图片


#endif

