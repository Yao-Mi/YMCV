#ifndef YMCV_HISTCAL_H
#define YMCV_HISTCAL_H
#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_PubType.h"
#include "../OPOBJ/YMCV_OBJ.h"
#include "./YMCV_ChannelCal.h"
#include "./YMCV_Padding.h"

typedef enum
{
	CV_HistOtsu,//Otsu��ֵ
	CV_HistIterate,//������ֵ
	CV_HistAver,//ƽ��ֵ
	CV_HistPTile,//�ٷֱ���ֵ��P-Tile
	CV_HistPeaksMin,//˫��ȵ���Сֵ
	CV_HistPeaksAver,//˫��ƽ��ֵ
	CV_HistMaxEntropy,//һά�����
	CV_HistTriangle,//������ֵ��
}CVHistAutoThresh;


void YMCV_Image_Hist_Get(CVIMAGE Imgin, int32* myHist, uint16 histSize, PIXChannelNum channel);//��ȡ�Ҷ�ͼ�ֲ�ֱ��ͼ
void YMCV_GrayHist_BackProject(int32* myHist, uint16 histSize, CVIMAGE myimg);//�Ҷ�ͼֱ��ͼ����ͶӰ

void YMCV_Image_Hist_Equalize(CVIMAGE Imgin, CVIMAGE Imgout);//ͼ��Ҷ�ֱ��ͼ���⻯
void YMCV_Image_Hist_MapTo(CVIMAGE Imgto, CVIMAGE Imgfrom);//ֱ��ͼ�涨 
void YMCV_Image_Hist_To(CVIMAGE Imgin, CVIMAGE Imgout, int32* myHist, uint16 histSize, PIXChannelNum channel);//ͼ��ֱ��ͼ�涨��
void YMCV_Image_Hist_Equalize_UGray(CVIMAGE Imgin, CVIMAGE Imgout);//ͼ�� ���ݻҶ�ֱ��ͼ���о��⻯
void YMCV_Hist_FindPeaks(int32* myHist, uint16 histSize, int8* peaksout);//��ֵ����

CVIMAGE YMCV_Gray_Local_Hist_Equalize(CVIMAGE myimg, uint8 radius, YMCVPADDINGType padding);//�ֲ�ֱ��ͼ����

uint8 YMCV_GrayHist_Get_AutoThresh(int32 myHist[256], CVHistAutoThresh threshType);//��ȡ�Ҷ�ͳ��ֱ��ͼ����ֵ

void YMCV_U16Hist_LenCompress_To255(int32 inHist_outPixMap[], uint16 maxIndex, uint32 tolnum);//���ݳ���ѹ��

#endif // !YMCV_HISTCAL_H

