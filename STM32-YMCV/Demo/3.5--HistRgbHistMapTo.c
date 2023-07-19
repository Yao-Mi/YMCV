#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� ֱ��ͼӳ��Ϊ����չʾ�˽�һ��ͼ���ֱ��ͼ������һ��ͼʹ�õķ�����ֱ��ͼ��ʾ
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(linglong00b));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, (void*)linglong00b, sizeof(linglong00b));
    CVIMAGE myImage1 = YMCV_Creat_DataToImg((uint8*)myData, 100, 87, CVRgb16Type);//��ɫͼ1

	  LCD_Clear(0);
    //ֱ��ͼ���飬���ڲ���static�����Բ��Ը�demoҪ�������ļ��е�ջ��С������3000����
    int32 myImage0Hist[3*256], myImage1Hist[3*256], myImage2Hist[3*256];

    /*--------------------------��ɫͼֱ��ͼ��ӳ��--------------------------------------------------*/

    //��ȡͼ��ֱ��ͼ
    YMCV_Image_Hist_Get(myImage0, myImage0Hist, 3*256, PIXChannelNum3);
    //��ʾͼƬ��ֱ��ͼ
    LCD_show_image(stx + 0, 0, myImage0);
    LCDHistShow(myImage0Hist, PIXChannelNum3, 170, 60, 40);

    //��ȡͼ��ֱ��ͼ����ʾ
    YMCV_Image_Hist_Get(myImage1, myImage1Hist, 3*256, PIXChannelNum3);
    LCD_show_image(stx + 0, myImage0->height, myImage1);
    LCDHistShow(myImage1Hist, PIXChannelNum3, 170, 220, 40);
		
    sprintf(myInform, "target Hist");
    LCD_ShowString(stx + 0, myImage0->height, myInform);
    //��ͣ500ms
    Delay(500);

    //��ͼƬ0����һ��
    CVIMAGE myImage2 = YMCV_Creat_Img_BackUp(myImage0);
//    //��myImage0��ֱ��ͼ��״ �涨ΪmyImage1����״
//    YMCV_Image_Hist_MapTo(myImage2, myImage1);

//    //��ȡͼ��ֱ��ͼ����ʾ
//    YMCV_Image_Hist_Get(myImage2, myImage2Hist, 3*256, PIXChannelNum3);
//    LCD_show_image(stx + 0, myImage0->height, myImage2);
//    LCDHistShow(myImage2Hist, PIXChannelNum3,170, 220, 40);

    //��ͣ500ms
    Delay(500);

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);

}





