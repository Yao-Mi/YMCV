
/*******************************************************************************************
    *
    * ˵��  �������� Otsu��ֵ�ָ�Ϊ����չʾ�˻Ҷ�ͼ�ָ��Ч��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];
	  
    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage0_gray = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_gray, CV_Rgb16ToGray);//rgbת�Ҷ�ͼ
	
       //ֱ��ͼ����
    int32 myImage0Hist[256];
    /*--------------------------�Ҷ�ͼֱ��ͼ--------------------------------------------------*/
    //��ȡͼ��ֱ��ͼ
    YMCV_Image_Hist_Get(myImage0_gray, myImage0Hist, 256, PIXChannelNum1);
    //��ʾimage0ͼƬ��ֱ��ͼ
		LCD_show_image(stx + 0, 0, myImage0_gray);
    LCDHistShow(myImage0Hist, PIXChannelNum1, 150, 80, 60);

    /*--------------------------�Ҷ�Otsu��ֵ��ֵ��--------------------------------------------------*/
    //���������ֵͼ��
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0_gray->width, myImage0_gray->height, CVBinaryType);//��ֵͼ
    CV_binary_param myParam = {.hist_auto_t.thresh_type= CV_HistOtsu };//��ʼ��Ϊotsu��ֵ

    //������ֵ�任��Ⱦ��
    CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_HistThreshold);

    //����Ϊotsu��ֵ��
    myParam.hist_auto_t.thresh_type = CV_HistOtsu;//��ֵ�����ж��ֿ�ѡ����ʹ��p-tile������������ٷֱȲ���
    //myParam.hist_auto_t.p_tile_val = 40;//��������ٷֱȲ���
    
    //���ز�����������
    YMCV_BinaryKernel_Load(myRender1, &myParam);

    //��Ŀ��ͼ��Ľ�����Ⱦ
    YMCV_Image_ThreshTo_Binary(myImage0_gray, myImage2, myRender1);

    //��ʾͼƬ2
    LCD_show_image(stx + 0, myImage0_gray->height, myImage2);

    //��Ϣ��ʾ
    sprintf(myInform, "binary Otsu thresh ");
		LCD_ShowString(stx + 0, myImage0_gray->height - 16, myInform);
		//��ͣ500ms
		Delay(500);

    //�ͷ���Ⱦ���������´��õ�ʱ�����ڴ�й©
    YMCV_Free_Binary_RenderFree(myRender1);

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage2);

}





