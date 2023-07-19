#ifndef YMCV_AREAFILLCAL_H
#define YMCV_AREAFILLCAL_H

#include"../CONFIG/YMCV_PubDefine.h"
#include"../OPOBJ/YMCV_OBJ.h"

void YMCV_Img_Scanline_AreaFill(CVIMAGE pthis, CVpoint* ring, uint32 pointNum, CVcolor fillcolor);//ÇøÓòÌî³ä
void YMCV_Image_ScanLine_SeedFill(CVIMAGE myimg, CVpoint seed, int8 errorMax, CVcolor fillval);//ÖÖ×ÓÌî³ä
void YMCV_Image_FullFill(CVIMAGE myimg, CVcolor fillval);//È«²¿Ìî³ä
void YMCV_Image_CoverFill(CVIMAGE down, CVIMAGE up, uint16 startx, uint16 starty);//Í¼Ïñ¸²¸ÇÌî³ä
void YMCV_Image_MaskFill(CVIMAGE myimg, CVIMAGE mask, CVcolor fillval);//ÑÚÂëÌî³ä

#endif // !YMCV_AREAFILLCAL_H

