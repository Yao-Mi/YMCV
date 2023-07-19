#ifndef YMCV_FREQCAL_H
#define YMCV_FREQCAL_H
#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_PubType.h"
#include "../OPOBJ/YMCV_OBJ.h"
#include "./YMCV_FastMath.h"
typedef enum
{
	//FFT区
	CV_FFTFilterBegin=1,//基于FFT起始(未使用)
	CV_GaussHFilter, //高斯高通滤波
	CV_GaussLFilter, //高斯低通滤波
    CV_ButterWorthHFilter, //巴特沃斯高通滤波
    CV_ButterWorthLFilter, //巴特沃斯低通滤波
	CV_BandPassFilter, //带通滤波
	CV_BandStopFilter, //带阻滤波

	//HARR区
	CV_HarrFilterBegin,//基于Harr起始(未使用)
	CV_HarrThreshFilter, //哈尔小波硬阈值滤波

}CVFREQTYPE;

typedef enum
{
	CV_RealAndImag,//实部和虚部
	CV_RhoAndTheta,//幅度和相位
}CVCOMPLEXTYPE;//复数格式

typedef struct
{
	uint16 width;
	uint16 height;
	CVFFT_RENDER fftw;//横向fft核
	CVFFT_RENDER ffth;//纵向fft核
}CVfft2d_render;
typedef CVfft2d_render* CVFFT2D_RENDER;

typedef struct
{
	uint16 width;
	uint16 height;
	CVDCT_RENDER dctw;//横向fft核
	CVDCT_RENDER dcth;//纵向fft核
}CVdct2d_render;
typedef CVdct2d_render* CVDCT2D_RENDER;

typedef struct
{
	uint16 width;
	uint16 height;
	CVDWT_RENDER dwtw;//横向dwt核
	CVDWT_RENDER dwth;//纵向dwt核
}CVdwt2d_render;
typedef CVdwt2d_render* CVDWT2D_RENDER;


//单通道参数
typedef  union
{
	//高斯滤波
	struct
	{
		uint16 D0;//截至频率
	}gauss;
	//巴特沃斯
	struct
	{
		uint16 D0;//截至频率
		uint16 n;//n次方
	}butterworth;

	//带通、带阻
	struct
	{
		uint16 D0;//中心频率
		uint16 W;//带宽
	}band;
	//哈尔小波
	struct
	{
		uint16 threshold;//硬阈值
	}harr;
}CV_freq_1param;

typedef struct
{
	CVFREQTYPE KernelType;
	CVCOMPLEXTYPE ComplexType;
	uint16 threshold;
	uint16 reserve;
}CVfreq_render;
typedef CVfreq_render* CVFREQ_RENDER;

void YMCV_Freq_Kernel_Load(CVFREQ_RENDER render, CV_freq_1param param); //频域滤波器加载（初始化）

void YMCV_Gray_Image_FFT(CVFFT2D_RENDER render, CVIMAGE myimg, CVIMAGE imgc);//灰度图fft
void YMCV_Gray_Image_IFFT(CVFFT2D_RENDER render, CVIMAGE myimg, CVIMAGE imgc);
void YMCV_CGray_Image_FFT(CVFFT2D_RENDER render, CVIMAGE myimgc, CVIMAGE imgoc);//复数图fft
void YMCV_CGray_Image_IFFT(CVFFT2D_RENDER render, CVIMAGE myimgc, CVIMAGE imgc);
void YMCV_CGray_Image_FFTshift(CVIMAGE imgc);

void YMCV_CGray_Image_MagnitudeAndPhase(CVIMAGE imgc); //频域图像计算模长和相位
void YMCV_CGray_Image_MagPhase_ToComplex(CVIMAGE imgc);//频域图像模长相位转复数图

void YMCV_CGray_Image_LogMag_AndPhase(CVIMAGE imgc);//频域图像计算模长(含log变换)和相位 
void YMCV_CGray_Image_expMag_ToComplex(CVIMAGE imgc);//频域图像模长(含exp变换)相位转复数图

void YMCV_Gray_Image_DCT(CVDCT2D_RENDER render, CVIMAGE myimg, CVIMAGE imgf);
void YMCV_Gray_Image_IDCT(CVDCT2D_RENDER render, CVIMAGE myimg, CVIMAGE imgf);

void YMCV_Gray_Image_DWT(CVDWT2D_RENDER render, CVIMAGE myimg, CVIMAGE imgf);
void YMCV_Gray_Image_IDWT(CVDWT2D_RENDER render, CVIMAGE myimg, CVIMAGE imgf);

CVIMAGE YMCV_Gray_Image_Freq_Kernel_Render(CVIMAGE myimg, CVFREQ_RENDER render);//图像频域核滤波渲染

#endif //!YMCV_FREQCAL_H