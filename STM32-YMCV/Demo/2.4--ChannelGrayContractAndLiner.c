
/*******************************************************************************************
    *
    * ˵��  �������ԶԱȶȡ��ֶ�������ǿΪ����չʾ�˲���ȡֵ �������������Լ�ͨ��ת��ʾ��ͼ����ʾ
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
    /*--------------------------�Աȶ���ǿ--------------------------------------------------*/

    //����ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Contract, PIXChannelNum1);//ͼ��Աȶ���ǿ/����������
    //������Ⱦ������
    CV_pixchannel_1param myParam = { .gray_contract._k = -100.0f };
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
        sprintf(myInform, "contract: %01.2f .", myParam.gray_contract._k);
        LCD_ShowString(stx + 0, myImage1->height, myInform);
			 
        //��ͣ200ms
        Delay(200);

        //��������
        myParam.gray_contract._k += 20.0f;
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);


    /*--------------------------�ֶ�������ǿ--------------------------------------------------*/

    //���´���һ��ͨ����Ⱦ��
    myRender = YMCV_Creat_PixRender_Creat(CV_Liner_Enhance, PIXChannelNum1);//�ֶ�������ǿ
    //������Ⱦ������ ��[0,f_a][f_a,f_b][f_b,f_c][f_c,255] �任Ϊ [0,t_a][t_a,t_b][t_b,t_c][t_c,255] 
    CV_pixchannel_1param myParam1 = { .gray_liner_enhance.from_a = 20,.gray_liner_enhance.from_b = 100,.gray_liner_enhance.from_c = 200,
                                      .gray_liner_enhance.to_a = 30, .gray_liner_enhance.to_b = 30, .gray_liner_enhance.to_c = 230 };
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
        sprintf(myInform, "liner:[%03d %03d %03d]",
            myParam1.gray_liner_enhance.from_a, myParam1.gray_liner_enhance.from_b, myParam1.gray_liner_enhance.from_c);   
			  LCD_ShowString(stx + 0, myImage1->height, myInform);
			
        sprintf(myInform, "  ->  [%03d %03d %03d]",
            myParam1.gray_liner_enhance.to_a, myParam1.gray_liner_enhance.to_b, myParam1.gray_liner_enhance.to_c);
			  LCD_ShowString(stx + 0, myImage1->height + 20, myInform);
			
        //��ͣ200ms
        Delay(200);

        //�������� һ��ѡ��ù̶���from�����󣬵���to���������в�����Χ��[0 -- 255]
        myParam1.gray_liner_enhance.to_b += 20;//[to_a -- to_c]
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);


    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);

}





