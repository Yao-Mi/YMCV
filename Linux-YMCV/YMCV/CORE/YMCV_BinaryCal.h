#ifndef YMCV_BINARYCAL_H
#define YMCV_BINARYCAL_H
#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_PubType.h"
#include "../OPOBJ/YMCV_OBJ.h"
#include "./YMCV_HistCal.h"

typedef enum
{
	CV_SingleThreshold,//单阈值
	CV_DoubleThreshold,//双阈值
	CV_RGB2Threshold,//RGB双阈值
	CV_LAB2Threshold,//LAB双阈值
	CV_IntegralThreshold,//积分图二值化
	CV_HistThreshold,//直方图二值化
}CVThreshold;

typedef enum
{
	CV_And,//逻辑与
	CV_Or,//逻辑或
	CV_Not,//逻辑非
}CVLOGIC;

typedef  union
{
	//灰度单阈值
	struct
	{
		uint8 thresh;
	}gray_single_t;
	//灰度带通双阈值 
	struct
	{
		uint8 thresh_min;
		uint8 thresh_max;
	}gray_double_t;
	//RGB三通道双阈值 
	struct
	{
		uint8 thresh_Rmin;
		uint8 thresh_Rmax;
		uint8 thresh_Gmin;
		uint8 thresh_Gmax;
		uint8 thresh_Bmin;
		uint8 thresh_Bmax;
	}rgb_double_t;
	//直方图自动阈值
	struct
	{
		CVHistAutoThresh thresh_type;
		uint8 p_tile_val;//取值区间：[0，100]
	}hist_auto_t;
	//LAB三通道双阈值 
	struct
	{
		//L取值 0~100
		int8 thresh_Lmin;
		int8 thresh_Lmax;
		//A取值 -128~127
		int8 thresh_Amin;
		int8 thresh_Amax;
		//B取值 -128~127
		int8 thresh_Bmin;
		int8 thresh_Bmax;
	}lab_double_t;
	//积分图阈值 
	struct
	{
		uint8 thresh_percent;//取高于区域灰度均值的 T %部分
		uint8 kernerl_radius;//半径
	}gray_integral_t;

}CV_binary_param;


typedef  union
{
	//单通道
	struct
	{
		uint8* PixMap;
	}single;
	//三通道
	struct
	{
		uint8* PixMap1;
		uint8* PixMap2;
		uint8* PixMap3;
	}three;
}CV_binary_vec;

typedef struct
{
	CVThreshold ThreshType;//二值化类型
	CV_binary_vec myVector;//缓存向量
	uint8 reserve1;
	uint8 reserve2;
}CVbinary_render;

typedef CVbinary_render* CVBINARY_RENDER;


void YMCV_Image_ThreshTo_Binary(CVIMAGE Imgin, CVIMAGE Imgout, CVBINARY_RENDER render);//二值化渲染
void YMCV_BinaryKernel_Load(CVBINARY_RENDER render, CV_binary_param* Param);//二值化渲染器参数加载
void YMCV_Image_Binary_Logic_Cal(CVIMAGE Imgin, CVIMAGE Imginout, CVLOGIC LogicCalType);//二值图像逻辑运算
void YMCV_Binary_AreaFilter(CVIMAGE myimg, uint32 minArea);//二值图面积过滤
void YMCV_Image_BinaryMask_Cal(CVIMAGE Imgin, CVIMAGE BinaryMask);//图像掩码提取

#endif

