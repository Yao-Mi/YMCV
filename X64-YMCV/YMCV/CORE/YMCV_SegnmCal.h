#ifndef YMCV_SEGNMCAL_H
#define YMCV_SEGNMCAL_H
#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_PubType.h"
#include "../OPOBJ/YMCV_OBJ.h"


#define TriMapNoUseValue 255 //������Ǳ߽��
enum CVTriMapVal
{
	CVTriMapMinVl = 0,
	CVTriMapMidle = 128, //����ǵĵ�
	CVTriMapMaxVl = TriMapNoUseValue - 1,
	CVTriMapEdgev = TriMapNoUseValue //������Ǳ߽�
};
// ��ͬ����������䣺[ 0 , CVTriMapMidle)
// �����ֵĵ㣺CVTriMapMidle
// ��ͬǰ��������䣺[ CVTriMapMidle+1 ,CVTriMapMaxVl)

CVIMAGE YMCV_Gray_Region_Growing8(CVIMAGE myimg, uint16 w, uint16 h, uint8 Detathreshold);//��������������
CVIMAGE YMCV_Gray_Region_Splitand(CVIMAGE myimg, uint8 thresh);//�������
CVIMAGE YMCV_Gray_Region_Merge(CVIMAGE myimg, uint8 thresh);//����ϲ�
CVIMAGE YMCV_Gray_KMeans_Clustering(CVIMAGE myimg, uint8 K_Num, uint16 maxIteration);//kmeans����ָ�
CVIMAGE YMCV_Gray_GMM_Clustering(CVIMAGE myimg, uint8 ClusterNum, uint16 maxIteration);//GMM����ָ�
CVIMAGE YMCV_Binary_Distance_Transform(CVIMAGE myimg);//����任

CVIMAGE YMCV_BinaryTo_TriMap(CVIMAGE foreground_Binary, CVIMAGE background_Binary);//TriMapͼ����
void YMCV_TriMap_Init(CVIMAGE TriMapMarkers);//TriMap�ֶ���ʼ��Ϊ Midle ֵ
CVIMAGE YMCV_TriMap_GetEdge(CVIMAGE TriMapMarkers);//��ȡTrimap�ı߽�
void YMCV_Gray_Watershed(CVIMAGE myimg, CVIMAGE TriMapMarkers);//��ˮ��
void  YMCV_Gray_Snake(CVIMAGE myimg, //snake �����ָ�
	uint16* xs, uint16* ys, uint16 xynums,//�����꣬�����
	uint8 search_r, uint16 max_iter,//�����뾶������������
	float32 alpha, float32 beta, float32 gamma, //Esnake=��Econt+��Ecurv+��Eext
	float32 w_l, float32 w_e);//Eext=Eimage= -(w_l*image + w_e*gradient)

void YMCV_Gray_GrabCut(CVIMAGE myimg, CVIMAGE TriMapMarkers, int iterCount);//grabcut

void YMCV_Binary_Proj_Hist_Seg(CVVECTORS_U16 myhist, uint16 segMax, uint8 clear_shortLenMax);//ͶӰֱ��ͼ�ָ�

#endif

