#ifndef YMCV_GEOMCAL_H
#define YMCV_GEOMCAL_H

#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_PubType.h"
#include "../OPOBJ/YMCV_OBJ.h"
#include "./YMCV_FastMath.h"

typedef enum
{
	CV_WinSize_Const=0, //固定窗口
	CV_WinSize_Auto,//自动窗口
}CVWINSIZE;

typedef enum
{
	CV_Stretch=1,  //最邻近内插法 拉伸 | 压缩
	CV_StretchL2,//双线性内插法 拉伸 | 压缩

	CV_RotateWauto,//图像旋转变换 输出窗口自适应
	CV_RotateWconst,//图像旋转变换 固定窗口

	CV_LinerPolar,//图像线性极坐标变换
	CV_LogPolar,//图像对数极坐标变换
	CV_LinerPolar_Inverse,
	CV_LogPolar_Inverse,

	CV_Perspective,//透视变换
}CVGEOMETHOD;

typedef  union
{
	//拉伸
	struct
	{
		float32* X_a;
		float32* X_b;
		uint16* Xi;
		uint16* Yi;
	}stretch;
	//旋转
	struct
	{
		float32* X_a;
		float32* X_b;
	}rotate;
	//极坐标变换
	struct
	{
		float32* reserve;//未使用，保留
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
	//透视变换
	struct
	{
		float32* X_a;
		float32* X_b;
	}perspect;

}CV_geom_vec;

typedef struct
{
	CVGEOMETHOD GeomType;//几何变换类型
	uint16 outw, outh;
	CV_geom_vec myVector;//缓存向量
	void* (*VecMalloc)(size_t size);
}CVgeom_render;

typedef CVgeom_render* CVGEOM_RENDER;

//几何变换渲染参数
typedef  union
{
	//拉伸
	struct 
	{
		uint16 out_w;
		uint16 out_h;
		uint16 Imgin_w;
		uint16 Imgin_h;
	}stretch;
	//旋转
	struct 
	{
		uint16 Imgin_w;
		uint16 Imgin_h;
		float32 angle;
		uint16 out_w;
		uint16 out_h;
	}rotate;
	//极坐标变换
	struct 
	{
		uint16 Imgin_w;
		uint16 Imgin_h;
	}polar;

	//透视变换
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

void YMCV_GeomKernel_Load(CVGEOM_RENDER render, CV_geom_param* Param);//渲染器参数加载

CVIMAGE YMCV_Image_Geom_RenderTo(CVIMAGE Imgin, CVGEOM_RENDER render); //几何变换 

CVFMAT YMCV_FindHomography_Ransac(CVVECTORS_U16 X2, CVVECTORS_U16 X1, float32 pConfidence, uint32 innerThreshPow2);//计算单应性矩阵
CVGEOM_RENDER YMCV_HomographyMat_ToPerspective_GeomRender(CVFMAT myMat, uint16 Imgin_w, uint16 Imgin_h, CVWINSIZE wsize_type);//单应矩阵转透视渲染器

#endif // !YMCV_GEOMCAL_H
