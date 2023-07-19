#ifndef YMCV_KMEANSPLUSPLUS_H
#define YMCV_KMEANSPLUSPLUS_H

#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_Mem.h"
#include "../OPOBJ/YMCV_OBJ.h"
#include "../DEBUG/YMCV_Debug.h"

//��ȡ��������,������label����ᴫ������������label
void YMCV_FVector_Kmeans_GetCenter(CVVECTORS_F32 fvecp, uint8* dataLabel, CVVECTORS_F32 kCenterp, uint16 maxIter);

//�������ݱ�ǩ
void YMCV_FVector_Kmeans_Classify(CVVECTORS_F32 fvecp, CVVECTORS_F32 kCenterp, uint8* dataLabelout);

#endif // !YMCV_KMEANSPLUSPLUS_H

