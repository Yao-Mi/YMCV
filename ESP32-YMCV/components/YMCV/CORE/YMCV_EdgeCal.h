#ifndef YMCV_EDGECAL_H
#define YMCV_EDGECAL_H
#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_PubType.h"
#include "../OPOBJ/YMCV_OBJ.h"


typedef enum
{
	//输出原始图
	CV_Sobel_0,
	CV_Log_0,
	CV_Laplace_0,
	//只保留边界
	CV_Sobel,//sobel
	CV_Log,//log
	CV_Laplace,//laplace，一般用于二值图边缘检测
	CV_Canny,//canny
	CV_Sobel_Connect,//sobel + 局部相似连接
}CVEdgeType;

typedef struct
{
	CVEdgeType EdgeType;//二值化类型
	union
	{
		//sobel
		struct
		{
			uint16 minT;
		}sobel;
		//log
		struct
		{
			uint16 minT;
		}log;
		//laplace
		struct
		{
			uint16 minT;
		}laplace;

		//canny
		struct 
		{
			uint16 StrongT;
			uint16 WeakT;
		}canny;
		//sobel connect
		struct
		{
			uint16 minT;//幅度阈值，与sobel相同
			int8 connect_r;//相似半径
			uint8 valSimilarT;//幅度相似阈值
			uint8 angSimilarT;//角度相似阈值
		}sobel_connect;
	}Thresh;

}CV_edge_param;

typedef CV_edge_param* CV_Edge_Param;

void YMCV_Gray_EdgeDetect(CVIMAGE Imgin, CVIMAGE Imgout, CV_Edge_Param edgeParam);//边缘检测

uint16 YMCV_SobelCal_ValueAndMax(uint8* datap, uint16 Width, uint16 Height, uint16* sobelVal);//sobel计算幅度，同时返回最大值
void YMCV_Sobel_ValueCompress_To255(uint16* sobelImage, uint16 SobleVmax, uint32 ImageSize);//sobel幅度范围压缩到[0,255]

#endif

