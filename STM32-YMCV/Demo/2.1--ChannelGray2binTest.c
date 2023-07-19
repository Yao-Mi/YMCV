
/*******************************************************************************************
    *
    * ˵��  �������Ե���˫��ֵΪ����չʾ�� ͨ����Ⱦ����������Ⱦ�������������������Ⱦ����ʼ���Լ���ȾͼƬ����ʾ
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

	
    /*--------------------------����ֵ��ֵ��--------------------------------------------------*/

    //����ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_ThreshToBin, PIXChannelNum1);//��ͨ����ֵ����һ��ͨ��
    //������Ⱦ������
    CV_pixchannel_1param myParam = { .gray_tobin.Thresh = 20 };
    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_SingleChannel_MapTo(myRender,&myParam);

        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_ChannelRenderTo(myImage1, myImage2, myRender);
        //��ʾͼƬ2
        LCD_show_image(stx + 0, myImage1->height, myImage2);

        //��Ϣ��ʾ
        sprintf(myInform, "1 - 1 thresh: %03d .       ", myParam.gray_tobin.Thresh);
        LCD_ShowString(stx + 0, myImage1->height, myInform);
        //��ͣ200ms
        Delay(200);

        //��������
        myParam.gray_tobin.Thresh += 10;
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);


    /*--------------------------˫��ֵ��ֵ��--------------------------------------------------*/

    //���´���һ��ͨ����Ⱦ��
    myRender = YMCV_Creat_PixRender_Creat(CV_Thresh2ToBin, PIXChannelNum1);//��ͨ����ֵ����һ��ͨ��
    //������Ⱦ������
    CV_pixchannel_1param myParam1 = { .gray_2tobin.leftThresh = 0 ,.gray_2tobin.rightThresh = 20 };
    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_SingleChannel_MapTo(myRender, &myParam1);

        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_ChannelRenderTo(myImage1, myImage2, myRender);
        //��ʾͼƬ2
        LCD_show_image(stx + 0, myImage1->height, myImage2);

        //��Ϣ��ʾ
        sprintf(myInform, "1 - 2 thresh: %03d -- %03d .", myParam1.gray_2tobin.leftThresh, myParam1.gray_2tobin.rightThresh);
        LCD_ShowString(stx + 0, myImage1->height, myInform);
        //��ͣ200ms
        Delay(200);

        //��������
        myParam1.gray_2tobin.leftThresh += 10;
        myParam1.gray_2tobin.rightThresh += 10;
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
}





