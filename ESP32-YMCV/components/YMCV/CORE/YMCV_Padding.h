#ifndef YMCV_PADDING_H
#define YMCV_PADDING_H
#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_PubType.h"
#include "../OPOBJ/YMCV_OBJ.h"


typedef enum
{
	NOPadding = 0,
	OutPaddingConst,
}YMCVPADDINGType;

typedef enum
{
	CV_PaddingConst,
	CV_PaddingAutoSpace,
	CV_PaddingAutoPix,
}YMCVPADDINGFORM;

//padding±???
extern uint8_t YMCV_PADDING_VAL;
void YMCV_Image_Boundary_Padding_Const(CVIMAGE Imgio, uint16 pad_w, uint16 pad_h);
void YMCV_Gray_Image_Boundary_Padding_Const(uint8* datap, uint16 pad_w, uint16 pad_h, uint16 width, uint16 height);
void YMCV_Gray_Image_Boundary_Padding_Val(uint8* datap, uint16 pad_w, uint16 pad_h, uint16 width, uint16 height, uint8 paddingval);
void YMCV_F32Gray_Image_Boundary_Padding_Val(float32* datap, uint16 pad_w, uint16 pad_h, uint16 width, uint16 height, float32 paddingval);

void YMCV_ComplexGray_Image_Boundary_Padding_Const(float32* ak, float32* bk, uint16 pad_w, uint16 pad_h, uint16 width, uint16 height);
void YMCV_ComplexGray_Image_Boundary_Padding_Val(float32* ak, float32* bk, uint16 pad_w, uint16 pad_h, uint16 width, uint16 height, float32 val);


CVIMAGE YMCV_Image_Image_Padding(CVIMAGE Imgin, uint16 pad_w, uint16 pad_h, YMCVPADDINGFORM myform);
#endif// !YMCV_PADDING_H

