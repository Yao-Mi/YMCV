#ifndef YMCV_FASTMATH_H
#define YMCV_FASTMATH_H
#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_PubType.h"
#include "../OPOBJ/YMCV_OBJ.h"

#define YMCV_FMATH_FREQ_USE 1 //开启频域运算
#define YMCV_FMATH_FMAT_USE 1 //开启矩阵运算

#if YMCV_FMATH_FREQ_USE

typedef struct
{
	uint16 danum;//处理长度
	uint16 Lnum;//蝶形变换层数
	uint16 unum;
	uint8 ToAF;//ToAF 为1则输出 realout幅度（模长），imagout为夹角（相位） 
	uint32 dt_step;
	uint32 * packindex; //数据预处理重排
	float32* wcvr;//旋转向量表 实部
	float32* wcvi;//旋转向量表 虚部
}CVfft_render;
typedef CVfft_render* CVFFT_RENDER;

typedef struct
{
	uint16 danum;//处理长度 
	uint32 dt_step;
	float32 alpha0, alpha1;
	int32* index;//数据索引
	float32* wcvr;//旋转向量表 实部
	float32* wcvi;//旋转向量表 虚部
}CVdct_render;
typedef CVdct_render* CVDCT_RENDER;

typedef struct
{
	uint16 danum;//数据长度
	uint16 Lnum;//有效长度
	uint16 unum;//n级小波变换
	uint32 dt_step;//数据偏移长度
}CVdwt_render;
typedef CVdwt_render* CVDWT_RENDER;


void YMCV_FFT_Data_Cal(CVFFT_RENDER render, float32 realin[], float32 imagin[], float32 realout[], float32 imagout[]);
void YMCV_FFTShift_Data_Cal(CVFFT_RENDER render, float32 realin[], float32 imagin[], float32 realout[], float32 imagout[]);
void YMCV_IFFT_Data_Cal(CVFFT_RENDER render, float32 realin[], float32 imagin[], float32 realout[], float32 imagout[]);

void YMCV_DCT_Data_Cal(CVDCT_RENDER render, float32 datain[], float32 Calbuff1[], float32 dataout[], float32 Calbuff2[]);
void YMCV_IDCT_Data_Cal(CVDCT_RENDER render, float32 datain[], float32 Calbuff1[], float32 dataout[], float32 Calbuff2[]);

void YMCV_DWT_Data_Cal(CVDWT_RENDER render, float32 datain[], float32 dataout[]);
void YMCV_IDWT_Data_Cal(CVDWT_RENDER render, float32 datain[], float32 dataout[]);

#endif // YMCV_FMATH_FREQ_USE

#if YMCV_FMATH_FMAT_USE

CVFMAT YMCV_FMat_Mult_Cal(CVFMAT mat2, CVFMAT mat1);//矩阵乘法 mat2 * mat1
CVFMAT YMCV_FMat_Trans_Cal(CVFMAT mat1);//转置
float32 YMCV_FMat_Det_Cal(CVFMAT mat1);//行列式计算
CVFMAT YMCV_FMat_Del_RowAndCol_Cal(CVFMAT mat1, uint16 row, uint16 col);//除了某行某列的矩阵
CVFMAT YMCV_FMat_Inverse_Cal(CVFMAT mat1);//伴随矩阵法求逆矩阵
CVFMAT YMCV_FMat_DetAndInverse_Cal(CVFMAT mat1, float32* detval);//同时计算逆矩阵和行列式
void YMCV_FMat_InitTo_OneUnitMat_Cal(CVFMAT mat1);//初始化为单位矩阵

CVFMAT YMCV_FMat_ATxA_Cal(CVFMAT matA); //计算 A' * A 
CVFMAT YMCV_FMat_AxAT_Cal(CVFMAT matA); //计算 A  * A' 
CVFMAT YMCV_FMat_ATxB_Cal(CVFMAT matA, CVFMAT matB);//计算 A' * B
CVFMAT YMCV_FMat_Cholesky_Cal(CVFMAT matA);//计算Cholesky分解得到 L 下三角矩阵 ，其中 A = L * L'
void YMCV_FMat_PrimaryColumn_Cal(CVFMAT mat1, int16* PrimaryFlagArray, uint16 arrayLen);//计算各列的主元行号，某列无主元则为-1
CVFMAT YMCV_FMat_RowOperation_Inverse_Cal(CVFMAT mat1);//初等变换求逆矩阵
CVFMAT YMCV_FMat_Pseudo_Inverse_Cal(CVFMAT matA);//最小二乘求解伪逆矩阵
CVFMAT YMCV_FMat_AugMat_RowSimplified_X_Cal(CVFMAT mat1);//增广矩阵行化简
CVFMAT YMCV_FMat_Jacobi_Eigen_ValAndVec_Cal(CVFMAT mat1, float32* eigenValArray, uint8 arrayLen);//雅可比法求特征值和特征向量
void YMCV_FMat_SVD_USVT_Cal(CVFMAT matA_S, CVFMAT matU, CVFMAT matVT);//矩阵SVD分解 A=USV^T

#endif // YMCV_FMATH_FMAT_USE



#endif // !YMCV_FASTMATH_H

