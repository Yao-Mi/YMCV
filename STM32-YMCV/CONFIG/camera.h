#ifndef _CAM_H
#define _CAM_H
#include "Puse.h"

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

//JPEG�ߴ�֧���б�
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
//����ͷ����

#define pic_w Jpeg_QQVGA_W
#define pic_h Jpeg_QQVGA_H
//#define pic_w 220
//#define pic_h 165

#define picbuf_len (pic_h*pic_w+1)//�������ݻ���camera_buf�Ĵ�С(*4�ֽ�)������ʹ��uint16,ʹ��ʱ���2�������Ҫ+1
extern uint16_t camera_buf[];	//����ͷ���ݻ���buf
extern volatile u32 camera_data_len;//buf�е���Ч���ݳ��� 


//����ͷ����
void CameraInit(void);//��ʼ������ͷ
uint8_t getNextCameraImage(void);//��ȡ��һ֡ͼƬ


#endif

