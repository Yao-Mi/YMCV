
/*******************************************************************************************
    *
    * ˵��  ��������log������expָ���任Ϊ����չʾ��ʹ�� ��ͨ����Ⱦ�� ��Ⱦ��ɫͼ��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

	  //��ʾͼƬ0
    LCD_Clear(0);
    LCD_show_image(stx + 0, 0, myImage0);

    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVRgb16Type);

    /*--------------------------��ɫͼlog������ǿ--------------------------------------------------*/

    //����ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Log_Enhance, PIXChannelNum1);//ͼ�������ǿ
    //������Ⱦ������ �������ֵ����Ƽ�����Ϊ     a��5��    ��  b��0.005��    ��   c��0.01��
    CV_pixchannel_1param myParam = { .gray_log_enhance._a = -156.0f,.gray_log_enhance._b = 0.040f,.gray_log_enhance._c = 1.42f };
    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_SingleChannel_MapTo(myRender, &myParam);
        //ʹ��ͨ����Ⱦ��������Ⱦ��ɫͼƬ
        YMCV_Image_ChannelRenderTo_UGray(myImage0, myImage1, myRender);
        //��ʾͼƬ1
        LCD_show_image(stx + 0, myImage0->height, myImage1);
        //��ʾͨ��ת��ͼ
        LCDChannelTransformShow(myRender, 170, 220, 40);
        //��Ϣ��ʾ
        sprintf(myInform, "log : a%03.2f b%01.3f c%01.2f ", myParam.gray_log_enhance._a, myParam.gray_log_enhance._b, myParam.gray_log_enhance._c);
        LCD_ShowString(0, myImage0->height, myInform);

        //��ͣ200ms
        Delay(200);
			
        //��������
        myParam.gray_log_enhance._a += 5.0f;
        //myParam.gray_log_enhance._b += 0.005f;
        //myParam.gray_log_enhance._c += 0.01f;
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);

    /*--------------------------��ɫͼexpָ����ǿ--------------------------------------------------*/

    //���´���һ��ͨ����Ⱦ��
    myRender = YMCV_Creat_PixRender_Creat(CV_Exp_Enhance, PIXChannelNum1);//ָ����ǿ
    //������Ⱦ������ �������ֵ����Ƽ�����Ϊ     a��10��    ��  b��0.001��    ��   c��0.01��
    CV_pixchannel_1param myParam1 = { .gray_exp_enhance._a = -542.0f,.gray_exp_enhance._b = 1.04f,.gray_exp_enhance._c = 0.18f };
    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_SingleChannel_MapTo(myRender, &myParam1);

        //ʹ��ͨ����Ⱦ��������Ⱦ��ɫͼƬ
        YMCV_Image_ChannelRenderTo_UGray(myImage0, myImage1, myRender);
        //��ʾͼƬ1
        LCD_show_image(stx + 0, myImage0->height, myImage1);
        //��ʾͨ��ת��ͼ
        LCDChannelTransformShow(myRender, 170, 220, 40);

        //��Ϣ��ʾ
        sprintf(myInform, "exp : a%03.2f b%01.3f c%01.2f ", myParam1.gray_exp_enhance._a, myParam1.gray_exp_enhance._b, myParam1.gray_exp_enhance._c);
        LCD_ShowString(0, myImage0->height, myInform);

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
}





