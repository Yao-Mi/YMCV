#ifndef _YMCV_FREE_H
#define _YMCV_FREE_H

#include"../CONFIG/YMCV_PubDefine.h"
//#include"../YMCV_ChannelCal.h"
//#include"../YMCV_GeomCal.h"

#if YMCV_IMAGE_USE
#include"./YMCV_OBJ.h"
void YMCV_Free_ImgFree(CVIMAGE pthis);//ͼ���ڴ��ͷ�
#endif

#if YMCV_VECTOR_USE
#include"./YMCV_OBJ.h"
void YMCV_Free_Vector_F32_Free(CVVECTORS_F32 pthis);//���������ڴ��ͷ�
void YMCV_Free_Vector_U16_Free(CVVECTORS_U16 pthis);//���������ڴ��ͷ�

#endif // YMCV_VECTOR_USE

#if YMCV_F32MAT_USE
#include"./YMCV_OBJ.h"
void YMCV_Free_FMatFree(CVFMAT pthis);//��������ڴ��ͷ�
#endif // YMCV_F32MAT_USE

#if YMCV_CRENDER_USE
#include"../CORE/YMCV_ChannelCal.h"
void YMCV_Free_PIX_RenderFree(CVPIX_RENDER pthis);//CVPIX_RENDER ��Ⱦ���ڴ��ͷ�
#endif 

#if YMCV_SRENDER_USE
#include"../CORE/YMCV_SpaceCal.h" 
//�ռ��˲���Ⱦ������
void YMCV_Free_KERN_RenderFree(CVKERN_RENDER pthis);//CVKERN_RENDER ��Ⱦ���ڴ��ͷ�
#endif

#if YMCV_GRENDER_USE
#include"../CORE/YMCV_GeomCal.h"
void YMCV_Free_GEOM_RenderFree(CVGEOM_RENDER pthis);//CVGEOM_Render ��Ⱦ���ڴ��ͷ�
#endif

#if YMCV_FRENDER_USE
#include "../CORE/YMCV_FreqCal.h"
void YMCV_Free_FFT_RenderFree(CVFFT_RENDER pthis);//CVFFT_Render ��Ⱦ���ڴ��ͷ�
void YMCV_Free_DWT_RenderFree(CVDWT_RENDER pthis);//CVDWT_Render ��Ⱦ���ڴ��ͷ�
void YMCV_Free_DCT_RenderFree(CVDCT_RENDER pthis);//CVDCT_Render ��Ⱦ���ڴ��ͷ�
void YMCV_Free_FFT2D_RenderFree(CVFFT2D_RENDER pthis);//CVFFT2D_Render ��Ⱦ���ڴ��ͷ�
void YMCV_Free_DWT2D_RenderFree(CVDWT2D_RENDER pthis);//CVDWT2D_Render ��Ⱦ���ڴ��ͷ�
void YMCV_Free_DCT2D_RenderFree(CVDCT2D_RENDER pthis);//CVDCT2D_Render ��Ⱦ���ڴ��ͷ�
#endif

#if YMCV_BRENDER_USE
#include"../CORE/YMCV_BinaryCal.h"
//��ֵ����Ⱦ������
void YMCV_Free_Binary_RenderFree(CVBINARY_RENDER pthis);//��ֵ����Ⱦ���ڴ��ͷ�
#endif // #if YMCV_BRENDER_USE

#if YMCV_FEATURE_USE
#include"../CORE/YMCV_FeatCal.h"
void YMCV_Free_BlobSegRender_Free(CVBLOB_SEG_RENDER pthis);
void YMCV_Free_Chains_U16_Free(CVCHAINS_U16 pthis);
void YMCV_Free_FaceLbp_Hist_free(CV_FACE_LBP_DESC pthis);
#endif // YMCV_FEATURE_USE


#if YMCV_OTHERDA_USE
#include"../CORE/YMCV_GMM.h"
void YMCV_Free_Gmms_Free(CVGMMS pthis);//GMMs�����ڴ��ͷ�
void YMCV_Free_TargetTrack_Free(CV_TARGET_TRACK pthis);//Ŀ��������ڴ��ͷ�
void YMCV_Free_ParticleTrack_Free(CV_PARTICLE_TRACK pthis);//���Ӹ������ڴ��ͷ�

#endif // YMCV_OTHERDA_USE




#endif