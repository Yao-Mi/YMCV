#ifndef YMCV_INNER_MATH_H
#define YMCV_INNER_MATH_H
#include"../CONFIG/YMCV_PubDefine.h"

void YMCV_Srand(long int seed);
long int YMCV_Randint();
float YMCV_Rand0_1();
float YMCV_RandGaussian(float u, float sigma);

#endif
