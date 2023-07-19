#ifndef YMCV_BINARYCAL_H
#define YMCV_BINARYCAL_H
#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_PubType.h"
#include "../OPOBJ/YMCV_OBJ.h"
#include "./YMCV_HistCal.h"

typedef enum
{
	CV_SingleThreshold,//����ֵ
	CV_DoubleThreshold,//˫��ֵ
	CV_RGB2Threshold,//RGB˫��ֵ
	CV_LAB2Threshold,//LAB˫��ֵ
	CV_IntegralThreshold,//����ͼ��ֵ��
	CV_HistThreshold,//ֱ��ͼ��ֵ��
}CVThreshold;

typedef enum
{
	CV_And,//�߼���
	CV_Or,//�߼���
	CV_Not,//�߼���
}CVLOGIC;

typedef  union
{
	//�Ҷȵ���ֵ
	struct
	{
		uint8 thresh;
	}gray_single_t;
	//�Ҷȴ�ͨ˫��ֵ 
	struct
	{
		uint8 thresh_min;
		uint8 thresh_max;
	}gray_double_t;
	//RGB��ͨ��˫��ֵ 
	struct
	{
		uint8 thresh_Rmin;
		uint8 thresh_Rmax;
		uint8 thresh_Gmin;
		uint8 thresh_Gmax;
		uint8 thresh_Bmin;
		uint8 thresh_Bmax;
	}rgb_double_t;
	//ֱ��ͼ�Զ���ֵ
	struct
	{
		CVHistAutoThresh thresh_type;
		uint8 p_tile_val;//ȡֵ���䣺[0��100]
	}hist_auto_t;
	//LAB��ͨ��˫��ֵ 
	struct
	{
		//Lȡֵ 0~100
		int8 thresh_Lmin;
		int8 thresh_Lmax;
		//Aȡֵ -128~127
		int8 thresh_Amin;
		int8 thresh_Amax;
		//Bȡֵ -128~127
		int8 thresh_Bmin;
		int8 thresh_Bmax;
	}lab_double_t;
	//����ͼ��ֵ 
	struct
	{
		uint8 thresh_percent;//ȡ��������ҶȾ�ֵ�� T %����
		uint8 kernerl_radius;//�뾶
	}gray_integral_t;

}CV_binary_param;


typedef  union
{
	//��ͨ��
	struct
	{
		uint8* PixMap;
	}single;
	//��ͨ��
	struct
	{
		uint8* PixMap1;
		uint8* PixMap2;
		uint8* PixMap3;
	}three;
}CV_binary_vec;

typedef struct
{
	CVThreshold ThreshType;//��ֵ������
	CV_binary_vec myVector;//��������
	uint8 reserve1;
	uint8 reserve2;
}CVbinary_render;

typedef CVbinary_render* CVBINARY_RENDER;


void YMCV_Image_ThreshTo_Binary(CVIMAGE Imgin, CVIMAGE Imgout, CVBINARY_RENDER render);//��ֵ����Ⱦ
void YMCV_BinaryKernel_Load(CVBINARY_RENDER render, CV_binary_param* Param);//��ֵ����Ⱦ����������
void YMCV_Image_Binary_Logic_Cal(CVIMAGE Imgin, CVIMAGE Imginout, CVLOGIC LogicCalType);//��ֵͼ���߼�����
void YMCV_Binary_AreaFilter(CVIMAGE myimg, uint32 minArea);//��ֵͼ�������
void YMCV_Image_BinaryMask_Cal(CVIMAGE Imgin, CVIMAGE BinaryMask);//ͼ��������ȡ

#endif

