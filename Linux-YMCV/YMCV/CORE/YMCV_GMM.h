#ifndef YMCV_GMM_H
#define YMCV_GMM_H

#include "../OPOBJ/YMCV_OBJ.h"

typedef struct gmm
{
	int componentsNum;//高斯分布的个数
	int dataDim;//数据向量的维度
	float32* coefs;//属于某个高斯分布的概率
	float32* mean;//某个高斯分布的均值
	float32* covdet;//某个分布的协方差矩阵的行列式
	float32* icov;//某个分布的逆协方差矩阵


}CVgmms;
typedef CVgmms* CVGMMS;


void YMCV_Fdata_GMMs_Learning(CVGMMS gmms, CVVECTORS_F32 fvecp, uint8* dataLabel);//高斯模型参数学习
float32 YMCV_Fdata_GMMs_ComponentiP(CVGMMS gmms, int ci, float32* vecp, uint8 dim);//求取属于高斯模型第ci个分量的概率
float32 YMCV_Fdata_GMMs_BelongP(CVGMMS gmms, float32* vecp, uint8 dim);//求取属于高斯模型的概率
int YMCV_Fdata_GMMs_belongWhichComponent(CVGMMS gmms, float32* vec, uint8 dim);//求取属于高斯模型的哪个分量

#endif // !YMCV_GMM_H
