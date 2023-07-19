#ifndef YMCV_FREQCAL_H
#define YMCV_FREQCAL_H
#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_PubType.h"
#include "../OPOBJ/YMCV_OBJ.h"
#include "./YMCV_FastMath.h"
typedef enum
{
	//FFT��
	CV_FFTFilterBegin=1,//����FFT��ʼ(δʹ��)
	CV_GaussHFilter, //��˹��ͨ�˲�
	CV_GaussLFilter, //��˹��ͨ�˲�
    CV_ButterWorthHFilter, //������˹��ͨ�˲�
    CV_ButterWorthLFilter, //������˹��ͨ�˲�
	CV_BandPassFilter, //��ͨ�˲�
	CV_BandStopFilter, //�����˲�

	//HARR��
	CV_HarrFilterBegin,//����Harr��ʼ(δʹ��)
	CV_HarrThreshFilter, //����С��Ӳ��ֵ�˲�

}CVFREQTYPE;

typedef enum
{
	CV_RealAndImag,//ʵ�����鲿
	CV_RhoAndTheta,//���Ⱥ���λ
}CVCOMPLEXTYPE;//������ʽ

typedef struct
{
	uint16 width;
	uint16 height;
	CVFFT_RENDER fftw;//����fft��
	CVFFT_RENDER ffth;//����fft��
}CVfft2d_render;
typedef CVfft2d_render* CVFFT2D_RENDER;

typedef struct
{
	uint16 width;
	uint16 height;
	CVDCT_RENDER dctw;//����fft��
	CVDCT_RENDER dcth;//����fft��
}CVdct2d_render;
typedef CVdct2d_render* CVDCT2D_RENDER;

typedef struct
{
	uint16 width;
	uint16 height;
	CVDWT_RENDER dwtw;//����dwt��
	CVDWT_RENDER dwth;//����dwt��
}CVdwt2d_render;
typedef CVdwt2d_render* CVDWT2D_RENDER;


//��ͨ������
typedef  union
{
	//��˹�˲�
	struct
	{
		uint16 D0;//����Ƶ��
	}gauss;
	//������˹
	struct
	{
		uint16 D0;//����Ƶ��
		uint16 n;//n�η�
	}butterworth;

	//��ͨ������
	struct
	{
		uint16 D0;//����Ƶ��
		uint16 W;//����
	}band;
	//����С��
	struct
	{
		uint16 threshold;//Ӳ��ֵ
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

void YMCV_Freq_Kernel_Load(CVFREQ_RENDER render, CV_freq_1param param); //Ƶ���˲������أ���ʼ����

void YMCV_Gray_Image_FFT(CVFFT2D_RENDER render, CVIMAGE myimg, CVIMAGE imgc);//�Ҷ�ͼfft
void YMCV_Gray_Image_IFFT(CVFFT2D_RENDER render, CVIMAGE myimg, CVIMAGE imgc);
void YMCV_CGray_Image_FFT(CVFFT2D_RENDER render, CVIMAGE myimgc, CVIMAGE imgoc);//����ͼfft
void YMCV_CGray_Image_IFFT(CVFFT2D_RENDER render, CVIMAGE myimgc, CVIMAGE imgc);
void YMCV_CGray_Image_FFTshift(CVIMAGE imgc);

void YMCV_CGray_Image_MagnitudeAndPhase(CVIMAGE imgc); //Ƶ��ͼ�����ģ������λ
void YMCV_CGray_Image_MagPhase_ToComplex(CVIMAGE imgc);//Ƶ��ͼ��ģ����λת����ͼ

void YMCV_CGray_Image_LogMag_AndPhase(CVIMAGE imgc);//Ƶ��ͼ�����ģ��(��log�任)����λ 
void YMCV_CGray_Image_expMag_ToComplex(CVIMAGE imgc);//Ƶ��ͼ��ģ��(��exp�任)��λת����ͼ

void YMCV_Gray_Image_DCT(CVDCT2D_RENDER render, CVIMAGE myimg, CVIMAGE imgf);
void YMCV_Gray_Image_IDCT(CVDCT2D_RENDER render, CVIMAGE myimg, CVIMAGE imgf);

void YMCV_Gray_Image_DWT(CVDWT2D_RENDER render, CVIMAGE myimg, CVIMAGE imgf);
void YMCV_Gray_Image_IDWT(CVDWT2D_RENDER render, CVIMAGE myimg, CVIMAGE imgf);

CVIMAGE YMCV_Gray_Image_Freq_Kernel_Render(CVIMAGE myimg, CVFREQ_RENDER render);//ͼ��Ƶ����˲���Ⱦ

#endif //!YMCV_FREQCAL_H