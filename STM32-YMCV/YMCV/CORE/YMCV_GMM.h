#ifndef YMCV_GMM_H
#define YMCV_GMM_H

#include "../OPOBJ/YMCV_OBJ.h"

typedef struct gmm
{
	int componentsNum;//��˹�ֲ��ĸ���
	int dataDim;//����������ά��
	float32* coefs;//����ĳ����˹�ֲ��ĸ���
	float32* mean;//ĳ����˹�ֲ��ľ�ֵ
	float32* covdet;//ĳ���ֲ���Э������������ʽ
	float32* icov;//ĳ���ֲ�����Э�������


}CVgmms;
typedef CVgmms* CVGMMS;


void YMCV_Fdata_GMMs_Learning(CVGMMS gmms, CVVECTORS_F32 fvecp, uint8* dataLabel);//��˹ģ�Ͳ���ѧϰ
float32 YMCV_Fdata_GMMs_ComponentiP(CVGMMS gmms, int ci, float32* vecp, uint8 dim);//��ȡ���ڸ�˹ģ�͵�ci�������ĸ���
float32 YMCV_Fdata_GMMs_BelongP(CVGMMS gmms, float32* vecp, uint8 dim);//��ȡ���ڸ�˹ģ�͵ĸ���
int YMCV_Fdata_GMMs_belongWhichComponent(CVGMMS gmms, float32* vec, uint8 dim);//��ȡ���ڸ�˹ģ�͵��ĸ�����

#endif // !YMCV_GMM_H
