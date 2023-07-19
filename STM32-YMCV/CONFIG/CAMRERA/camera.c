#include "camera.h"
#include "ov5640.h" 
#include "delay.h" 
#include "lcd_cfg.h"
#include "../LCD/lcd.h"

//********************************************************************************	 
//������ֻ��ѧϰʹ�ã�δ����ɣ��������������κ���;
//YMKV������
//����ͷ�ӿ����ô���   
//��Ŀ�������Ӿ�
//��������:2019/11/5
//�汾��V1.0
//��Ȩ���У�����ؾ���
//�������׿Ƽ��������޹�˾
//********************************************************************************
uint16 camera_buf[picbuf_len];	//����ͷ���ݻ���buf
#define camflg 0
uint16 camera_get_imgok = camflg;

//����ͷ��ʼ��
void CameraInit() 
{
	while(OV5640_Init())//��ʼ��OV2640
	{
		LCD_ShowString(40,100,"Camera ERROR");
		HAL_Delay_ms(200);
		LCD_Clear(WHITE);
	}
	LCD_ShowString(40,100,"Camera OK");
	//OV5640_Focus_Init();//�Զ��Խ���ʼ��
  OV5640_RGB565_Mode();		//RGB565ģʽ
	OV5640_Light_Mode(0);	//�Զ�ģʽ0 ����4
	OV5640_Color_Saturation(3);//ɫ�ʱ��Ͷ�0
	OV5640_Brightness(4);	//����0
	OV5640_Contrast(3);		//�Աȶ�0
	OV5640_Sharpness(0);	//�Զ����33 �ر�0
	//OV5640_Focus_Constant();//���������Խ�
	OV5640_OutSize_Set(4,0,pic_w,pic_h);//��������ߴ� 
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
	
	//��ʼ����
	__HAL_DMA_ENABLE(&hdma_dcmi); 	//ʹ��DMA
	DCMI->CR|=DCMI_CR_CAPTURE;    			//DCMI����ʹ�� 
}


extern void LCD_show_rgbbuffer(u16 x,u16 y,u16 Width,u16 Height,u16 *pic);

//DCMI֡�жϴ�����
//����һ֡ͼ������
//hdcmi:DCMI���
void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{
	__HAL_DCMI_CLEAR_FLAG(hdcmi,DCMI_FLAG_FRAMERI);//���֡�ж�
	HAL_DCMI_Stop(hdcmi);//�رմ���
	
	camera_get_imgok = 1;
}

//��һ֡ͼƬ��ȡ
uint8_t getNextCameraImage()
{
	static uint8 firstokflag = 1;
	//��ʶ�״η����仯
	if(camera_get_imgok == 1 && firstokflag == 1)
	{
		firstokflag = 0;
		return camera_get_imgok;
	}
	
	//δ�仯, ����ɹ��������ʶ�����¿�ʼ
	if(camera_get_imgok == 1)
	{
		camera_get_imgok = 0;
		firstokflag = 1;//���³�ʼ��
		HAL_DCMI_Start_DMA(&hdcmi,DCMI_MODE_CONTINUOUS,(u32)camera_buf, picbuf_len/2);//��������ģʽ
	}
	return camera_get_imgok;
}



