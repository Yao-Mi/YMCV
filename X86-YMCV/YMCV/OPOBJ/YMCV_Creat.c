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
  *	@Description: 对象创建,该模块主管对象创建、封装，以及存放内存区
  *	@Version:     1.0
  *
  ***************************************************************************************************************************
  *
  * 备注信息：
  * 1.涵盖了图像创建、裁剪、拷贝、数据转图像格式
  *
  *
  * 存在问题备注：
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

//图像数据结构相关
#if YMCV_IMAGE_USE

/**
  * @brief 图像数据结构创建，申请图像内存
  */
CVIMAGE YMCV_Creat_Img_Creat(uint16 width,uint16 height, CVImgType Imgform)
{
	CVIMAGE myimg = NULL;
	uint16 k,num;
	myimg=(CVIMAGE)CV_malloc0(sizeof(CVimage));//图像头信息
	
	//内存指针判断
	cv_assert(myimg);
	cv_log_explain(myimg == NULL, CV_LOG_Mem0, "图像头内存申请失败");

	myimg->width=width;
	myimg->height=height;
	myimg->type=Imgform;
	myimg->sizet =  (Imgform == CVBinaryType) ? CVBinarySize :
					(Imgform == CVTrimapType) ? CVGraySize ://和灰度图一样大小
					(Imgform == CVGrayType)   ? CVGraySize :
					(Imgform == CVRgb16Type)  ? CVRgb16Size :
					(Imgform == CVFGrayType)  ? CVFGRAYSize :
					(Imgform == CVCGrayType)  ? CVCGRAYSize :
					(Imgform == CVIGrayType)  ? CVIGRAYSize : CVIGRAYSize;//默认int

	myimg->pmlc = 0;
	//complex gray 扩张图像大小
	if (Imgform == CVCGrayType)
	{
		//宽度重规划
		for ( k = 0, num = 0; num < width;)
		{
			k++;
			num = 1 << k;
		}
		myimg->width = num;

		//高度重规划
		for ( k = 0, num = 0; num < height;)
		{
			k++;
			num = 1 << k;
		}
		myimg->height = num;
	}

	myimg->data= (_color8_t *)CV_malloc1((myimg->height)*(myimg->width)*(myimg->sizet));//数据内存申请

	//内存指针判断
	cv_assert(myimg->data);
	cv_log_explain(myimg->data == NULL, CV_LOG_Mem1, "图像数据内存申请失败");
	//内存指针判断
	if (myimg->data == NULL)
	{
		CV_free0(myimg);
		return NULL;
	}
	return myimg;
}

/**
  * @brief 图像裁剪区拷贝，
  */
CVIMAGE YMCV_Creat_ImgCopyTo(CVIMAGE myimg,uint16 startw,uint16 starth,uint16 width,uint16 height)
{
	CVIMAGE newimg;
	CVGRAY graypin,graypout;
	CVRGB16 rgb16pin,rgb16pout;

    uint16 i,j;
	//输入指针判断
	cv_assert(myimg);
	cv_assert(myimg->data);
	cv_log_explain(myimg == NULL, CV_LOG_PtrI, "输入图像不存在");


    newimg= YMCV_Creat_Img_Creat(width,height,myimg->type);//创建一副图片
	//内存指针判断
	cv_assert(newimg);
	cv_log_explain(newimg == NULL, CV_LOG_Mem0, "图像头内存申请失败");

	//越界拷贝判别
	cv_log_explain((startw+ width)> myimg->width, CV_LOG_ParamI, "裁剪越界，请调整裁剪宽度");
	cv_log_explain((starth + height) > myimg->height, CV_LOG_ParamI, "裁剪越界，请调整裁剪高度");

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
		cv_log_explain(myimg->type != NULL, CV_LOG_TypeI, "暂未支持该类型图像处理");
		break;
	}
	return newimg;
}

/**
  * @brief 图像ROI裁剪,裁剪区拷贝，
  */
CVIMAGE YMCV_Creat_ImgROICopyTo(CVIMAGE myimg, CVrect* myroi)
{
	return YMCV_Creat_ImgCopyTo(myimg, myroi->x, myroi->y, myroi->w, myroi->h);
}
/**
  * @brief 图像拷贝备份
  */
CVIMAGE YMCV_Creat_Img_BackUp(CVIMAGE myimg)
{
	CVIMAGE newimg;
	CVGRAY graypin, graypout;
	CVRGB16 rgb16pin, rgb16pout;

    uint32 i,tolnum;
    uint16 width,height;

	//输入指针判断
	cv_assert(myimg);
	cv_assert(myimg->data);
	cv_log_explain(myimg == NULL, CV_LOG_PtrI, "输入图像不存在");

    width=myimg->width;
    height=myimg->height;
	tolnum = width * height;


	newimg = YMCV_Creat_Img_Creat(width, height, myimg->type);//创建一副图片

	//内存指针判断
	cv_assert(newimg);
	cv_log_explain(newimg == NULL, CV_LOG_Mem0, "输出图像内存申请失败");

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
		cv_log_explain(myimg->type != NULL, CV_LOG_TypeI, "未支持该类型图像处理");
		break;
	}
	return newimg;
}


/**
  * @brief 数据转为图像格式，数据必须放在使用 CV_malloc1申请的内存中
  */
CVIMAGE YMCV_Creat_DataToImg(uint8* data,uint16 width,uint16 height, CVImgType Imgform)
{
	CVIMAGE myimg;
	
	//输入指针判断
	cv_assert(data);
	cv_log_explain(data == NULL, CV_LOG_PtrI, "输入图像数据不存在");

	myimg=(CVIMAGE)CV_malloc0(sizeof(CVimage));//图像头信息
	//内存指针判断
	cv_assert(myimg);
	cv_log_explain(myimg == NULL, CV_LOG_Mem0, "图像头内存申请失败");

	myimg->width=width;
	myimg->height=height;
	myimg->type=Imgform;
	myimg->sizet =  (Imgform == CVBinaryType) ? CVBinarySize :
					(Imgform == CVGrayType) ? CVGraySize :
					(Imgform == CVRgb16Type) ? CVRgb16Size :
					(Imgform == CVFGrayType) ? CVFGRAYSize :
					(Imgform == CVCGrayType) ? CVCGRAYSize :
					(Imgform == CVIGrayType) ? CVIGRAYSize : CVIGRAYSize;//默认int


	myimg->pmlc = 0;
	myimg->data= data;

	//内存指针判断
	if (myimg->data == NULL)
	{
		CV_free0(myimg);
		return NULL;
	}
	return myimg;
}


#endif // YMCV_V2_0

//向量数据结构相关
#if YMCV_VECTOR_USE
#include"./YMCV_OBJ.h"
/**
  * @brief float32向量创建，内存申请
  */
CVVECTORS_F32 YMCV_Creat_Vector_F32(uint16 dim, uint32 dataNum)
{
	CVVECTORS_F32 myvec;

	myvec = (CVVECTORS_F32)CV_malloc0(sizeof(CVvectors_f32));
	cv_log_explain(dim > 1024, CV_LOG_ParamI, "默认维数不能超过1024，请调整相应变量");
	cv_log_explain(dataNum > 41910000, CV_LOG_ParamI, "默认数据长度不能超过41910000，请调整相应变量");
	//申请失败
	cv_assert(myvec);
	cv_log_explain(myvec == NULL, CV_LOG_Mem0, "向量头内存申请失败");
	if (myvec == NULL)//内存不足
	{
		return NULL;
	}
	myvec->datap = (float32*)CV_malloc1(dim * dataNum * sizeof(float32));
	cv_log_explain(myvec->datap == NULL, CV_LOG_Mem1, "向量数组内存申请失败");
	myvec->dim = dim;
	myvec->dataNum = dataNum;
	return myvec;
}

/**
  * @brief uint16向量创建，内存申请
  */
CVVECTORS_U16 YMCV_Creat_Vector_U16(uint16 dim, uint32 dataNum)
{
	CVVECTORS_U16 myvec;

	cv_log_explain(dim > 255, CV_LOG_ParamI, "默认维数不能超过1024，请调整相应变量");
	cv_log_explain(dataNum > 16770000, CV_LOG_ParamI, "默认数据长度不能超过16770000，请调整相应变量");

	myvec = (CVVECTORS_U16)CV_malloc0(sizeof(CVvectors_u16));
	//申请失败
	cv_assert(myvec);
	cv_log_explain(myvec == NULL, CV_LOG_Mem0, "向量头内存申请失败");
	if (myvec == NULL)//内存不足
	{
		return NULL;
	}
	myvec->datap = (uint16*)CV_malloc1(dim * dataNum * sizeof(uint16));
	cv_log_explain(myvec->datap == NULL, CV_LOG_Mem1, "向量数组内存申请失败");
	myvec->dim = dim;
	myvec->dataNum = dataNum;
	return myvec;
}

#endif // YMCV_VECTOR_USE

//矩阵数据结构相关
#if YMCV_F32MAT_USE
#include"./YMCV_OBJ.h"

/**
  * @brief 创建float类型矩阵
  */
CVFMAT YMCV_Creat_FMAT_Creat(uint16 row, uint16 col)
{
	CVFMAT mymat;

	cv_log_explain(row* col == 0, CV_LOG_ParamI, "不能创建大小为0的矩阵");

	mymat = (CVFMAT)CV_malloc0(sizeof(CVfmat));
	//申请失败
	cv_assert(mymat);
	cv_log_explain(mymat == NULL, CV_LOG_Mem0, "矩阵头内存申请失败");
	if (mymat == NULL)//内存不足
	{
		return NULL;
	}
	mymat->datap = (float32*)CV_malloc0(row * col * sizeof(float32));
	cv_log_explain(mymat->datap == NULL, CV_LOG_Mem0, "矩阵数组内存申请失败");
	mymat->col = col;
	mymat->row = row;
	return mymat;
}
#endif // YMCV_VECTOR_USE

//图像通道渲染相关
#if YMCV_CRENDER_USE

/**
  * @brief 图像通道渲染器创建创建，申请通道内存，都采用小区域 CV_malloc0
  */
CVPIX_RENDER YMCV_Creat_PixRender_Creat(CVPXPMap rendform, PIXChannelNum channelnum)
{
	CVPIX_RENDER myrend = NULL;

	myrend = (CVPIX_RENDER)CV_malloc0(sizeof(CVpix_render));
	
	//内存指针判断
	cv_assert(myrend);
	cv_log_explain(myrend == NULL, CV_LOG_Mem0, "通道渲染器头内存申请失败");
	cv_log_explain(!(channelnum > 0), CV_LOG_ParamI, "渲染器通道数错误");

	//渲染通道内存申请
	myrend->PixTrTable = (uint8 *)CV_malloc0( 256 * channelnum);//内存申请

	cv_log_explain(myrend->PixTrTable == NULL, CV_LOG_Mem0, "渲染器通道申请失败");

	myrend->PXPType = rendform;//渲染类型
	myrend->ChannelNum = channelnum;//渲染器通道数
	return myrend;
}

#endif // YMCV_V2_0


//几何位置渲染相关
#if YMCV_GRENDER_USE
//动态内存申请
void* YMCV_Geom_VectBuff_Malloc(size_t size)
{
	return(void*)CV_malloc0(size);
}
/**
  * @brief 几何变换渲染器创建
  */
CVGEOM_RENDER YMCV_Creat_GeomRender_Creat(CVGEOMETHOD rendform)
{
	CVGEOM_RENDER myrend = NULL;

	myrend = (CVGEOM_RENDER)CV_malloc0(sizeof(CVgeom_render));

	//内存指针判断
	cv_assert(myrend);
	cv_log_explain(myrend == NULL, CV_LOG_Mem0, "通道渲染器头内存申请失败");

	myrend->VecMalloc = NULL;
	myrend->myVector.perspect.X_a = NULL;
	myrend->myVector.perspect.X_b = NULL;

	switch (rendform)
	{
	case CV_Stretch: //最邻近内插法 拉伸 | 压缩
	case CV_StretchL2: //双线性内插法 拉伸 | 压缩
	{
		myrend->GeomType = rendform;
		myrend->VecMalloc = YMCV_Geom_VectBuff_Malloc;
		break;
	}
	case CV_RotateWauto: //图像旋转变换 输出窗口自适应
	case CV_RotateWconst: //图像旋转变换 固定输出窗口
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
		cv_log_explain(rendform > 0, CV_LOG_ParamI, "暂不支持该渲染方式");
		break;
	}
	return myrend;
}

#endif // YMCV_V2_0


//空域滤波渲染相关
#if YMCV_SRENDER_USE

/**
  * @brief 图像空域渲染器创建创建，滤波核半径 ：[1 -- 128],
  */
CVKERN_RENDER YMCV_Creat_SpaceRender_Creat(CVKERNELTYPE rendform, PIXChannelNum channelnum, uint8 kernel_r, YMCVPADDINGType padding)
{
	CVKERN_RENDER myrend = NULL;
	float32 *ker_data;

	myrend = (CVKERN_RENDER)CV_malloc0(sizeof(CVkern_render));

	//内存申请失败
	cv_assert(myrend);
	cv_log_explain(myrend == NULL, CV_LOG_Mem0, "空域渲染器头内存申请失败");
	cv_log_explain(!(channelnum > 0), CV_LOG_ParamI, "渲染器通道数错误");
	cv_log_explain(kernel_r > 128, CV_LOG_ParamI, "渲染器半径过大");

	switch (rendform)
	{
	case CV_GaussFilter: //高斯滤波
	case CV_LOGFilter: //LOG滤波
	case CV_BilFilter: //双边滤波
	case CV_CustomFilter://自定义滤波核
	{
		ker_data = (float32*)CV_malloc0((2 * kernel_r + 1) * (2 * kernel_r + 1) * sizeof(float32));
		cv_log_explain(ker_data == NULL, CV_LOG_Mem0, "空域渲染滤波器内存申请失败");

		//设置通用参数
		myrend->KernelType = rendform;
		myrend->ChannelNum = channelnum;
		myrend->padding = padding;
		myrend->rsize = kernel_r;
		myrend->data = ker_data;
		break;
	}
	case CV_AverFilter:  //均值滤波
	case CV_LMSEFilter://局部均方差滤波
	case CV_MinFilter://最小值滤波
	case CV_MedFilter://中值滤波
	case CV_MaxFilter://最大值滤波
	case CV_ModeFilter://众数滤波
	case CV_MidPointFilter://中点滤波
	{
		//设置通用参数
		myrend->KernelType = rendform;
		myrend->ChannelNum = channelnum;
		myrend->padding = padding;
		myrend->rsize = kernel_r;
		myrend->data = NULL;
		break;
	}
	default:
	{
		cv_log_explain(rendform>0, CV_LOG_ParamI, "暂不支持该渲染方式");
		break;
	}
	}
	return myrend;
}
#endif // YMCV_V2_0


//频域滤波渲染相关
#if YMCV_FRENDER_USE
/**
  * @brief 以四点fft为基础 一组数据数据内的间隔不得超过65536
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

	//内存申请失败
	cv_assert(myfft1d);
	cv_log_explain(myfft1d == NULL, CV_LOG_Mem0, "一维fft渲染器头内存申请失败");
	if (myfft1d == NULL)//内存不足
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
	myfft1d->Lnum = k;//一共有k层
	myfft1d->unum = kn; //其中n = 2 ^ k
	//申请索引内存
	index = (uint32*)CV_malloc0(myfft1d->unum * sizeof(uint32));
	//申请失败
	cv_log_explain(index == NULL, CV_LOG_Mem0, "一维fft渲染器编码内存申请失败");
	if (index == NULL)
	{
		CV_free0(myfft1d);
		return NULL;
	}
	//二进制编码倒序，数据预处理重排索引
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
	myfft1d->packindex = index;//记录索引

	//申请FFT基向量内存
	wrp = (float32*)CV_malloc0(myfft1d->unum * sizeof(float32));
	wip = (float32*)CV_malloc0(myfft1d->unum * sizeof(float32));
	//申请失败
	cv_log_explain((wrp == NULL || wip == NULL), CV_LOG_Mem0, "一维fft渲染器编码基向量内存申请失败");
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
	//旋转角度
	radp = YMCV_2Pai / ((float32)myfft1d->unum); //2* pai / num 
	//基频向量  w1=( ew1 , ew2)
	wrp[1] = YMCV_Cos(radp); //实部
	wip[1] = -YMCV_Sin(radp);//虚部
	for (i = 2; i < myfft1d->unum; i++)//旋转n次
	{
		//计算基频的 i 倍
		wrp[i] = wrp[1] * wrp[i - 1] - wip[1] * wip[i - 1];
		wip[i] = wrp[1] * wip[i - 1] + wip[1] * wrp[i - 1];
	}
	//记录基内容
	myfft1d->wcvr = wrp;
	myfft1d->wcvi = wip;

	//启用幅度和相位
	myfft1d->ToAF = ToAF;

	return myfft1d;
}

/**
  * @brief 数据长度需为2的倍数,即偶数，否则自动增加一个
  */
CVDWT_RENDER  YMCV_Creat_DWT_Render_Creat(int16 processnum, uint16 data_step, uint8 gradenum)
{
	int i;
	CVDWT_RENDER render;

	render = (CVDWT_RENDER)CV_malloc0(sizeof(CVdwt_render));
	//申请失败
	cv_assert(render);
	cv_log_explain(render == NULL, CV_LOG_Mem0, "一维dwt渲染器头内存申请失败");
	if (render == NULL)//内存不足
	{
		return NULL;
	}
	render->danum = processnum;
	render->Lnum = 2 * ((processnum + 1) / 2);//有效长度

	//有效变换级数
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
  * @brief 数据长度任意 一组数据数据内的间隔不得超过65536
  */
CVDCT_RENDER YMCV_Creat_DCT_Render_Creat(int16 processnum, uint16 data_step)
{
	float32* wrp, * wip;
	uint32* index;
	float32 radp;

	CVDCT_RENDER mydctt1d = NULL;

	mydctt1d = (CVDCT_RENDER)CV_malloc0(sizeof(CVdct_render));

	//内存申请失败
	cv_assert(mydctt1d);
	cv_log_explain(mydctt1d == NULL, CV_LOG_Mem0, "一维dct渲染器头内存申请失败");
	if (mydctt1d == NULL) return NULL;

	mydctt1d->dt_step = data_step;//数据间隔
	mydctt1d->danum = processnum;
	//申请索引内存
	index = (uint32*)CV_malloc0(mydctt1d->danum * sizeof(uint32));
	//申请失败
	cv_log_explain(index == NULL, CV_LOG_Mem0, "一维fft渲染器编码内存申请失败");
	if (index == NULL)
	{
		CV_free0(mydctt1d);
		return NULL;
	}
	// 计算索引向量
	for (int i = 0; i < mydctt1d->danum; i++)
	{
		index[i] = i * data_step;
	}
	mydctt1d->index = index;

	//申请DCT基向量内存
	wrp = (float32*)CV_malloc0(mydctt1d->danum * sizeof(float32));
	wip = (float32*)CV_malloc0(mydctt1d->danum * sizeof(float32));
	//申请失败
	cv_log_explain((wrp == NULL || wip == NULL), CV_LOG_Mem0, "一维dct基向量内存申请失败");
	if (wrp == NULL || wip == NULL)
	{
		CV_free0(wrp); CV_free0(wip);
		CV_free0(mydctt1d);
		return NULL;
	}
	//计算alpha参数
	mydctt1d->alpha0 = YMCV_Sqrt(1.0 / mydctt1d->danum);
	mydctt1d->alpha1 = YMCV_Sqrt(2.0 / mydctt1d->danum);

	// 计算基频向量
	for (int i = 0; i < mydctt1d->danum; i++)
	{
		//令u=1
		radp = (i + 0.5f) * YMCV_Pai / mydctt1d->danum;//旋转角度
		wrp[i] = YMCV_Cos(radp);
		wip[i] = YMCV_Sin(radp);
	}

	//记录基内容
	mydctt1d->wcvr = wrp;
	mydctt1d->wcvi = wip;

	return mydctt1d;
}


/**
  * @brief 以一维fft为基础 二维fft变换渲染器
  */
CVFFT2D_RENDER YMCV_Creat_FFT2D_Render_Creat(int16 width, int16 height)
{
	CVFFT2D_RENDER render;

	render = (CVFFT2D_RENDER)CV_malloc0(sizeof(CVfft2d_render));
	//申请失败
	cv_assert(render);
	cv_log_explain(render == NULL, CV_LOG_Mem0, "二维fft渲染器头内存申请失败");
	if (render == NULL)//内存不足
	{
		return NULL;
	}
	//跨度为1，不开启幅频转换
	render->fftw = YMCV_Creat_FFT_Render_Creat(width, 2, 0);//横向fft 数据间隔为2个float  不转为模长夹角
	render->ffth = YMCV_Creat_FFT_Render_Creat(height, 2 * render->fftw->unum, 0);//纵向fft，数据间隔为 2 * width

	//申请失败
	cv_log_explain((render->fftw == NULL || render->ffth == NULL), CV_LOG_Mem0, "二维fft渲染器内存申请失败");
	if (render->fftw == NULL || render->ffth == NULL)//内存不足
	{
		YMCV_Free_FFT_RenderFree(render->fftw);
		YMCV_Free_FFT_RenderFree(render->ffth);
		CV_free0(render);
		return NULL;
	}
	//更新宽高
	render->width = render->fftw->unum;
	render->height = render->ffth->unum;

	return render;
}


/**
  * @brief 以一维dct为基础 二维dct变换渲染器
  */
CVDCT2D_RENDER YMCV_Creat_DCT2D_Render_Creat(int16 width, int16 height)
{
	CVDCT2D_RENDER render;

	render = (CVDCT2D_RENDER)CV_malloc0(sizeof(CVdct2d_render));
	//申请失败
	cv_assert(render);
	cv_log_explain(render == NULL, CV_LOG_Mem0, "二维fft渲染器头内存申请失败");
	if (render == NULL)//内存不足
	{
		return NULL;
	}
	//跨度为1，不开启幅频转换
	render->dctw = YMCV_Creat_DCT_Render_Creat(width, 1);//横向dct 数据间隔为1个float 
	render->dcth = YMCV_Creat_DCT_Render_Creat(height, render->dctw->danum);//纵向fft，数据间隔为 width

	//申请失败
	cv_log_explain((render->dctw == NULL || render->dcth == NULL), CV_LOG_Mem0, "二维dct渲染器内存申请失败");
	if (render->dctw == NULL || render->dcth == NULL)//内存不足
	{
		YMCV_Free_DCT_RenderFree(render->dctw);
		YMCV_Free_DCT_RenderFree(render->dcth);
		CV_free0(render);
		return NULL;
	}
	//更新宽高
	render->width = render->dctw->danum;
	render->height = render->dcth->danum;

	return render;
}


/**
  * @brief 以一维dwt为基础 二维dwt变换渲染器
  */
CVDWT2D_RENDER YMCV_Creat_DWT2D_Render_Creat(int16 width, int16 height)
{
	CVDWT2D_RENDER render;

	render = (CVDWT2D_RENDER)CV_malloc0(sizeof(CVdwt2d_render));
	//申请失败
	cv_assert(render);
	cv_log_explain(render == NULL, CV_LOG_Mem0, "二维dwt渲染器头内存申请失败");
	if (render == NULL)//内存不足
	{
		return NULL;
	}
	//跨度为1，默认1级变换
	render->dwtw = YMCV_Creat_DWT_Render_Creat(width, 1, 1);
	render->dwth = YMCV_Creat_DWT_Render_Creat(height, render->dwtw->Lnum,1 );
	//申请失败
	cv_log_explain((render->dwtw == NULL || render->dwth == NULL), CV_LOG_Mem0, "二维fft渲染器内存申请失败");
	if (render->dwtw == NULL || render->dwth == NULL)	//内存不足
	{
		YMCV_Free_DWT_RenderFree(render->dwtw);
		YMCV_Free_DWT_RenderFree(render->dwth);
		CV_free0(render);
		return NULL;
	}
	//更新宽高
	render->width = render->dwtw->Lnum;
	render->height = render->dwth->Lnum;

	return render;
}

/**
  * @brief 创建频域滤波渲染器
  */
CVFREQ_RENDER YMCV_Creat_FreqKernel_Render_Creat(CVFREQTYPE kernlType, CVCOMPLEXTYPE ComplexType)
{
	CVFREQ_RENDER render;

	render = (CVFREQ_RENDER)CV_malloc0(sizeof(CVfreq_render));
	//申请失败
	cv_assert(render);
	cv_log_explain(render == NULL, CV_LOG_Mem0, "频域滤波渲染器头内存申请失败");
	if (render == NULL)//内存不足
	{
		return NULL;
	}

	switch (kernlType)
	{
	case CV_GaussHFilter: //高斯高通滤波
	case CV_GaussLFilter: //高斯低通滤波
	case CV_ButterWorthHFilter:  //巴特沃斯高通滤波
	case CV_ButterWorthLFilter:  //巴特沃斯低通滤波
	case CV_BandPassFilter: //带通滤波
	case CV_BandStopFilter: //带阻滤波
	case CV_HarrThreshFilter: //哈尔小波硬阈值滤波
	{
		//设置通用参数
		render->KernelType = kernlType;
		render->ComplexType = ComplexType;
		break;
	}
	default:
	{
		cv_log_explain(kernlType > 0, CV_LOG_ParamI, "暂不支持该渲染方式");
		break;
	}
	}
	return render;
}

#endif // YMCV_V2_0


#if YMCV_BRENDER_USE
//动态内存申请
void* YMCV_Binary_VectBuff_Malloc(size_t size)
{
	return(void*)CV_malloc0(size);
}

/**
  * @brief 图像二值化渲染器创建
  */
CVBINARY_RENDER YMCV_Creat_BinaryRender_Creat(CVThreshold rendform)
{
	CVBINARY_RENDER myrend = NULL;

	myrend = (CVBINARY_RENDER)CV_malloc0(sizeof(CVbinary_render));

	//内存指针判断
	cv_assert(myrend);
	cv_log_explain(myrend == NULL, CV_LOG_Mem0, "通道渲染器头内存申请失败");

	myrend->myVector.single.PixMap = NULL;
	myrend->myVector.three.PixMap1 = NULL;
	myrend->myVector.three.PixMap2 = NULL;
	myrend->myVector.three.PixMap3 = NULL;


	switch (rendform)
	{
	case CV_SingleThreshold: //单阈值
	case CV_DoubleThreshold: //双阈值
	case CV_IntegralThreshold://积分图二值化
	case CV_HistThreshold://Otsu二值化
	{
		myrend->ThreshType = rendform;
		myrend->myVector.single.PixMap = CV_malloc0(256 * sizeof(uint8));
		break;
	}
	case CV_RGB2Threshold: //RGB双阈值
	case CV_LAB2Threshold: //LAB双阈值
	{
		myrend->ThreshType = rendform;
		myrend->myVector.three.PixMap1 = CV_malloc0(256 * sizeof(uint8));
		myrend->myVector.three.PixMap2 = CV_malloc0(256 * sizeof(uint8));
		myrend->myVector.three.PixMap3 = CV_malloc0(256 * sizeof(uint8));
		break;
	}
	default:
		cv_log_explain(rendform > 0, CV_LOG_ParamI, "暂不支持该渲染方式");
		break;
	}
	return myrend;
}

#endif // YMCV_BRENDER_USE

#if YMCV_FEATURE_USE

/**
  * @brief 图像色块二值化渲染器创建
  */
CVBLOB_SEG_RENDER YMCV_Creat_BlobSegRender_Creat(CVThreshold rendform)
{
	CVBLOB_SEG_RENDER myrend = NULL;

	myrend = (CVBLOB_SEG_RENDER)CV_malloc0(sizeof(CVblob_seg_render));

	//内存指针判断
	cv_assert(myrend);
	cv_log_explain(myrend == NULL, CV_LOG_Mem0, "通道渲染器头内存申请失败");

	myrend->myVector.single.PixMap = NULL;
	myrend->myVector.three.PixMap1 = NULL;
	myrend->myVector.three.PixMap2 = NULL;
	myrend->myVector.three.PixMap3 = NULL;


	switch (rendform)
	{
	case CV_SingleThreshold: //单阈值
	case CV_DoubleThreshold: //双阈值
	{
		myrend->ThreshType = rendform;
		myrend->myVector.single.PixMap = CV_malloc0(256 * sizeof(uint8));
		break;
	}
	case CV_RGB2Threshold: //RGB双阈值
	case CV_LAB2Threshold: //LAB双阈值
	{
		myrend->ThreshType = rendform;
		myrend->myVector.three.PixMap1 = CV_malloc0(256 * sizeof(uint8));
		myrend->myVector.three.PixMap2 = CV_malloc0(256 * sizeof(uint8));
		myrend->myVector.three.PixMap3 = CV_malloc0(256 * sizeof(uint8));
		break;
	}
	default:
		cv_log_explain(rendform > 0, CV_LOG_ParamI, "暂不支持该渲染方式");
		break;
	}
	return myrend;
}

/**
  * @brief 链码创建
  */
CVCHAINS_U16 YMCV_Creat_Chains_U16(uint32 ChainsNum)
{
	CVCHAINS_U16 mychain;

	mychain = (CVCHAINS_U16)CV_malloc0(sizeof(CVchains_u16));
	//申请失败
	cv_assert(mychain);
	cv_log_explain(mychain == NULL, CV_LOG_Mem0, "链码头内存申请失败");
	if (mychain == NULL)//内存不足
	{
		return NULL;
	}
	mychain->chain = (CVVECTORS_U16*)CV_malloc1(ChainsNum * sizeof(CVVECTORS_U16));
	cv_log_explain(mychain->chain == NULL, CV_LOG_Mem1, "链码数组内存申请失败");
	//初始化为 0
	for (int i = 0; i < ChainsNum; i++)
	{
		mychain->chain[i] = NULL;
	}

	mychain->cNum = ChainsNum;
	return mychain;
}

/**
  * @brief 直方图创建
  */
CV_FACE_LBP_DESC YMCV_Creat_FaceLbp_Hist()
{
	CV_FACE_LBP_DESC mylbphis = NULL;

	mylbphis = (CV_FACE_LBP_DESC)CV_malloc1(sizeof(CV_face_lbp_desc));//直方图创建

	//内存指针判断
	cv_assert(mylbphis);
	cv_log_explain(mylbphis == NULL, CV_LOG_Mem1, "人脸LBP等价直方图创建失败");
	//初始化为0
	CV_memset(mylbphis, 0, sizeof(CV_face_lbp_desc));
	return mylbphis;
}
#endif // YMCV_FEATURE_USE

//其余数据结构
#if YMCV_OTHERDA_USE
CVGMMS YMCV_Creat_Gmms_Creat(uint8 dataDim, uint8 componentsNum)
{
	CVGMMS mygmms;
	mygmms = (CVIMAGE)CV_malloc0(sizeof(CVgmms));//gmm头

	//内存指针判断
	cv_assert(mygmms);
	cv_log_explain(mygmms == NULL, CV_LOG_Mem0, "GMM对象头内存申请失败");
	mygmms->componentsNum = componentsNum;
	mygmms->dataDim = dataDim;

	mygmms->coefs = (float32*)CV_malloc1(componentsNum * sizeof(float32));//选中概率
	mygmms->mean = (float32*)CV_malloc1(componentsNum * dataDim * sizeof(float32));//各维度均值
	mygmms->icov = (float32*)CV_malloc1(componentsNum * dataDim * dataDim * sizeof(float32));//逆协方差矩阵
	mygmms->covdet = (float32*)CV_malloc1(componentsNum * sizeof(float32));//行列式

	return mygmms;
}

//目标跟踪器创建
CV_TARGET_TRACK YMCV_Creat_TargetTrack_Creat(CVIMAGE image, CVrect* myroi)
{
	int rech, recw, recx, recy;
	uint16 histnum = 256;//直方图个数

	cv_log_explain((image == NULL) || (myroi == NULL), CV_LOG_PtrI, "输入图像或ROI不存在");
	rech = myroi->h;
	recw = myroi->w;
	recx = myroi->x;
	recy = myroi->y;
	cv_log_explain(((recx + recw) > image->width) || ((recy + rech) > image->height), CV_LOG_ParamI, "输入的ROI在图像上越界");

	CV_TARGET_TRACK mytarget = CV_malloc0(sizeof(CV_target_track));
	mytarget->histnum = histnum;
	mytarget->hist = CV_malloc1(histnum * sizeof(float32));
	mytarget->Kdis = CV_malloc1(recw * rech * sizeof(float32));
	mytarget->roi = (*myroi);
	float32* hist1 = mytarget->hist;
	float32* m_wei = mytarget->Kdis;

	float32 recw_2 = recw / 2.0f;
	float32 rech_2 = rech / 2.0f;
	float32 h = recw_2 * recw_2 + rech_2 * rech_2;			//带宽

	//初始化权值矩阵和目标直方图
	for (int i = 0, imax = recw * rech; i < imax; i++)
		m_wei[i] = 0.0f;

	for (int i = 0; i < histnum; i++)
		hist1[i] = 0.0f;

	//计算权重
	float32 wsum = 0.0f;
	{
		//加速预先计算
		float32* jj2 = CV_malloc0(recw * sizeof(float32));
		for (int j = 0; j < recw; j++)
		{
			float32 j2 = j - recw_2;
			jj2[j] = j2 * j2;
		}

		//权重是（距离的函数）
		for (int i = 0; i < rech; i++)
		{
			float32 i2 = i - rech_2;
			i2 *= i2;//i*i
			float32* weiLine = &m_wei[i * recw];
			for (int j = 0; j < recw; j++)
			{
				float32 dist2 = i2 + jj2[j];//i*i+j*j
				weiLine[j] = 1.0f - (dist2 / h); //Epannechnikov函数 K(dis)= 1 - dis^2
				wsum += weiLine[j];//用于计算参数 C =1/sum{ K(dis)}
			}
		}
		float32 Csum = 1.0f / wsum;//参数C
		//归一化权重
		for (int i = 0, imax = rech * recw; i < imax; i++)
		{
			m_wei[i] *= Csum;
		}
		//释放内存
		CV_free0(jj2);
	}

	//计算目标权值直方
	for (int i = 0; i < rech; i++)
	{
		float32* weiLine = &m_wei[i * recw];
		uint8* grayLine = &image->data[(recy + i) * image->width + recx];//改行起始位置
		for (int j = 0; j < recw; j++)
		{
			uint8 gvl = grayLine[j];//获取直方图的位置
			//对应直方图位置加权重*1
			hist1[gvl] += weiLine[j];
		}
	}
	return mytarget;
}


//创建粒子跟踪器
CV_PARTICLE_TRACK YMCV_Creat_ParticleTrack_Creat(CVIMAGE image, CVrect* myroi, uint16 particleNum, float32 velocity_delta, float32 scale_delta)
{
	cv_log_explain((image == NULL) || (myroi == NULL), CV_LOG_PtrI, "输入图像或ROI不存在");
	cv_log_explain(((myroi->x + myroi->w) > image->width) || ((myroi->y + myroi->h) > image->height), CV_LOG_ParamI, "输入的ROI在图像上越界");

	uint16 histnum = 256;//直方图个数
	int xmax = myroi->x + myroi->w;
	int ymax = myroi->y + myroi->h;
	CV_PARTICLE_TRACK mytrack = CV_malloc0(sizeof(CV_particle_track));
	mytrack->histNum = histnum;
	mytrack->particleNum = particleNum;
	mytrack->scale_delta = scale_delta;//尺度变换速度扰动幅度
	mytrack->velocity_delta = velocity_delta;//速度扰动幅值
	mytrack->hists = (float32*)CV_malloc1(histnum * sizeof(float32));//直方图
	mytrack->states = (CV_particle_state*)CV_malloc1(particleNum * sizeof(CV_particle_state));//粒子状态
	mytrack->weights = (float32*)CV_malloc1(particleNum * sizeof(float32));//粒子权重
	CV_particle_state* mystates = mytrack->states;
	float32* myhists = mytrack->hists;
	float32* myweights = mytrack->weights;
	//初始化
	for (int i = 0; i < histnum; i++)
		myhists[i] = 0.0f;

	int halfw = myroi->w / 2;
	int halfh = myroi->h / 2;
	int centx = myroi->x + halfw;
	int centy = myroi->y + halfh;
	float divaa2 = 1.0f / (halfw * halfw + halfh * halfh); //计算核函数半径平方a^2

	//加速预先计算
	int* xx2 = CV_malloc0(xmax * sizeof(int));
	for (uint16 x = myroi->x; x < xmax; x++)
	{
		int x2 = x - centx;
		xx2[x] = x2 * x2;
	}
	//计算直方图
	for (uint16 y = myroi->y; y < ymax; y++)
	{
		int yy2 = y - centy;
		yy2 *= yy2;// y^2

		uint8* gline = &image->data[y * image->width];
		for (uint16 x = myroi->x; x < xmax; x++)
		{
			uint8 grayv = gline[x];
			float r2 = (yy2 + xx2[x]) * divaa2;//计算半径平方r^2
			float kr = (r2 <1.0f)? (1.0f - r2) : 0.0f; //核函数k(r) = 1-r^2, |r| < 1; 其他值 k(r) = 0 
			myhists[grayv] += kr;//计算核密度加权直方图
		}
	}

	CV_free0(xx2);//释放内存

	//直方图归一化
	float ftol = 0.0f;
	for (int i = 0; i < histnum; i++)
		ftol += myhists[i];

	if (ftol != 0.0f)
	{
		ftol = 1.0f / ftol;//总数

		for (int i = 0; i < histnum; i++)
		{
			myhists[i] *= ftol;//归一化
		}
	}


	// 初始化粒子状态(以(x0,y0,1,1,Wx,Hy,0.1)为中心呈N(0,0.4)正态分布)
	mystates[0].x = centx;
	mystates[0].y = centy;
	mystates[0].Hx = halfw;
	mystates[0].Hy = halfh;
	mystates[0].vx = 0.0f;
	mystates[0].vy = 0.0f;
	mystates[0].vs = 0.0f;
	float divwei = 1.0f / particleNum;
	myweights[0] = divwei; //权重相等

	float rng5[5];
	for (int i = 1; i < particleNum; i++)
	{
		for (int j = 0; j < 5; j++)
			rng5[j] = YMCV_RandGaussian(0.0f, 0.001f); /* 产生5个随机高斯分布的数 */
		//进行初始化
		mystates[i].x = centx + rng5[0] * halfw;
		mystates[i].y = centy + rng5[1] * halfh;
		mystates[i].Hx = halfw;
		mystates[i].Hy = halfh;
		mystates[i].vx = 0.0f + rng5[2] * velocity_delta;
		mystates[i].vy = 0.0f + rng5[3] * velocity_delta;
		mystates[i].vs = 0.0f + rng5[4] * scale_delta;
		myweights[i] = divwei; //权重统一为1/N，让每个粒子有相等的机会
	}

	return mytrack;
}
#endif // YMCV_OTHERDA_USE

