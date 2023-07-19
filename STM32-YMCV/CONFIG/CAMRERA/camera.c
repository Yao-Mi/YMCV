#include "camera.h"
#include "ov5640.h" 
#include "delay.h" 
#include "lcd_cfg.h"
#include "../LCD/lcd.h"

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
uint16 camera_buf[picbuf_len];	//摄像头数据缓存buf
#define camflg 0
uint16 camera_get_imgok = camflg;

//摄像头初始化
void CameraInit() 
{
	while(OV5640_Init())//初始化OV2640
	{
		LCD_ShowString(40,100,"Camera ERROR");
		HAL_Delay_ms(200);
		LCD_Clear(WHITE);
	}
	LCD_ShowString(40,100,"Camera OK");
	//OV5640_Focus_Init();//自动对焦初始化
  OV5640_RGB565_Mode();		//RGB565模式
	OV5640_Light_Mode(0);	//自动模式0 家里4
	OV5640_Color_Saturation(3);//色彩饱和度0
	OV5640_Brightness(4);	//亮度0
	OV5640_Contrast(3);		//对比度0
	OV5640_Sharpness(0);	//自动锐度33 关闭0
	//OV5640_Focus_Constant();//启动持续对焦
	OV5640_OutSize_Set(4,0,pic_w,pic_h);//设置输出尺寸 
	LCD_ShowString(40,100,"start camera");
	
// The function HAL_DCMI_Init() turns on many interrupts by default
// We need to handle these interrupts in the future, otherwise it will cause all kinds of problems.
// However, many of these interrupts are not needed, so we turn them all off, that is, we clear the IER register to zero.
// turn off all interrupts and then make corresponding interrupts according to your actual needs.
	DCMI->IER=0x0;	
	
	__HAL_DCMI_ENABLE_IT(&hdcmi,DCMI_IT_FRAME);		//Open frame interrupt
	__HAL_DCMI_ENABLE(&hdcmi);						//Can make the DCMI
	
//Hal_DMA_STATRT () Hal_DMAEX_MULTIBUFFERSTART () first uses __HAL_LOCK() to lock the DMA, and __HAL_LOCK() determines whether the current DMA state is locked or not.
//Hal_busy is returned if it is locked. This will cause subsequent DMA configurations of the functions HAL_DMA_Statrt() and HAL_DMAEx_MultiBufferStart() to be skipped!
//DMA doesn't work properly, to avoid this, __HAL_UNLOC() is called to unlock the DMA once before starting it.
	__HAL_UNLOCK(&hdma_dcmi);
	
	HAL_DMA_Start(&hdma_dcmi,(u32)&DCMI->DR,(u32)camera_buf,picbuf_len/2);
	__HAL_DMA_ENABLE_IT(&hdma_dcmi,DMA_IT_TC);    //Enables transmission completion interrupt
	
	//开始传输
	__HAL_DMA_ENABLE(&hdma_dcmi); 	//使能DMA
	DCMI->CR|=DCMI_CR_CAPTURE;    			//DCMI捕获使能 
}


extern void LCD_show_rgbbuffer(u16 x,u16 y,u16 Width,u16 Height,u16 *pic);

//DCMI帧中断处理函数
//捕获到一帧图像处理函数
//hdcmi:DCMI句柄
void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{
	__HAL_DCMI_CLEAR_FLAG(hdcmi,DCMI_FLAG_FRAMERI);//清除帧中断
	HAL_DCMI_Stop(hdcmi);//关闭传输
	
	camera_get_imgok = 1;
}

//下一帧图片获取
uint8_t getNextCameraImage()
{
	static uint8 firstokflag = 1;
	//标识首次发生变化
	if(camera_get_imgok == 1 && firstokflag == 1)
	{
		firstokflag = 0;
		return camera_get_imgok;
	}
	
	//未变化, 捕获成功，清除标识并重新开始
	if(camera_get_imgok == 1)
	{
		camera_get_imgok = 0;
		firstokflag = 1;//重新初始化
		HAL_DCMI_Start_DMA(&hdcmi,DCMI_MODE_CONTINUOUS,(u32)camera_buf, picbuf_len/2);//连续拍照模式
	}
	return camera_get_imgok;
}



