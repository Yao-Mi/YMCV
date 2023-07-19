#ifndef YMCV_HISTCAL_H
#define YMCV_HISTCAL_H
#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_PubType.h"
#include "../OPOBJ/YMCV_OBJ.h"
#include "./YMCV_ChannelCal.h"
#include "./YMCV_Padding.h"

typedef enum
{
	CV_HistOtsu,//Otsu阈值
	CV_HistIterate,//迭代阈值
	CV_HistAver,//平均值
	CV_HistPTile,//百分比阈值，P-Tile
	CV_HistPeaksMin,//双峰谷底最小值
	CV_HistPeaksAver,//双峰平均值
	CV_HistMaxEntropy,//一维最大熵
	CV_HistTriangle,//三角阈值法
}CVHistAutoThresh;


void YMCV_Image_Hist_Get(CVIMAGE Imgin, int32* myHist, uint16 histSize, PIXChannelNum channel);//获取灰度图分布直方图
void YMCV_GrayHist_BackProject(int32* myHist, uint16 histSize, CVIMAGE myimg);//灰度图直方图反向投影

void YMCV_Image_Hist_Equalize(CVIMAGE Imgin, CVIMAGE Imgout);//图像灰度直方图均衡化
void YMCV_Image_Hist_MapTo(CVIMAGE Imgto, CVIMAGE Imgfrom);//直方图规定 
void YMCV_Image_Hist_To(CVIMAGE Imgin, CVIMAGE Imgout, int32* myHist, uint16 histSize, PIXChannelNum channel);//图像直方图规定化
void YMCV_Image_Hist_Equalize_UGray(CVIMAGE Imgin, CVIMAGE Imgout);//图像 依据灰度直方图进行均衡化
void YMCV_Hist_FindPeaks(int32* myHist, uint16 histSize, int8* peaksout);//峰值查找

CVIMAGE YMCV_Gray_Local_Hist_Equalize(CVIMAGE myimg, uint8 radius, YMCVPADDINGType padding);//局部直方图均衡

uint8 YMCV_GrayHist_Get_AutoThresh(int32 myHist[256], CVHistAutoThresh threshType);//获取灰度统计直方图的阈值

void YMCV_U16Hist_LenCompress_To255(int32 inHist_outPixMap[], uint16 maxIndex, uint32 tolnum);//数据长度压缩

#endif // !YMCV_HISTCAL_H

