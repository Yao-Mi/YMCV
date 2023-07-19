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
    uint8* myData = CV_malloc1(sizeof(linglong00b_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
	  if(myData == NULL) printf("cvmalloc1�ڴ治��");
    CV_memcpy(myData, (void*)linglong00b_gray, sizeof(linglong00b_gray));
    CVIMAGE myImage2 = YMCV_Creat_DataToImg((uint8*)myData, 100, 87, CVGrayType);//�Ҷ�ͼ0
	
    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage1, CV_Rgb16ToGray);//rgbת�Ҷ�ͼ
	
		LCD_Clear(0);
    //ֱ��ͼ����
    int32 myImage1Hist[256], myImage2Hist[256], myImage3Hist[256];

    /*--------------------------�Ҷ�ͼֱ��ͼӳ��--------------------------------------------------*/

    //��ȡͼ��ֱ��ͼ
    YMCV_Image_Hist_Get(myImage1, myImage1Hist, 256, PIXChannelNum1);
    //��ʾimage1ͼƬ��ֱ��ͼ
    LCD_show_image(stx + 0, 0, myImage1);
    LCDHistShow(myImage1Hist, PIXChannelNum1, 170, 60, 40);

    //��ȡimage2ֱ��ͼ����ʾ
    YMCV_Image_Hist_Get(myImage2, myImage2Hist, 256, PIXChannelNum1);
    LCD_show_image(stx + 0,myImage1->height, myImage2);
    LCDHistShow(myImage2Hist, PIXChannelNum1, 170, 220, 40);
		
    sprintf(myInform, "target Hist");
    LCD_ShowString(stx + 0, myImage1->height, myInform);
    //��ͣ500ms
    Delay(500);

    //��ͼƬ1����һ��
    CVIMAGE myImage3 = YMCV_Creat_Img_BackUp(myImage1);
    //��myImage2��ֱ��ͼ��״ �涨ΪmyImage3����״
    YMCV_Image_Hist_MapTo(myImage3, myImage2);//image3��ֱ��ͼ��ӳ��Ϊimage2ֱ��ͼ��״

    //��ȡͼ��ֱ��ͼ����ʾ
    YMCV_Image_Hist_Get(myImage3, myImage3Hist, 256, PIXChannelNum1);
    LCD_show_image(stx + 0,myImage1->height, myImage3);
    LCDHistShow(myImage3Hist, PIXChannelNum1, 170, 220, 40);

    //��ͣ500ms
    Delay(500);
		
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);
}





