
/*******************************************************************************************
    *
    * ˵��  �������� ��ֵ����ֵ�˲�Ϊ����չʾ��һ��ͼ�� ���˲�Ч��
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
    uint8 kernel_r = 0;
    CVKERN_RENDER myKernel;
    YMCV_PADDING_VAL = 125;//�������ֵ


    //������Ⱦ�����������ڽ��г�ʼ��
    CV_kernel_1param myParam ;//��ֵ�˲��������

    //���ڻ�����
    CVIMAGE myImage1 = NULL;
 
    /*--------------------------��ֵ�˲� �޸��˲��뾶--------------------------------------------------*/
    for (kernel_r = 1; kernel_r < 10; kernel_r++)
    {
        //������Ӧ�뾶���˲���
        myKernel = YMCV_Creat_SpaceRender_Creat(CV_AverFilter, PIXChannelNum1, kernel_r, OutPaddingConst);

        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_Space_SingleKernel_Load(myKernel, &myParam);//��ֵ�˲�������أ��ò���Ҳ��ʡ��

        //ʹ�ÿռ���Ⱦ��������Ⱦ�Ҷ�ͼƬ
        myImage1 = YMCV_Image_Space_SigleKernel_Render(myImage0_gray, myKernel);
        //��ʾͼƬ1
        LCD_show_image(stx + 0,  myImage0_gray->height, myImage1);


        //��Ϣ��ʾ
        sprintf(myInform, "averf radius: %02d   . ", kernel_r);
        LCD_ShowString(stx + 0,  myImage0_gray->height, myInform);
        //��ͣ300ms
        Delay(300);

        //���ͷ�ͼ�񣬱����´��õ�ʱ�����ڴ�й©
        YMCV_Free_ImgFree(myImage1);
        //�ͷſռ���Ⱦ��
        YMCV_Free_KERN_RenderFree(myKernel);
    }

    /*--------------------------��ֵ�˲� �޸��˲��뾶--------------------------------------------------*/
    for (kernel_r = 1; kernel_r < 10; kernel_r++)
    {
        //������Ӧ�뾶���˲���
        myKernel = YMCV_Creat_SpaceRender_Creat(CV_MedFilter, PIXChannelNum1, kernel_r, OutPaddingConst);

        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_Space_SingleKernel_Load(myKernel, &myParam);//��ֵ�˲�������أ��ò���Ҳ��ʡ��

        //ʹ�ÿռ���Ⱦ��������Ⱦ�Ҷ�ͼƬ
        myImage1 = YMCV_Image_Space_SigleKernel_Render(myImage0_gray, myKernel);
        //��ʾͼƬ1
        LCD_show_image(stx + 0,  myImage0_gray->height, myImage1);


        //��Ϣ��ʾ
        sprintf(myInform, "medf radius: %02d   . ", kernel_r);
        LCD_ShowString(stx + 0,  myImage0_gray->height, myInform);
        //��ͣ300ms
        Delay(300);

        //���ͷ�ͼ�񣬱����´��õ�ʱ�����ڴ�й©
        YMCV_Free_ImgFree(myImage1);
        //�ͷſռ���Ⱦ��
        YMCV_Free_KERN_RenderFree(myKernel);
    }
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
}





