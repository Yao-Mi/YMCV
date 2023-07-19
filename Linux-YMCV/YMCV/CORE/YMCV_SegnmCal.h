#ifndef YMCV_SEGNMCAL_H
#define YMCV_SEGNMCAL_H
#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_PubType.h"
#include "../OPOBJ/YMCV_OBJ.h"


#define TriMapNoUseValue 255 //用来标记边界的
enum CVTriMapVal
{
	CVTriMapMinVl = 0,
	CVTriMapMidle = 128, //待标记的点
	CVTriMapMaxVl = TriMapNoUseValue - 1,
	CVTriMapEdgev = TriMapNoUseValue //用来标记边界
};
// 不同背景标记区间：[ 0 , CVTriMapMidle)
// 待区分的点：CVTriMapMidle
// 不同前景标记区间：[ CVTriMapMidle+1 ,CVTriMapMaxVl)

CVIMAGE YMCV_Gray_Region_Growing8(CVIMAGE myimg, uint16 w, uint16 h, uint8 Detathreshold);//八邻域区域生长
CVIMAGE YMCV_Gray_Region_Splitand(CVIMAGE myimg, uint8 thresh);//区域分裂
CVIMAGE YMCV_Gray_Region_Merge(CVIMAGE myimg, uint8 thresh);//区域合并
CVIMAGE YMCV_Gray_KMeans_Clustering(CVIMAGE myimg, uint8 K_Num, uint16 maxIteration);//kmeans聚类分割
CVIMAGE YMCV_Gray_GMM_Clustering(CVIMAGE myimg, uint8 ClusterNum, uint16 maxIteration);//GMM聚类分割
CVIMAGE YMCV_Binary_Distance_Transform(CVIMAGE myimg);//距离变换

CVIMAGE YMCV_BinaryTo_TriMap(CVIMAGE foreground_Binary, CVIMAGE background_Binary);//TriMap图生成
void YMCV_TriMap_Init(CVIMAGE TriMapMarkers);//TriMap手动初始化为 Midle 值
CVIMAGE YMCV_TriMap_GetEdge(CVIMAGE TriMapMarkers);//获取Trimap的边界
void YMCV_Gray_Watershed(CVIMAGE myimg, CVIMAGE TriMapMarkers);//分水岭
void  YMCV_Gray_Snake(CVIMAGE myimg, //snake 轮廓分割
	uint16* xs, uint16* ys, uint16 xynums,//点坐标，点个数
	uint8 search_r, uint16 max_iter,//搜索半径，最大迭代次数
	float32 alpha, float32 beta, float32 gamma, //Esnake=αEcont+βEcurv+γEext
	float32 w_l, float32 w_e);//Eext=Eimage= -(w_l*image + w_e*gradient)

void YMCV_Gray_GrabCut(CVIMAGE myimg, CVIMAGE TriMapMarkers, int iterCount);//grabcut

void YMCV_Binary_Proj_Hist_Seg(CVVECTORS_U16 myhist, uint16 segMax, uint8 clear_shortLenMax);//投影直方图分割

#endif

