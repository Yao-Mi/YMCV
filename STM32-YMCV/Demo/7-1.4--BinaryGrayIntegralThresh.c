
/*******************************************************************************************
    *
    * ˵��  �������� ����ͼ��ֵ��Ϊ����չʾ�˻Ҷ���ֵ�ָ�Ч��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];
	  
    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage0_gray = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_gray, CV_Rgb16ToGray);//rgbת�Ҷ�ͼ
	
		//��ʾͼƬ0
		LCD_show_image(stx + 0, 0, myImage0_gray);
    /*--------------------------�ҶȻ���ͼ��ֵ��--------------------------------------------------*/
    //���������ֵͼ��
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0_gray->width, myImage0_gray->height, CVBinaryType);//��ֵͼ
    CV_binary_param myParam = { .gray_integral_t.kernerl_radius = 10 ,.gray_integral_t.thresh_percent = 85 };

    for (int i = 1; i < 10; i++)
    {

        //���ò���
        myParam.gray_integral_t.kernerl_radius = 5 + 2 * i;//�뾶
        myParam.gray_integral_t.thresh_percent = 97;//��������97%�Ĳ���


        //������ֵ�任��Ⱦ��,����ͼ��ֵ��
        CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_IntegralThreshold);

        //���ز���������Ⱦ���á���ʼ��
        YMCV_BinaryKernel_Load(myRender1, &myParam);

        //��Ŀ��ͼ��Ľ�����Ⱦ
        YMCV_Image_ThreshTo_Binary(myImage0_gray, myImage2, myRender1);

        //��ʾͼƬ2
        LCD_show_image(stx + 0, myImage0_gray->height, myImage2);

        //��Ϣ��ʾ
        sprintf(myInform, "binary intergral:\n      radius: %3d \n      thresh: %3d %% ",
																																myParam.gray_integral_t.kernerl_radius,
                                                                myParam.gray_integral_t.thresh_percent);
		    LCD_ShowString(stx + 0, myImage0_gray->height - 16, myInform);
				//��ͣ500ms
				Delay(500);

        //�ͷ���Ⱦ���������´��õ�ʱ�����ڴ�й©
        YMCV_Free_Binary_RenderFree(myRender1);

    }

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage2);

}





