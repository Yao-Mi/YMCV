#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� ����ͼ��ȡ Ϊ����չʾ�� LBP����ͼ����ȡ
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(mm02_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, (void*)mm02_gray, sizeof(mm02_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVGrayType);//�Ҷ�ͼ0

    LCD_show_image(stx + 0, 0, myImage0_gray);
	  int sty =  myImage0_gray->height;
    /*------------------------ LBP����ͼ��ȡ ------------------------------*/

    CVIMAGE myImage1 = YMCV_Image_FeatureMap_Detect(myImage0_gray, CV_FeatMap_LBP);
    LCD_show_image(stx + 0, sty + 0, myImage1);

    //��Ϣ��ʾ
    sprintf(myInform, "LBP featMap");
    LCD_ShowString(stx + 0, sty + 0, myInform);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage1);
    Delay(500);
    /*------------------------ ��Сѭ��LBP����ͼ��ȡ ------------------------------*/

    myImage1 = YMCV_Image_FeatureMap_Detect(myImage0_gray, CV_FeatMap_MRLBP);
    LCD_show_image(stx + 0, sty + 0, myImage1);

    //��Ϣ��ʾ
    sprintf(myInform, "MRLBP featMap");
    LCD_ShowString(stx + 0, sty + 0, myInform);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage1);
    Delay(500);
    /*------------------------ USAN����ͼ��ȡ ------------------------------*/

    myImage1 = YMCV_Image_FeatureMap_Detect(myImage0_gray, CV_FeatMap_USAN);
    LCD_show_image(stx + 0, sty + 0, myImage1);

    //��Ϣ��ʾ
    sprintf(myInform, "USAN  featMap ");
    LCD_ShowString(stx + 0, sty + 0, myInform);
    Delay(500);

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage1);
}





