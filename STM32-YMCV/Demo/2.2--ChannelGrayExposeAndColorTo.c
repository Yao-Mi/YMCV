
/*******************************************************************************************
    *
    * ˵��  ���������ع���ֵ���Ҷ�ȾɫΪ����չʾ�� ��Ⱦ����ʹ�ã��Լ��Ҷ�Ⱦɫ�����ֲ���ȡֵ��
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

    /*--------------------------��ֵ�ع�--------------------------------------------------*/

    //����ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Expose, PIXChannelNum1);//ͼ���ع�
    //������Ⱦ������
    CV_pixchannel_1param myParam = { .gray_expose.thresh = 0 };
    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_SingleChannel_MapTo(myRender, &myParam);

        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_ChannelRenderTo(myImage1, myImage2, myRender);

        //��ʾͼƬ2
        LCD_show_image(stx + 0, myImage1->height, myImage2);

        //��Ϣ��ʾ
        sprintf(myInform, "Expose Thresh: %03d .", myParam.gray_expose.thresh);
        LCD_ShowString(stx + 0, myImage1->height, myInform);
        //��ͣ200ms
        Delay(200);

        //��������
        myParam.gray_expose.thresh += 10;
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);

    /*--------------------------�Ҷ�ͼȾɫ--------------------------------------------------*/

    //���´���һ��ͨ����Ⱦ��
    myRender = YMCV_Creat_PixRender_Creat(CV_ColorTo, PIXChannelNum1);//�Ҷ�Ⱦɫ
    //������Ⱦ������ ȡֵ��[0.0 -- 1.0]���� ���� [0 --- 255]����
    CV_pixchannel_1param myParam1 = { .gray_colorto.graycolor = 0.0 };
    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_SingleChannel_MapTo(myRender, &myParam1);

        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_ChannelRenderTo(myImage1, myImage2, myRender);

        //��ʾͼƬ2
        LCD_show_image(stx + 0, myImage1->height, myImage2);

        //��Ϣ��ʾ
        sprintf(myInform, "gray color: %03.2f %", myParam1.gray_colorto.graycolor);
        LCD_ShowString(stx + 0, myImage1->height, myInform);
        //��ͣ200ms
        Delay(200);

        //��������
        myParam1.gray_colorto.graycolor = myParam1.gray_colorto.graycolor + 0.10f;//[0.0 -- 1.0]
    }
    
    //���³�ʼ��Ⱦɫ����
    myParam1.gray_colorto.graycolor = 0.0f;
    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_SingleChannel_MapTo(myRender, &myParam1);

        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_ChannelRenderTo(myImage1, myImage2, myRender);

        //��ʾͼƬ2
        LCD_show_image(stx + 0, myImage1->height, myImage2);

        //��Ϣ��ʾ
        sprintf(myInform, "gray color: %03.2f", myParam1.gray_colorto.graycolor);
        LCD_ShowString(stx + 0, myImage1->height, myInform);
        //��ͣ200ms
        Delay(200);

        //��������
        myParam1.gray_colorto.graycolor = myParam1.gray_colorto.graycolor + 10.00f;//ȡֵ�� [0 -- 255]����
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);



    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);

}





