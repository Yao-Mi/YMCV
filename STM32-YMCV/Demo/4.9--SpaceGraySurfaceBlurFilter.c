
/*******************************************************************************************
    *
    * ˵��  �������� ����ģ���˲�Ϊ����չʾ�˽�һ��ͼ������˲����˲�Ч�����Լ�������ʹ��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage0_gray = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_gray, CV_Rgb16ToGray);//rgbת�Ҷ�ͼ
	
    //��ʾԭͼ
    LCD_show_image(stx + 0, 0, myImage0_gray);


    //�����ռ���Ⱦ��
    uint8 kernel_r = 1;
    uint8 T_v = 20;
    YMCV_PADDING_VAL = 50;//����߽����ֵ

    //���ڻ�����
    CVIMAGE myImage1 = NULL;

    /*--------------------------����ģ���˲� �޸��˲��뾶--------------------------------------------------*/
    for (kernel_r = 1; kernel_r < 10; kernel_r++)
    {
        //ʹ�ñ���ģ�������˲�
        myImage1 = YMCV_Gray_SurfaceBlur(myImage0_gray, kernel_r, T_v, OutPaddingConst);
        //��ʾͼƬ1
        LCD_show_image(stx + 0,  myImage0_gray->height, myImage1);

        //��Ϣ��ʾ
        sprintf(myInform, "surfaceBL raadius: %02d\n      thresh:%d  ", kernel_r, T_v);
        LCD_ShowString(stx +0,  myImage0_gray->height, myInform);
        //��ͣ300ms
        Delay(300);

        //���ͷ�ͼ�񣬱����´��õ�ʱ�����ڴ�й©
        YMCV_Free_ImgFree(myImage1);
    }


    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);

}








