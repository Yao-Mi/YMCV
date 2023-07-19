#ifndef YMCV_PIX_COVER_H
#define YMCV_PIX_COVER_H

#include "../CONFIG/YMCV_PubType.h"
#include "./YMCV_PubData.h"
/**
  ***************************************************************************************************************************
  *	@FileName:    YMCV_PixConv.h
  *	@Author:      yaomimaoren
  *	@Date:        2021-10-31
  *	@Description: 像素格式转换
  *	@Version:     1.0
  *
  ***************************************************************************************************************************
  *
  * 备注信息：
  * 1.支持转换格式：
  *
  * 			rgb565  -->   R、G、B
  * 			rgb565  -->   RGB[3]
  *				rgb565  -->   gray
  * 			rgb565  -->   L、A、B
  * 			RGB[3]  -->   rgb565
  * 
  * 			gray    -->   rgb565
  * 			gray    -->   binary
  *
  * 存在问题备注：
  *
  ***************************************************************************************************************************
  * <author> <time> <version > <desc>
  * yaomi 21/10/31 1.0 build this moudle
  *
  *   __  __ ___    ____   __  ___ ____     ______ ______ ______ __  __
  *   \ \/ //   |  / __ \ /  |/  //  _/    /_  __// ____// ____// / / /
  *    \  // /| | / / / // /|_/ / / /       / /  / __/  / /    / /_/ /
  *    / // ___ |/ /_/ // /  / /_/ /       / /  / /___ / /___ / __  /
  *   /_//_/  |_|\____//_/  /_//___/      /_/  /_____/ \____//_/ /_/
  *
  * Copyright (C), 2021-2031, YAOMI Tech. Co., Ltd.
  ***************************************************************************************************************************/

//RGB565提取三通道
#define  RGB565GET_R(rgb565)   ((rgb565 & 0xF800) >> 8)
#define  RGB565GET_G(rgb565)   ((rgb565 & 0x7E0) >> 3)
#define  RGB565GET_B(rgb565)   ((rgb565 & 0x001F) << 3)
//三通道转RGB565
#define  R_TORGB565(R)   ((((CVrgb16)R) << 8) & 0xF800)
#define  G_TORGB565(G)   ((((CVrgb16)G) << 3) & 0x7E0)
#define  B_TORGB565(B)   ((((CVrgb16)B) >> 3) & 0x001F)

#define  iColorLimitToGray(iColorX)  (((iColorX) > 255) ? 255u : ((iColorX) < 0) ? 0u : (CVgray)(iColorX))

/**
  * @brief RGB565格式像素 分别提取三通道RGB
  */
inline void YMCV_Rgb565ToRgb_3(CVrgb16 rgb565, _color8_t* R, _color8_t* G, _color8_t* B)
{
	*R = RGB565GET_R(rgb565);//5
	*G = RGB565GET_G(rgb565);//6
	*B = RGB565GET_B(rgb565);//5
}

/**
  * @brief RGB565格式像素 连续提取三通道RGB
  */
inline void YMCV_Rgb565ToRgb24(CVrgb16 rgb565, CVRGB24 RGB)
{
	RGB->R = RGB565GET_R(rgb565);//5
	RGB->G = RGB565GET_G(rgb565);//6
	RGB->B = RGB565GET_B(rgb565);//5
}

/**
  * @brief 三通道RGB 转为RGB565格式像素
  */
inline CVrgb16 YMCV_Rgb3ToRgb565(_color8_t R, _color8_t G, _color8_t B)
{
	return (CVrgb16)(R_TORGB565(R) + G_TORGB565(G) + B_TORGB565(B));
}

/**
  * @brief 三通道RGB 转为灰度格式像素
  */
inline CVgray YMCV_Rgb3ToGray(_color8_t R, _color8_t G, _color8_t B)
{
	return (CVgray)((R * 76 + G * 150 + B * 30) >> 8);
}

/**
  * @brief 灰度单通道 转为RGB565格式像素
  */
inline CVrgb16 YMCV_GrayToRgb565(CVgray gray)
{
	return (CVrgb16)(R_TORGB565(gray) + G_TORGB565(gray) + B_TORGB565(gray));
}

/**
  * @brief RGB565格式像素 转为灰度单通道
  */
inline CVgray YMCV_Rgb565ToGray(CVrgb16 rgb565)
{
	return (CVgray)((RGB565GET_R(rgb565) * 76 + RGB565GET_G(rgb565) * 150 + RGB565GET_B(rgb565) * 30) >> 8);
}

/**
  * @brief RGB565格式像素 转lab格式
  */
inline void YMCV_Rgb565ToLAB(CVrgb16 rgb565, int16* L, int16* A, int16* B)
{
	uint32 r, g, b, x, y, z;
	//提取三通道
	r = (uint32)RGB565GET_R(rgb565);
	g = (uint32)RGB565GET_G(rgb565);
	b = (uint32)RGB565GET_B(rgb565);
	
	//转xyz max=2040  ,先放大8倍，再缩小2040=8*255倍
	x = (r * 3555 + g * 2967 + b * 1555) >> 10;
	y = (r * 1742 + g * 5859 + b * 591) >> 10;
	z = (r * 145 + g * 897 + b * 7150) >> 10;
	//f映射 范围从[0,1]放大16384倍，再加上以500为基准的舍入误差，约为16.384
	x = YMCV_Labf_table[x];
	y = YMCV_Labf_table[y];
	z = YMCV_Labf_table[z];
	//转lab 范围 L : [0--100]
	*L = (short)(((116 * y) >> 14) - 16);
	// 范围  A、B : [-128,127]
	*A = (short)((500 * (x - y)) >> 14);
	*B = (short)((200 * (y - z)) >> 14);
}

/**
  * @brief LAB格式像素 转rgb565格式
  * 原理参考：https://blog.csdn.net/lz0499/article/details/77345166
  */
inline void YMCV_LABToRgb565(CVrgb16* rgb565, int16 L, int16 A, int16 B)
{
	//范围[0,1],整体放大1024倍,再放大1024倍，缩小1024倍
	int32 x,y,z,r,g,b;
	x = (((L + 16) * 9039) + (A * 2097)) >> 10;
	y = ((L + 16) * 9039) >> 10;
	z = (((L + 16) * 9039) - (B * 5242)) >> 10;

	//结果范围[0,1],放大256*255倍，max=256*255
	x = YMCV_Labfn_table[x];
	y = YMCV_Labfn_table[y];
	z = YMCV_Labfn_table[z];
	//xyz转rgb，缩小256 >> 放大1024，缩小1024*256倍
	r = ((x *  3165) + (y * -1530) + (z * -549)+ 512) >> 18;
	g = ((x * -943) + (y * +1921) + (z * +46) + 512) >> 18;
	b = ((x * +54) + (y * -209) + (z * +1178) + 512) >> 18;

	//限幅
	r = iColorLimitToGray(r);
	g = iColorLimitToGray(g);
	b = iColorLimitToGray(b);

	//转rgb565
	*rgb565 = R_TORGB565(r) + G_TORGB565(g) + B_TORGB565(b);
}


/**
  * @brief RGB565格式像素 转YUV格式(YCbCr格式)
  */
inline void YMCV_Rgb565ToYCbCr(CVrgb16 rgb565, int16* Y, int16* Cb, int16* Cr)
{
	uint32 r, g, b;
	//提取三通道
	r = (uint32)RGB565GET_R(rgb565);
	g = (uint32)RGB565GET_G(rgb565);
	b = (uint32)RGB565GET_B(rgb565);
	//转xyz  Y取值范围:[0,255]
	*Y = (306 * r + 601 * g + 117 * b) >> 10;
	// Cb,Cr取值范围:[-127,128]
	*Cb = ((-173 * r - 339 * g + 512 * b) >> 10) + 128;
	*Cr = ((512 * r - 429 * g - 83 * b) >> 10) + 128;
}

/**
  * @brief YUV格式(YCbCr格式)像素 转RGB565格式
  * 原理参考：https://www.cnblogs.com/qiqibaby/p/5260167.html
  */
inline void YMCV_YCbCrToRgb565( CVrgb16* rgb565,int16 Y, int16 Cb, int16 Cr)
{
	int32 r, g, b;
	r = Y + ((2871 * (Cr - 128)) >> 11);
	g = Y - ((352 * (Cb - 128)) >> 10) - ((731 * (Cr - 128)) >> 10);
	b = Y + ((3629 * (Cb - 128)) >> 11);
	//限幅
	r = iColorLimitToGray(r);
	g = iColorLimitToGray(g);
	b = iColorLimitToGray(b);
	//转rgb565
	*rgb565 = (CVrgb16)(R_TORGB565(r) + G_TORGB565(g) + B_TORGB565(b));
}

/**
  * @brief 灰度格式 阈值分割转二值输出
  */
inline CVgray YMCV_GrayToBinary(CVgray gray, CVgray threshold)
{
	return (CVgray)((gray > threshold) ? 255u : 0);//二值化阈值分割
}

/**
  * @brief 灰度格式 双阈值分割 阈值分割转二值  255输出
  */
inline CVgray YMCV_GrayToBinary_2Threshold_255(CVgray gray, CVgray leftthresh, CVgray rightthresh)
{
	return ((gray >= leftthresh) && (gray <= rightthresh)) ? 255u : 0u;
}

/**
  * @brief 灰度格式 双阈值分割 阈值分割转二值  1输出
  */
inline uint8 YMCV_GrayToBinary_2Threshold_1(short gray, short thresholdmin, short thresholdmax)
{
	return ((gray >= thresholdmin) && (gray <= thresholdmax)) ? 1u : 0u;//二值化双阈值分割
}


#endif // !YMCV_PIX_COVER_H

