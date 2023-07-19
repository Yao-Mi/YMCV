#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_HistCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� ֱ��ͼ���⻯��ֱ��ͼ�涨��Ϊ����չʾ��ֱ��ͼ��ʹ�ü���ʾ
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 450);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(linglong00_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, linglong00_gray, sizeof(linglong00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 500, 255, CVGrayType);//�Ҷ�ͼ0


    //ֱ��ͼ����
    int32 myImage0Hist[256], myImage1Hist[256];

    /*--------------------------�Ҷ�ͼֱ��ͼ���⻯--------------------------------------------------*/

    //��ȡͼ��ֱ��ͼ
    YMCV_Image_Hist_Get(myImage0, myImage0Hist, 256, PIXChannelNum1);
    //��ʾimage0ͼƬ��ֱ��ͼ
    LCD_show_image(0, 0, myImage0);
    LCDHistShow(myImage0Hist, PIXChannelNum1, 50, 420, 128);

    //��image0����һ��
    CVIMAGE myImage2 = YMCV_Creat_Img_BackUp(myImage0);
    //��myImage2�Ľ���ֱ��ͼ���⻯����
    YMCV_Image_Hist_Equalize(myImage2, myImage2);//�����������

    //��ȡͼ��ֱ��ͼ����ʾ
    YMCV_Image_Hist_Get(myImage2, myImage1Hist, 256, PIXChannelNum1);
    LCD_show_image(500, 0, myImage2);
    LCDHistShow(myImage1Hist, PIXChannelNum1, 550, 420, 128);//���ڴ�С������128������ת����״���ܲ���ʵ��������256�۲���ʵֱ��ͼ

    //�����ͷ��ڴ�
    YMCV_Free_ImgFree(myImage2);
    PAUSE();

    /*--------------------------�ֲ�ֱ��ͼȥ�� �޸��˲��뾶--------------------------------------------------*/
    uint8 ker_r = 60;
    //��Padding��䣬����ͼ��߽�,����뾶Ϊr
    CVIMAGE myImage1 = YMCV_Image_Image_Padding(myImage0, ker_r, ker_r, CV_PaddingAutoSpace);
    //LCD_show_image(0, 0, myImage1);

    //�ֲ�ֱ��ͼ����
    myImage2 = YMCV_Gray_Local_Hist_Equalize(myImage1, ker_r, NOPadding);
    //��ȡͼ��ֱ��ͼ����ʾ
    YMCV_Image_Hist_Get(myImage2, myImage1Hist, 256, PIXChannelNum1);
    LCD_show_image(500, 0, myImage2);
    LCDHistShow(myImage1Hist, PIXChannelNum1, 550, 420, 128);//���ڴ�С������128������ת����״���ܲ���ʵ��������256�۲���ʵֱ��ͼ


    //��Ϣ��ʾ
    sprintf(myInform, "�ֲ�����뾶: %d  ", ker_r);
    LCD_ShowString(100, 510, myInform);
    //�ͷ��ڴ�
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage1);
    PAUSE();

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);

    PAUSE();
    LCD_Destory();
    return 0;
}





