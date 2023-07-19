#ifndef YMCV_SPECIAL_EFFECT_H
#define YMCV_SPECIAL_EFFECT_H


#include "../OPOBJ/YMCV_Creat.h"
#include "../OPOBJ/YMCV_Free.h"

void YMCV_Gray_Exemplar_Based_Inpainting(CVIMAGE myimg, CVIMAGE mask, uint8 patch_r, uint16 search_r);//inpaint修复
CVIMAGE YMCV_Gray_Image_GetDarkChannel(CVIMAGE myimg, uint8 kernel_r);//获取暗通道
uint8  YMCV_Gray_Image_GetAtmosphericlighting(CVIMAGE myimg, CVIMAGE darkimg);//计算大气光照
CVIMAGE YMCV_Gray_Image_RemovalFog(CVIMAGE myimg, CVIMAGE darkimg, uint8 AtmosValue, float32 omiga);//暗通道去雾
CVIMAGE YMCV_Gray_FishEye_Effect(CVIMAGE myimg, uint16 imgOutw, uint16 imgOuth);//鱼眼特效
CVIMAGE YMCV_Gray_invFishEye_Effect(CVIMAGE myimg, float32 strength, float32 scalex, float32 scaley);//鱼眼失真校准
CVIMAGE YMCV_Gray_Cartoon_Effect(CVIMAGE myimg, uint8 kernel_r);//卡通特效
CVIMAGE YMCV_Gray_RigidDeformation_Effect(CVIMAGE myimg, CVVECTORS_F32 pList, CVVECTORS_F32 qList);//控制点刚性变换 p -> q

#endif // YMCV_SPECIAL_EFFECT_H
