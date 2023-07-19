
/*******************************************************************************************
    *
    * ˵��  �������� �����˲�Ϊ����չʾ�˽�һ��ͼ�� �������˲�Ч����
    *       ͨ��һ������ͼG����Ŀ��ͼ��P������ͼ�񣩽����˲�����
    *       ʹ���������ͼ���������Ŀ��ͼ��P���ƣ�����������������ͼG����
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

    //�����뾶 = 6�����ֵ�˲���
    myKernel = YMCV_Creat_SpaceRender_Creat(CV_MaxFilter, PIXChannelNum1, 5, OutPaddingConst);
    //ʹ�����ֵ�˲��õ�����Ŀ��ͼ��
    CVIMAGE myImage1 = YMCV_Image_Space_SigleKernel_Render(myImage0_gray, myKernel);
    //��ʾͼƬ1
    LCD_show_image(stx + 0,  myImage0_gray->height, myImage1);

		//��Ϣ��ʾ
		sprintf(myInform, "guidef image");
		LCD_ShowString(stx + 0,  myImage0_gray->height, myInform);
		//��ͣ1000ms
		Delay(1000);

    CVIMAGE myImage2 = NULL;
    /*--------------------------�����˲� �޸��˲��뾶--------------------------------------------------*/
    for (kernel_r = 1; kernel_r < 10; kernel_r++)
    {
        //��Ŀ��ͼ�������������ԭͼ��ʹ��ԭͼ��Ϊ��������ͼ����������˲�
        myImage2 = YMCV_Gray_Image_GuideFilter(myImage1, myImage0_gray, OutPaddingConst, kernel_r,0.001);
        //��ʾͼƬ1
        LCD_show_image(stx + 0,  myImage0_gray->height, myImage2);

        //��Ϣ��ʾ
        sprintf(myInform, "guidef radius: %02d   ", kernel_r);
        LCD_ShowString(stx + 0,  myImage0_gray->height, myInform);
        //��ͣ300ms
        Delay(300);

        //���ͷ�ͼ�񣬱����´��õ�ʱ�����ڴ�й©
        YMCV_Free_ImgFree(myImage2);
    }


    //�ͷſռ���Ⱦ��
    YMCV_Free_KERN_RenderFree(myKernel);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage1);

}








