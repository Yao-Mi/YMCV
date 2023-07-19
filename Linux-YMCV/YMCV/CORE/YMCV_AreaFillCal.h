#ifndef YMCV_AREAFILLCAL_H
#define YMCV_AREAFILLCAL_H

#include"../CONFIG/YMCV_PubDefine.h"
#include"../OPOBJ/YMCV_OBJ.h"

void YMCV_Img_Scanline_AreaFill(CVIMAGE pthis, CVpoint* ring, uint32 pointNum, CVcolor fillcolor);//区域填充
void YMCV_Image_ScanLine_SeedFill(CVIMAGE myimg, CVpoint seed, int8 errorMax, CVcolor fillval);//种子填充
void YMCV_Image_FullFill(CVIMAGE myimg, CVcolor fillval);//全部填充
void YMCV_Image_CoverFill(CVIMAGE down, CVIMAGE up, uint16 startx, uint16 starty);//图像覆盖填充
void YMCV_Image_MaskFill(CVIMAGE myimg, CVIMAGE mask, CVcolor fillval);//掩码填充

#endif // !YMCV_AREAFILLCAL_H

