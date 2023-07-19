/*******************************************************************************************
    *
    * ˵��  ��������log������expָ���任Ϊ����չʾ�˲���ȡֵ �������������Լ�ͨ��ת��ʾ��ͼ����ʾ
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

    //��ʾͼƬ0
    LCD_show_image(stx + 0, 0, myImage0);
	
    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVRgb16Type);


    /*--------------------------��ɫͼlog������ǿ--------------------------------------------------*/

    //����ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Log_Enhance, PIXChannelNum3);//ͼ�������ǿ
    //������Ⱦ������ �������ֵ����Ƽ�����Ϊ     a��5��    ��  b��0.005��    ��   c��0.01��
    CV_pixchannel_3param myParam = { .rgb_log_enhance.R_a = -156.0f,.rgb_log_enhance.R_b = 0.040f,.rgb_log_enhance.R_c = 1.42f,
                                     .rgb_log_enhance.G_a = -156.0f,.rgb_log_enhance.G_b = 0.040f,.rgb_log_enhance.G_c = 1.42f,
                                     .rgb_log_enhance.B_a = -156.0f,.rgb_log_enhance.B_b = 0.040f,.rgb_log_enhance.B_c = 1.42f };
    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_ThreeChannel_MapTo(myRender, &myParam);
        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
			
        //��ʾͼƬ1
        LCD_show_image(stx + 0, myImage0->height, myImage1);
			  //��ʾͨ��ת��ͼ
        LCDChannelTransformShow(myRender, 170, 220, 40);
			
        //��Ϣ��ʾ
			  sprintf(myInform, "log RGB:");
			   LCD_ShowString(0, 0, myInform);
			  sprintf(myInform, "R: a=%03.2f b= %01.3f c=%01.2f  ", myParam.rgb_log_enhance.R_a, myParam.rgb_log_enhance.R_b, myParam.rgb_log_enhance.R_c);
         LCD_ShowString(0, 0+16, myInform);
        sprintf(myInform, "G: a=%03.2f b= %01.3f c=%01.2f  ", myParam.rgb_log_enhance.G_a, myParam.rgb_log_enhance.G_b, myParam.rgb_log_enhance.G_c);
         LCD_ShowString(0, 0+16*2, myInform);
        sprintf(myInform, "B: a=%03.2f b= %01.3f c=%01.2f  ", myParam.rgb_log_enhance.B_a, myParam.rgb_log_enhance.B_b, myParam.rgb_log_enhance.B_c);
         LCD_ShowString(0, 0+16*3, myInform);

        //��ͣ200ms
        Delay(200);

        //��������
        myParam.rgb_log_enhance.R_a += 5.0f;
        //myParam.gray_log_enhance._b += 0.005f;
        //myParam.gray_log_enhance._c += 0.01f;
        myParam.rgb_log_enhance.G_a += 3.0f;
        myParam.rgb_log_enhance.B_a += 1.5f;
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);

    /*--------------------------��ɫͼexpָ����ǿ--------------------------------------------------*/

    //���´���һ��ͨ����Ⱦ��
    myRender = YMCV_Creat_PixRender_Creat(CV_Exp_Enhance, PIXChannelNum3);//ָ����ǿ
    //������Ⱦ������ �������ֵ����Ƽ�����Ϊ     a��10��    ��  b��0.001��    ��   c��0.01��
    CV_pixchannel_3param myParam1 = { .rgb_exp_enhance.R_a = -542.0f,.rgb_exp_enhance.R_b = 1.04f,.rgb_exp_enhance.R_c = 0.18f,
                                      .rgb_exp_enhance.G_a = -542.0f,.rgb_exp_enhance.G_b = 1.04f,.rgb_exp_enhance.G_c = 0.18f,
                                      .rgb_exp_enhance.B_a = -542.0f,.rgb_exp_enhance.B_b = 1.04f,.rgb_exp_enhance.B_c = 0.18f };
    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_ThreeChannel_MapTo(myRender, &myParam1);

        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
			
        //��ʾͼƬ1
        LCD_show_image(stx + 0, myImage0->height, myImage1);
			  //��ʾͨ��ת��ͼ
        LCDChannelTransformShow(myRender, 170, 220, 40);

        //��Ϣ��ʾ
			  sprintf(myInform, "exp RGB:");
			   LCD_ShowString(0, 0, myInform);
        sprintf(myInform, "R: a=%03.2f b= %01.3f c=%01.2f  ", myParam1.rgb_exp_enhance.R_a, myParam1.rgb_exp_enhance.R_b, myParam1.rgb_exp_enhance.R_c);
        LCD_ShowString(0, 0+16, myInform);
        sprintf(myInform, "G: a=%03.2f b= %01.3f c=%01.2f  ", myParam1.rgb_exp_enhance.G_a, myParam1.rgb_exp_enhance.G_b, myParam1.rgb_exp_enhance.G_c);
        LCD_ShowString(0, 0+16*2, myInform);
        sprintf(myInform, "B: a=%03.2f b= %01.3f c=%01.2f  ", myParam1.rgb_exp_enhance.B_a, myParam1.rgb_exp_enhance.B_b, myParam1.rgb_exp_enhance.B_c);
        LCD_ShowString(0, 0+16*3, myInform);
        
				//��ͣ200ms
        Delay(200);

        //��������
        myParam1.rgb_exp_enhance.R_a += 2.0f;
        //myParam1.gray_exp_enhance._b += 0.001f;
        //myParam1.gray_exp_enhance._c += 0.01f;
        myParam1.rgb_exp_enhance.G_a += 5.0f;
        myParam1.rgb_exp_enhance.B_a += 10.0f;
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);


    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage1);
}





