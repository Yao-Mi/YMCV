#ifndef YMCV_MERGECAL_H
#define YMCV_MERGECAL_H
#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_PubType.h"
#include "../OPOBJ/YMCV_OBJ.h"

void YMCV_Gray_ColorTo_Rgb16(CVIMAGE gray, CVIMAGE rgb);//将灰度强度映射给彩图
CVIMAGE YMCV_Image_Mask(CVIMAGE mask, CVIMAGE image);//使用掩码进行区域提取
void YMCV_Image_Cover(CVIMAGE upimg, CVIMAGE downimg, uint16 upx, uint16 upy);//图像覆盖合并

#endif// !YMCV_MERGECAL_H

