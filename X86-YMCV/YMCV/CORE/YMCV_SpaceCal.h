#ifndef YMCV_SPACECAL_H
#define YMCV_SPACECAL_H
#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_PubType.h"
#include "../OPOBJ/YMCV_OBJ.h"
#include "./YMCV_Padding.h"

typedef enum
{
	CV_GaussFilter=1, //高斯滤波
	CV_LOGFilter,	  //LOG滤波
	CV_BilFilter,//双边滤波
	CV_LMSEFilter, //局部均方差滤波  Local Mean Square Error Filter
	CV_MeanShiftFilter,//均值偏移滤波

	//不需要sigma参数进行初始化
	CV_AverFilter,  //均值滤波
	CV_MinFilter,//最小值滤波
	CV_MedFilter,//中值滤波
	CV_MaxFilter,//最大值滤波
	CV_ModeFilter,//众数滤波
	CV_MidPointFilter,//中点滤波

	//自定义滤波核，需要自己初始化
	CV_CustomFilter,
}CVKERNELTYPE;



//单通道参数
typedef  union
{
	//高斯滤波
	struct
	{
		float32 sigma;
	}gauss;
	//双边滤波
	struct 
	{
		float32 sigma;
		float32 sgmgray;
	}bilateral;
	//拉普拉斯 - 高斯滤波
	struct
	{
		float32 sigma;
	}log;
	//局部均方差滤波
	struct
	{
		float32 sigma;
	}lms;

}CV_kernel_1param;

//三通道参数
typedef  union
{
	//灰度单阈值二值化参数
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
	//预留参数
	float32 reserve1;
	float32 reserve2;
	float32 reserve3;
	float32 reserve4;
}CVkern_render;
typedef CVkern_render* CVKERN_RENDER;

void YMCV_Space_SingleKernel_Load(CVKERN_RENDER render, CV_kernel_1param* Param); //空间滤波器加载（初始化）
CVIMAGE YMCV_Image_Space_SigleKernel_Render(CVIMAGE myimg, CVKERN_RENDER render);//图像空间核滤波渲染
CVIMAGE YMCV_Image_Space_CustomKernel_Render(CVIMAGE myimg, CVKERN_RENDER render);//
CVIMAGE YMCV_Gray_Image_GuideFilter(CVIMAGE gray, CVIMAGE guid, YMCVPADDINGType padding, uint8 ker_r, float32 eta);//灰度图引导滤波
CVIMAGE YMCV_Image_Space_MeanShift(CVIMAGE myimg, uint16 radius, uint16 threshold, uint16 maxIteration);//均值偏移滤波 油画效果
CVIMAGE YMCV_Gray_SurfaceBlur(CVIMAGE gray, uint8 radius, uint8 threshold, YMCVPADDINGType padding);//表面模糊
CVIMAGE YMCV_Binary_Morph_Hit_Cal(CVIMAGE myimg, CVFMAT morphMat,float32 hitRate);//形态学击中计算

void YMCV_GrayConvFker_3X3(float32* mykernel, CVIMAGE myimg, uint8* outp);//3*3滤波核滤波 含越界自适应处理
void YMCV_GrayConvIker_3X3(int16* mykernel, CVIMAGE myimg, uint8* outp);//3*3滤波核滤波 越界则输出为0
//分离加速
void YMCV_GrayGaussConvIker_3X3(uint8* outp, uint16 width, uint16 height);//3x3高斯核快速计算 越界则输出为0
void YMCV_I16_GaussConvIker_3X3(int16* outp, uint16 width, uint16 height);//3x3高斯核快速计算 越界则输出为0
void YMCV_GrayGaussConvIker_5X5(uint8* outp, uint16 width, uint16 height);//5x5高斯核快速计算 越界则输出为0
//合并加速
void YMCV_GrayGaussConvFker_3X3(uint8* inp, float32* outp, uint16 width, uint16 height);
void YMCV_F32_GaussConvFker_3X3(float32* inp, float32* outp, uint16 width, uint16 height);
void YMCV_GrayGaussConvFker_5X5(uint8* inp, float32* outp, uint16 width, uint16 height);

#endif// !YMCV_SPACECAL_H


