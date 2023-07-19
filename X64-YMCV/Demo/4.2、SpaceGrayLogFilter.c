#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_HistCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� Log�˲�Ϊ����չʾ�˽�һ��ͼ��ʹ�� ������˹-��˹�任 ���б߽���ȡ
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
    uint8 kernel_r = 1;
    CVKERN_RENDER myKernel = YMCV_Creat_SpaceRender_Creat(CV_LOGFilter, PIXChannelNum1, kernel_r, OutPaddingConst);
    YMCV_PADDING_VAL = 125;//�������ֵ


    //������Ⱦ�����������г�ʼ��
    CV_kernel_1param myParam = { .log.sigma = 0.1 };

    //���ڻ�����
    CVIMAGE myImage1 = NULL;
    //ע�⣺���ģ���СȡΪ������NxN��N��С��3*sigma ��r=1����N=3��sigma��ȡ1���µ�ֵ��
    /*--------------------------LOG�˲��޸ĸ�˹����---------------------------------------------*/
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
        sprintf(myInform, "Laplacian of Gauss�˲�sigma: %01.1f .", myParam.log.sigma);
        LCD_ShowString(100, 510, myInform);
        //��ͣ
        PAUSE();

        //��������
        myParam.log.sigma += 0.1f; 
    }
    //�ͷ���Ⱦ��
    YMCV_Free_KERN_RenderFree(myKernel);
    
    /*--------------------------��˹�˲��޸��˲��뾶--------------------------------------------------*/
    for (kernel_r = 0; kernel_r < 10; kernel_r++)
    {
        //����N >=3*sigmaԭ���ʵ�����sigma����
        myParam.log.sigma = (2*kernel_r+1)/9.0f;

        //������Ӧ�뾶���˲���
        myKernel = YMCV_Creat_SpaceRender_Creat(CV_LOGFilter, PIXChannelNum1, kernel_r, OutPaddingConst);

        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_Space_SingleKernel_Load(myKernel, &myParam);

        //ʹ�ÿռ���Ⱦ��������Ⱦ�Ҷ�ͼƬ
        myImage1 = YMCV_Image_Space_SigleKernel_Render(myImage0, myKernel);
        //��ʾͼƬ1
        LCD_show_image(500, 0, myImage1);


        //��Ϣ��ʾ
        sprintf(myInform, "������˹ - ��˹ �˲��뾶: %02d sigma:%01.1f .  ", kernel_r, myParam.log.sigma);
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





