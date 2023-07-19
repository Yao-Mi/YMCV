#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_SpaceCal.h"
#include "../YMCV/CORE/YMCV_FeatCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� ���ʶ��Ϊ����չʾ�� �������ȡЧ��
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1200, 700);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(tablerecg_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, tablerecg_gray, sizeof(tablerecg_gray));

    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 509, 668, CVBinaryType);//��ֵͼ0
    //��ʾԭͼ
    LCD_show_image(0, 0, myImage0);


    uint8* myData2 = CV_malloc1(sizeof(tablerecg_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData2, tablerecg_gray, sizeof(tablerecg_gray));
    for (int i = 0; i < sizeof(tablerecg_gray); i++)
    {
        myData2[i] = (myData[i] > 220) ? 0 : 255;//���ٶ�ֵ��
    }
    CVIMAGE myImageb = YMCV_Creat_DataToImg((uint8*)myData2, 509, 668, CVBinaryType);//��ֵͼ0
    int pstrx = 510;
    LCD_show_image(pstrx, 0, myImageb);//�������ʾ���ұ�
    PAUSE();

    ///*--------------------------��̬ѧ��ʴ�˲� ��ȡ��ֱ�߶�--------------------------------------------------*/
    //������Ӧ�뾶����̬ѧ�˲���
    uint8 kernel_d = 3;
    CVFMAT myKernelH = YMCV_Creat_FMAT_Creat(kernel_d, 1);//3x1 ��ȡ��ֱֱ��

    int morp_invpx = -YMCV_Ceil(myKernelH->col / 2.0f);//����ƫ����
    int morp_invpy = -YMCV_Ceil(myKernelH->row / 2.0f);//����ƫ����

    //�ֶ����û��к˽ṹ
    CV_memset(myKernelH->datap, 0, myKernelH->row * myKernelH->col * sizeof(float));//ȫ����ʼΪ0
    //ѡ����Ҫ���еĲ��ּ�Ϊ1
    for (int i = 0; i < myKernelH->row; i++)
    {
        for (int j = 0; j < myKernelH->col; j++)
        {
            myKernelH->datap[i * myKernelH->col + j] = 1.0f;
        }
    }

    //������̬ѧ���н��
    CVIMAGE myImage1 = YMCV_Binary_Morph_Hit_Cal(myImageb, myKernelH, 99);//���������>99%�Ĳ��� ����ʴЧ��
    //��ʾͼƬ1
    LCD_show_image(pstrx, 0, myImage1);

    //��Ϣ��ʾ
    sprintf(myInform, "��ʴ�˲� ");
    LCD_ShowString(pstrx, 0, myInform);
    //�ͷſռ���Ⱦ��
    YMCV_Free_FMatFree(myKernelH);
    //��ͣ
    PAUSE();
    /*--------------------------�Ǽ�ϸ������--------------------------------------------------*/
    YMCV_Image_Binary_Bone_Thin(myImage1);

    LCD_show_image(pstrx, 0, myImage1);//�������ʾ���ұ�
    //��Ϣ��ʾ
    sprintf(myInform, "�Ǽ�ϸ��");
    LCD_ShowString(pstrx, 0, myInform);
    //��ͣ
    PAUSE();
    /*------------------------ ��ֵ������� ------------------------------*/
    //�����������12������
    YMCV_Binary_AreaFilter(myImage1, 30);
    LCD_show_image(pstrx, 0, myImage1);
    //��Ϣ��ʾ
    sprintf(myInform, "�������");
    LCD_ShowString(pstrx, 0, myInform);
    PAUSE();
    /*-------------------------- ���ֱ�߼���� --------------------------------------------------*/
    CVVECTORS_U16 Hlines = YMCV_WormFindLines(myImage1, 0.001, 10, 1);
    color24_t rgbt = { .R = 255 };
    LCD_Point_Color_Set(rgbt);
    if (Hlines != NULL)
    {
        for (int i = 0; i < Hlines->dataNum; i++)
        {
            CVline* thisline = Hlines->datap + i * Hlines->dim;
            LCD_Draw_Line(morp_invpx + thisline->x1, morp_invpy + thisline->y1, morp_invpx + thisline->x2, morp_invpy + thisline->y2);//�����߶�
        }
    }
    //��Ϣ��ʾ
    sprintf(myInform, "�߶μ��");
    LCD_ShowString(pstrx, 0, myInform);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage1);
    PAUSE();

    ///*--------------------------��̬ѧ��ʴ�˲� ��ȡˮƽ�߶�--------------------------------------------------*/
    //������Ӧ�뾶����̬ѧ�˲���
    kernel_d = 3;
    CVFMAT myKernelV = YMCV_Creat_FMAT_Creat(1, kernel_d);//1x3 ��ȡˮƽֱ��

    morp_invpx = -YMCV_Ceil(myKernelV->col / 2.0f);//����ƫ����
    morp_invpy = -YMCV_Ceil(myKernelV->row / 2.0f);//����ƫ����

    //�ֶ����û��к˽ṹ
    CV_memset(myKernelV->datap, 0, myKernelV->row * myKernelV->col * sizeof(float));//ȫ����ʼΪ0
    //ѡ����Ҫ���еĲ��ּ�Ϊ1
    for (int i = 0; i < myKernelV->row; i++)
    {
        for (int j = 0; j < myKernelV->col; j++)
        {
            myKernelV->datap[i * myKernelV->col + j] = 1.0f;
        }
    }

    //������̬ѧ���н��
    myImage1 = YMCV_Binary_Morph_Hit_Cal(myImageb, myKernelV, 99);//���������>99%�Ĳ��� ����ʴЧ��
    //��ʾͼƬ1
    LCD_show_image(pstrx, 0, myImage1);

    //��Ϣ��ʾ
    sprintf(myInform, "��ʴ�˲� ");
    LCD_ShowString(pstrx, 0, myInform);
    //�ͷſռ���Ⱦ��
    YMCV_Free_FMatFree(myKernelV);
    //��ͣ
    PAUSE();
    /*--------------------------�Ǽ�ϸ������--------------------------------------------------*/
    YMCV_Image_Binary_Bone_Thin(myImage1);

    LCD_show_image(pstrx, 0, myImage1);//�������ʾ���ұ�
    //��Ϣ��ʾ
    sprintf(myInform, "�Ǽ�ϸ��");
    LCD_ShowString(pstrx, 0, myInform);
    //��ͣ
    PAUSE();
    /*------------------------ ��ֵ������� ------------------------------*/
    //�����������12������
    YMCV_Binary_AreaFilter(myImage1, 40);
    LCD_show_image(pstrx, 0, myImage1);
    //��Ϣ��ʾ
    sprintf(myInform, "�������");
    LCD_ShowString(pstrx, 0, myInform);
    PAUSE();
    /*-------------------------- ���ֱ�߼���� --------------------------------------------------*/
    CVVECTORS_U16 Vlines = YMCV_WormFindLines(myImage1, 0.001, 10, 1);
    if (Vlines != NULL)
    {
        for (int i = 0; i < Vlines->dataNum; i++)
        {
            CVline* thisline = Vlines->datap + i * Vlines->dim;
            LCD_Draw_Line(morp_invpx + thisline->x1, morp_invpy + thisline->y1, morp_invpx + thisline->x2, morp_invpy + thisline->y2);//�����߶�
        }
    }
    //��Ϣ��ʾ
    sprintf(myInform, "�߶μ��");
    LCD_ShowString(pstrx, 0, myInform);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage1);
    PAUSE();
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImageb);
    YMCV_Free_Vector_U16_Free(Vlines);
    YMCV_Free_Vector_U16_Free(Hlines);

    PAUSE();
    LCD_Destory();
    return 0;
}





