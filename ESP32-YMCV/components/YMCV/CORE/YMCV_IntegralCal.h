#ifndef YMCV_INTERGRALCAL_H
#define YMCV_INTERGRALCAL_H
#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_PubType.h"
#include "../OPOBJ/YMCV_OBJ.h"

#define IntegralImgW(w)  (w+1)
#define IntegralImgH(h)  (h+1)

//图像积分图计算
void YMCV_Gray_Image_Integral(uint8* input, uint32* output, uint16 width, uint16 height);
void YMCV_Gray_Image_IntegralSelf_F32(float32* src_out, uint16 width, uint16 height);

//图像平方积分图计算
void YMCV_Gray_Image_Integral_Pow2(uint8* input, uint32* output, uint16 width, uint16 height);
void YMCV_Gray_Image_Integral_Dot2(uint8* src, uint8* src2, uint32* output, uint16 width, uint16 height);

//同时计算，并带尺度因子
void YMCV_Gray_Integral_SumAndPow2_Scale(uint8* src, uint32* output_sum, uint32* output_ssq, int outw, int outh, int inw, int inh);

#endif // !YMCV_INTERGRALCAL_H
