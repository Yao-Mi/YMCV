#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_HistCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� �Զ����˲�Ϊ����չʾ�˽�һ��ͼ��ʹ���Զ����˲��˽��и�����ʾ��Ч��
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


    //�����Զ���ռ���Ⱦ��
    uint8 kernel_r = 1;
    uint8 Kernel_N = (2 * kernel_r + 1) * (2 * kernel_r + 1);
    //ע�⣺���ģ���СȡΪ������NxN����r=1����N=3��
    CVKERN_RENDER myKernel = YMCV_Creat_SpaceRender_Creat(CV_CustomFilter, PIXChannelNum1, kernel_r, OutPaddingConst);
    YMCV_PADDING_VAL = 125;//�������ֵ

    //���ڻ�����
    CVIMAGE myImage1 = NULL;
    float32 FDkernel0[9] = { -1,-1, 0,  //�����˲���
                           -1, 0, 1,
                           0, 1, 1 };
    float32 FDkernel1[9] = { -1,-1, 0,  //����Ч��
                             -1, 1, 1,
                              0, 1, 1 };
    float32 FDkernel2[9] = { 2, 0, 0,
                             0,-1, 0,
                             0, 0,-1 };
    /*--------------------------�����˲�---------------------------------------------*/

        //�ֶ��Բ���������Ⱦ��ͨ�����á���ʼ��
    CV_memcpy(myKernel->data,FDkernel1, Kernel_N*sizeof(float32));

        //ʹ���Զ���ռ���Ⱦ��������Ⱦ�Ҷ�ͼƬ
        myImage1 = YMCV_Image_Space_CustomKernel_Render(myImage0, myKernel);
        //��ʾͼƬ1
        LCD_show_image(500, 0, myImage1);

        //������ͷ�ͼ�񣬱����´��õ�ʱ�����ڴ�й©
        YMCV_Free_ImgFree(myImage1);

        //��Ϣ��ʾ
        sprintf(myInform, "�Զ����˲��� .");
        LCD_ShowString(100, 510, myInform);
        //��ͣ
        PAUSE();

    
    //�ͷ���Ⱦ��
    YMCV_Free_KERN_RenderFree(myKernel);
  
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);

    PAUSE();
    LCD_Destory();
    return 0;
}





