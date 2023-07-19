#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_HistCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� ֱ��ͼ�涨��Ϊ����չʾ�˽�һ��ͼ���ֱ��ͼ������һ��ͼʹ�õķ�����ֱ��ͼ��ʾ
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 700);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(linglong00));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, linglong00, sizeof(linglong00));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 500, 255, CVRgb16Type);//��ɫͼ1

    uint8* myData1 = CV_malloc1(sizeof(linglong01));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData1, linglong01, sizeof(linglong01));
    CVIMAGE myImage1 = YMCV_Creat_DataToImg((uint8*)myData1, 500, 255, CVRgb16Type);//��ɫͼ1

    //ֱ��ͼ����
    int32 myImage0Hist[3*256], myImage1Hist[3*256], myImage2Hist[3*256];

    /*--------------------------��ɫͼֱ��ͼ��ӳ��--------------------------------------------------*/

    //��ȡͼ��ֱ��ͼ
    YMCV_Image_Hist_Get(myImage0, myImage0Hist, 3*256, PIXChannelNum3);
    //��ʾͼƬ��ֱ��ͼ
    LCD_show_image(0, 0, myImage0);
    LCDHistShow(myImage0Hist, PIXChannelNum3, 550, 420, 128);

    //��ȡͼ��ֱ��ͼ����ʾ
    YMCV_Image_Hist_Get(myImage1, myImage1Hist, 3*256, PIXChannelNum3);
    LCD_show_image(500, 0, myImage1);
    LCDHistShow(myImage1Hist, PIXChannelNum3, 800, 420, 128);

    //��ͼƬ0����һ��
    CVIMAGE myImage2 = YMCV_Creat_Img_BackUp(myImage0);
    //��myImage0��ֱ��ͼ��״ �涨ΪmyImage1����״
    YMCV_Image_Hist_MapTo(myImage2, myImage1);

    //��ȡͼ��ֱ��ͼ����ʾ
    YMCV_Image_Hist_Get(myImage2, myImage2Hist, 3*256, PIXChannelNum3);
    LCD_show_image(0, 350, myImage2);
    LCDHistShow(myImage2Hist, PIXChannelNum3, 550, 600, 128);//���ڴ�С������128������ת����״���ܲ���ʵ��������256�۲���ʵֱ��ͼ


    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);

    PAUSE();
    LCD_Destory();
    return 0;
}





