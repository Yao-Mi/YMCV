#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
/*******************************************************************************************
    *
    * ˵��  ������չʾ�� ͼƬ�������Լ�ͼƬ�Ľ�ȡ����
    *
    ***************************************************************************************/
int main()
{
    LCD_Init(640, 480);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(linglong00));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, linglong00, sizeof(linglong00));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 500, 255, CVRgb16Type);

    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(200,200, CVRgb16Type);

    //��ȡ����ͼƬ��һ�����򿽱�������תΪһ���µ�ͼ��
    CVIMAGE myImage2 = YMCV_Creat_ImgCopyTo(myImage0, 100, 20, 200, 200);
    //����ROI�ķ�ʽ��ȡ
    CVrect myroi = { .x = 200,.y = 20,.w = 200,.h = 200 };
    CVIMAGE myImage3 = YMCV_Creat_ImgROICopyTo(myImage0, &myroi);


    //��ʾͼƬ1
    LCD_show_image(0, 0, myImage1);
    //��ͣ
    PAUSE();

    //��ʾͼƬ2
    LCD_show_image(0, 0, myImage2);
    //��ͣ
    PAUSE();

    //��ʾͼƬ3
    LCD_show_image(0, 0, myImage3);
    //��ͣ
    PAUSE();

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);


    PAUSE();
    LCD_Destory();
    return 0;
}





