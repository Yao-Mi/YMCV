#ifndef YMCV_CHANNELCAL_H
#define YMCV_CHANNELCAL_H
#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_PubType.h"
#include "../OPOBJ/YMCV_OBJ.h"

typedef enum
{
	CV_Channel_CustomTransform = 1,//自定义变换
	CV_Channel_SingleToThree,//单通道转三通道
	CV_Channel_ThreeToSingle,//三通道转单通道
	CV_ThreshToBin,//单阈值二值化
	CV_Thresh2ToBin,//双阈值二值化
	CV_Expose,//曝光
	CV_ColorTo,//染色
	CV_Bright,//亮度增强
	CV_Invers,//反色
	CV_Contract,//对比度增强
	CV_Liner_Enhance,//分段线性增强
	CV_Log_Enhance,//对数非线性增强
	CV_Exp_Enhance,//指数非线性增强
	CV_Gama_Enhance,//gama变换增强
	CV_Scurve_Enhance,//S曲线变换增强
}CVPXPMap;

//单通道参数
typedef  union
{
	//灰度单阈值二值化参数
	struct
	{
		CVgray Thresh;
	}gray_tobin;
	//灰度双阈值二值化
	struct
	{
		CVgray leftThresh;
		CVgray rightThresh;
	}gray_2tobin;
	//曝光    x<128? (255-x):x  
	struct
	{
		CVgray thresh;
	}gray_expose;
	//染色 值在[0,1]
	struct
	{
		float32 graycolor;
	}gray_colorto;
	//亮度增强  亮度增益系数  范围:0~5 
	struct
	{
		float32 _k;
	}gray_bright;
	//反色灰度反色增益系数  范围:0~5   k * (255-x)
	struct
	{
		float32 _k;
	}gray_invers;
	//对比度增强 对比度增强系数 范围:-100~100
	struct
	{
		float32 _k;
	}gray_contract;
	//分段线性增强 [0,a） [a,b） [b,c）,[c,255] 区间重映射
	struct
	{
		CVgray from_a;
		CVgray from_b;
		CVgray from_c;
		CVgray to_a;
		CVgray to_b;
		CVgray to_c;
	}gray_liner_enhance;
	//对数非线性增强 a + ln(x + 1)/(b * ln(c))
	struct
	{
		float32 _a;
		float32 _b;
		float32 _c;
	}gray_log_enhance;
	//指数非线性增强 1 + b ^( c*(x - a))
	struct
	{
		float32 _b;
		float32 _c;
		float32 _a;
	}gray_exp_enhance;
	//gama变换增强 c * (x/255)^gama
	struct
	{
		float32 _c;
		float32 _gama;
	}gray_gama_enhance;
	//S曲线变换增强 255*(1/( 1 + (m/x)^E))
	struct
	{
		float32 _m;
		float32 _E;
	}gray_scurve_enhance;
}CV_pixchannel_1param;

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
	//灰度双阈值二值化
	struct
	{
		CVgray R_leftThresh;
		CVgray R_rightThresh;

		CVgray G_leftThresh;
		CVgray G_rightThresh;

		CVgray B_leftThresh;
		CVgray B_rightThresh;
	}rgb_2tobin;
	//曝光
	struct
	{
		CVgray R_thresh;
		CVgray G_thresh;
		CVgray B_thresh;
	}rgb_expose;
	//染色 值在[0,1]
	struct
	{
		float32 R_color;
		float32 G_color;
		float32 B_color;
	}rgb_colorto;
	//亮度增强  亮度增益系数  范围:0~5
	struct
	{
		float32 R_k;
		float32 G_k;
		float32 B_k;
	}rgb_bright;
	//反色灰度反色增益系数  范围:0~5
	struct
	{
		float32 R_k;
		float32 G_k;
		float32 B_k;
	}rgb_invers;
	//对比度增强 对比度增强系数 范围:-100~100
	struct
	{
		float32 R_k;
		float32 G_k;
		float32 B_k;
	}rgb_contract;
	//分段线性增强 [0,a] [a,b] [b,255]
	struct
	{
		CVgray R_from_a;
		CVgray R_from_b;
		CVgray R_from_c;
		CVgray R_to_a;
		CVgray R_to_b;
		CVgray R_to_c;

		CVgray G_from_a;
		CVgray G_from_b;
		CVgray G_from_c;
		CVgray G_to_a;
		CVgray G_to_b;
		CVgray G_to_c;

		CVgray B_from_a;
		CVgray B_from_b;
		CVgray B_from_c;
		CVgray B_to_a;
		CVgray B_to_b;
		CVgray B_to_c;
	}rgb_liner_enhance;
	//对数非线性增强 a + ln(x + 1)/(b * ln(c))
	struct
	{
		float32 R_a;
		float32 R_b;
		float32 R_c;

		float32 G_a;
		float32 G_b;
		float32 G_c;

		float32 B_a;
		float32 B_b;
		float32 B_c;
	}rgb_log_enhance;
	//指数非线性增强 1 + b ^( c*(x - a))
	struct
	{
		float32 R_b;
		float32 R_c;
		float32 R_a;

		float32 G_b;
		float32 G_c;
		float32 G_a;

		float32 B_b;
		float32 B_c;
		float32 B_a;
	}rgb_exp_enhance;
	//gama变换增强 c * (x/255)^gama
	struct
	{
		float32 R_c;
		float32 R_gama;

		float32 G_c;
		float32 G_gama;

		float32 B_c;
		float32 B_gama;
	}rgb_gama_enhance;
	//S曲线变换增强 255*(1/( 1 + (m/x)^E))
	struct
	{
		float32 R_m;
		float32 R_E;

		float32 G_m;
		float32 G_E;

		float32 B_m;
		float32 B_E;
	}rgb_scurve_enhance;
}CV_pixchannel_3param;


//通道渲染器
typedef struct
{
	CVPXPMap PXPType;//映射类型
	PIXChannelNum ChannelNum;//单/3通道
	uint8* PixTrTable;
}CVpix_render;
typedef CVpix_render* CVPIX_RENDER;

void YMCV_SingleChannel_MapTo(CVPIX_RENDER render, CV_pixchannel_1param* Param);//初始化单通渲染通道
void YMCV_ThreeChannel_MapTo(CVPIX_RENDER render, CV_pixchannel_3param* Param);//初始化三通渲染通道
void YMCV_Image_ChannelRenderTo(CVIMAGE Imgin, CVIMAGE Imgout, CVPIX_RENDER render);//图像通道渲染
void YMCV_Image_ChannelRenderTo_UGray(CVIMAGE Imgin, CVIMAGE Imgout, CVPIX_RENDER render);//图像通道染色 使用灰度强度做参考

void YMCV_SingleChannel_CustomMapTo(CVPIX_RENDER render, int32(*RenderFunction)(int32 PixValue));//单通道映射渲染器初始化
void YMCV_ThreeChannel_CustomMapTo(CVPIX_RENDER render, int32(*R_RenderFunction)(int32 R_PixValue), int32(*G_RenderFunction)(int32 G_PixValue), int32(*B_RenderFunction)(int32 B_PixValue));//三通道映射渲染器初始化
void YMCV_ThreeChannel_PaletteMapTo(CVPIX_RENDER render, color24_t* palette256);//用调色板初始化渲染通道
void YMCV_Image_SingleToThree_ChannelRenderTo(CVIMAGE Imgin, CVIMAGE Imgout, CVPIX_RENDER render);//单通道转三通道渲染
void YMCV_Image_ThreeToSingle_ChannelRenderTo(CVIMAGE Imgin, CVIMAGE Imgout, CVPIX_RENDER render);//三通道转单通道渲染

#endif // !YMCV_CHANNELCAL_H

