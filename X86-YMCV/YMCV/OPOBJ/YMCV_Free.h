#ifndef _YMCV_FREE_H
#define _YMCV_FREE_H

#include"../CONFIG/YMCV_PubDefine.h"
//#include"../YMCV_ChannelCal.h"
//#include"../YMCV_GeomCal.h"

#if YMCV_IMAGE_USE
#include"./YMCV_OBJ.h"
void YMCV_Free_ImgFree(CVIMAGE pthis);//图像内存释放
#endif

#if YMCV_VECTOR_USE
#include"./YMCV_OBJ.h"
void YMCV_Free_Vector_F32_Free(CVVECTORS_F32 pthis);//浮点向量内存释放
void YMCV_Free_Vector_U16_Free(CVVECTORS_U16 pthis);//整形向量内存释放

#endif // YMCV_VECTOR_USE

#if YMCV_F32MAT_USE
#include"./YMCV_OBJ.h"
void YMCV_Free_FMatFree(CVFMAT pthis);//浮点矩阵内存释放
#endif // YMCV_F32MAT_USE

#if YMCV_CRENDER_USE
#include"../CORE/YMCV_ChannelCal.h"
void YMCV_Free_PIX_RenderFree(CVPIX_RENDER pthis);//CVPIX_RENDER 渲染器内存释放
#endif 

#if YMCV_SRENDER_USE
#include"../CORE/YMCV_SpaceCal.h" 
//空间滤波渲染器创建
void YMCV_Free_KERN_RenderFree(CVKERN_RENDER pthis);//CVKERN_RENDER 渲染器内存释放
#endif

#if YMCV_GRENDER_USE
#include"../CORE/YMCV_GeomCal.h"
void YMCV_Free_GEOM_RenderFree(CVGEOM_RENDER pthis);//CVGEOM_Render 渲染器内存释放
#endif

#if YMCV_FRENDER_USE
#include "../CORE/YMCV_FreqCal.h"
void YMCV_Free_FFT_RenderFree(CVFFT_RENDER pthis);//CVFFT_Render 渲染器内存释放
void YMCV_Free_DWT_RenderFree(CVDWT_RENDER pthis);//CVDWT_Render 渲染器内存释放
void YMCV_Free_DCT_RenderFree(CVDCT_RENDER pthis);//CVDCT_Render 渲染器内存释放
void YMCV_Free_FFT2D_RenderFree(CVFFT2D_RENDER pthis);//CVFFT2D_Render 渲染器内存释放
void YMCV_Free_DWT2D_RenderFree(CVDWT2D_RENDER pthis);//CVDWT2D_Render 渲染器内存释放
void YMCV_Free_DCT2D_RenderFree(CVDCT2D_RENDER pthis);//CVDCT2D_Render 渲染器内存释放
#endif

#if YMCV_BRENDER_USE
#include"../CORE/YMCV_BinaryCal.h"
//二值化渲染器创建
void YMCV_Free_Binary_RenderFree(CVBINARY_RENDER pthis);//二值化渲染器内存释放
#endif // #if YMCV_BRENDER_USE

#if YMCV_FEATURE_USE
#include"../CORE/YMCV_FeatCal.h"
void YMCV_Free_BlobSegRender_Free(CVBLOB_SEG_RENDER pthis);
void YMCV_Free_Chains_U16_Free(CVCHAINS_U16 pthis);
void YMCV_Free_FaceLbp_Hist_free(CV_FACE_LBP_DESC pthis);
#endif // YMCV_FEATURE_USE


#if YMCV_OTHERDA_USE
#include"../CORE/YMCV_GMM.h"
void YMCV_Free_Gmms_Free(CVGMMS pthis);//GMMs对象内存释放
void YMCV_Free_TargetTrack_Free(CV_TARGET_TRACK pthis);//目标跟踪器内存释放
void YMCV_Free_ParticleTrack_Free(CV_PARTICLE_TRACK pthis);//粒子跟踪器内存释放

#endif // YMCV_OTHERDA_USE




#endif