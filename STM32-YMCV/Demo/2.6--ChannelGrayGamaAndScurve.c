
/*******************************************************************************************
    *
    * ˵��   ��������gama��s-curve��ǿΪ����չʾ�˲���ȡֵ �������������Լ�ͨ��ת��ʾ��ͼ����ʾ
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

    /*--------------------------٤��������ǿ--------------------------------------------------*/

    //����ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Gama_Enhance, PIXChannelNum1);//gama������ǿ
    //������Ⱦ������ �������ֵ����Ƽ�����Ϊ     c��5��    ��  gama��0.1��
    CV_pixchannel_1param myParam = { .gray_gama_enhance._c = 309.0f,.gray_gama_enhance._gama = 1.6f };
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
        sprintf(myInform, "gama: c=%03.2f gama= %01.3f   ", myParam.gray_gama_enhance._c, myParam.gray_gama_enhance._gama);
			  LCD_ShowString(0, myImage1->height, myInform);
			
        //��ͣ200ms
        Delay(200);

        //��������
        myParam.gray_gama_enhance._c += 20.0f;
        //myParam.gray_gama_enhance._gama += 0.1f;
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);


    /*--------------------------S������ǿ--------------------------------------------------*/

    //���´���һ��ͨ����Ⱦ��
    myRender = YMCV_Creat_PixRender_Creat(CV_Scurve_Enhance, PIXChannelNum1);//S������ǿ
    //������Ⱦ������ �������ֵ����Ƽ�����Ϊ     a��10��    ��  b��0.001��    ��   c��0.01��
    CV_pixchannel_1param myParam1 = { .gray_scurve_enhance._m = 132.0f,.gray_scurve_enhance._E = -5.0f }; 
    for (int i = 0; i < 20; i++)
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
        sprintf(myInform, "Scure: m = %03.2f  E = %02.2f ", myParam1.gray_scurve_enhance._m, myParam1.gray_scurve_enhance._E);
        LCD_ShowString(0, myImage1->height, myInform);
			
        //��ͣ200ms
        Delay(200);

        //��������
        //myParam1.gray_scurve_enhance._m += 10.0f;
        myParam1.gray_scurve_enhance._E += 0.5f;
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);


    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);

}





