#ifndef YMCV_FASTMATH_H
#define YMCV_FASTMATH_H
#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_PubType.h"
#include "../OPOBJ/YMCV_OBJ.h"

#define YMCV_FMATH_FREQ_USE 1 //����Ƶ������
#define YMCV_FMATH_FMAT_USE 1 //������������

#if YMCV_FMATH_FREQ_USE

typedef struct
{
	uint16 danum;//������
	uint16 Lnum;//���α任����
	uint16 unum;
	uint8 ToAF;//ToAF Ϊ1����� realout���ȣ�ģ������imagoutΪ�нǣ���λ�� 
	uint32 dt_step;
	uint32 * packindex; //����Ԥ��������
	float32* wcvr;//��ת������ ʵ��
	float32* wcvi;//��ת������ �鲿
}CVfft_render;
typedef CVfft_render* CVFFT_RENDER;

typedef struct
{
	uint16 danum;//������ 
	uint32 dt_step;
	float32 alpha0, alpha1;
	int32* index;//��������
	float32* wcvr;//��ת������ ʵ��
	float32* wcvi;//��ת������ �鲿
}CVdct_render;
typedef CVdct_render* CVDCT_RENDER;

typedef struct
{
	uint16 danum;//���ݳ���
	uint16 Lnum;//��Ч����
	uint16 unum;//n��С���任
	uint32 dt_step;//����ƫ�Ƴ���
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

CVFMAT YMCV_FMat_Mult_Cal(CVFMAT mat2, CVFMAT mat1);//����˷� mat2 * mat1
CVFMAT YMCV_FMat_Trans_Cal(CVFMAT mat1);//ת��
float32 YMCV_FMat_Det_Cal(CVFMAT mat1);//����ʽ����
CVFMAT YMCV_FMat_Del_RowAndCol_Cal(CVFMAT mat1, uint16 row, uint16 col);//����ĳ��ĳ�еľ���
CVFMAT YMCV_FMat_Inverse_Cal(CVFMAT mat1);//��������������
CVFMAT YMCV_FMat_DetAndInverse_Cal(CVFMAT mat1, float32* detval);//ͬʱ��������������ʽ
void YMCV_FMat_InitTo_OneUnitMat_Cal(CVFMAT mat1);//��ʼ��Ϊ��λ����

CVFMAT YMCV_FMat_ATxA_Cal(CVFMAT matA); //���� A' * A 
CVFMAT YMCV_FMat_AxAT_Cal(CVFMAT matA); //���� A  * A' 
CVFMAT YMCV_FMat_ATxB_Cal(CVFMAT matA, CVFMAT matB);//���� A' * B
CVFMAT YMCV_FMat_Cholesky_Cal(CVFMAT matA);//����Cholesky�ֽ�õ� L �����Ǿ��� ������ A = L * L'
void YMCV_FMat_PrimaryColumn_Cal(CVFMAT mat1, int16* PrimaryFlagArray, uint16 arrayLen);//������е���Ԫ�кţ�ĳ������Ԫ��Ϊ-1
CVFMAT YMCV_FMat_RowOperation_Inverse_Cal(CVFMAT mat1);//���ȱ任�������
CVFMAT YMCV_FMat_Pseudo_Inverse_Cal(CVFMAT matA);//��С�������α�����
CVFMAT YMCV_FMat_AugMat_RowSimplified_X_Cal(CVFMAT mat1);//��������л���
CVFMAT YMCV_FMat_Jacobi_Eigen_ValAndVec_Cal(CVFMAT mat1, float32* eigenValArray, uint8 arrayLen);//�ſɱȷ�������ֵ����������
void YMCV_FMat_SVD_USVT_Cal(CVFMAT matA_S, CVFMAT matU, CVFMAT matVT);//����SVD�ֽ� A=USV^T

#endif // YMCV_FMATH_FMAT_USE



#endif // !YMCV_FASTMATH_H

