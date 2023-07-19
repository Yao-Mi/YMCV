#ifndef YMCV_CHANNELCAL_H
#define YMCV_CHANNELCAL_H
#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_PubType.h"
#include "../OPOBJ/YMCV_OBJ.h"

typedef enum
{
	CV_Channel_CustomTransform = 1,//�Զ���任
	CV_Channel_SingleToThree,//��ͨ��ת��ͨ��
	CV_Channel_ThreeToSingle,//��ͨ��ת��ͨ��
	CV_ThreshToBin,//����ֵ��ֵ��
	CV_Thresh2ToBin,//˫��ֵ��ֵ��
	CV_Expose,//�ع�
	CV_ColorTo,//Ⱦɫ
	CV_Bright,//������ǿ
	CV_Invers,//��ɫ
	CV_Contract,//�Աȶ���ǿ
	CV_Liner_Enhance,//�ֶ�������ǿ
	CV_Log_Enhance,//������������ǿ
	CV_Exp_Enhance,//ָ����������ǿ
	CV_Gama_Enhance,//gama�任��ǿ
	CV_Scurve_Enhance,//S���߱任��ǿ
}CVPXPMap;

//��ͨ������
typedef  union
{
	//�Ҷȵ���ֵ��ֵ������
	struct
	{
		CVgray Thresh;
	}gray_tobin;
	//�Ҷ�˫��ֵ��ֵ��
	struct
	{
		CVgray leftThresh;
		CVgray rightThresh;
	}gray_2tobin;
	//�ع�    x<128? (255-x):x  
	struct
	{
		CVgray thresh;
	}gray_expose;
	//Ⱦɫ ֵ��[0,1]
	struct
	{
		float32 graycolor;
	}gray_colorto;
	//������ǿ  ��������ϵ��  ��Χ:0~5 
	struct
	{
		float32 _k;
	}gray_bright;
	//��ɫ�Ҷȷ�ɫ����ϵ��  ��Χ:0~5   k * (255-x)
	struct
	{
		float32 _k;
	}gray_invers;
	//�Աȶ���ǿ �Աȶ���ǿϵ�� ��Χ:-100~100
	struct
	{
		float32 _k;
	}gray_contract;
	//�ֶ�������ǿ [0,a�� [a,b�� [b,c��,[c,255] ������ӳ��
	struct
	{
		CVgray from_a;
		CVgray from_b;
		CVgray from_c;
		CVgray to_a;
		CVgray to_b;
		CVgray to_c;
	}gray_liner_enhance;
	//������������ǿ a + ln(x + 1)/(b * ln(c))
	struct
	{
		float32 _a;
		float32 _b;
		float32 _c;
	}gray_log_enhance;
	//ָ����������ǿ 1 + b ^( c*(x - a))
	struct
	{
		float32 _b;
		float32 _c;
		float32 _a;
	}gray_exp_enhance;
	//gama�任��ǿ c * (x/255)^gama
	struct
	{
		float32 _c;
		float32 _gama;
	}gray_gama_enhance;
	//S���߱任��ǿ 255*(1/( 1 + (m/x)^E))
	struct
	{
		float32 _m;
		float32 _E;
	}gray_scurve_enhance;
}CV_pixchannel_1param;

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
	//�Ҷ�˫��ֵ��ֵ��
	struct
	{
		CVgray R_leftThresh;
		CVgray R_rightThresh;

		CVgray G_leftThresh;
		CVgray G_rightThresh;

		CVgray B_leftThresh;
		CVgray B_rightThresh;
	}rgb_2tobin;
	//�ع�
	struct
	{
		CVgray R_thresh;
		CVgray G_thresh;
		CVgray B_thresh;
	}rgb_expose;
	//Ⱦɫ ֵ��[0,1]
	struct
	{
		float32 R_color;
		float32 G_color;
		float32 B_color;
	}rgb_colorto;
	//������ǿ  ��������ϵ��  ��Χ:0~5
	struct
	{
		float32 R_k;
		float32 G_k;
		float32 B_k;
	}rgb_bright;
	//��ɫ�Ҷȷ�ɫ����ϵ��  ��Χ:0~5
	struct
	{
		float32 R_k;
		float32 G_k;
		float32 B_k;
	}rgb_invers;
	//�Աȶ���ǿ �Աȶ���ǿϵ�� ��Χ:-100~100
	struct
	{
		float32 R_k;
		float32 G_k;
		float32 B_k;
	}rgb_contract;
	//�ֶ�������ǿ [0,a] [a,b] [b,255]
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
	//������������ǿ a + ln(x + 1)/(b * ln(c))
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
	//ָ����������ǿ 1 + b ^( c*(x - a))
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
	//gama�任��ǿ c * (x/255)^gama
	struct
	{
		float32 R_c;
		float32 R_gama;

		float32 G_c;
		float32 G_gama;

		float32 B_c;
		float32 B_gama;
	}rgb_gama_enhance;
	//S���߱任��ǿ 255*(1/( 1 + (m/x)^E))
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


//ͨ����Ⱦ��
typedef struct
{
	CVPXPMap PXPType;//ӳ������
	PIXChannelNum ChannelNum;//��/3ͨ��
	uint8* PixTrTable;
}CVpix_render;
typedef CVpix_render* CVPIX_RENDER;

void YMCV_SingleChannel_MapTo(CVPIX_RENDER render, CV_pixchannel_1param* Param);//��ʼ����ͨ��Ⱦͨ��
void YMCV_ThreeChannel_MapTo(CVPIX_RENDER render, CV_pixchannel_3param* Param);//��ʼ����ͨ��Ⱦͨ��
void YMCV_Image_ChannelRenderTo(CVIMAGE Imgin, CVIMAGE Imgout, CVPIX_RENDER render);//ͼ��ͨ����Ⱦ
void YMCV_Image_ChannelRenderTo_UGray(CVIMAGE Imgin, CVIMAGE Imgout, CVPIX_RENDER render);//ͼ��ͨ��Ⱦɫ ʹ�ûҶ�ǿ�����ο�

void YMCV_SingleChannel_CustomMapTo(CVPIX_RENDER render, int32(*RenderFunction)(int32 PixValue));//��ͨ��ӳ����Ⱦ����ʼ��
void YMCV_ThreeChannel_CustomMapTo(CVPIX_RENDER render, int32(*R_RenderFunction)(int32 R_PixValue), int32(*G_RenderFunction)(int32 G_PixValue), int32(*B_RenderFunction)(int32 B_PixValue));//��ͨ��ӳ����Ⱦ����ʼ��
void YMCV_ThreeChannel_PaletteMapTo(CVPIX_RENDER render, color24_t* palette256);//�õ�ɫ���ʼ����Ⱦͨ��
void YMCV_Image_SingleToThree_ChannelRenderTo(CVIMAGE Imgin, CVIMAGE Imgout, CVPIX_RENDER render);//��ͨ��ת��ͨ����Ⱦ
void YMCV_Image_ThreeToSingle_ChannelRenderTo(CVIMAGE Imgin, CVIMAGE Imgout, CVPIX_RENDER render);//��ͨ��ת��ͨ����Ⱦ

#endif // !YMCV_CHANNELCAL_H

