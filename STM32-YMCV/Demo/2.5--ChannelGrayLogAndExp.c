
/*******************************************************************************************
    *
    * ˵��  ��������log������expָ���任Ϊ����չʾ�˲���ȡֵ �������������Լ�ͨ��ת��ʾ��ͼ����ʾ
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage1->width, myImage1->height, CVGrayType);

	
		YMCV_Image_Pix_Change_Form(myImage0, myImage1, CV_Rgb16ToGray);//rgbת�Ҷ�ͼ
	  
	  //��ʾͼƬ1
    LCD_show_image(stx + 0, 0, myImage1);
    /*--------------------------log������ǿ--------------------------------------------------*/

    //����ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Log_Enhance, PIXChannelNum1);//ͼ��Աȶ���ǿ/����������
    //������Ⱦ������ �������ֵ����Ƽ�����Ϊ     a��5��    ��  b��0.005��    ��   c��0.01��
    CV_pixchannel_1param myParam = { .gray_log_enhance._a = -156.0f,.gray_log_enhance._b = 0.041f,.gray_log_enhance._c = 1.42f };
    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_SingleChannel_MapTo(myRender, &myParam);
        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_ChannelRenderTo(myImage1, myImage2, myRender);
        //��ʾͼƬ2
        LCD_show_image(stx + 0, myImage1->height, myImage2);
			  //��ʾͨ��ת��ͼ
        LCDChannelTransformShow(myRender, 170, 220, 40);
			
        //��Ϣ��ʾ
        sprintf(myInform, "log: a=%03.2f b=%01.3f c=%01.2f", myParam.gray_log_enhance._a, myParam.gray_log_enhance._b, myParam.gray_log_enhance._c);
        LCD_ShowString(0, myImage1->height, myInform);

        //��ͣ200ms
        Delay(200);

        //��������
        myParam.gray_log_enhance._a += 5.0f;
        //myParam.gray_log_enhance._b += 0.005f;
        //myParam.gray_log_enhance._c += 0.01f;
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);


    /*--------------------------expָ����ǿ--------------------------------------------------*/

    //���´���һ��ͨ����Ⱦ��
    myRender = YMCV_Creat_PixRender_Creat(CV_Exp_Enhance, PIXChannelNum1);//ָ����ǿ
    //������Ⱦ������ �������ֵ����Ƽ�����Ϊ     a��10��    ��  b��0.001��    ��   c��0.01��
    CV_pixchannel_1param myParam1 = { .gray_exp_enhance._a = -542.0f,.gray_exp_enhance._b = 1.04f,.gray_exp_enhance._c = 0.18f };
    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_SingleChannel_MapTo(myRender, &myParam1);

        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_ChannelRenderTo(myImage1, myImage2, myRender);
			  //��ʾͼƬ2
        LCD_show_image(stx + 0, myImage1->height, myImage2);
			  //��ʾͨ��ת��ͼ
        LCDChannelTransformShow(myRender, 170, 220, 40);

        //��Ϣ��ʾ
        sprintf(myInform, "exp: a=%03.2f b=%01.3f c=%01.2f", myParam1.gray_exp_enhance._a, myParam1.gray_exp_enhance._b, myParam1.gray_exp_enhance._c);
        LCD_ShowString(0, myImage1->height, myInform);
        //��ͣ200ms
        Delay(200);

        //��������
        myParam1.gray_exp_enhance._a += 10.0f;
        //myParam1.gray_exp_enhance._b += 0.001f;
        //myParam1.gray_exp_enhance._c += 0.01f;
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);


    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
}





