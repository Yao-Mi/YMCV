
/*******************************************************************************************
    *
    * ˵��  �������� �Զ����˲�Ϊ����չʾ�˽�һ��ͼ��ʹ���Զ����˲��˽��и�����ʾ��Ч��
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

    //�����Զ���ռ���Ⱦ��
    uint8 kernel_r = 1;
    uint8 Kernel_N = (2 * kernel_r + 1) * (2 * kernel_r + 1);
    //ע�⣺���ģ���СȡΪ������NxN����r=1����N=3��
    CVKERN_RENDER myKernel = YMCV_Creat_SpaceRender_Creat(CV_CustomFilter, PIXChannelNum1, kernel_r, OutPaddingConst);
    YMCV_PADDING_VAL = 125;//�������ֵ

    //���ڻ�����
    CVIMAGE myImage1 = NULL;
    float32 FDkernel0[9] = { -1,-1, 0,  //�����˲���
                           -1, 0, 1,
                           0, 1, 1 };
    float32 FDkernel1[9] = { -1,-1, 0,  //����Ч��
                             -1, 1, 1,
                              0, 1, 1 };
    float32 FDkernel2[9] = { 2, 0, 0,
                             0,-1, 0,
                             0, 0,-1 };
    /*--------------------------�����˲�---------------------------------------------*/

        //�ֶ��Բ���������Ⱦ��ͨ�����á���ʼ��
    CV_memcpy(myKernel->data,FDkernel1, Kernel_N*sizeof(float32));

        //ʹ���Զ���ռ���Ⱦ��������Ⱦ�Ҷ�ͼƬ
        myImage1 = YMCV_Image_Space_CustomKernel_Render(myImage0_gray, myKernel);
        //��ʾͼƬ1
        LCD_show_image(stx + 0,  myImage0_gray->height, myImage1);

        //������ͷ�ͼ�񣬱����´��õ�ʱ�����ڴ�й©
        YMCV_Free_ImgFree(myImage1);

        //��Ϣ��ʾ
        sprintf(myInform, "custom kernel filter");
        LCD_ShowString(stx + 0,  myImage0_gray->height, myInform);
        //��ͣ300ms
		    Delay(300);

    
    //�ͷ���Ⱦ��
    YMCV_Free_KERN_RenderFree(myKernel);
  
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
}





