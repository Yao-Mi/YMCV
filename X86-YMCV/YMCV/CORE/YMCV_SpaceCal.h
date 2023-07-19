#ifndef YMCV_SPACECAL_H
#define YMCV_SPACECAL_H
#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_PubType.h"
#include "../OPOBJ/YMCV_OBJ.h"
#include "./YMCV_Padding.h"

typedef enum
{
	CV_GaussFilter=1, //��˹�˲�
	CV_LOGFilter,	  //LOG�˲�
	CV_BilFilter,//˫���˲�
	CV_LMSEFilter, //�ֲ��������˲�  Local Mean Square Error Filter
	CV_MeanShiftFilter,//��ֵƫ���˲�

	//����Ҫsigma�������г�ʼ��
	CV_AverFilter,  //��ֵ�˲�
	CV_MinFilter,//��Сֵ�˲�
	CV_MedFilter,//��ֵ�˲�
	CV_MaxFilter,//���ֵ�˲�
	CV_ModeFilter,//�����˲�
	CV_MidPointFilter,//�е��˲�

	//�Զ����˲��ˣ���Ҫ�Լ���ʼ��
	CV_CustomFilter,
}CVKERNELTYPE;



//��ͨ������
typedef  union
{
	//��˹�˲�
	struct
	{
		float32 sigma;
	}gauss;
	//˫���˲�
	struct 
	{
		float32 sigma;
		float32 sgmgray;
	}bilateral;
	//������˹ - ��˹�˲�
	struct
	{
		float32 sigma;
	}log;
	//�ֲ��������˲�
	struct
	{
		float32 sigma;
	}lms;

}CV_kernel_1param;

//��ͨ������
typedef  union
{
	//�Ҷȵ���ֵ��ֵ������
	struct
	{
		CVgray R_Thresh;
		CVgray G_Thresh;
		CVgray B_Thresh;
	}rgb_tobin;
	
}CV_kernel_3param;


typedef struct
{
	CVKERNELTYPE KernelType;
	PIXChannelNum ChannelNum;
	uint8 padding;
	uint8 rsize;
	float32* data;
	//Ԥ������
	float32 reserve1;
	float32 reserve2;
	float32 reserve3;
	float32 reserve4;
}CVkern_render;
typedef CVkern_render* CVKERN_RENDER;

void YMCV_Space_SingleKernel_Load(CVKERN_RENDER render, CV_kernel_1param* Param); //�ռ��˲������أ���ʼ����
CVIMAGE YMCV_Image_Space_SigleKernel_Render(CVIMAGE myimg, CVKERN_RENDER render);//ͼ��ռ���˲���Ⱦ
CVIMAGE YMCV_Image_Space_CustomKernel_Render(CVIMAGE myimg, CVKERN_RENDER render);//
CVIMAGE YMCV_Gray_Image_GuideFilter(CVIMAGE gray, CVIMAGE guid, YMCVPADDINGType padding, uint8 ker_r, float32 eta);//�Ҷ�ͼ�����˲�
CVIMAGE YMCV_Image_Space_MeanShift(CVIMAGE myimg, uint16 radius, uint16 threshold, uint16 maxIteration);//��ֵƫ���˲� �ͻ�Ч��
CVIMAGE YMCV_Gray_SurfaceBlur(CVIMAGE gray, uint8 radius, uint8 threshold, YMCVPADDINGType padding);//����ģ��
CVIMAGE YMCV_Binary_Morph_Hit_Cal(CVIMAGE myimg, CVFMAT morphMat,float32 hitRate);//��̬ѧ���м���

void YMCV_GrayConvFker_3X3(float32* mykernel, CVIMAGE myimg, uint8* outp);//3*3�˲����˲� ��Խ������Ӧ����
void YMCV_GrayConvIker_3X3(int16* mykernel, CVIMAGE myimg, uint8* outp);//3*3�˲����˲� Խ�������Ϊ0
//�������
void YMCV_GrayGaussConvIker_3X3(uint8* outp, uint16 width, uint16 height);//3x3��˹�˿��ټ��� Խ�������Ϊ0
void YMCV_I16_GaussConvIker_3X3(int16* outp, uint16 width, uint16 height);//3x3��˹�˿��ټ��� Խ�������Ϊ0
void YMCV_GrayGaussConvIker_5X5(uint8* outp, uint16 width, uint16 height);//5x5��˹�˿��ټ��� Խ�������Ϊ0
//�ϲ�����
void YMCV_GrayGaussConvFker_3X3(uint8* inp, float32* outp, uint16 width, uint16 height);
void YMCV_F32_GaussConvFker_3X3(float32* inp, float32* outp, uint16 width, uint16 height);
void YMCV_GrayGaussConvFker_5X5(uint8* inp, float32* outp, uint16 width, uint16 height);

#endif// !YMCV_SPACECAL_H


