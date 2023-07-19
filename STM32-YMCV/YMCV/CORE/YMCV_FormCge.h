#ifndef _YMCV_FORMCGE_H
#define _YMCV_FORMCGE_H

#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_PubType.h"
#include "../OPOBJ/YMCV_OBJ.h"

typedef enum
{
	CV_Rgb16ToGray,  //Rgb565�ҶȻ�
	CV_Rgb16ToBinary,//Rgb565��ֵ��

	CV_GrayToRgb16,  //Gray����ͨ��ͼ
	CV_GrayToBinary,//Gray��ֵ��
	CV_GrayToDotBinary,//�Ҷ�ת��ֵ��ͼ
	CV_GrayToCGray,//Gray��complex gray
	CV_GrayToFGray,//Gray��float gray

	CV_BinaryToRgb16,  //Binary����ͨ��ͼ
	CV_BinaryToGray,//Binary�ҶȻ�


	CV_IGrayToGray,  //IGray תGrayͼ ��Ч���ֽض�
	CV_IGrayToGray2,  //IGray תGrayͼ ����Ӧ����

	CV_FGrayToGray,  //FGray תGrayͼ ����ֵ��Ч���ֽض�
	CV_FGrayToGray2,  //FGray תGrayͼ����Ӧ����

	CV_CGrayToGray,   //complex gray ת �Ҷ� ȡʵ���ض�
	CV_CGrayToGray2,   //Ƶ��complex ת �Ҷ� ��������Ӧ����

}CVPIXCFORM;


void YMCV_Image_Pix_Change_Form(CVIMAGE Imgin, CVIMAGE Imgout, CVPIXCFORM CForm);//���ظ�ʽת��

#endif //!_YMCV_FORMCGE_H

