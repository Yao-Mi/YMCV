#ifndef YMCV_IO_FILE_H
#define YMCV_IO_FILE_H
#include "../OPOBJ/YMCV_Creat.h"
#include "../OPOBJ/YMCV_Free.h"

#include"../Code/gifdec.h"
#include"../Code/avilib.h"


#define YMCV_AVI_FRAME_SIZE_MAX  (50*1024) //����avi��Ƶ��һ��ͼƬΪΪ��С������50k��jpeg���ò������Ը�����Ƶ��С����


CVIMAGE YMCV_Image_Read(const char* filepath, CVImgType outType);


/*
** ���ز��� ��
**             1���ؼ�֡
**             0���ǹؼ�֡(��ͨ֡)
**            -1����֡����ȡ�������޷�������ȡ��
**/
int YMCV_AVI_Decode_Next_Jpeg_Frame(avi_t* myavi, CVIMAGE outimg);


#endif // !YMCV_IO_FILE_H

