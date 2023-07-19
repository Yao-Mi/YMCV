#include"./YMCV_OBJ.h"
#include"./YMCV_Free.h"
#include"../DEBUG/YMCV_Debug.h"
#include"../CONFIG/YMCV_Mem.h"

//ͼ�����ݽṹ���
#if YMCV_IMAGE_USE

/**
  * @brief image ͼ���ڴ��ͷ�
  */
void YMCV_Free_ImgFree(CVIMAGE pthis)
{
	cv_log_explain(pthis == NULL, CV_LOG_Mem0, "ͼ�񲻴��ڣ��ڴ��ͷ�ʧ��");
	if (pthis == NULL)
		return;
	CV_free1(pthis->data);
	CV_free0(pthis);
}
#endif //YMCV_IMAGE_USE

//�������
#if YMCV_VECTOR_USE
#include"./YMCV_OBJ.h"
//���������ڴ��ͷ�
void YMCV_Free_Vector_F32_Free(CVVECTORS_F32 pthis)
{
	if (pthis == NULL)return;
	CV_free1(pthis->datap);
	CV_free0(pthis);
}

//���������ڴ��ͷ�
void YMCV_Free_Vector_U16_Free(CVVECTORS_U16 pthis)
{
	if (pthis == NULL)return;
	CV_free1(pthis->datap);
	CV_free0(pthis);
}
#endif // YMCV_VECTOR_USE

//�������
#if YMCV_F32MAT_USE
#include"./YMCV_OBJ.h"
//��������ڴ��ͷ�
void YMCV_Free_FMatFree(CVFMAT pthis)
{
	if (pthis == NULL)return;
	CV_free0(pthis->datap);
	CV_free0(pthis);
}
#endif // YMCV_F32MAT_USE
#if YMCV_CRENDER_USE

//CVPIX_RENDER ��Ⱦ���ڴ��ͷ�
void YMCV_Free_PIX_RenderFree(CVPIX_RENDER pthis)
{
	cv_log_explain(pthis == NULL, CV_LOG_Mem0, "��Ⱦ�������ڣ��ڴ��ͷ�ʧ��");
	if (pthis == NULL)
		return;
	CV_free0(pthis->PixTrTable);
	CV_free0(pthis);
}
#endif // YMCV_CRENDER_USE

#if YMCV_SRENDER_USE
//CVKERN_RENDER ��Ⱦ���ڴ��ͷ�
void YMCV_Free_KERN_RenderFree(CVKERN_RENDER pthis)
{
	cv_log_explain(pthis == NULL, CV_LOG_Mem0, "��Ⱦ�������ڣ��ڴ��ͷ�ʧ��");
	if (pthis == NULL)return;
	//ʹ�õ����ݵĻ����������ڴ��ͷ�
	if (pthis->data != NULL)
	{
		CV_free0(pthis->data);
	}
	CV_free0(pthis);
}
#endif // YMCV_SRENDER_USE

#if YMCV_GRENDER_USE
//��̬�ڴ��ͷ�
void YMCV_Geom_VectBuff_Free(void* ptr)
{
	CV_free0(ptr);
}

//CVGEOM_Render ��Ⱦ���ڴ��ͷ�
void YMCV_Free_GEOM_RenderFree(CVGEOM_RENDER pthis)
{
	cv_log_explain(pthis == NULL, CV_LOG_Mem0, "��Ⱦ�������ڣ��ڴ��ͷ�ʧ��");
	if (pthis == NULL)return;

	//���������ͷ�
	switch (pthis->GeomType)
	{
	case CV_Stretch: //���ڽ��ڲ巨 ���� | ѹ��
	{
		//����������û��
		if (pthis->myVector.stretch.X_a)YMCV_Geom_VectBuff_Free(pthis->myVector.stretch.X_a);
		if (pthis->myVector.stretch.X_b)YMCV_Geom_VectBuff_Free(pthis->myVector.stretch.X_b);
		//�ͷ�����
		YMCV_Geom_VectBuff_Free(pthis->myVector.stretch.Xi);
		YMCV_Geom_VectBuff_Free(pthis->myVector.stretch.Yi);
		break;
	}
	case CV_StretchL2: //˫�����ڲ巨 ���� | ѹ��
	{
		//�ͷ�����
		YMCV_Geom_VectBuff_Free(pthis->myVector.stretch.X_a);
		YMCV_Geom_VectBuff_Free(pthis->myVector.stretch.X_b);
		YMCV_Geom_VectBuff_Free(pthis->myVector.stretch.Xi);
		YMCV_Geom_VectBuff_Free(pthis->myVector.stretch.Yi);
		break;
	}
	case CV_RotateWauto: //ͼ����ת�任 �����������Ӧ
	case CV_RotateWconst: //ͼ����ת�任 �̶��������
	{
		//�ͷ�����
		YMCV_Geom_VectBuff_Free(pthis->myVector.rotate.X_a);
		YMCV_Geom_VectBuff_Free(pthis->myVector.rotate.X_b);
		break;
	}
	case CV_LinerPolar:
	{
		//�ͷ�����
		break;
	}
	case CV_LogPolar:
	{
		//�ͷ�����
		YMCV_Geom_VectBuff_Free(pthis->myVector.Logpolar.pRho);
		break;
	}
	case CV_LinerPolar_Inverse:
	case CV_LogPolar_Inverse:
	{
		//�ͷ�����
		YMCV_Geom_VectBuff_Free(pthis->myVector.Inversepolar.pDetai);
		YMCV_Geom_VectBuff_Free(pthis->myVector.Inversepolar.pIs2);
		break;
	}
	case CV_Perspective:
	{
		//�ͷ�����
		YMCV_Geom_VectBuff_Free(pthis->myVector.perspect.X_a);
		YMCV_Geom_VectBuff_Free(pthis->myVector.perspect.X_b);
		break;
	}
	default:
		cv_log_explain(pthis->GeomType > 0, CV_LOG_ParamI, "δ֪����Ⱦ�ں�");
		break;
	}

	CV_free0(pthis);
}

#endif // YMCV_CRENDER_USE

#if YMCV_FRENDER_USE
//CVFFT_Render ��Ⱦ���ڴ��ͷ�
void YMCV_Free_FFT_RenderFree(CVFFT_RENDER pthis)
{
	if (pthis == NULL)return;
	CV_free0(pthis->wcvi);
	CV_free0(pthis->wcvr);
	CV_free0(pthis->packindex);
	CV_free0(pthis);
}
//CVDCT_Render ��Ⱦ���ڴ��ͷ�
void YMCV_Free_DCT_RenderFree(CVDCT_RENDER pthis)
{
	if (pthis == NULL)return;
	CV_free0(pthis->wcvi);
	CV_free0(pthis->wcvr);
	CV_free0(pthis->index);
	CV_free0(pthis);
}
//CVDWT_Render ��Ⱦ���ڴ��ͷ�
void YMCV_Free_DWT_RenderFree(CVDWT_RENDER pthis)
{
	if (pthis == NULL)return;
	CV_free0(pthis);
}

//CVFFT2D_Render ��Ⱦ���ڴ��ͷ�
void YMCV_Free_FFT2D_RenderFree(CVFFT2D_RENDER pthis)
{
	if (pthis == NULL)return;
	YMCV_Free_FFT_RenderFree(pthis->ffth);
	YMCV_Free_FFT_RenderFree(pthis->fftw);
	CV_free0(pthis);
}

//CVDCT2D_Render ��Ⱦ���ڴ��ͷ�
void YMCV_Free_DCT2D_RenderFree(CVDCT2D_RENDER pthis)
{
	if (pthis == NULL)return;
	YMCV_Free_DCT_RenderFree(pthis->dcth);
	YMCV_Free_DCT_RenderFree(pthis->dctw);
	CV_free0(pthis);
}

//CVDWT2D_Render ��Ⱦ���ڴ��ͷ�
void YMCV_Free_DWT2D_RenderFree(CVDWT2D_RENDER pthis)
{
	if (pthis == NULL)return;
	YMCV_Free_DWT_RenderFree(pthis->dwth);
	YMCV_Free_DWT_RenderFree(pthis->dwtw);
	CV_free0(pthis);
}

#endif

#if YMCV_BRENDER_USE
/**
  * @brief ͼ���ֵ����Ⱦ���ڴ��ͷ�
  */
void YMCV_Free_Binary_RenderFree(CVBINARY_RENDER pthis)
{
	//�ڴ�ָ���ж�
	cv_log_explain(pthis == NULL, CV_LOG_PtrI, "��Ⱦ��������");

	switch (pthis->ThreshType)
	{
	case CV_SingleThreshold: //����ֵ
	case CV_DoubleThreshold: //˫��ֵ
	case CV_IntegralThreshold://����ͼ��ֵ��
	case CV_HistThreshold://Otsu��ֵ��
	{
		CV_free0(pthis->myVector.single.PixMap);
		break;
	}
	case CV_RGB2Threshold: //RGB˫��ֵ
	case CV_LAB2Threshold: //LAB˫��ֵ
	{
		CV_free0(pthis->myVector.three.PixMap1);
		CV_free0(pthis->myVector.three.PixMap2);
		CV_free0(pthis->myVector.three.PixMap3);
		break;
	}
	default:
		cv_log_explain(pthis->ThreshType != NULL, CV_LOG_ParamI, "�ݲ�֧�ָ���Ⱦ��ʽ");
		break;
	}

	CV_free0(pthis);
}

#endif // YMCV_BRENDER_USE

#if YMCV_FEATURE_USE
/**
  * @brief blob�ڴ��ͷ�
  */
void YMCV_Free_BlobSegRender_Free(CVBLOB_SEG_RENDER pthis)
{
	//�ڴ�ָ���ж�
	cv_log_explain(pthis == NULL, CV_LOG_PtrI, "��Ⱦ��������");

	switch (pthis->ThreshType)
	{
	case CV_SingleThreshold: //����ֵ
	case CV_DoubleThreshold: //˫��ֵ
	{
		CV_free0(pthis->myVector.single.PixMap);
		break;
	}
	case CV_RGB2Threshold: //RGB˫��ֵ
	case CV_LAB2Threshold: //LAB˫��ֵ
	{
		CV_free0(pthis->myVector.three.PixMap1);
		CV_free0(pthis->myVector.three.PixMap2);
		CV_free0(pthis->myVector.three.PixMap3);
		break;
	}
	default:
		cv_log_explain(pthis->ThreshType != NULL, CV_LOG_ParamI, "�ݲ�֧�ָ���Ⱦ��ʽ");
		break;
	}
	CV_free0(pthis);
}

/**
  * @brief �����ͷ�
  */
void YMCV_Free_Chains_U16_Free(CVCHAINS_U16 pthis)
{
    cv_log_explain(pthis == NULL, CV_LOG_PtrI, "���벻����");

	//�����ͷ�
	for (int i = 0; i < pthis->cNum; i++)
	{
		if (pthis->chain[i] != NULL)
			YMCV_Free_Vector_U16_Free(pthis->chain[i]);
	}

	CV_free1(pthis->chain);
	CV_free0(pthis);
}

/**
  * @brief lbpֱ��ͼ�ͷ�
  */
void YMCV_Free_FaceLbp_Hist_free(CV_FACE_LBP_DESC pthis)
{
	cv_log_explain(pthis == NULL, CV_LOG_PtrI, "lbpֱ��ͼ������");
	CV_free1(pthis);
}

#endif // YMCV_FEATURE_USE

#if YMCV_OTHERDA_USE
void YMCV_Free_Gmms_Free(CVGMMS pthis)//GMMs�����ڴ��ͷ�
{
	//�ڴ�ָ���ж�
	cv_log_explain(pthis == NULL, CV_LOG_PtrI, "GMM���󲻴���");
	CV_free1(pthis->coefs);
	CV_free1(pthis->covdet);
	CV_free1(pthis->icov);
	CV_free1(pthis->mean);

	CV_free0(pthis);
}

//Ŀ�������
void YMCV_Free_TargetTrack_Free(CV_TARGET_TRACK pthis)
{
	//�ڴ�ָ���ж�
	cv_log_explain(pthis == NULL, CV_LOG_PtrI, "Ŀ�������������");
	CV_free1(pthis->hist);
	CV_free1(pthis->Kdis);
	CV_free0(pthis);
}

//���Ӹ�����
void YMCV_Free_ParticleTrack_Free(CV_PARTICLE_TRACK pthis)
{
	//�ڴ�ָ���ж�
	cv_log_explain(pthis == NULL, CV_LOG_PtrI, "Ŀ�������������");
	CV_free1(pthis->hists);
	CV_free1(pthis->weights);
	CV_free1(pthis->states);
	CV_free0(pthis);
}
#endif // YMCV_OTHERDA_USE


