
/*******************************************************************************************
    *
    * ˵��  �������ԶԱȶȡ��ֶ�������ǿΪ����չʾ�˲���ȡֵ �������������Լ�ͨ��ת��ʾ��ͼ����ʾ
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
    LCD_Clear(0);
	  //��ʾͼƬ0
    LCD_show_image(stx + 0, 0, myImage0);

    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVRgb16Type);

    /*--------------------------��ɫͼ�Աȶ���ǿ--------------------------------------------------*/

    //����ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Contract, PIXChannelNum3);//ͼ��Աȶ���ǿ/����������
    //������Ⱦ������
    CV_pixchannel_3param myParam = { .rgb_contract.R_k = -100.0f, .rgb_contract.G_k = -100.0f, .rgb_contract.B_k = -100.0f };
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
			  sprintf(myInform, "contract:\n R%01.2f G%01.2f B%01.2f  ",
              			myParam.rgb_contract.R_k, myParam.rgb_contract.G_k, myParam.rgb_contract.B_k);
        LCD_ShowString(0, 0, myInform);
			 
        //��ͣ200ms
        Delay(200);

        //��������
        myParam.rgb_contract.R_k += 20.0f;
        myParam.rgb_contract.G_k += 15.0f;
        myParam.rgb_contract.B_k += 10.0f;
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);


    /*--------------------------��ɫͼ�ֶ�������ǿ--------------------------------------------------*/

    //���´���һ��ͨ����Ⱦ��
    myRender = YMCV_Creat_PixRender_Creat(CV_Liner_Enhance, PIXChannelNum3);//�ֶ�������ǿ
    //������Ⱦ������ ��[0,f_a][f_a,f_b][f_b,f_c][f_c,255] �任Ϊ [0,t_a][t_a,t_b][t_b,t_c][t_c,255] 
    CV_pixchannel_3param myParam1 = { .rgb_liner_enhance.R_from_a = 20,.rgb_liner_enhance.R_from_b = 100,.rgb_liner_enhance.R_from_c = 200,//R����
                                      .rgb_liner_enhance.R_to_a = 30, .rgb_liner_enhance.R_to_b = 30, .rgb_liner_enhance.R_to_c = 230 ,
                                      .rgb_liner_enhance.G_from_a = 20,.rgb_liner_enhance.G_from_b = 100,.rgb_liner_enhance.G_from_c = 200,//G����
                                      .rgb_liner_enhance.G_to_a = 30, .rgb_liner_enhance.G_to_b = 30, .rgb_liner_enhance.G_to_c = 230 ,
                                      .rgb_liner_enhance.B_from_a = 20,.rgb_liner_enhance.B_from_b = 100,.rgb_liner_enhance.B_from_c = 200,//B����
                                      .rgb_liner_enhance.B_to_a = 30, .rgb_liner_enhance.B_to_b = 30, .rgb_liner_enhance.B_to_c = 230 , };
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
			  sprintf(myInform, "liner RGB:");
			      LCD_ShowString(0, 0, myInform);
        sprintf(myInform, "R[%03d %03d %03d]->[%03d %03d %03d]",
            myParam1.rgb_liner_enhance.R_from_a, myParam1.rgb_liner_enhance.R_from_b, myParam1.rgb_liner_enhance.R_from_c, \
            myParam1.rgb_liner_enhance.R_to_a, myParam1.rgb_liner_enhance.R_to_b, myParam1.rgb_liner_enhance.R_to_c); \
            LCD_ShowString(0, 0 + 16, myInform);
        sprintf(myInform, "G[%03d %03d %03d]->[%03d %03d %03d]",
            myParam1.rgb_liner_enhance.G_from_a, myParam1.rgb_liner_enhance.G_from_b, myParam1.rgb_liner_enhance.G_from_c, \
            myParam1.rgb_liner_enhance.G_to_a, myParam1.rgb_liner_enhance.G_to_b, myParam1.rgb_liner_enhance.G_to_c); \
            LCD_ShowString(0, 0 + 16*2, myInform);
        sprintf(myInform, "B[%03d %03d %03d]->[%03d %03d %03d]",
            myParam1.rgb_liner_enhance.B_from_a, myParam1.rgb_liner_enhance.B_from_b, myParam1.rgb_liner_enhance.B_from_c, \
            myParam1.rgb_liner_enhance.B_to_a, myParam1.rgb_liner_enhance.B_to_b, myParam1.rgb_liner_enhance.B_to_c); \
            LCD_ShowString(0, 0 + 16*3, myInform);
        //��ͣ200ms
        Delay(200);

        //�������� һ��ѡ��ù̶���from�����󣬵���to���������в�����Χ��[0 -- 255]
        myParam1.rgb_liner_enhance.R_to_b += 20;//[to_a -- to_c]
        myParam1.rgb_liner_enhance.G_to_b += 15;
        myParam1.rgb_liner_enhance.B_to_b += 10;
    }
    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);


    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage1);
}





