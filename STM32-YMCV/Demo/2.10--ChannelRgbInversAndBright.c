
/*******************************************************************************************
    *
    * ˵��  �������Է�ɫ��������ǿΪ����չʾ�� ͨ����Ⱦ����ʹ�ã��Լ�����ȡֵ ����������
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

    /*--------------------------��ɫͼ������ǿ--------------------------------------------------*/

    //����ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Bright, PIXChannelNum3);//ͼ��������ǿ
    //������Ⱦ������
    CV_pixchannel_3param myParam = { .rgb_bright.R_k = 0.0f ,.rgb_bright.G_k = 0.0f ,.rgb_bright.B_k = 0.0f };
    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_ThreeChannel_MapTo(myRender, &myParam);

        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //��ʾͼƬ1
        LCD_show_image(stx + 0, myImage0->height, myImage1);

        //��Ϣ��ʾ
        sprintf(myInform, "bright: R%01.2f G%01.2f B%01.2f .", myParam.rgb_bright.R_k, myParam.rgb_bright.G_k, myParam.rgb_bright.B_k);
        LCD_ShowString(0, myImage1->height, myInform);
			
        //��ͣ200ms
        Delay(200);

        //��������
        myParam.rgb_bright.R_k += 0.5f;
        myParam.rgb_bright.G_k += 0.5f;
        myParam.rgb_bright.B_k += 0.5f;
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);

    /*--------------------------��ɫͼ��ɫ��ǿ--------------------------------------------------*/

    //���´���һ��ͨ����Ⱦ��
    myRender = YMCV_Creat_PixRender_Creat(CV_Invers, PIXChannelNum3);//��ɫ
    //������Ⱦ������ ȡֵ��[0.0 -- 1.0]���� ���� [0 --- 255]����
    CV_pixchannel_3param myParam1 = { .rgb_invers.R_k = 0.0f,.rgb_invers.G_k = 0.0f,.rgb_invers.B_k = 0.0f };
    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_ThreeChannel_MapTo(myRender, &myParam1);

        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //��ʾͼƬ1
        LCD_show_image(stx + 0, myImage0->height, myImage1);

        //��Ϣ��ʾ
        sprintf(myInform, "invers: R%01.2f G%01.2f B%01.2f  ", myParam1.rgb_invers.R_k, myParam1.rgb_invers.G_k, myParam1.rgb_invers.B_k);
        LCD_ShowString(0, myImage1->height, myInform);
			
        //��ͣ200ms
        Delay(200);

        //��������
        myParam1.rgb_invers.R_k += 0.50f;//[0.0 -- 1.0]
        myParam1.rgb_invers.G_k += 0.50f;
        myParam1.rgb_invers.B_k += 0.50f;
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);


    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage1);
}





