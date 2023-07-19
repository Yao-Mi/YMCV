#ifndef YMCV_SPECIAL_EFFECT_H
#define YMCV_SPECIAL_EFFECT_H


#include "../OPOBJ/YMCV_Creat.h"
#include "../OPOBJ/YMCV_Free.h"

void YMCV_Gray_Exemplar_Based_Inpainting(CVIMAGE myimg, CVIMAGE mask, uint8 patch_r, uint16 search_r);//inpaint�޸�
CVIMAGE YMCV_Gray_Image_GetDarkChannel(CVIMAGE myimg, uint8 kernel_r);//��ȡ��ͨ��
uint8  YMCV_Gray_Image_GetAtmosphericlighting(CVIMAGE myimg, CVIMAGE darkimg);//�����������
CVIMAGE YMCV_Gray_Image_RemovalFog(CVIMAGE myimg, CVIMAGE darkimg, uint8 AtmosValue, float32 omiga);//��ͨ��ȥ��
CVIMAGE YMCV_Gray_FishEye_Effect(CVIMAGE myimg, uint16 imgOutw, uint16 imgOuth);//������Ч
CVIMAGE YMCV_Gray_invFishEye_Effect(CVIMAGE myimg, float32 strength, float32 scalex, float32 scaley);//����ʧ��У׼
CVIMAGE YMCV_Gray_Cartoon_Effect(CVIMAGE myimg, uint8 kernel_r);//��ͨ��Ч
CVIMAGE YMCV_Gray_RigidDeformation_Effect(CVIMAGE myimg, CVVECTORS_F32 pList, CVVECTORS_F32 qList);//���Ƶ���Ա任 p -> q

#endif // YMCV_SPECIAL_EFFECT_H
