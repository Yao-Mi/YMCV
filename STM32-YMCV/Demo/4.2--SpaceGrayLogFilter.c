
/*******************************************************************************************
    *
    * ˵��  �������� Log�˲�Ϊ����չʾ�˽�һ��ͼ��ʹ�� ������˹-��˹�任 ���б߽���ȡ
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
    uint8 kernel_r = 1;
    CVKERN_RENDER myKernel = YMCV_Creat_SpaceRender_Creat(CV_LOGFilter, PIXChannelNum1, kernel_r, OutPaddingConst);
    YMCV_PADDING_VAL = 125;//�������ֵ


    //������Ⱦ�����������г�ʼ��
    CV_kernel_1param myParam = { .log.sigma = 0.1 };

    //���ڻ�����
    CVIMAGE myImage2 = NULL;
    //ע�⣺���ģ���СȡΪ������NxN��N��С��3*sigma ��r=1����N=3��sigma��ȡ1���µ�ֵ��
    /*--------------------------LOG�˲��޸ĸ�˹����---------------------------------------------*/
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
        sprintf(myInform, "LOG sigma: %01.1f          ", myParam.log.sigma);
        LCD_ShowString(stx + 0, myImage1->height, myInform);
        //��ͣ300ms
        Delay(300);

        //��������
        myParam.log.sigma += 0.1f; 
    }
    //�ͷ���Ⱦ��
    YMCV_Free_KERN_RenderFree(myKernel);
    
    /*--------------------------��˹�˲��޸��˲��뾶--------------------------------------------------*/
    for (kernel_r = 0; kernel_r < 10; kernel_r++)
    {
        //����N >=3*sigmaԭ���ʵ�����sigma����
        myParam.log.sigma = (2*kernel_r+1)/9.0f;

        //������Ӧ�뾶���˲���
        myKernel = YMCV_Creat_SpaceRender_Creat(CV_LOGFilter, PIXChannelNum1, kernel_r, OutPaddingConst);

        //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
        YMCV_Space_SingleKernel_Load(myKernel, &myParam);

			  //ʹ�ÿռ���Ⱦ��������Ⱦ�Ҷ�ͼƬ
        myImage2 = YMCV_Image_Space_SigleKernel_Render(myImage1, myKernel);
        //��ʾͼƬ2
        LCD_show_image(stx + 0,  myImage1->height, myImage2);


        //��Ϣ��ʾ
        sprintf(myInform, "log radius:%02d sigma:%01.1f ", kernel_r, myParam.log.sigma);
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





