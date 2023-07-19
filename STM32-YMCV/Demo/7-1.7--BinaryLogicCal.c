
/*******************************************************************************************
    *
    * ˵��  �������� ��ֵͼ���߼�����Ϊ����չʾ���߼� ���� �� ���
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 60;
    char myInform[100];
	  static uint8_t clearlcd = 1;
    
	  if(clearlcd) 
			clearlcd = 0,
	    LCD_Clear(GRAY);
	
	  //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage0_g = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_g, CV_Rgb16ToGray);//rgbת�Ҷ�ͼ
		//��ʾͼƬ0
		LCD_show_image(stx + 0, 0, myImage0_g);
	
	  //������Ļ̫С��ֻȡһ������������ʾ
	  CVrect myroi = { .x = 80 - 30,.y = 60 - 30,.w = 60,.h = 60 };
    CVIMAGE myImage0_roi = YMCV_Creat_ImgROICopyTo(myImage0_g, &myroi);
		LCD_Draw_Rect(stx + myroi.x,myroi.y,myroi.w,myroi.h,(color24_t){.R = 255});
	  //��ͣ300ms
    Delay(300);
	
    //����������ֵͼ��
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0_roi->width, myImage0_roi->height, CVBinaryType);//��ֵͼ
    CVIMAGE myImage3 = YMCV_Creat_Img_Creat(myImage0_roi->width, myImage0_roi->height, CVBinaryType);//��ֵͼ

    /*-------------------------- ��ֵ���� myImage2 --------------------------------------------------*/
    CV_binary_param myParam = { .gray_single_t.thresh = 150 };
    //������ֵ�任��Ⱦ��
    CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_SingleThreshold);
    YMCV_BinaryKernel_Load(myRender1, &myParam);   //���ز���������Ⱦ���á���ʼ��

    //��Ŀ��ͼ��Ľ�����Ⱦ
    YMCV_Image_ThreshTo_Binary(myImage0_roi, myImage2, myRender1);

    //��ʾͼƬ2
    LCD_show_image(0, 0, myImage2);

    //��Ϣ��ʾ
    sprintf(myInform, "1 thresh:%3d", myParam.gray_single_t.thresh);
    LCD_ShowString(stx + 0, 0, myInform);

		//�ͷ���Ⱦ���ڴ�
    YMCV_Free_Binary_RenderFree(myRender1);

    /*-------------------------- ��ֵ���� myImage3 --------------------------------------------------*/
    //�����µĲ���
    myParam.gray_double_t.thresh_min = 100;
    myParam.gray_double_t.thresh_max = 180;
    //�����µĶ�ֵ�任��Ⱦ��
    myRender1 = YMCV_Creat_BinaryRender_Creat(CV_DoubleThreshold);
    YMCV_BinaryKernel_Load(myRender1, &myParam);   //���ز���������Ⱦ���á���ʼ��

    //��Ŀ��ͼ��Ľ�����Ⱦ
    YMCV_Image_ThreshTo_Binary(myImage0_roi, myImage3, myRender1);

    //��ʾͼƬ3
    LCD_show_image(0, myImage2->height, myImage3);

    //��Ϣ��ʾ
    sprintf(myInform, "2 thresh:[%3d,%3d]", myParam.gray_double_t.thresh_min,myParam.gray_double_t.thresh_max);
    LCD_ShowString(stx + 0,  myImage2->height, myInform);
    //��ͣ300ms
    Delay(300);
    
    //�ͷ���Ⱦ���ڴ�
    YMCV_Free_Binary_RenderFree(myRender1);
    /*-------------------------- myImage3 = myImage3 & myImage2 --------------------------------------------------*/
    CVIMAGE img3back1 = YMCV_Creat_Img_BackUp(myImage3);
		YMCV_Image_Binary_Logic_Cal(myImage2, img3back1, CV_And);//ͼ2 �߼����� ͼ3 ������������ͼ3

    //��ʾͼƬ3
    LCD_show_image(20, myImage0->height + 40, img3back1);
    
    /*-------------------------- myImage3 = myImage2 | myImage3 --------------------------------------------------*/
    CVIMAGE img3back2 = YMCV_Creat_Img_BackUp(myImage3);
		YMCV_Image_Binary_Logic_Cal(myImage2, img3back2, CV_Or);//ͼ2 �߼����� ͼ3 ������������ͼ3

    //��ʾͼƬ3
    LCD_show_image(90, myImage0->height + 40, img3back2);
		
    /*-------------------------- myImage3 = ! myImage2 --------------------------------------------------*/
    YMCV_Image_Binary_Logic_Cal(myImage2, img3back2, CV_Not);//ͼ2 �߼���  ������������ͼ3

    //��ʾͼƬ3
    LCD_show_image(160, myImage0->height + 40, img3back2);
		
		
		//��Ϣ��ʾ
    sprintf(myInform, "logic:\n   1 and 2  1 or 2    not 1");
    LCD_ShowString(0, myImage0->height, myInform);
    //��ͣ500ms
    Delay(500);


    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_g);
    YMCV_Free_ImgFree(myImage0_roi);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);
    YMCV_Free_ImgFree(img3back1);
    YMCV_Free_ImgFree(img3back2);

}





