#include"../CONFIG/YMCV_Mem.h"
#include"./YMCV_Creat.h"
#include"./YMCV_Free.h"
#include"../DEBUG/YMCV_Debug.h"
#include "../CONFIG/YMCV_PubDefine.h"
#include "../CORE/YMCV_InnerMath.h"
//#include"../YMCV_PubData.h"

/**
  ***************************************************************************************************************************
  *	@FileName:    YMCV_Creat.c
  *	@Author:      yaomimaoren
  *	@Date:        2021-10-31
  *	@Description: ���󴴽�,��ģ�����ܶ��󴴽�����װ���Լ�����ڴ���
  *	@Version:     1.0
  *
  ***************************************************************************************************************************
  *
  * ��ע��Ϣ��
  * 1.������ͼ�񴴽����ü�������������תͼ���ʽ
  *
  *
  * �������ⱸע��
  *
  ***************************************************************************************************************************
  * <author> <time> <version > <desc>
  * yaomi 21/10/31 1.0 build this moudle
  *
  *   __  __ ___    ____   __  ___ ____     ______ ______ ______ __  __
  *   \ \/ //   |  / __ \ /  |/  //  _/    /_  __// ____// ____// / / /
  *    \  // /| | / / / // /|_/ / / /       / /  / __/  / /    / /_/ /
  *    / // ___ |/ /_/ // /  / /_/ /       / /  / /___ / /___ / __  /
  *   /_//_/  |_|\____//_/  /_//___/      /_/  /_____/ \____//_/ /_/
  *
  * Copyright (C), 2021-2031, YAOMI Tech. Co., Ltd.
  ***************************************************************************************************************************/

//ͼ�����ݽṹ���
#if YMCV_IMAGE_USE

/**
  * @brief ͼ�����ݽṹ����������ͼ���ڴ�
  */
CVIMAGE YMCV_Creat_Img_Creat(uint16 width,uint16 height, CVImgType Imgform)
{
	CVIMAGE myimg = NULL;
	uint16 k,num;
	myimg=(CVIMAGE)CV_malloc0(sizeof(CVimage));//ͼ��ͷ��Ϣ
	
	//�ڴ�ָ���ж�
	cv_assert(myimg);
	cv_log_explain(myimg == NULL, CV_LOG_Mem0, "ͼ��ͷ�ڴ�����ʧ��");

	myimg->width=width;
	myimg->height=height;
	myimg->type=Imgform;
	myimg->sizet =  (Imgform == CVBinaryType) ? CVBinarySize :
					(Imgform == CVTrimapType) ? CVGraySize ://�ͻҶ�ͼһ����С
					(Imgform == CVGrayType)   ? CVGraySize :
					(Imgform == CVRgb16Type)  ? CVRgb16Size :
					(Imgform == CVFGrayType)  ? CVFGRAYSize :
					(Imgform == CVCGrayType)  ? CVCGRAYSize :
					(Imgform == CVIGrayType)  ? CVIGRAYSize : CVIGRAYSize;//Ĭ��int

	myimg->pmlc = 0;
	//complex gray ����ͼ���С
	if (Imgform == CVCGrayType)
	{
		//����ع滮
		for ( k = 0, num = 0; num < width;)
		{
			k++;
			num = 1 << k;
		}
		myimg->width = num;

		//�߶��ع滮
		for ( k = 0, num = 0; num < height;)
		{
			k++;
			num = 1 << k;
		}
		myimg->height = num;
	}

	myimg->data= (_color8_t *)CV_malloc1((myimg->height)*(myimg->width)*(myimg->sizet));//�����ڴ�����

	//�ڴ�ָ���ж�
	cv_assert(myimg->data);
	cv_log_explain(myimg->data == NULL, CV_LOG_Mem1, "ͼ�������ڴ�����ʧ��");
	//�ڴ�ָ���ж�
	if (myimg->data == NULL)
	{
		CV_free0(myimg);
		return NULL;
	}
	return myimg;
}

/**
  * @brief ͼ��ü���������
  */
CVIMAGE YMCV_Creat_ImgCopyTo(CVIMAGE myimg,uint16 startw,uint16 starth,uint16 width,uint16 height)
{
	CVIMAGE newimg;
	CVGRAY graypin,graypout;
	CVRGB16 rgb16pin,rgb16pout;

    uint16 i,j;
	//����ָ���ж�
	cv_assert(myimg);
	cv_assert(myimg->data);
	cv_log_explain(myimg == NULL, CV_LOG_PtrI, "����ͼ�񲻴���");


    newimg= YMCV_Creat_Img_Creat(width,height,myimg->type);//����һ��ͼƬ
	//�ڴ�ָ���ж�
	cv_assert(newimg);
	cv_log_explain(newimg == NULL, CV_LOG_Mem0, "ͼ��ͷ�ڴ�����ʧ��");

	//Խ�翽���б�
	cv_log_explain((startw+ width)> myimg->width, CV_LOG_ParamI, "�ü�Խ�磬������ü����");
	cv_log_explain((starth + height) > myimg->height, CV_LOG_ParamI, "�ü�Խ�磬������ü��߶�");

	switch (myimg->type)
	{
	case CVBinaryType:
	case CVGrayType:
	{
		graypin = (CVGRAY)myimg->data;
		graypout = (CVGRAY)newimg->data;
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				graypout[i * width + j] = graypin[(i + starth) * myimg->width + (j + startw)];
			}
		}
		break;
	}
	case CVRgb16Type:
	{
		rgb16pin = (CVRGB16)myimg->data;
		rgb16pout = (CVRGB16)newimg->data;
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				rgb16pout[i * width + j] = rgb16pin[(i + starth) * myimg->width + (j + startw)];
			}
		}
		break;
	}
	default:
		cv_log_explain(myimg->type != NULL, CV_LOG_TypeI, "��δ֧�ָ�����ͼ����");
		break;
	}
	return newimg;
}

/**
  * @brief ͼ��ROI�ü�,�ü���������
  */
CVIMAGE YMCV_Creat_ImgROICopyTo(CVIMAGE myimg, CVrect* myroi)
{
	return YMCV_Creat_ImgCopyTo(myimg, myroi->x, myroi->y, myroi->w, myroi->h);
}
/**
  * @brief ͼ�񿽱�����
  */
CVIMAGE YMCV_Creat_Img_BackUp(CVIMAGE myimg)
{
	CVIMAGE newimg;
	CVGRAY graypin, graypout;
	CVRGB16 rgb16pin, rgb16pout;

    uint32 i,tolnum;
    uint16 width,height;

	//����ָ���ж�
	cv_assert(myimg);
	cv_assert(myimg->data);
	cv_log_explain(myimg == NULL, CV_LOG_PtrI, "����ͼ�񲻴���");

    width=myimg->width;
    height=myimg->height;
	tolnum = width * height;


	newimg = YMCV_Creat_Img_Creat(width, height, myimg->type);//����һ��ͼƬ

	//�ڴ�ָ���ж�
	cv_assert(newimg);
	cv_log_explain(newimg == NULL, CV_LOG_Mem0, "���ͼ���ڴ�����ʧ��");

	switch (myimg->type)
	{
	case CVBinaryType:
	case CVGrayType:
	{
		graypin = (CVGRAY)myimg->data;
		graypout = (CVGRAY)newimg->data;
		for (i = 0; i < tolnum; i++)
		{
			graypout[i] = graypin[i];
		}
		break;
	}
	case CVRgb16Type:
	{
		rgb16pin = (CVRGB16)myimg->data;
		rgb16pout = (CVRGB16)newimg->data;
		for (i = 0; i < tolnum; i++)
		{
			rgb16pout[i] = rgb16pin[i];
		}
		break;
	}
	default:
		cv_log_explain(myimg->type != NULL, CV_LOG_TypeI, "δ֧�ָ�����ͼ����");
		break;
	}
	return newimg;
}


/**
  * @brief ����תΪͼ���ʽ�����ݱ������ʹ�� CV_malloc1������ڴ���
  */
CVIMAGE YMCV_Creat_DataToImg(uint8* data,uint16 width,uint16 height, CVImgType Imgform)
{
	CVIMAGE myimg;
	
	//����ָ���ж�
	cv_assert(data);
	cv_log_explain(data == NULL, CV_LOG_PtrI, "����ͼ�����ݲ�����");

	myimg=(CVIMAGE)CV_malloc0(sizeof(CVimage));//ͼ��ͷ��Ϣ
	//�ڴ�ָ���ж�
	cv_assert(myimg);
	cv_log_explain(myimg == NULL, CV_LOG_Mem0, "ͼ��ͷ�ڴ�����ʧ��");

	myimg->width=width;
	myimg->height=height;
	myimg->type=Imgform;
	myimg->sizet =  (Imgform == CVBinaryType) ? CVBinarySize :
					(Imgform == CVGrayType) ? CVGraySize :
					(Imgform == CVRgb16Type) ? CVRgb16Size :
					(Imgform == CVFGrayType) ? CVFGRAYSize :
					(Imgform == CVCGrayType) ? CVCGRAYSize :
					(Imgform == CVIGrayType) ? CVIGRAYSize : CVIGRAYSize;//Ĭ��int


	myimg->pmlc = 0;
	myimg->data= data;

	//�ڴ�ָ���ж�
	if (myimg->data == NULL)
	{
		CV_free0(myimg);
		return NULL;
	}
	return myimg;
}


#endif // YMCV_V2_0

//�������ݽṹ���
#if YMCV_VECTOR_USE
#include"./YMCV_OBJ.h"
/**
  * @brief float32�����������ڴ�����
  */
CVVECTORS_F32 YMCV_Creat_Vector_F32(uint16 dim, uint32 dataNum)
{
	CVVECTORS_F32 myvec;

	myvec = (CVVECTORS_F32)CV_malloc0(sizeof(CVvectors_f32));
	cv_log_explain(dim > 1024, CV_LOG_ParamI, "Ĭ��ά�����ܳ���1024���������Ӧ����");
	cv_log_explain(dataNum > 41910000, CV_LOG_ParamI, "Ĭ�����ݳ��Ȳ��ܳ���41910000���������Ӧ����");
	//����ʧ��
	cv_assert(myvec);
	cv_log_explain(myvec == NULL, CV_LOG_Mem0, "����ͷ�ڴ�����ʧ��");
	if (myvec == NULL)//�ڴ治��
	{
		return NULL;
	}
	myvec->datap = (float32*)CV_malloc1(dim * dataNum * sizeof(float32));
	cv_log_explain(myvec->datap == NULL, CV_LOG_Mem1, "���������ڴ�����ʧ��");
	myvec->dim = dim;
	myvec->dataNum = dataNum;
	return myvec;
}

/**
  * @brief uint16�����������ڴ�����
  */
CVVECTORS_U16 YMCV_Creat_Vector_U16(uint16 dim, uint32 dataNum)
{
	CVVECTORS_U16 myvec;

	cv_log_explain(dim > 255, CV_LOG_ParamI, "Ĭ��ά�����ܳ���1024���������Ӧ����");
	cv_log_explain(dataNum > 16770000, CV_LOG_ParamI, "Ĭ�����ݳ��Ȳ��ܳ���16770000���������Ӧ����");

	myvec = (CVVECTORS_U16)CV_malloc0(sizeof(CVvectors_u16));
	//����ʧ��
	cv_assert(myvec);
	cv_log_explain(myvec == NULL, CV_LOG_Mem0, "����ͷ�ڴ�����ʧ��");
	if (myvec == NULL)//�ڴ治��
	{
		return NULL;
	}
	myvec->datap = (uint16*)CV_malloc1(dim * dataNum * sizeof(uint16));
	cv_log_explain(myvec->datap == NULL, CV_LOG_Mem1, "���������ڴ�����ʧ��");
	myvec->dim = dim;
	myvec->dataNum = dataNum;
	return myvec;
}

#endif // YMCV_VECTOR_USE

//�������ݽṹ���
#if YMCV_F32MAT_USE
#include"./YMCV_OBJ.h"

/**
  * @brief ����float���;���
  */
CVFMAT YMCV_Creat_FMAT_Creat(uint16 row, uint16 col)
{
	CVFMAT mymat;

	cv_log_explain(row* col == 0, CV_LOG_ParamI, "���ܴ�����СΪ0�ľ���");

	mymat = (CVFMAT)CV_malloc0(sizeof(CVfmat));
	//����ʧ��
	cv_assert(mymat);
	cv_log_explain(mymat == NULL, CV_LOG_Mem0, "����ͷ�ڴ�����ʧ��");
	if (mymat == NULL)//�ڴ治��
	{
		return NULL;
	}
	mymat->datap = (float32*)CV_malloc0(row * col * sizeof(float32));
	cv_log_explain(mymat->datap == NULL, CV_LOG_Mem0, "���������ڴ�����ʧ��");
	mymat->col = col;
	mymat->row = row;
	return mymat;
}
#endif // YMCV_VECTOR_USE

//ͼ��ͨ����Ⱦ���
#if YMCV_CRENDER_USE

/**
  * @brief ͼ��ͨ����Ⱦ����������������ͨ���ڴ棬������С���� CV_malloc0
  */
CVPIX_RENDER YMCV_Creat_PixRender_Creat(CVPXPMap rendform, PIXChannelNum channelnum)
{
	CVPIX_RENDER myrend = NULL;

	myrend = (CVPIX_RENDER)CV_malloc0(sizeof(CVpix_render));
	
	//�ڴ�ָ���ж�
	cv_assert(myrend);
	cv_log_explain(myrend == NULL, CV_LOG_Mem0, "ͨ����Ⱦ��ͷ�ڴ�����ʧ��");
	cv_log_explain(!(channelnum > 0), CV_LOG_ParamI, "��Ⱦ��ͨ��������");

	//��Ⱦͨ���ڴ�����
	myrend->PixTrTable = (uint8 *)CV_malloc0( 256 * channelnum);//�ڴ�����

	cv_log_explain(myrend->PixTrTable == NULL, CV_LOG_Mem0, "��Ⱦ��ͨ������ʧ��");

	myrend->PXPType = rendform;//��Ⱦ����
	myrend->ChannelNum = channelnum;//��Ⱦ��ͨ����
	return myrend;
}

#endif // YMCV_V2_0


//����λ����Ⱦ���
#if YMCV_GRENDER_USE
//��̬�ڴ�����
void* YMCV_Geom_VectBuff_Malloc(size_t size)
{
	return(void*)CV_malloc0(size);
}
/**
  * @brief ���α任��Ⱦ������
  */
CVGEOM_RENDER YMCV_Creat_GeomRender_Creat(CVGEOMETHOD rendform)
{
	CVGEOM_RENDER myrend = NULL;

	myrend = (CVGEOM_RENDER)CV_malloc0(sizeof(CVgeom_render));

	//�ڴ�ָ���ж�
	cv_assert(myrend);
	cv_log_explain(myrend == NULL, CV_LOG_Mem0, "ͨ����Ⱦ��ͷ�ڴ�����ʧ��");

	myrend->VecMalloc = NULL;
	myrend->myVector.perspect.X_a = NULL;
	myrend->myVector.perspect.X_b = NULL;

	switch (rendform)
	{
	case CV_Stretch: //���ڽ��ڲ巨 ���� | ѹ��
	case CV_StretchL2: //˫�����ڲ巨 ���� | ѹ��
	{
		myrend->GeomType = rendform;
		myrend->VecMalloc = YMCV_Geom_VectBuff_Malloc;
		break;
	}
	case CV_RotateWauto: //ͼ����ת�任 �����������Ӧ
	case CV_RotateWconst: //ͼ����ת�任 �̶��������
	{
		myrend->GeomType = rendform;
		myrend->VecMalloc = YMCV_Geom_VectBuff_Malloc;
		break;
	}
	case CV_LinerPolar:
	case CV_LogPolar:
	case CV_LinerPolar_Inverse:
	case CV_LogPolar_Inverse:
	{
		myrend->GeomType = rendform;
		myrend->VecMalloc = YMCV_Geom_VectBuff_Malloc;
		break;
	}
	case CV_Perspective:
	{
		myrend->GeomType = rendform;
		myrend->VecMalloc = YMCV_Geom_VectBuff_Malloc;
		break;
	}
	default:
		cv_log_explain(rendform > 0, CV_LOG_ParamI, "�ݲ�֧�ָ���Ⱦ��ʽ");
		break;
	}
	return myrend;
}

#endif // YMCV_V2_0


//�����˲���Ⱦ���
#if YMCV_SRENDER_USE

/**
  * @brief ͼ�������Ⱦ�������������˲��˰뾶 ��[1 -- 128],
  */
CVKERN_RENDER YMCV_Creat_SpaceRender_Creat(CVKERNELTYPE rendform, PIXChannelNum channelnum, uint8 kernel_r, YMCVPADDINGType padding)
{
	CVKERN_RENDER myrend = NULL;
	float32 *ker_data;

	myrend = (CVKERN_RENDER)CV_malloc0(sizeof(CVkern_render));

	//�ڴ�����ʧ��
	cv_assert(myrend);
	cv_log_explain(myrend == NULL, CV_LOG_Mem0, "������Ⱦ��ͷ�ڴ�����ʧ��");
	cv_log_explain(!(channelnum > 0), CV_LOG_ParamI, "��Ⱦ��ͨ��������");
	cv_log_explain(kernel_r > 128, CV_LOG_ParamI, "��Ⱦ���뾶����");

	switch (rendform)
	{
	case CV_GaussFilter: //��˹�˲�
	case CV_LOGFilter: //LOG�˲�
	case CV_BilFilter: //˫���˲�
	case CV_CustomFilter://�Զ����˲���
	{
		ker_data = (float32*)CV_malloc0((2 * kernel_r + 1) * (2 * kernel_r + 1) * sizeof(float32));
		cv_log_explain(ker_data == NULL, CV_LOG_Mem0, "������Ⱦ�˲����ڴ�����ʧ��");

		//����ͨ�ò���
		myrend->KernelType = rendform;
		myrend->ChannelNum = channelnum;
		myrend->padding = padding;
		myrend->rsize = kernel_r;
		myrend->data = ker_data;
		break;
	}
	case CV_AverFilter:  //��ֵ�˲�
	case CV_LMSEFilter://�ֲ��������˲�
	case CV_MinFilter://��Сֵ�˲�
	case CV_MedFilter://��ֵ�˲�
	case CV_MaxFilter://���ֵ�˲�
	case CV_ModeFilter://�����˲�
	case CV_MidPointFilter://�е��˲�
	{
		//����ͨ�ò���
		myrend->KernelType = rendform;
		myrend->ChannelNum = channelnum;
		myrend->padding = padding;
		myrend->rsize = kernel_r;
		myrend->data = NULL;
		break;
	}
	default:
	{
		cv_log_explain(rendform>0, CV_LOG_ParamI, "�ݲ�֧�ָ���Ⱦ��ʽ");
		break;
	}
	}
	return myrend;
}
#endif // YMCV_V2_0


//Ƶ���˲���Ⱦ���
#if YMCV_FRENDER_USE
/**
  * @brief ���ĵ�fftΪ���� һ�����������ڵļ�����ó���65536
  */
CVFFT_RENDER YMCV_Creat_FFT_Render_Creat(int16 processnum, uint16 data_step,uint8 ToAF)
{
	float32 *wrp, *wip;
	uint32 *index;
	float32 radp;
	uint16 k, i, it, kn;
	uint16 m, is, j;

	CVFFT_RENDER myfft1d = NULL;

	myfft1d = (CVFFT_RENDER)CV_malloc0(sizeof(CVfft_render));

	//�ڴ�����ʧ��
	cv_assert(myfft1d);
	cv_log_explain(myfft1d == NULL, CV_LOG_Mem0, "һάfft��Ⱦ��ͷ�ڴ�����ʧ��");
	if (myfft1d == NULL)//�ڴ治��
	{
		return NULL;
	}

	myfft1d->dt_step = data_step;
	myfft1d->danum = processnum;

	for (k = 0, kn = 0; kn < processnum;)
	{
		k++;
		kn = 1 << k;
	}
	myfft1d->Lnum = k;//һ����k��
	myfft1d->unum = kn; //����n = 2 ^ k
	//���������ڴ�
	index = (uint32*)CV_malloc0(myfft1d->unum * sizeof(uint32));
	//����ʧ��
	cv_log_explain(index == NULL, CV_LOG_Mem0, "һάfft��Ⱦ�������ڴ�����ʧ��");
	if (index == NULL)
	{
		CV_free0(myfft1d);
		return NULL;
	}
	//�����Ʊ��뵹������Ԥ������������
	for (it = 0; it < myfft1d->unum; it++)
	{
		m = it;
		is = 0;
		for (i = 0; i <= k - 1; i++)
		{
			j = m % 2;
			is = (is << 1) + j;
			m = m >> 1;
		}
		index[it] = is * (uint32)data_step;
	}
	myfft1d->packindex = index;//��¼����

	//����FFT�������ڴ�
	wrp = (float32*)CV_malloc0(myfft1d->unum * sizeof(float32));
	wip = (float32*)CV_malloc0(myfft1d->unum * sizeof(float32));
	//����ʧ��
	cv_log_explain((wrp == NULL || wip == NULL), CV_LOG_Mem0, "һάfft��Ⱦ������������ڴ�����ʧ��");
	if (wrp == NULL || wip == NULL)
	{
		CV_free0(wrp); CV_free0(wip);
		CV_free0(index);
		CV_free0(myfft1d);
		return NULL;
	}
	// w0
	wrp[0] = 1.0f;
	wip[0] = 0.0f;
	//��ת�Ƕ�
	radp = YMCV_2Pai / ((float32)myfft1d->unum); //2* pai / num 
	//��Ƶ����  w1=( ew1 , ew2)
	wrp[1] = YMCV_Cos(radp); //ʵ��
	wip[1] = -YMCV_Sin(radp);//�鲿
	for (i = 2; i < myfft1d->unum; i++)//��תn��
	{
		//�����Ƶ�� i ��
		wrp[i] = wrp[1] * wrp[i - 1] - wip[1] * wip[i - 1];
		wip[i] = wrp[1] * wip[i - 1] + wip[1] * wrp[i - 1];
	}
	//��¼������
	myfft1d->wcvr = wrp;
	myfft1d->wcvi = wip;

	//���÷��Ⱥ���λ
	myfft1d->ToAF = ToAF;

	return myfft1d;
}

/**
  * @brief ���ݳ�����Ϊ2�ı���,��ż���������Զ�����һ��
  */
CVDWT_RENDER  YMCV_Creat_DWT_Render_Creat(int16 processnum, uint16 data_step, uint8 gradenum)
{
	int i;
	CVDWT_RENDER render;

	render = (CVDWT_RENDER)CV_malloc0(sizeof(CVdwt_render));
	//����ʧ��
	cv_assert(render);
	cv_log_explain(render == NULL, CV_LOG_Mem0, "һάdwt��Ⱦ��ͷ�ڴ�����ʧ��");
	if (render == NULL)//�ڴ治��
	{
		return NULL;
	}
	render->danum = processnum;
	render->Lnum = 2 * ((processnum + 1) / 2);//��Ч����

	//��Ч�任����
	for (i = 0; i < gradenum; i++)
	{
		processnum = processnum / 2;
		if (processnum == 1)
			break;
	}
	render->unum = i;


	render->dt_step = data_step;

	return render;
}
/**
  * @brief ���ݳ������� һ�����������ڵļ�����ó���65536
  */
CVDCT_RENDER YMCV_Creat_DCT_Render_Creat(int16 processnum, uint16 data_step)
{
	float32* wrp, * wip;
	uint32* index;
	float32 radp;

	CVDCT_RENDER mydctt1d = NULL;

	mydctt1d = (CVDCT_RENDER)CV_malloc0(sizeof(CVdct_render));

	//�ڴ�����ʧ��
	cv_assert(mydctt1d);
	cv_log_explain(mydctt1d == NULL, CV_LOG_Mem0, "һάdct��Ⱦ��ͷ�ڴ�����ʧ��");
	if (mydctt1d == NULL) return NULL;

	mydctt1d->dt_step = data_step;//���ݼ��
	mydctt1d->danum = processnum;
	//���������ڴ�
	index = (uint32*)CV_malloc0(mydctt1d->danum * sizeof(uint32));
	//����ʧ��
	cv_log_explain(index == NULL, CV_LOG_Mem0, "һάfft��Ⱦ�������ڴ�����ʧ��");
	if (index == NULL)
	{
		CV_free0(mydctt1d);
		return NULL;
	}
	// ������������
	for (int i = 0; i < mydctt1d->danum; i++)
	{
		index[i] = i * data_step;
	}
	mydctt1d->index = index;

	//����DCT�������ڴ�
	wrp = (float32*)CV_malloc0(mydctt1d->danum * sizeof(float32));
	wip = (float32*)CV_malloc0(mydctt1d->danum * sizeof(float32));
	//����ʧ��
	cv_log_explain((wrp == NULL || wip == NULL), CV_LOG_Mem0, "һάdct�������ڴ�����ʧ��");
	if (wrp == NULL || wip == NULL)
	{
		CV_free0(wrp); CV_free0(wip);
		CV_free0(mydctt1d);
		return NULL;
	}
	//����alpha����
	mydctt1d->alpha0 = YMCV_Sqrt(1.0 / mydctt1d->danum);
	mydctt1d->alpha1 = YMCV_Sqrt(2.0 / mydctt1d->danum);

	// �����Ƶ����
	for (int i = 0; i < mydctt1d->danum; i++)
	{
		//��u=1
		radp = (i + 0.5f) * YMCV_Pai / mydctt1d->danum;//��ת�Ƕ�
		wrp[i] = YMCV_Cos(radp);
		wip[i] = YMCV_Sin(radp);
	}

	//��¼������
	mydctt1d->wcvr = wrp;
	mydctt1d->wcvi = wip;

	return mydctt1d;
}


/**
  * @brief ��һάfftΪ���� ��άfft�任��Ⱦ��
  */
CVFFT2D_RENDER YMCV_Creat_FFT2D_Render_Creat(int16 width, int16 height)
{
	CVFFT2D_RENDER render;

	render = (CVFFT2D_RENDER)CV_malloc0(sizeof(CVfft2d_render));
	//����ʧ��
	cv_assert(render);
	cv_log_explain(render == NULL, CV_LOG_Mem0, "��άfft��Ⱦ��ͷ�ڴ�����ʧ��");
	if (render == NULL)//�ڴ治��
	{
		return NULL;
	}
	//���Ϊ1����������Ƶת��
	render->fftw = YMCV_Creat_FFT_Render_Creat(width, 2, 0);//����fft ���ݼ��Ϊ2��float  ��תΪģ���н�
	render->ffth = YMCV_Creat_FFT_Render_Creat(height, 2 * render->fftw->unum, 0);//����fft�����ݼ��Ϊ 2 * width

	//����ʧ��
	cv_log_explain((render->fftw == NULL || render->ffth == NULL), CV_LOG_Mem0, "��άfft��Ⱦ���ڴ�����ʧ��");
	if (render->fftw == NULL || render->ffth == NULL)//�ڴ治��
	{
		YMCV_Free_FFT_RenderFree(render->fftw);
		YMCV_Free_FFT_RenderFree(render->ffth);
		CV_free0(render);
		return NULL;
	}
	//���¿��
	render->width = render->fftw->unum;
	render->height = render->ffth->unum;

	return render;
}


/**
  * @brief ��һάdctΪ���� ��άdct�任��Ⱦ��
  */
CVDCT2D_RENDER YMCV_Creat_DCT2D_Render_Creat(int16 width, int16 height)
{
	CVDCT2D_RENDER render;

	render = (CVDCT2D_RENDER)CV_malloc0(sizeof(CVdct2d_render));
	//����ʧ��
	cv_assert(render);
	cv_log_explain(render == NULL, CV_LOG_Mem0, "��άfft��Ⱦ��ͷ�ڴ�����ʧ��");
	if (render == NULL)//�ڴ治��
	{
		return NULL;
	}
	//���Ϊ1����������Ƶת��
	render->dctw = YMCV_Creat_DCT_Render_Creat(width, 1);//����dct ���ݼ��Ϊ1��float 
	render->dcth = YMCV_Creat_DCT_Render_Creat(height, render->dctw->danum);//����fft�����ݼ��Ϊ width

	//����ʧ��
	cv_log_explain((render->dctw == NULL || render->dcth == NULL), CV_LOG_Mem0, "��άdct��Ⱦ���ڴ�����ʧ��");
	if (render->dctw == NULL || render->dcth == NULL)//�ڴ治��
	{
		YMCV_Free_DCT_RenderFree(render->dctw);
		YMCV_Free_DCT_RenderFree(render->dcth);
		CV_free0(render);
		return NULL;
	}
	//���¿��
	render->width = render->dctw->danum;
	render->height = render->dcth->danum;

	return render;
}


/**
  * @brief ��һάdwtΪ���� ��άdwt�任��Ⱦ��
  */
CVDWT2D_RENDER YMCV_Creat_DWT2D_Render_Creat(int16 width, int16 height)
{
	CVDWT2D_RENDER render;

	render = (CVDWT2D_RENDER)CV_malloc0(sizeof(CVdwt2d_render));
	//����ʧ��
	cv_assert(render);
	cv_log_explain(render == NULL, CV_LOG_Mem0, "��άdwt��Ⱦ��ͷ�ڴ�����ʧ��");
	if (render == NULL)//�ڴ治��
	{
		return NULL;
	}
	//���Ϊ1��Ĭ��1���任
	render->dwtw = YMCV_Creat_DWT_Render_Creat(width, 1, 1);
	render->dwth = YMCV_Creat_DWT_Render_Creat(height, render->dwtw->Lnum,1 );
	//����ʧ��
	cv_log_explain((render->dwtw == NULL || render->dwth == NULL), CV_LOG_Mem0, "��άfft��Ⱦ���ڴ�����ʧ��");
	if (render->dwtw == NULL || render->dwth == NULL)	//�ڴ治��
	{
		YMCV_Free_DWT_RenderFree(render->dwtw);
		YMCV_Free_DWT_RenderFree(render->dwth);
		CV_free0(render);
		return NULL;
	}
	//���¿��
	render->width = render->dwtw->Lnum;
	render->height = render->dwth->Lnum;

	return render;
}

/**
  * @brief ����Ƶ���˲���Ⱦ��
  */
CVFREQ_RENDER YMCV_Creat_FreqKernel_Render_Creat(CVFREQTYPE kernlType, CVCOMPLEXTYPE ComplexType)
{
	CVFREQ_RENDER render;

	render = (CVFREQ_RENDER)CV_malloc0(sizeof(CVfreq_render));
	//����ʧ��
	cv_assert(render);
	cv_log_explain(render == NULL, CV_LOG_Mem0, "Ƶ���˲���Ⱦ��ͷ�ڴ�����ʧ��");
	if (render == NULL)//�ڴ治��
	{
		return NULL;
	}

	switch (kernlType)
	{
	case CV_GaussHFilter: //��˹��ͨ�˲�
	case CV_GaussLFilter: //��˹��ͨ�˲�
	case CV_ButterWorthHFilter:  //������˹��ͨ�˲�
	case CV_ButterWorthLFilter:  //������˹��ͨ�˲�
	case CV_BandPassFilter: //��ͨ�˲�
	case CV_BandStopFilter: //�����˲�
	case CV_HarrThreshFilter: //����С��Ӳ��ֵ�˲�
	{
		//����ͨ�ò���
		render->KernelType = kernlType;
		render->ComplexType = ComplexType;
		break;
	}
	default:
	{
		cv_log_explain(kernlType > 0, CV_LOG_ParamI, "�ݲ�֧�ָ���Ⱦ��ʽ");
		break;
	}
	}
	return render;
}

#endif // YMCV_V2_0


#if YMCV_BRENDER_USE
//��̬�ڴ�����
void* YMCV_Binary_VectBuff_Malloc(size_t size)
{
	return(void*)CV_malloc0(size);
}

/**
  * @brief ͼ���ֵ����Ⱦ������
  */
CVBINARY_RENDER YMCV_Creat_BinaryRender_Creat(CVThreshold rendform)
{
	CVBINARY_RENDER myrend = NULL;

	myrend = (CVBINARY_RENDER)CV_malloc0(sizeof(CVbinary_render));

	//�ڴ�ָ���ж�
	cv_assert(myrend);
	cv_log_explain(myrend == NULL, CV_LOG_Mem0, "ͨ����Ⱦ��ͷ�ڴ�����ʧ��");

	myrend->myVector.single.PixMap = NULL;
	myrend->myVector.three.PixMap1 = NULL;
	myrend->myVector.three.PixMap2 = NULL;
	myrend->myVector.three.PixMap3 = NULL;


	switch (rendform)
	{
	case CV_SingleThreshold: //����ֵ
	case CV_DoubleThreshold: //˫��ֵ
	case CV_IntegralThreshold://����ͼ��ֵ��
	case CV_HistThreshold://Otsu��ֵ��
	{
		myrend->ThreshType = rendform;
		myrend->myVector.single.PixMap = CV_malloc0(256 * sizeof(uint8));
		break;
	}
	case CV_RGB2Threshold: //RGB˫��ֵ
	case CV_LAB2Threshold: //LAB˫��ֵ
	{
		myrend->ThreshType = rendform;
		myrend->myVector.three.PixMap1 = CV_malloc0(256 * sizeof(uint8));
		myrend->myVector.three.PixMap2 = CV_malloc0(256 * sizeof(uint8));
		myrend->myVector.three.PixMap3 = CV_malloc0(256 * sizeof(uint8));
		break;
	}
	default:
		cv_log_explain(rendform > 0, CV_LOG_ParamI, "�ݲ�֧�ָ���Ⱦ��ʽ");
		break;
	}
	return myrend;
}

#endif // YMCV_BRENDER_USE

#if YMCV_FEATURE_USE

/**
  * @brief ͼ��ɫ���ֵ����Ⱦ������
  */
CVBLOB_SEG_RENDER YMCV_Creat_BlobSegRender_Creat(CVThreshold rendform)
{
	CVBLOB_SEG_RENDER myrend = NULL;

	myrend = (CVBLOB_SEG_RENDER)CV_malloc0(sizeof(CVblob_seg_render));

	//�ڴ�ָ���ж�
	cv_assert(myrend);
	cv_log_explain(myrend == NULL, CV_LOG_Mem0, "ͨ����Ⱦ��ͷ�ڴ�����ʧ��");

	myrend->myVector.single.PixMap = NULL;
	myrend->myVector.three.PixMap1 = NULL;
	myrend->myVector.three.PixMap2 = NULL;
	myrend->myVector.three.PixMap3 = NULL;


	switch (rendform)
	{
	case CV_SingleThreshold: //����ֵ
	case CV_DoubleThreshold: //˫��ֵ
	{
		myrend->ThreshType = rendform;
		myrend->myVector.single.PixMap = CV_malloc0(256 * sizeof(uint8));
		break;
	}
	case CV_RGB2Threshold: //RGB˫��ֵ
	case CV_LAB2Threshold: //LAB˫��ֵ
	{
		myrend->ThreshType = rendform;
		myrend->myVector.three.PixMap1 = CV_malloc0(256 * sizeof(uint8));
		myrend->myVector.three.PixMap2 = CV_malloc0(256 * sizeof(uint8));
		myrend->myVector.three.PixMap3 = CV_malloc0(256 * sizeof(uint8));
		break;
	}
	default:
		cv_log_explain(rendform > 0, CV_LOG_ParamI, "�ݲ�֧�ָ���Ⱦ��ʽ");
		break;
	}
	return myrend;
}

/**
  * @brief ���봴��
  */
CVCHAINS_U16 YMCV_Creat_Chains_U16(uint32 ChainsNum)
{
	CVCHAINS_U16 mychain;

	mychain = (CVCHAINS_U16)CV_malloc0(sizeof(CVchains_u16));
	//����ʧ��
	cv_assert(mychain);
	cv_log_explain(mychain == NULL, CV_LOG_Mem0, "����ͷ�ڴ�����ʧ��");
	if (mychain == NULL)//�ڴ治��
	{
		return NULL;
	}
	mychain->chain = (CVVECTORS_U16*)CV_malloc1(ChainsNum * sizeof(CVVECTORS_U16));
	cv_log_explain(mychain->chain == NULL, CV_LOG_Mem1, "���������ڴ�����ʧ��");
	//��ʼ��Ϊ 0
	for (int i = 0; i < ChainsNum; i++)
	{
		mychain->chain[i] = NULL;
	}

	mychain->cNum = ChainsNum;
	return mychain;
}

/**
  * @brief ֱ��ͼ����
  */
CV_FACE_LBP_DESC YMCV_Creat_FaceLbp_Hist()
{
	CV_FACE_LBP_DESC mylbphis = NULL;

	mylbphis = (CV_FACE_LBP_DESC)CV_malloc1(sizeof(CV_face_lbp_desc));//ֱ��ͼ����

	//�ڴ�ָ���ж�
	cv_assert(mylbphis);
	cv_log_explain(mylbphis == NULL, CV_LOG_Mem1, "����LBP�ȼ�ֱ��ͼ����ʧ��");
	//��ʼ��Ϊ0
	CV_memset(mylbphis, 0, sizeof(CV_face_lbp_desc));
	return mylbphis;
}
#endif // YMCV_FEATURE_USE

//�������ݽṹ
#if YMCV_OTHERDA_USE
CVGMMS YMCV_Creat_Gmms_Creat(uint8 dataDim, uint8 componentsNum)
{
	CVGMMS mygmms;
	mygmms = (CVIMAGE)CV_malloc0(sizeof(CVgmms));//gmmͷ

	//�ڴ�ָ���ж�
	cv_assert(mygmms);
	cv_log_explain(mygmms == NULL, CV_LOG_Mem0, "GMM����ͷ�ڴ�����ʧ��");
	mygmms->componentsNum = componentsNum;
	mygmms->dataDim = dataDim;

	mygmms->coefs = (float32*)CV_malloc1(componentsNum * sizeof(float32));//ѡ�и���
	mygmms->mean = (float32*)CV_malloc1(componentsNum * dataDim * sizeof(float32));//��ά�Ⱦ�ֵ
	mygmms->icov = (float32*)CV_malloc1(componentsNum * dataDim * dataDim * sizeof(float32));//��Э�������
	mygmms->covdet = (float32*)CV_malloc1(componentsNum * sizeof(float32));//����ʽ

	return mygmms;
}

//Ŀ�����������
CV_TARGET_TRACK YMCV_Creat_TargetTrack_Creat(CVIMAGE image, CVrect* myroi)
{
	int rech, recw, recx, recy;
	uint16 histnum = 256;//ֱ��ͼ����

	cv_log_explain((image == NULL) || (myroi == NULL), CV_LOG_PtrI, "����ͼ���ROI������");
	rech = myroi->h;
	recw = myroi->w;
	recx = myroi->x;
	recy = myroi->y;
	cv_log_explain(((recx + recw) > image->width) || ((recy + rech) > image->height), CV_LOG_ParamI, "�����ROI��ͼ����Խ��");

	CV_TARGET_TRACK mytarget = CV_malloc0(sizeof(CV_target_track));
	mytarget->histnum = histnum;
	mytarget->hist = CV_malloc1(histnum * sizeof(float32));
	mytarget->Kdis = CV_malloc1(recw * rech * sizeof(float32));
	mytarget->roi = (*myroi);
	float32* hist1 = mytarget->hist;
	float32* m_wei = mytarget->Kdis;

	float32 recw_2 = recw / 2.0f;
	float32 rech_2 = rech / 2.0f;
	float32 h = recw_2 * recw_2 + rech_2 * rech_2;			//����

	//��ʼ��Ȩֵ�����Ŀ��ֱ��ͼ
	for (int i = 0, imax = recw * rech; i < imax; i++)
		m_wei[i] = 0.0f;

	for (int i = 0; i < histnum; i++)
		hist1[i] = 0.0f;

	//����Ȩ��
	float32 wsum = 0.0f;
	{
		//����Ԥ�ȼ���
		float32* jj2 = CV_malloc0(recw * sizeof(float32));
		for (int j = 0; j < recw; j++)
		{
			float32 j2 = j - recw_2;
			jj2[j] = j2 * j2;
		}

		//Ȩ���ǣ�����ĺ�����
		for (int i = 0; i < rech; i++)
		{
			float32 i2 = i - rech_2;
			i2 *= i2;//i*i
			float32* weiLine = &m_wei[i * recw];
			for (int j = 0; j < recw; j++)
			{
				float32 dist2 = i2 + jj2[j];//i*i+j*j
				weiLine[j] = 1.0f - (dist2 / h); //Epannechnikov���� K(dis)= 1 - dis^2
				wsum += weiLine[j];//���ڼ������ C =1/sum{ K(dis)}
			}
		}
		float32 Csum = 1.0f / wsum;//����C
		//��һ��Ȩ��
		for (int i = 0, imax = rech * recw; i < imax; i++)
		{
			m_wei[i] *= Csum;
		}
		//�ͷ��ڴ�
		CV_free0(jj2);
	}

	//����Ŀ��Ȩֱֵ��
	for (int i = 0; i < rech; i++)
	{
		float32* weiLine = &m_wei[i * recw];
		uint8* grayLine = &image->data[(recy + i) * image->width + recx];//������ʼλ��
		for (int j = 0; j < recw; j++)
		{
			uint8 gvl = grayLine[j];//��ȡֱ��ͼ��λ��
			//��Ӧֱ��ͼλ�ü�Ȩ��*1
			hist1[gvl] += weiLine[j];
		}
	}
	return mytarget;
}


//�������Ӹ�����
CV_PARTICLE_TRACK YMCV_Creat_ParticleTrack_Creat(CVIMAGE image, CVrect* myroi, uint16 particleNum, float32 velocity_delta, float32 scale_delta)
{
	cv_log_explain((image == NULL) || (myroi == NULL), CV_LOG_PtrI, "����ͼ���ROI������");
	cv_log_explain(((myroi->x + myroi->w) > image->width) || ((myroi->y + myroi->h) > image->height), CV_LOG_ParamI, "�����ROI��ͼ����Խ��");

	uint16 histnum = 256;//ֱ��ͼ����
	int xmax = myroi->x + myroi->w;
	int ymax = myroi->y + myroi->h;
	CV_PARTICLE_TRACK mytrack = CV_malloc0(sizeof(CV_particle_track));
	mytrack->histNum = histnum;
	mytrack->particleNum = particleNum;
	mytrack->scale_delta = scale_delta;//�߶ȱ任�ٶ��Ŷ�����
	mytrack->velocity_delta = velocity_delta;//�ٶ��Ŷ���ֵ
	mytrack->hists = (float32*)CV_malloc1(histnum * sizeof(float32));//ֱ��ͼ
	mytrack->states = (CV_particle_state*)CV_malloc1(particleNum * sizeof(CV_particle_state));//����״̬
	mytrack->weights = (float32*)CV_malloc1(particleNum * sizeof(float32));//����Ȩ��
	CV_particle_state* mystates = mytrack->states;
	float32* myhists = mytrack->hists;
	float32* myweights = mytrack->weights;
	//��ʼ��
	for (int i = 0; i < histnum; i++)
		myhists[i] = 0.0f;

	int halfw = myroi->w / 2;
	int halfh = myroi->h / 2;
	int centx = myroi->x + halfw;
	int centy = myroi->y + halfh;
	float divaa2 = 1.0f / (halfw * halfw + halfh * halfh); //����˺����뾶ƽ��a^2

	//����Ԥ�ȼ���
	int* xx2 = CV_malloc0(xmax * sizeof(int));
	for (uint16 x = myroi->x; x < xmax; x++)
	{
		int x2 = x - centx;
		xx2[x] = x2 * x2;
	}
	//����ֱ��ͼ
	for (uint16 y = myroi->y; y < ymax; y++)
	{
		int yy2 = y - centy;
		yy2 *= yy2;// y^2

		uint8* gline = &image->data[y * image->width];
		for (uint16 x = myroi->x; x < xmax; x++)
		{
			uint8 grayv = gline[x];
			float r2 = (yy2 + xx2[x]) * divaa2;//����뾶ƽ��r^2
			float kr = (r2 <1.0f)? (1.0f - r2) : 0.0f; //�˺���k(r) = 1-r^2, |r| < 1; ����ֵ k(r) = 0 
			myhists[grayv] += kr;//������ܶȼ�Ȩֱ��ͼ
		}
	}

	CV_free0(xx2);//�ͷ��ڴ�

	//ֱ��ͼ��һ��
	float ftol = 0.0f;
	for (int i = 0; i < histnum; i++)
		ftol += myhists[i];

	if (ftol != 0.0f)
	{
		ftol = 1.0f / ftol;//����

		for (int i = 0; i < histnum; i++)
		{
			myhists[i] *= ftol;//��һ��
		}
	}


	// ��ʼ������״̬(��(x0,y0,1,1,Wx,Hy,0.1)Ϊ���ĳ�N(0,0.4)��̬�ֲ�)
	mystates[0].x = centx;
	mystates[0].y = centy;
	mystates[0].Hx = halfw;
	mystates[0].Hy = halfh;
	mystates[0].vx = 0.0f;
	mystates[0].vy = 0.0f;
	mystates[0].vs = 0.0f;
	float divwei = 1.0f / particleNum;
	myweights[0] = divwei; //Ȩ�����

	float rng5[5];
	for (int i = 1; i < particleNum; i++)
	{
		for (int j = 0; j < 5; j++)
			rng5[j] = YMCV_RandGaussian(0.0f, 0.001f); /* ����5�������˹�ֲ����� */
		//���г�ʼ��
		mystates[i].x = centx + rng5[0] * halfw;
		mystates[i].y = centy + rng5[1] * halfh;
		mystates[i].Hx = halfw;
		mystates[i].Hy = halfh;
		mystates[i].vx = 0.0f + rng5[2] * velocity_delta;
		mystates[i].vy = 0.0f + rng5[3] * velocity_delta;
		mystates[i].vs = 0.0f + rng5[4] * scale_delta;
		myweights[i] = divwei; //Ȩ��ͳһΪ1/N����ÿ����������ȵĻ���
	}

	return mytrack;
}
#endif // YMCV_OTHERDA_USE

