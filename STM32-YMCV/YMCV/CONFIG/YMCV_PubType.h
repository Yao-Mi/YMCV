#ifndef _YMCV_PUBTYPE_H
#define _YMCV_PUBTYPE_H

#include"../../CONFIG/Pubtype.h"

//自定义数据结构
typedef uint32 size_t;
typedef float32 _color32_f;
typedef int32 _color32_i;
typedef uint16 _color16_t;
typedef uint8 _color8_t;

#pragma pack(1)

typedef  struct _color24_t
{
	_color8_t R : 8;
	_color8_t G : 8;
	_color8_t B : 8;
}color24_t;

//通用颜色
typedef union
{
	_color8_t gray;
	_color16_t rgb16;
}CVcolor;
#pragma pack()


typedef _color8_t CVgray;
typedef CVgray* CVGRAY;


typedef _color16_t CVrgb16;
typedef CVrgb16* CVRGB16;

typedef color24_t CVrgb24;
typedef CVrgb24* CVRGB24;

//图像通道数
typedef enum
{
	PIXChannelNum1 = 1,//单通道
	PIXChannelNum3 = 3,//三通道
}PIXChannelNum;


#endif
