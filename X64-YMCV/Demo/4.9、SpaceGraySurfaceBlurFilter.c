#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_HistCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� ����ģ���˲�Ϊ����չʾ�˽�һ��ͼ������˲����˲�Ч�����Լ�������ʹ��
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
    uint8 kernel_r = 1;
    uint8 T_v = 20;
    YMCV_PADDING_VAL = 50;//����߽����ֵ

    //���ڻ�����
    CVIMAGE myImage1 = NULL;

    /*--------------------------����ģ���˲� �޸��˲��뾶--------------------------------------------------*/
    for (kernel_r = 1; kernel_r < 10; kernel_r++)
    {
        //ʹ�ñ���ģ�������˲�
        myImage1 = YMCV_Gray_SurfaceBlur(myImage0, kernel_r, T_v, OutPaddingConst);
        //��ʾͼƬ1
        LCD_show_image(500, 0, myImage1);

        //��Ϣ��ʾ
        sprintf(myInform, "����ģ�� ����뾶: %02d ��ֵ:%d  . ", kernel_r, T_v);
        LCD_ShowString(100, 510, myInform);
        //��ͣ
        PAUSE();

        //���ͷ�ͼ�񣬱����´��õ�ʱ�����ڴ�й©
        YMCV_Free_ImgFree(myImage1);
    }


    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);

    PAUSE();
    LCD_Destory();
    return 0;
}








