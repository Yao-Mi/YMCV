
/*******************************************************************************************
    *
    * ˵��  �������� ��˹�˲� Ϊ����չʾ�˽�һ��ͼ���  ��ʾ
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage1, CV_Rgb16ToGray);//rgbת�Ҷ�ͼ
	
    //��ʾԭͼ
    LCD_show_image(stx + 0, 0, myImage1);

    //�����ռ���Ⱦ��
    uint8 kernel_r = 5;
    CVKERN_RENDER myKernel = YMCV_Creat_SpaceRender_Creat(CV_GaussFilter, PIXChannelNum1, kernel_r, OutPaddingConst);
    YMCV_PADDING_VAL = 125;//�������ֵ


    //������Ⱦ�����������г�ʼ��
    CV_kernel_1param myParam = { .gauss.sigma = 1.0 };

    //���ڻ�����
    CVIMAGE myImage2 = NULL;

    /*--------------------------��˹�˲��޸ĸ�˹����---------------------------------------------*/
    for (int i = 0; i < 10; i++)
    {
        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_Space_SingleKernel_Load(myKernel, &myParam);

        //ʹ�ÿռ���Ⱦ��������Ⱦ�Ҷ�ͼƬ
        myImage2 = YMCV_Image_Space_SigleKernel_Render(myImage1, myKernel);
        //��ʾͼƬ2
        LCD_show_image(stx + 0,  myImage1->height, myImage2);

        //������ͷ�ͼ�񣬱����´��õ�ʱ�����ڴ�й©
        YMCV_Free_ImgFree(myImage2);

        //��Ϣ��ʾ
        sprintf(myInform, "gauss sigma: %01.2f ", myParam.gauss.sigma);
        LCD_ShowString(stx + 0, myImage1->height, myInform);
        //��ͣ300ms
        Delay(300);

        //��������
        myParam.gauss.sigma += 0.5f;
    }
    //�ͷ���Ⱦ��
    YMCV_Free_KERN_RenderFree(myKernel);
    
    /*--------------------------��˹�˲��޸��˲��뾶--------------------------------------------------*/
    for (kernel_r = 0; kernel_r < 10; kernel_r++)
    {
        //������Ӧ�뾶���˲���
        myKernel = YMCV_Creat_SpaceRender_Creat(CV_GaussFilter, PIXChannelNum1, kernel_r, OutPaddingConst);

        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_Space_SingleKernel_Load(myKernel, &myParam);

        //ʹ�ÿռ���Ⱦ��������Ⱦ�Ҷ�ͼƬ
        myImage2 = YMCV_Image_Space_SigleKernel_Render(myImage1, myKernel);
        //��ʾͼƬ2
        LCD_show_image(stx + 0,  myImage1->height, myImage2);


        //��Ϣ��ʾ
        sprintf(myInform, "gauss radius: %02d      ", kernel_r);
        LCD_ShowString(stx + 0,  myImage1->height, myInform);
        //��ͣ300ms
        Delay(300);

        //���ͷ�ͼ�񣬱����´��õ�ʱ�����ڴ�й©
        YMCV_Free_ImgFree(myImage2);
        //�ͷſռ���Ⱦ��
        YMCV_Free_KERN_RenderFree(myKernel);
    }

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage1);

}





