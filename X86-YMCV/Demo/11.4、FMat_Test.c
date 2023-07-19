#include "../EGE_LCD/EGE_LCD.h"
#include"../YMCV/CORE/YMCV_PubData.h"
#include"../YMCV/CORE/YMCV_FastMath.h"

#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"


void print_Mat(CVFMAT mymat,const uint8* tips)
{
    printf(tips);
    printf("  size(%d��%d)\n", mymat->row, mymat->col);
    for (int i = 0; i < mymat->row; i++)
    {
        for (int j = 0; j < mymat->col; j++)
        {
            printf("%10.3f  ", mymat->datap[i * mymat->col + j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main()
{
    LCD_Init(300, 300);
    float a[5][4] = { {1, 1, 1, 1},
                      {1, 2, 3, 4},
                      {2, 5, 6,8},
                      {8, 3, 6,10},
                      {1, 4,9,20} };


    CVFMAT mat0 = YMCV_Creat_FMAT_Creat(5, 4);
    //��ʼ��
    for (int i = 0; i < mat0->row; i++)
    {
        for (int j = 0; j < mat0->col; j++)
        {
            mat0->datap[i * 4 + j] = a[i][j];
        }
    }
    print_Mat(mat0, "��ʼ������");
    PAUSE();
    /*-------------------------- FMat ����������� --------------------------------------------------*/

    //����ת��
    CVFMAT mat0_T = YMCV_FMat_Trans_Cal(mat0);
    print_Mat(mat0_T, "ת�þ���");
    PAUSE();

    //����˷�
    CVFMAT mat0_MTM = YMCV_FMat_Mult_Cal(mat0_T, mat0);//m = m0^T * m0
    print_Mat(mat0_MTM, "����˷���M0^T * M0");
    PAUSE();

    //����ʽ����
    float m0det=YMCV_FMat_Det_Cal(mat0_MTM);
    printf("����ʽdetֵ��%f\n\n", m0det);
    PAUSE();

    //�����������
    CVFMAT mat0_InV0 =YMCV_FMat_Inverse_Cal(mat0_MTM);//m^-1
    print_Mat(mat0_InV0, "��������������");
    PAUSE();

    //����˷���֤
    CVFMAT mat0_m0 = YMCV_FMat_Mult_Cal(mat0_InV0, mat0_MTM);//m^-1 * m
    print_Mat(mat0_m0, "�������֤��M0^-1 * M");
    PAUSE();

    //�����б任����
    CVFMAT mat0_InV1 = YMCV_FMat_RowOperation_Inverse_Cal(mat0_MTM);
    print_Mat(mat0_InV1, "��������б任����");
    PAUSE();

    //����˷���֤
    CVFMAT mat0_m1 = YMCV_FMat_Mult_Cal(mat0_InV1, mat0_MTM);//m^-1 * m
    print_Mat(mat0_m1, "�������֤��M0^-1 * M");
    PAUSE();

    //��С���˷���α��
    CVFMAT mat0_Pinv= YMCV_FMat_Pseudo_Inverse_Cal(mat0);
    print_Mat(mat0_Pinv, "α�����(M0^T * M0)^-1  * M0^T");
    PAUSE();

    //����˷���֤
    CVFMAT mat0_m1m0T = YMCV_FMat_Mult_Cal(mat0_InV1, mat0_T);//m^-1 * m0^T
    print_Mat(mat0_m1m0T, "α�������֤��M^-1  * M0^T");
    PAUSE();

    //�ſɱȷ�����������������ֵ
    float32 eigenVal[4];
    CVFMAT mat0_evec = YMCV_FMat_Jacobi_Eigen_ValAndVec_Cal(mat0_MTM, eigenVal,4);
    printf("����ֵ�� ");
    for (int i = 0; i < 4; i++)
    {
        printf("%10.4f  ", eigenVal[i]);
    }
    print_Mat(mat0_evec, "\n��������");
    PAUSE();

    //Cholesky �ֽ�
    CVFMAT mat0_L = YMCV_FMat_Cholesky_Cal(mat0_MTM);
    print_Mat(mat0_L, "Cholesky�ֽ� �����Ǿ���");
    PAUSE();

    //L*L^-1
    CVFMAT  mat0_LLT = YMCV_FMat_AxAT_Cal(mat0_L);
    print_Mat(mat0_LLT, "Cholesky�ֽ� ��֤L*L^T");
    PAUSE();

    //�����Գ˷�
    CVFMAT  mat0_MT_M = YMCV_FMat_ATxA_Cal(mat0);//m0^T * m0
    print_Mat(mat0_MT_M, "�����Գˣ�M^T * M");
    PAUSE();

    CVFMAT mat0_MT_M2 = YMCV_FMat_ATxB_Cal(mat0, mat0);//m0^T * m0
    print_Mat(mat0_MT_M2, "����˷���M^T * M");
    PAUSE();

    //�����Գ˷�
    CVFMAT  mat0_M_MT = YMCV_FMat_AxAT_Cal(mat0);//m0 * m0^T
    print_Mat(mat0_M_MT, "�����Գˣ�M * M^T");
    PAUSE();

    //��������л���
    float zg[] = { 0,3,-6,6,4,-5,
                    3,-7,8,-5,8,9,
                    3,-9,12,-9,6,15 };
   
    CVFMAT  mat0_AuM = YMCV_Creat_FMAT_Creat(3, 6); //�����������
    for (int i = 0; i < 3*6; i++)
    {
        mat0_AuM->datap[i] = zg[i];//��������
    }
    CVFMAT  mat0_AuMrs = YMCV_FMat_AugMat_RowSimplified_X_Cal(mat0_AuM);
    print_Mat(mat0_AuMrs, "��������л���");
    PAUSE();

    //���ȱ任��Ԫ����ȡ
    int16 pr_id[6];
    YMCV_FMat_PrimaryColumn_Cal(mat0_AuM, pr_id,6);//��Ԫ����ȡ
    printf("(ע�������Ԫ�У����в�������Ԫ��Ϊ-1)�� ");
    for (int i = 0; i < 6; i++)
    {
        printf("%5d  ", pr_id[i]);
    }
    printf("\n\n ");
    PAUSE();

    //��ʼ��Ϊ��λ����
    YMCV_FMat_InitTo_OneUnitMat_Cal(mat0_MTM);
    print_Mat(mat0_MTM, "�����ʼ��Ϊ��λ����");
    PAUSE();

    print_Mat(mat0_T, "SVD: A����");
    CVFMAT mat0T_U = YMCV_Creat_FMAT_Creat(mat0_T->row, mat0_T->row);
    CVFMAT mat0T_VT = YMCV_Creat_FMAT_Creat(mat0_T->col, mat0_T->col);
    YMCV_FMat_SVD_USVT_Cal(mat0_T, mat0T_U, mat0T_VT);
    print_Mat(mat0T_U, "U����");
    print_Mat(mat0_T, "S����");
    print_Mat(mat0T_VT, "V^T����");

    //�ͷž����ڴ�
    YMCV_Free_FMatFree(mat0_MT_M2);
    YMCV_Free_FMatFree(mat0_T);
    YMCV_Free_FMatFree(mat0T_U);
    YMCV_Free_FMatFree(mat0T_VT);
    YMCV_Free_FMatFree(mat0_m0);
    YMCV_Free_FMatFree(mat0_InV0);
    YMCV_Free_FMatFree(mat0_InV1);
    YMCV_Free_FMatFree(mat0_m1);
    YMCV_Free_FMatFree(mat0_evec);
    YMCV_Free_FMatFree(mat0_LLT);
    YMCV_Free_FMatFree(mat0_L);
    YMCV_Free_FMatFree(mat0);
    YMCV_Free_FMatFree(mat0_M_MT);
    YMCV_Free_FMatFree(mat0_AuMrs);
    YMCV_Free_FMatFree(mat0_AuM);
    YMCV_Free_FMatFree(mat0_MTM);
}