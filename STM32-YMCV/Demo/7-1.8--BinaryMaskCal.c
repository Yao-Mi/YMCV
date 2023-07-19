
/*******************************************************************************************
    *
    * ˵��  �������� ��ֵͼ����������Ϊ����չʾ�����뱣�� �� ���
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];
	  static uint8_t clearlcd = 1;
    
	  if(clearlcd) 
			clearlcd = 0,
	    LCD_Clear(GRAY);
	
	  //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage0_g = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_g, CV_Rgb16ToGray);//rgbת�Ҷ�ͼ
		//��ʾͼƬ0
		LCD_show_image(stx + 0, 0, myImage0_g);
	  //��ͣ300ms
		Delay(300);
		
    //����������ֵͼ��
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0_g->width, myImage0_g->height, CVBinaryType);//��ֵͼ

    /*-------------------------- ��ֵ���� myImage2 --------------------------------------------------*/
    CV_binary_param myParam = { .gray_single_t.thresh = 180 };
    //������ֵ�任��Ⱦ��
    CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_SingleThreshold);
    YMCV_BinaryKernel_Load(myRender1, &myParam);   //���ز���������Ⱦ���á���ʼ��

    //��Ŀ��ͼ��Ľ�����Ⱦ
    YMCV_Image_ThreshTo_Binary(myImage0_g, myImage2, myRender1);

    //���ٷ�תһ��
    for (int i = 0; i < myImage2->width* myImage2->height; i++)
    {
        myImage2->data[i] = 255 - myImage2->data[i];
    }

    //��ʾͼƬ
    LCD_show_image(stx + 0, myImage0_g->height, myImage2);

    //��Ϣ��ʾ
    sprintf(myInform, "binary thresh: %3d ", myParam.gray_single_t.thresh);
    LCD_ShowString(stx + 0, myImage0_g->height, myInform);
    //��ͣ500ms
    Delay(500);
    //�ͷ���Ⱦ���ڴ�
    YMCV_Free_Binary_RenderFree(myRender1);

    /*-------------------------- ���뱣�� myImage0 = myImage0 & Mask --------------------------------------------------*/
    YMCV_Image_BinaryMask_Cal(myImage0_g, myImage2);//���뱣��������������ͼ0

    //��ʾͼƬ
    LCD_show_image(stx + 0, myImage0_g->height, myImage0_g);
    //��Ϣ��ʾ
    sprintf(myInform, " image mask  saved  ");
    LCD_ShowString(stx + 0, myImage0_g->height, myInform);
    //��ͣ800ms
    Delay(800);

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_g);
    YMCV_Free_ImgFree(myImage2);
}






