#ifndef YMCV_MERGECAL_H
#define YMCV_MERGECAL_H
#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_PubType.h"
#include "../OPOBJ/YMCV_OBJ.h"

void YMCV_Gray_ColorTo_Rgb16(CVIMAGE gray, CVIMAGE rgb);//���Ҷ�ǿ��ӳ�����ͼ
CVIMAGE YMCV_Image_Mask(CVIMAGE mask, CVIMAGE image);//ʹ���������������ȡ
void YMCV_Image_Cover(CVIMAGE upimg, CVIMAGE downimg, uint16 upx, uint16 upy);//ͼ�񸲸Ǻϲ�

#endif// !YMCV_MERGECAL_H

