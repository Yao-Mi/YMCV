#ifndef _YM_CREAT_H
#define _YM_CREAT_H
#include"../CONFIG/YMCV_PubDefine.h"


#if YMCV_IMAGE_USE
#include"./YMCV_OBJ.h"
CVIMAGE YMCV_Creat_Img_Creat(uint16 width,uint16 height, CVImgType Imgform);//����һ��ͼ��
CVIMAGE YMCV_Creat_ImgCopyTo(CVIMAGE myimg,uint16 startw,uint16 starth,uint16 width,uint16 height);//ͼ���ȡ����
CVIMAGE YMCV_Creat_ImgROICopyTo(CVIMAGE myimg, CVrect* myroi);//ͼ��ROI������
CVIMAGE YMCV_Creat_Img_BackUp(CVIMAGE myimg);//ͼ�񱸷�
CVIMAGE YMCV_Creat_DataToImg(uint8* data,uint16 width,uint16 height, CVImgType Imgform);//ͼ�����ݷ�װ
#endif // YMCV_IMAGE_USE

#if YMCV_VECTOR_USE
#include"./YMCV_OBJ.h"
CVVECTORS_F32 YMCV_Creat_Vector_F32(uint16 dim, uint32 dataNum);//����float32����
CVVECTORS_U16 YMCV_Creat_Vector_U16(uint16 dim, uint32 dataNum);//����uint16����
#endif // YMCV_VECTOR_USE

#if YMCV_F32MAT_USE
#include"./YMCV_OBJ.h"
CVFMAT YMCV_Creat_FMAT_Creat(uint16 row, uint16 col);//���󴴽�
#endif // YMCV_F32MAT_USE


#if YMCV_CRENDER_USE
#include"../CORE/YMCV_ChannelCal.h"
CVPIX_RENDER YMCV_Creat_PixRender_Creat(CVPXPMap rendform, PIXChannelNum channelnum);//����ͨ����Ⱦ������
#endif // YMCV_CRENDER_USE


#if YMCV_GRENDER_USE
#include"../CORE/YMCV_GeomCal.h"
CVGEOM_RENDER YMCV_Creat_GeomRender_Creat(CVGEOMETHOD rendform);//���α任��Ⱦ������
#endif

#if YMCV_SRENDER_USE
#include"../CORE/YMCV_SpaceCal.h" 
#include"../CORE/YMCV_Padding.h" 
//�ռ��˲���Ⱦ������
CVKERN_RENDER YMCV_Creat_SpaceRender_Creat(CVKERNELTYPE rendform, PIXChannelNum channelnum, uint8 kernel_r, YMCVPADDINGType padding);
#endif

#if YMCV_FRENDER_USE
#include"../CORE/YMCV_FreqCal.h"
//һά FFT �˲�������
CVFFT_RENDER YMCV_Creat_FFT_Render_Creat(int16 processnum, uint16 data_step, uint8 ToAF);
//һά DCT �˲�������
CVDCT_RENDER YMCV_Creat_DCT_Render_Creat(int16 processnum, uint16 data_step);
//һά DWT Harr �˲�������
CVDWT_RENDER  YMCV_Creat_DWT_Render_Creat(int16 processnum, uint16 data_step, uint8 gradenum);

//��ά FFT �˲�������
CVFFT2D_RENDER YMCV_Creat_FFT2D_Render_Creat(int16 width, int16 height);
//��ά DCT �˲�������
CVDCT2D_RENDER YMCV_Creat_DCT2D_Render_Creat(int16 width, int16 height);
//��ά DWT �˲�������
CVDWT2D_RENDER YMCV_Creat_DWT2D_Render_Creat(int16 width, int16 height);

//Ƶ���˲�������
CVFREQ_RENDER YMCV_Creat_FreqKernel_Render_Creat(CVFREQTYPE kernlType, CVCOMPLEXTYPE ComplexType);

#endif

#if YMCV_BRENDER_USE
#include"../CORE/YMCV_BinaryCal.h"
//��ֵ����Ⱦ������
CVBINARY_RENDER YMCV_Creat_BinaryRender_Creat(CVThreshold rendform);
#endif // #if YMCV_BRENDER_USE

#if YMCV_FEATURE_USE
#include"../CORE/YMCV_FeatCal.h"
//blob�ָ���Ⱦ������
CVBLOB_SEG_RENDER YMCV_Creat_BlobSegRender_Creat(CVThreshold rendform);
//�����ڴ洴��
CVCHAINS_U16 YMCV_Creat_Chains_U16(uint32 ChainsNum);
//LBPֱ��ͼ
CV_FACE_LBP_DESC YMCV_Creat_FaceLbp_Hist();

#endif // YMCV_FEATURE_USE


#if YMCV_OTHERDA_USE
#include"../CORE/YMCV_GMM.h"
CVGMMS YMCV_Creat_Gmms_Creat(uint8 dataDim, uint8 componentsNum);

//Ŀ�����������
CV_TARGET_TRACK YMCV_Creat_TargetTrack_Creat(CVIMAGE image, CVrect* myroi);
//���Ӹ�����
CV_PARTICLE_TRACK YMCV_Creat_ParticleTrack_Creat(CVIMAGE image, CVrect* myroi, uint16 particleNum, float32 velocity_delta, float32 scale_delta);

#endif // YMCV_OTHERDA_USE

#endif
