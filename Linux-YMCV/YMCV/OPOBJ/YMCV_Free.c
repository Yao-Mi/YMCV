#include"./YMCV_OBJ.h"
#include"./YMCV_Free.h"
#include"../DEBUG/YMCV_Debug.h"
#include"../CONFIG/YMCV_Mem.h"

//图像数据结构相关
#if YMCV_IMAGE_USE

/**
  * @brief image 图像内存释放
  */
void YMCV_Free_ImgFree(CVIMAGE pthis)
{
	cv_log_explain(pthis == NULL, CV_LOG_Mem0, "图像不存在，内存释放失败");
	if (pthis == NULL)
		return;
	CV_free1(pthis->data);
	CV_free0(pthis);
}
#endif //YMCV_IMAGE_USE

//向量相关
#if YMCV_VECTOR_USE
#include"./YMCV_OBJ.h"
//浮点向量内存释放
void YMCV_Free_Vector_F32_Free(CVVECTORS_F32 pthis)
{
	if (pthis == NULL)return;
	CV_free1(pthis->datap);
	CV_free0(pthis);
}

//整形向量内存释放
void YMCV_Free_Vector_U16_Free(CVVECTORS_U16 pthis)
{
	if (pthis == NULL)return;
	CV_free1(pthis->datap);
	CV_free0(pthis);
}
#endif // YMCV_VECTOR_USE

//矩阵相关
#if YMCV_F32MAT_USE
#include"./YMCV_OBJ.h"
//浮点矩阵内存释放
void YMCV_Free_FMatFree(CVFMAT pthis)
{
	if (pthis == NULL)return;
	CV_free0(pthis->datap);
	CV_free0(pthis);
}
#endif // YMCV_F32MAT_USE
#if YMCV_CRENDER_USE

//CVPIX_RENDER 渲染器内存释放
void YMCV_Free_PIX_RenderFree(CVPIX_RENDER pthis)
{
	cv_log_explain(pthis == NULL, CV_LOG_Mem0, "渲染器不存在，内存释放失败");
	if (pthis == NULL)
		return;
	CV_free0(pthis->PixTrTable);
	CV_free0(pthis);
}
#endif // YMCV_CRENDER_USE

#if YMCV_SRENDER_USE
//CVKERN_RENDER 渲染器内存释放
void YMCV_Free_KERN_RenderFree(CVKERN_RENDER pthis)
{
	cv_log_explain(pthis == NULL, CV_LOG_Mem0, "渲染器不存在，内存释放失败");
	if (pthis == NULL)return;
	//使用到数据的话进行数据内存释放
	if (pthis->data != NULL)
	{
		CV_free0(pthis->data);
	}
	CV_free0(pthis);
}
#endif // YMCV_SRENDER_USE

#if YMCV_GRENDER_USE
//动态内存释放
void YMCV_Geom_VectBuff_Free(void* ptr)
{
	CV_free0(ptr);
}

//CVGEOM_Render 渲染器内存释放
void YMCV_Free_GEOM_RenderFree(CVGEOM_RENDER pthis)
{
	cv_log_explain(pthis == NULL, CV_LOG_Mem0, "渲染器不存在，内存释放失败");
	if (pthis == NULL)return;

	//缓冲向量释放
	switch (pthis->GeomType)
	{
	case CV_Stretch: //最邻近内插法 拉伸 | 压缩
	{
		//这两个正常没有
		if (pthis->myVector.stretch.X_a)YMCV_Geom_VectBuff_Free(pthis->myVector.stretch.X_a);
		if (pthis->myVector.stretch.X_b)YMCV_Geom_VectBuff_Free(pthis->myVector.stretch.X_b);
		//释放向量
		YMCV_Geom_VectBuff_Free(pthis->myVector.stretch.Xi);
		YMCV_Geom_VectBuff_Free(pthis->myVector.stretch.Yi);
		break;
	}
	case CV_StretchL2: //双线性内插法 拉伸 | 压缩
	{
		//释放向量
		YMCV_Geom_VectBuff_Free(pthis->myVector.stretch.X_a);
		YMCV_Geom_VectBuff_Free(pthis->myVector.stretch.X_b);
		YMCV_Geom_VectBuff_Free(pthis->myVector.stretch.Xi);
		YMCV_Geom_VectBuff_Free(pthis->myVector.stretch.Yi);
		break;
	}
	case CV_RotateWauto: //图像旋转变换 输出窗口自适应
	case CV_RotateWconst: //图像旋转变换 固定输出窗口
	{
		//释放向量
		YMCV_Geom_VectBuff_Free(pthis->myVector.rotate.X_a);
		YMCV_Geom_VectBuff_Free(pthis->myVector.rotate.X_b);
		break;
	}
	case CV_LinerPolar:
	{
		//释放向量
		break;
	}
	case CV_LogPolar:
	{
		//释放向量
		YMCV_Geom_VectBuff_Free(pthis->myVector.Logpolar.pRho);
		break;
	}
	case CV_LinerPolar_Inverse:
	case CV_LogPolar_Inverse:
	{
		//释放向量
		YMCV_Geom_VectBuff_Free(pthis->myVector.Inversepolar.pDetai);
		YMCV_Geom_VectBuff_Free(pthis->myVector.Inversepolar.pIs2);
		break;
	}
	case CV_Perspective:
	{
		//释放向量
		YMCV_Geom_VectBuff_Free(pthis->myVector.perspect.X_a);
		YMCV_Geom_VectBuff_Free(pthis->myVector.perspect.X_b);
		break;
	}
	default:
		cv_log_explain(pthis->GeomType > 0, CV_LOG_ParamI, "未知的渲染内核");
		break;
	}

	CV_free0(pthis);
}

#endif // YMCV_CRENDER_USE

#if YMCV_FRENDER_USE
//CVFFT_Render 渲染器内存释放
void YMCV_Free_FFT_RenderFree(CVFFT_RENDER pthis)
{
	if (pthis == NULL)return;
	CV_free0(pthis->wcvi);
	CV_free0(pthis->wcvr);
	CV_free0(pthis->packindex);
	CV_free0(pthis);
}
//CVDCT_Render 渲染器内存释放
void YMCV_Free_DCT_RenderFree(CVDCT_RENDER pthis)
{
	if (pthis == NULL)return;
	CV_free0(pthis->wcvi);
	CV_free0(pthis->wcvr);
	CV_free0(pthis->index);
	CV_free0(pthis);
}
//CVDWT_Render 渲染器内存释放
void YMCV_Free_DWT_RenderFree(CVDWT_RENDER pthis)
{
	if (pthis == NULL)return;
	CV_free0(pthis);
}

//CVFFT2D_Render 渲染器内存释放
void YMCV_Free_FFT2D_RenderFree(CVFFT2D_RENDER pthis)
{
	if (pthis == NULL)return;
	YMCV_Free_FFT_RenderFree(pthis->ffth);
	YMCV_Free_FFT_RenderFree(pthis->fftw);
	CV_free0(pthis);
}

//CVDCT2D_Render 渲染器内存释放
void YMCV_Free_DCT2D_RenderFree(CVDCT2D_RENDER pthis)
{
	if (pthis == NULL)return;
	YMCV_Free_DCT_RenderFree(pthis->dcth);
	YMCV_Free_DCT_RenderFree(pthis->dctw);
	CV_free0(pthis);
}

//CVDWT2D_Render 渲染器内存释放
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
  * @brief 图像二值化渲染器内存释放
  */
void YMCV_Free_Binary_RenderFree(CVBINARY_RENDER pthis)
{
	//内存指针判断
	cv_log_explain(pthis == NULL, CV_LOG_PtrI, "渲染器不存在");

	switch (pthis->ThreshType)
	{
	case CV_SingleThreshold: //单阈值
	case CV_DoubleThreshold: //双阈值
	case CV_IntegralThreshold://积分图二值化
	case CV_HistThreshold://Otsu二值化
	{
		CV_free0(pthis->myVector.single.PixMap);
		break;
	}
	case CV_RGB2Threshold: //RGB双阈值
	case CV_LAB2Threshold: //LAB双阈值
	{
		CV_free0(pthis->myVector.three.PixMap1);
		CV_free0(pthis->myVector.three.PixMap2);
		CV_free0(pthis->myVector.three.PixMap3);
		break;
	}
	default:
		cv_log_explain(pthis->ThreshType != NULL, CV_LOG_ParamI, "暂不支持该渲染方式");
		break;
	}

	CV_free0(pthis);
}

#endif // YMCV_BRENDER_USE

#if YMCV_FEATURE_USE
/**
  * @brief blob内存释放
  */
void YMCV_Free_BlobSegRender_Free(CVBLOB_SEG_RENDER pthis)
{
	//内存指针判断
	cv_log_explain(pthis == NULL, CV_LOG_PtrI, "渲染器不存在");

	switch (pthis->ThreshType)
	{
	case CV_SingleThreshold: //单阈值
	case CV_DoubleThreshold: //双阈值
	{
		CV_free0(pthis->myVector.single.PixMap);
		break;
	}
	case CV_RGB2Threshold: //RGB双阈值
	case CV_LAB2Threshold: //LAB双阈值
	{
		CV_free0(pthis->myVector.three.PixMap1);
		CV_free0(pthis->myVector.three.PixMap2);
		CV_free0(pthis->myVector.three.PixMap3);
		break;
	}
	default:
		cv_log_explain(pthis->ThreshType != NULL, CV_LOG_ParamI, "暂不支持该渲染方式");
		break;
	}
	CV_free0(pthis);
}

/**
  * @brief 链码释放
  */
void YMCV_Free_Chains_U16_Free(CVCHAINS_U16 pthis)
{
    cv_log_explain(pthis == NULL, CV_LOG_PtrI, "链码不存在");

	//链码释放
	for (int i = 0; i < pthis->cNum; i++)
	{
		if (pthis->chain[i] != NULL)
			YMCV_Free_Vector_U16_Free(pthis->chain[i]);
	}

	CV_free1(pthis->chain);
	CV_free0(pthis);
}

/**
  * @brief lbp直方图释放
  */
void YMCV_Free_FaceLbp_Hist_free(CV_FACE_LBP_DESC pthis)
{
	cv_log_explain(pthis == NULL, CV_LOG_PtrI, "lbp直方图不存在");
	CV_free1(pthis);
}

#endif // YMCV_FEATURE_USE

#if YMCV_OTHERDA_USE
void YMCV_Free_Gmms_Free(CVGMMS pthis)//GMMs对象内存释放
{
	//内存指针判断
	cv_log_explain(pthis == NULL, CV_LOG_PtrI, "GMM对象不存在");
	CV_free1(pthis->coefs);
	CV_free1(pthis->covdet);
	CV_free1(pthis->icov);
	CV_free1(pthis->mean);

	CV_free0(pthis);
}

//目标跟踪器
void YMCV_Free_TargetTrack_Free(CV_TARGET_TRACK pthis)
{
	//内存指针判断
	cv_log_explain(pthis == NULL, CV_LOG_PtrI, "目标跟踪器不存在");
	CV_free1(pthis->hist);
	CV_free1(pthis->Kdis);
	CV_free0(pthis);
}

//粒子跟踪器
void YMCV_Free_ParticleTrack_Free(CV_PARTICLE_TRACK pthis)
{
	//内存指针判断
	cv_log_explain(pthis == NULL, CV_LOG_PtrI, "目标跟踪器不存在");
	CV_free1(pthis->hists);
	CV_free1(pthis->weights);
	CV_free1(pthis->states);
	CV_free0(pthis);
}
#endif // YMCV_OTHERDA_USE


