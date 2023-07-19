#ifndef YMCV_GEOMCAL_H
#define YMCV_GEOMCAL_H

#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_PubType.h"
#include "../OPOBJ/YMCV_OBJ.h"
#include "./YMCV_FastMath.h"

typedef enum
{
	CV_WinSize_Const=0, //�̶�����
	CV_WinSize_Auto,//�Զ�����
}CVWINSIZE;

typedef enum
{
	CV_Stretch=1,  //���ڽ��ڲ巨 ���� | ѹ��
	CV_StretchL2,//˫�����ڲ巨 ���� | ѹ��

	CV_RotateWauto,//ͼ����ת�任 �����������Ӧ
	CV_RotateWconst,//ͼ����ת�任 �̶�����

	CV_LinerPolar,//ͼ�����Լ�����任
	CV_LogPolar,//ͼ�����������任
	CV_LinerPolar_Inverse,
	CV_LogPolar_Inverse,

	CV_Perspective,//͸�ӱ任
}CVGEOMETHOD;

typedef  union
{
	//����
	struct
	{
		float32* X_a;
		float32* X_b;
		uint16* Xi;
		uint16* Yi;
	}stretch;
	//��ת
	struct
	{
		float32* X_a;
		float32* X_b;
	}rotate;
	//������任
	struct
	{
		float32* reserve;//δʹ�ã�����
	}Linpolar;
	struct
	{
		float32* pRho;
	}Logpolar;
	struct
	{
		float32* pDetai;
		float32* pIs2;
	}Inversepolar;
	//͸�ӱ任
	struct
	{
		float32* X_a;
		float32* X_b;
	}perspect;

}CV_geom_vec;

typedef struct
{
	CVGEOMETHOD GeomType;//���α任����
	uint16 outw, outh;
	CV_geom_vec myVector;//��������
	void* (*VecMalloc)(size_t size);
}CVgeom_render;

typedef CVgeom_render* CVGEOM_RENDER;

//���α任��Ⱦ����
typedef  union
{
	//����
	struct 
	{
		uint16 out_w;
		uint16 out_h;
		uint16 Imgin_w;
		uint16 Imgin_h;
	}stretch;
	//��ת
	struct 
	{
		uint16 Imgin_w;
		uint16 Imgin_h;
		float32 angle;
		uint16 out_w;
		uint16 out_h;
	}rotate;
	//������任
	struct 
	{
		uint16 Imgin_w;
		uint16 Imgin_h;
	}polar;

	//͸�ӱ任
	struct
	{
		float32 x_angle;
		float32 y_angle;
		float32 z_angle;
		float32 x_move;
		float32 y_move;
		float32 scale;
		uint16 Imgin_w;
		uint16 Imgin_h;
		CVWINSIZE wsize_type;
	}perspect;

}CV_geom_param;

void YMCV_GeomKernel_Load(CVGEOM_RENDER render, CV_geom_param* Param);//��Ⱦ����������

CVIMAGE YMCV_Image_Geom_RenderTo(CVIMAGE Imgin, CVGEOM_RENDER render); //���α任 

CVFMAT YMCV_FindHomography_Ransac(CVVECTORS_U16 X2, CVVECTORS_U16 X1, float32 pConfidence, uint32 innerThreshPow2);//���㵥Ӧ�Ծ���
CVGEOM_RENDER YMCV_HomographyMat_ToPerspective_GeomRender(CVFMAT myMat, uint16 Imgin_w, uint16 Imgin_h, CVWINSIZE wsize_type);//��Ӧ����ת͸����Ⱦ��

#endif // !YMCV_GEOMCAL_H
