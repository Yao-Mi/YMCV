#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� ֱ��ͼ��ͶӰΪ����չʾ�˽�һ��ͼ���ֱ��ͼ��ͶӰ����һ��ͼʹ�õķ�����ֱ��ͼ��ʾ
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage1, CV_Rgb16ToGray);//rgbת�Ҷ�ͼ
	
    CVrect myroi = { .x = 80 - 20,.y = 60 - 20,.w = 40,.h = 40 };
    CVIMAGE myImage2 = YMCV_Creat_ImgROICopyTo(myImage1, &myroi);
		
		LCD_show_image(stx + 0, 0, myImage1);
		LCD_Draw_Rect(stx + myroi.x,myroi.y,myroi.w,myroi.h,(color24_t){.R = 255});

    //ֱ��ͼ����
    int32 myImage1Hist[256];

    /*--------------------------�Ҷ�ͼֱ��ͼ�����뷴��ӳ��--------------------------------------------------*/

    //��ȡͼ��ֱ��ͼ
    YMCV_Image_Hist_Get(myImage2, myImage1Hist, 256, PIXChannelNum1);
    //��ʾimage2����ֱ��ͼ
    LCD_show_image(0, 0, myImage2);
    LCDHistShow(myImage1Hist, PIXChannelNum1, 170, 60, 40);

    //��Ϣ��ʾ
    sprintf(myInform, "get roi hist     ");
    LCD_ShowString(stx + 0, 0,myInform);

    //��ͣ500ms
    Delay(500);
		
    //image1ֱ��ͼ��ͶӰ��image0 ����ʾ
    YMCV_GrayHist_BackProject(myImage1Hist, 256, myImage1);
    LCD_show_image(stx + 0,  myImage1->height, myImage1);

    //��Ϣ��ʾ
    sprintf(myInform, "grayHist backProj");
    LCD_ShowString(stx + 0, myImage1->height,myInform);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
}





