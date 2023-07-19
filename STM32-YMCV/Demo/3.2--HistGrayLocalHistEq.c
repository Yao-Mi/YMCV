
/*******************************************************************************************
    *
    * ˵��  �������� ֱ��ͼ���⻯��ֱ��ͼ�涨��Ϊ����չʾ��ֱ��ͼ��ʹ�ü���ʾ
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage1, CV_Rgb16ToGray);//rgbת�Ҷ�ͼ
	  //��ʾͼƬ1
    LCD_show_image(stx + 0, 0, myImage1);
	
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

    //�����ͷ��ڴ�
    YMCV_Free_ImgFree(myImage2);
		//�ֲ�ֱ��ͼȥ���ʱ�ϳ������ﲻ����ʱ
//    //��ͣ500ms
//    Delay(500);

    /*--------------------------�ֲ�ֱ��ͼȥ�� �޸��˲��뾶--------------------------------------------------*/
    uint8 ker_r = 60;
    //��Padding��䣬����ͼ��߽�,����뾶Ϊr
    CVIMAGE myImage3 = YMCV_Image_Image_Padding(myImage1, ker_r, ker_r, CV_PaddingAutoSpace);
    //LCD_show_image(0, 0, myImage3);

    //�ֲ�ֱ��ͼ����
    myImage2 = YMCV_Gray_Local_Hist_Equalize(myImage3, ker_r, NOPadding);//image2�ڴ�֮ǰ�Ѿ��ͷ���
    //��ȡͼ��ֱ��ͼ����ʾ
    YMCV_Image_Hist_Get(myImage2, myImage2Hist, 256, PIXChannelNum1);
    LCD_show_image(stx + 0, myImage1->height, myImage2);
    LCDHistShow(myImage2Hist, PIXChannelNum1, 170, 220, 40);//���ڴ�С������40������ת����״���ܲ���ʵ��������256�۲���ʵֱ��ͼ


    //��Ϣ��ʾ
    sprintf(myInform, "local HistEqRadius:%d  ", ker_r);
    LCD_ShowString(stx + 0,myImage1->height , myInform);
		
		//��ͣ1000ms
    Delay(1000);
		
    //�ͷ��ڴ�
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);
}





