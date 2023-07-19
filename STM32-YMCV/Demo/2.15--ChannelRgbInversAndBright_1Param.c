
/*******************************************************************************************
    *
    * ˵��  �������Է�ɫ��������ǿΪ����չʾ��ʹ�� ��ͨ����Ⱦ�� ��Ⱦ��ɫͼ��
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


    /*--------------------------��ɫͼ������ǿ--------------------------------------------------*/

    //����ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Bright, PIXChannelNum1);//ͼ��������ǿ
    //������Ⱦ������
    CV_pixchannel_1param myParam = { .gray_bright._k = 0.0f };
    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_SingleChannel_MapTo(myRender, &myParam);

        //ʹ��ͨ����Ⱦ��������Ⱦ��ɫͼƬ
        YMCV_Image_ChannelRenderTo_UGray(myImage0, myImage1, myRender);
        //��ʾͼƬ1
        LCD_show_image(stx + 0, myImage0->height, myImage1);

        //��Ϣ��ʾ
        sprintf(myInform, "bright: %01.2f .", myParam.gray_bright._k);
        LCD_ShowString(stx + 0, myImage1->height, myInform);
			
        //��ͣ200ms
        Delay(200);

        //��������
        myParam.gray_bright._k += 0.5f;
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);


    /*--------------------------��ɫͼ��ɫ--------------------------------------------------*/

    //���´���һ��ͨ����Ⱦ��
    myRender = YMCV_Creat_PixRender_Creat(CV_Invers, PIXChannelNum1);//�Ҷȷ�ɫ
    //������Ⱦ������ ȡֵ��[0.0 -- 1.0]���� ���� [0 --- 255]����
    CV_pixchannel_1param myParam1 = { .gray_invers._k = 0.0f };
    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_SingleChannel_MapTo(myRender, &myParam1);

        //ʹ��ͨ����Ⱦ��������Ⱦ��ɫͼƬ
        YMCV_Image_ChannelRenderTo_UGray(myImage0, myImage1, myRender);
        //��ʾͼƬ1
        LCD_show_image(stx + 0, myImage0->height, myImage1);

        //��Ϣ��ʾ
        sprintf(myInform, "invers: %01.2f .", myParam1.gray_invers._k);
        LCD_ShowString(stx + 0, myImage1->height, myInform);
			
        //��ͣ200ms
        Delay(200);

        //��������
        myParam1.gray_invers._k += 0.50f;//[0.0 -- 1.0]
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);


    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage1);

}





