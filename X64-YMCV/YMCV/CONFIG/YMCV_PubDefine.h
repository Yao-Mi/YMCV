#ifndef _YMCV_Pubdef_H
#define _YMCV_Pubdef_H

#define _VS201X_USE 1 //使用VS平台

#ifdef _VS201X_USE
#pragma warning(disable:4996)
#endif

#include <stdio.h>

#include"./YMCV_PubType.h"

#define NULL (void*)0

#define YMCV_IMAGE_USE		1//图像创建开启
#define YMCV_VECTOR_USE     1//向量创建开启
#define YMCV_F32MAT_USE     1//矩阵创建开启
#define YMCV_OTHERDA_USE     1//其他数据结构开启

#define YMCV_CRENDER_USE	1//图像通道映射渲染器开启
#define YMCV_GRENDER_USE	1//图像几何位置渲染器创建
#define YMCV_SRENDER_USE	1//图像空间滤波渲染器创建
#define YMCV_FRENDER_USE	1//图像频域滤波渲染器创建
#define YMCV_BRENDER_USE    1//图像二值化渲染器创建
#define YMCV_FEATURE_USE    1//图像特征渲染开启

//文件系统定义
#define YMCV_FILE FILE
#define YMCV_fopen fopen
#define YMCV_fread fread
#define YMCV_fseek fseek
#define YMCV_ftell ftell
#define YMCV_fclose fclose

#define YMCV_V2_0 20210313

//常用量

#define CVGraySize sizeof(CVgray)
#define CVBinarySize sizeof(CVgray)
#define CVRgb16Size sizeof(CVrgb16)

#define CVFGRAYSize sizeof(float32)
#define CVCGRAYSize (2*sizeof(float32))
#define CVIGRAYSize sizeof(int32)

#define CVUint8ySize sizeof(uint8)
#define CVUint16Size sizeof(uint16)
#define CVFloat32Size sizeof(float32)
#define CVInt32Size sizeof(int32)


#define  CVLimitMaxMin(xMin,x,xMax)    (((x) > xMax) ? xMax : ((x) < xMin) ? xMin : x)
#define  CVMax(x,y)    (((x) > (y)) ? (x) : (y))
#define  CVMin(x,y)    (((x) < (y)) ? (x) : (y))
//二值位置推测 binP[index/8]&(1<<(index %8))
#define CVGetBin(bin8P,index)	(bin8P[index >> 3] & (1 << (index & 7u)))
#define CVSetBin(bin8P,index)	(bin8P[index >> 3] |= (1 << (index & 7u)))
#define CVReSetBin(bin8P,index)	(bin8P[index >> 3] &= ~(1 << (index & 7u)))

#include <math.h>
#define YMCV_Abs(x)   abs((int)(x))
#define YMCV_Fabs(x)  fabsf((float)(x))
#define YMCV_Sin(x)   sinf((float)(x))
#define YMCV_Cos(x)   cosf((float)(x))
#define YMCV_Asin(x)  asinf((float)(x))
#define YMCV_Tan(x)   tanf((float)(x))
#define YMCV_Atan(x)  atanf((float)(x))
#define YMCV_Atan2(y,x)  atan2f((float)(y),(float)(x))
#define YMCV_Log(x)   logf((float)(x))
#define YMCV_Log2(x)   log2f((float)(x))
#define YMCV_Pow(x,y) powf((float)(x),(float)(y))
#define YMCV_Exp(x)   expf((float)(x))
#define YMCV_Fabs(x)  fabsf((float)(x))
#define YMCV_Sqrt(x)  sqrtf((float)(x))
#define YMCV_Ceil(x) ceilf((float)(x))
#define YMCV_Floor(x) floorf((float)(x))
#define YMCV_Round(x) roundf((float)(x))
#define YMCV_Cbrt(x) cbrtf((float)(x))//立方根

#define YMCV_2Pai 6.2831853f
#define YMCV_Pai 3.1415926f
#define YMCV_Pai2 1.57079632f
#define YMCV_Pai4 0.78539816f
#define YMCV_Deg2Rad 0.0174532922f//(pai/180)
#define YMCV_Rad2Deg 57.2957805f//(180/pai)


#define YMCV_RAND_MAX 2147483647 // 2^31 -1
#define YMCV_IntMax 2147483647
#define YMCV_Float32Max 3.402823466e+38f
#define YMCV_Float32Min 1.175494351e-38f

#include<string.h>
#define YMCV_Strcmp(a,b) strcmp(a,b)
#define YMCV_Memcmp(a,b,s) memcmp(a,b,s)
#define YMCV_Strrchr(str,findchar) strrchr(str,findchar)

#endif // __Pubdef_H


