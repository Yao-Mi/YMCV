#ifndef YMCV_IO_FILE_H
#define YMCV_IO_FILE_H
#include "../OPOBJ/YMCV_Creat.h"
#include "../OPOBJ/YMCV_Free.h"

#include"../Code/gifdec.h"
#include"../Code/avilib.h"


#define YMCV_AVI_FRAME_SIZE_MAX  (50*1024) //假设avi视频中一张图片为为大小不超过50k的jpeg，该参数可以根据视频大小调整


CVIMAGE YMCV_Image_Read(const char* filepath, CVImgType outType);


/*
** 返回参数 ：
**             1：关键帧
**             0：非关键帧(普通帧)
**            -1：非帧（读取结束，无法继续读取）
**/
int YMCV_AVI_Decode_Next_Jpeg_Frame(avi_t* myavi, CVIMAGE outimg);


#endif // !YMCV_IO_FILE_H

