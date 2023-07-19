#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_HistCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� ˫���˲�Ϊ����չʾ�˽�һ��ͼ��Ľ���ĥƤ���յ�Ч���Ĳ�������
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 700);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(mm01_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, mm01_gray, sizeof(mm01_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 375, 500, CVGrayType);//�Ҷ�ͼ
    //��ʾԭͼ
    LCD_show_image(0, 0, myImage0);


    //�����ռ���Ⱦ��
    uint8 kernel_r = 10;
    CVKERN_RENDER myKernel = YMCV_Creat_SpaceRender_Creat(CV_BilFilter, PIXChannelNum1, kernel_r, OutPaddingConst);
    YMCV_PADDING_VAL = 125;//�������ֵ


    //������Ⱦ�����������г�ʼ��
    CV_kernel_1param myParam = { .bilateral.sigma =10.0,.bilateral.sgmgray=7.0 };//�ռ�˷�����ҶȺ˷���

    //���ڻ�����
    CVIMAGE myImage1 = NULL;
    //�ȵ��뾶��Ȼ����ռ�˷������Ч�����ٵ��ҶȺ�ʹ���������±�����
    /*--------------------------˫���˲��޸ĸ�˹����---------------------------------------------*/
    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_Space_SingleKernel_Load(myKernel, &myParam);

        //ʹ�ÿռ���Ⱦ��������Ⱦ�Ҷ�ͼƬ
        myImage1 = YMCV_Image_Space_SigleKernel_Render(myImage0, myKernel);
        //��ʾͼƬ1
        LCD_show_image(500, 0, myImage1);

        //������ͷ�ͼ�񣬱����´��õ�ʱ�����ڴ�й©
        YMCV_Free_ImgFree(myImage1);

        //��Ϣ��ʾ
        sprintf(myInform, " ˫���˲�sigma: %01.1f gary-sigma:%01.1f.", myParam.bilateral.sigma, myParam.bilateral.sgmgray);
        LCD_ShowString(100, 510, myInform);
        //��ͣ
        PAUSE();

        //��������
        myParam.bilateral.sgmgray += 2.0f;
        //myParam.bilateral.sigma += 1.0f;
    }
    //�ͷ���Ⱦ��
    YMCV_Free_KERN_RenderFree(myKernel);

    /*--------------------------��˹�˲��޸��˲��뾶--------------------------------------------------*/
    for (kernel_r = 0; kernel_r < 10; kernel_r++)
    {
        //������Ӧ�뾶���˲���
        myKernel = YMCV_Creat_SpaceRender_Creat(CV_BilFilter, PIXChannelNum1, kernel_r, OutPaddingConst);

        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_Space_SingleKernel_Load(myKernel, &myParam);

        //ʹ�ÿռ���Ⱦ��������Ⱦ�Ҷ�ͼƬ
        myImage1 = YMCV_Image_Space_SigleKernel_Render(myImage0, myKernel);
        //��ʾͼƬ1
        LCD_show_image(500, 0, myImage1);


        //��Ϣ��ʾ
        sprintf(myInform, "˫���˲��ĸ�˹�� �˲��뾶: %02d          ", kernel_r);
        LCD_ShowString(100, 510, myInform);
        //��ͣ
        PAUSE();

        //���ͷ�ͼ�񣬱����´��õ�ʱ�����ڴ�й©
        YMCV_Free_ImgFree(myImage1);
        //�ͷſռ���Ⱦ��
        YMCV_Free_KERN_RenderFree(myKernel);
    }

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);

    PAUSE();
    LCD_Destory();
    return 0;
}


   


