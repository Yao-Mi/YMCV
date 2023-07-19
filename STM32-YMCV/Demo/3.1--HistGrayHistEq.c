#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� ֱ��ͼ���⻯��ֱ��ͼ�涨��Ϊ����չʾ��ֱ��ͼ��ʹ�ü���ʾ
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
    CVIMAGE myImage01 = YMCV_Creat_DataToImg((uint8*)myData, 100, 87, CVGrayType);//�Ҷ�ͼ0
	
    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage1, CV_Rgb16ToGray);//rgbת�Ҷ�ͼ
	
    //ֱ��ͼ����
    int32 myImage1Hist[256], myImage2Hist[256];

    /*--------------------------�Ҷ�ͼֱ��ͼ���⻯--------------------------------------------------*/

    //��ȡͼ��ֱ��ͼ
    YMCV_Image_Hist_Get(myImage1, myImage1Hist, 256, PIXChannelNum1);
    //��ʾimage1ͼƬ��ֱ��ͼ
    LCD_show_image(stx + 0, 0, myImage1);
    LCDHistShow(myImage1Hist, PIXChannelNum1, 170, 60, 40);


    //��image1����һ��
    CVIMAGE myImage2 = YMCV_Creat_Img_BackUp(myImage1);
    //��myImage2�Ľ���ֱ��ͼ���⻯����
    YMCV_Image_Hist_Equalize(myImage2, myImage2);//�����������

    //��ȡͼ��ֱ��ͼ����ʾ
    YMCV_Image_Hist_Get(myImage2, myImage2Hist, 256, PIXChannelNum1);
    LCD_show_image(stx + 0, myImage1->height, myImage2);
    LCDHistShow(myImage2Hist, PIXChannelNum1, 170, 220, 40);//���ڴ�С������40������ת����״���ܲ���ʵ��������256�۲���ʵֱ��ͼ

    sprintf(myInform, "1 channel equalize"); //3ͨ��ͬʱ�����⻯���ϲ�ʽ���⻯��
    LCD_ShowString(stx + 0,  myImage1->height, myInform);
    //��ͣ1000ms
    Delay(1000);


    /*--------------------------�Ҷ�ͼֱ��ͼ�涨��--------------------------------------------------*/

    ////�ֶ���ֱ��ͼ�涨Ϊĳ����״
    //for (int i = 0; i < 256; i++)
    //{
    //    myImage1Hist[i] = i + 50;//���Ժ�������ֱ��ͼΪ���ν���
    //}
    //��ͼ��01��ȡֱ��ͼ����ʾ
    YMCV_Image_Hist_Get(myImage01, myImage2Hist, 256, PIXChannelNum1);
    LCD_show_image(stx + 0, 0, myImage01);
    LCDHistShow(myImage2Hist, PIXChannelNum1, 170, 60, 40);
		
    sprintf(myInform, "target Hist");
    LCD_ShowString(stx + 0, 0, myInform);
    //��ͣ500ms
    Delay(500);

    //��ͼ��1����ֱ��ͼ�涨��
    YMCV_Image_Hist_To(myImage1, myImage2, myImage2Hist, 256, PIXChannelNum1);
    //��ȡͼ��ֱ��ͼ����ʾ
    YMCV_Image_Hist_Get(myImage2, myImage2Hist, 256, PIXChannelNum1);
    LCD_show_image(stx + 0, myImage1->height, myImage2);
    LCDHistShow(myImage2Hist, PIXChannelNum1, 170, 220, 40);//���ڴ�С������40������ת����״���ܲ���ʵ��������256�۲���ʵֱ��ͼ
    
		sprintf(myInform, "hist specification");
    LCD_ShowString(stx + 0, myImage1->height, myInform);
    //��ͣ1000ms
    Delay(1000);
		
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage01);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);

}



