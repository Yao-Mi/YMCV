#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_HistCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� �����˲�Ϊ����չʾ�˽�һ��ͼ�� �������˲�Ч����
    *       ͨ��һ������ͼG����Ŀ��ͼ��P������ͼ�񣩽����˲�����
    *       ʹ���������ͼ���������Ŀ��ͼ��P���ƣ�����������������ͼG����
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
    uint8 kernel_r = 0;
    CVKERN_RENDER myKernel;
    YMCV_PADDING_VAL = 125;//�������ֵ

    //�����뾶 = 6�����ֵ�˲���
    myKernel = YMCV_Creat_SpaceRender_Creat(CV_MaxFilter, PIXChannelNum1, 5, OutPaddingConst);
    //ʹ�����ֵ�˲��õ�����Ŀ��ͼ��
    CVIMAGE myImage1 = YMCV_Image_Space_SigleKernel_Render(myImage0, myKernel);


    CVIMAGE myImage2 = NULL;
    /*--------------------------�����˲� �޸��˲��뾶--------------------------------------------------*/
    for (kernel_r = 1; kernel_r < 10; kernel_r++)
    {
        //��Ŀ��ͼ�������������ԭͼ��ʹ��ԭͼ��Ϊ��������ͼ����������˲�
        myImage2 = YMCV_Gray_Image_GuideFilter(myImage1, myImage0, OutPaddingConst, kernel_r,0.001);
        //��ʾͼƬ1
        LCD_show_image(500, 0, myImage2);

        //��Ϣ��ʾ
        sprintf(myInform, "�����˲� �˲��뾶: %02d   . ", kernel_r);
        LCD_ShowString(100, 510, myInform);
        //��ͣ
        PAUSE();

        //���ͷ�ͼ�񣬱����´��õ�ʱ�����ڴ�й©
        YMCV_Free_ImgFree(myImage2);
    }


    //�ͷſռ���Ⱦ��
    YMCV_Free_KERN_RenderFree(myKernel);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);

    PAUSE();
    LCD_Destory();
    return 0;
}








