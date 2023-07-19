
/*******************************************************************************************
    *
    * ˵��  ���������ع���ֵ���Ҷ�ȾɫΪ����չʾ�� ��Ⱦ����ʹ�ã��Լ��Ҷ�Ⱦɫ�����ֲ���ȡֵ��
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

    /*--------------------------��ɫͼ��ֵ�ع�--------------------------------------------------*/

    //����ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Expose, PIXChannelNum3);//ͼ���ع�
    //������Ⱦ������
    CV_pixchannel_3param myParam = { .rgb_expose.R_thresh = 0,.rgb_expose.G_thresh = 0,.rgb_expose.B_thresh = 0 };
    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_ThreeChannel_MapTo(myRender, &myParam);

        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //��ʾͼƬ1
        LCD_show_image(stx + 0, myImage0->height, myImage1);

        //��Ϣ��ʾ
        sprintf(myInform, "Expose Thresh: R%03d G%03d B%03d ", myParam.rgb_expose.R_thresh, myParam.rgb_expose.G_thresh, myParam.rgb_expose.B_thresh);
        LCD_ShowString(0, myImage1->height, myInform);
        //��ͣ200ms
        Delay(200);

        //��������
        myParam.rgb_expose.R_thresh += 10;
        myParam.rgb_expose.G_thresh += 10;
        myParam.rgb_expose.B_thresh += 10;
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);


    /*--------------------------��ɫͼȾɫ��1--------------------------------------------------*/

    //���´���һ��ͨ����Ⱦ��
    myRender = YMCV_Creat_PixRender_Creat(CV_ColorTo, PIXChannelNum3);//��ɫ��ͨ��Ⱦɫ
    //������Ⱦ������ ȡֵ��[0.0 -- 1.0]���� ���� [0 --- 255]����
    CV_pixchannel_3param myParam1 = { .rgb_colorto.R_color = 0.0 , .rgb_colorto.G_color = 0.0 , .rgb_colorto.B_color = 0.0 };
    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_ThreeChannel_MapTo(myRender, &myParam1);

        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //��ʾͼƬ1
        LCD_show_image(stx + 0, myImage0->height, myImage1);

        //��Ϣ��ʾ
        sprintf(myInform, "colorTo: R%03.2f%% G%03.2f%% B%03.2f%%  ", myParam1.rgb_colorto.R_color, myParam1.rgb_colorto.G_color, myParam1.rgb_colorto.B_color);
        LCD_ShowString(0, myImage1->height, myInform);
        //��ͣ200ms
        Delay(200);

        //��������
        myParam1.rgb_colorto.R_color +=  0.11f;//[0.0 -- 1.0]
        myParam1.rgb_colorto.G_color +=  0.08f;
        myParam1.rgb_colorto.B_color +=  0.07f;
    }

    /*--------------------------��ɫͼȾɫ��2--------------------------------------------------*/

    //���³�ʼ��Ⱦɫ����
    myParam1.rgb_colorto.R_color = 0.0f;
    myParam1.rgb_colorto.G_color = 0.0f;
    myParam1.rgb_colorto.B_color = 0.0f;

    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_ThreeChannel_MapTo(myRender, &myParam1);

        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //��ʾͼƬ1
        LCD_show_image(stx + 0, myImage0->height, myImage1);

        //��Ϣ��ʾ
        sprintf(myInform, "colorTo: R%03.1f G%03.1f B%03.1f   ", myParam1.rgb_colorto.R_color, myParam1.rgb_colorto.G_color, myParam1.rgb_colorto.B_color);
        LCD_ShowString(0, myImage1->height, myInform);
        //��ͣ200ms
        Delay(200);

        //��������
        myParam1.rgb_colorto.R_color += 13.0f;//ȡֵ�� [0 -- 255]����
        myParam1.rgb_colorto.G_color += 19.0f;
        myParam1.rgb_colorto.B_color += 26.0f;
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);



    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage1);
}


