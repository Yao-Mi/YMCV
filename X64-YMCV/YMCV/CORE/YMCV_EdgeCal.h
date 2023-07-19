#ifndef YMCV_EDGECAL_H
#define YMCV_EDGECAL_H
#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_PubType.h"
#include "../OPOBJ/YMCV_OBJ.h"


typedef enum
{
	//���ԭʼͼ
	CV_Sobel_0,
	CV_Log_0,
	CV_Laplace_0,
	//ֻ�����߽�
	CV_Sobel,//sobel
	CV_Log,//log
	CV_Laplace,//laplace��һ�����ڶ�ֵͼ��Ե���
	CV_Canny,//canny
	CV_Sobel_Connect,//sobel + �ֲ���������
}CVEdgeType;

typedef struct
{
	CVEdgeType EdgeType;//��ֵ������
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
			uint16 minT;//������ֵ����sobel��ͬ
			int8 connect_r;//���ư뾶
			uint8 valSimilarT;//����������ֵ
			uint8 angSimilarT;//�Ƕ�������ֵ
		}sobel_connect;
	}Thresh;

}CV_edge_param;

typedef CV_edge_param* CV_Edge_Param;

void YMCV_Gray_EdgeDetect(CVIMAGE Imgin, CVIMAGE Imgout, CV_Edge_Param edgeParam);//��Ե���

uint16 YMCV_SobelCal_ValueAndMax(uint8* datap, uint16 Width, uint16 Height, uint16* sobelVal);//sobel������ȣ�ͬʱ�������ֵ
void YMCV_Sobel_ValueCompress_To255(uint16* sobelImage, uint16 SobleVmax, uint32 ImageSize);//sobel���ȷ�Χѹ����[0,255]

#endif

