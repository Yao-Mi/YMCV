#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_FeatCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� ����ͼ��ȡ Ϊ����չʾ�� LBP����ͼ����ȡ
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(640, 480);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(mm00_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, mm00_gray, sizeof(mm00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//�Ҷ�ͼ0

    /*------------------------ LBP����ͼ��ȡ ------------------------------*/

    CVIMAGE myImage1 = YMCV_Image_FeatureMap_Detect(myImage0, CV_FeatMap_LBP);
    LCD_show_image(0, 0, myImage1);
    LCD_show_image(300, 0, myImage0);

    //��Ϣ��ʾ
    sprintf(myInform, "LBP����ͼ  ");
    LCD_ShowString(400, 310, myInform);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage1);
    PAUSE();
    /*------------------------ ��Сѭ��LBP����ͼ��ȡ ------------------------------*/

    myImage1 = YMCV_Image_FeatureMap_Detect(myImage0, CV_FeatMap_MRLBP);
    LCD_show_image(0, 0, myImage1);

    //��Ϣ��ʾ
    sprintf(myInform, "��Сѭ��LBP����ͼ  ");
    LCD_ShowString(400, 310, myInform);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage1);
    PAUSE();
    /*------------------------ USAN����ͼ��ȡ ------------------------------*/

    myImage1 = YMCV_Image_FeatureMap_Detect(myImage0, CV_FeatMap_USAN);
    LCD_show_image(0, 0, myImage1);

    //��Ϣ��ʾ
    sprintf(myInform, "USAN����ͼ  ");
    LCD_ShowString(400, 310, myInform);

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);

    PAUSE();
    LCD_Destory();
    return 0;
}





