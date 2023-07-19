#ifndef _YMCV_FORMCGE_H
#define _YMCV_FORMCGE_H

#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_PubType.h"
#include "../OPOBJ/YMCV_OBJ.h"

typedef enum
{
	CV_Rgb16ToGray,  //Rgb565灰度化
	CV_Rgb16ToBinary,//Rgb565二值化

	CV_GrayToRgb16,  //Gray升三通道图
	CV_GrayToBinary,//Gray二值化
	CV_GrayToDotBinary,//灰度转二值点图
	CV_GrayToCGray,//Gray升complex gray
	CV_GrayToFGray,//Gray升float gray

	CV_BinaryToRgb16,  //Binary升三通道图
	CV_BinaryToGray,//Binary灰度化


	CV_IGrayToGray,  //IGray 转Gray图 有效部分截断
	CV_IGrayToGray2,  //IGray 转Gray图 自适应拉伸

	CV_FGrayToGray,  //FGray 转Gray图 绝对值有效部分截断
	CV_FGrayToGray2,  //FGray 转Gray图自适应拉伸

	CV_CGrayToGray,   //complex gray 转 灰度 取实部截断
	CV_CGrayToGray2,   //频域complex 转 灰度 区域自适应拉伸

}CVPIXCFORM;


void YMCV_Image_Pix_Change_Form(CVIMAGE Imgin, CVIMAGE Imgout, CVPIXCFORM CForm);//像素格式转换

#endif //!_YMCV_FORMCGE_H

