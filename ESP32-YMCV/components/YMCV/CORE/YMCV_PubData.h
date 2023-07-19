#ifndef YMCV_PUBDATA_H
#define YMCV_PUBDATA_H

#include "../CONFIG/YMCV_PubType.h"

extern const short YMCV_DisCalmb[8];
extern const int16 YMCV_NeighbPos9[9][2];//9邻居=自身+8邻域
extern const int16 YMCV_Neighbour8[8][2]; //8邻域
extern const int8 YMCV_Search5[5];//循环邻域的搜索索引
extern const int16 YMCV_RoundSearch8[8][2];//循环邻域 r=1
extern const int16 YMCV_RoundSearch16[16][2];//循环邻域 r=2
extern const int16 YMCV_CircleSearch3x[37];//圆形邻域 r=3
extern const int16 YMCV_CircleSearch3y[37];
extern const int16 YMCV_CircleSearch16x[16];//外园邻域 r=3
extern const int16 YMCV_CircleSearch16y[16];
extern const uint8 YMCV_CircleSearch16id[16];


extern const float32 YMCV_Gauss_Kernel[9];//gauss
extern const int16 YMCV_Sobelx_Kernel[9];//sobel
extern const int16 YMCV_Sobely_Kernel[9];
extern const int16 YMCV_Log_Kernel[9];//log
extern const int16 YMCV_Laplace_Kernel[9];//拉普拉斯滤波核

extern const uint16 YMCV_Labf_table[2041];//lab映射函数
extern const uint16 YMCV_Labfn_table[1025];//lab逆映射函数
extern const color24_t YMCV_HeatPalette[256];//热力图调色板

extern const float32 YMCV_Sinx360[361];//正余弦计算
extern const float32 YMCV_Cosx360[361];

extern const uint8 YMCV_LBP_Uniform_Pattern[256];//LBP等价模式

#endif
