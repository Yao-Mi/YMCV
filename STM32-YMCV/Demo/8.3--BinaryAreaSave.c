
/*******************************************************************************************
    *
    * ˵��  �������� ������Ϊ����չʾ�� �����ϴ���������Ч��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage0_gray = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_gray, CV_Rgb16ToGray);//rgbת�Ҷ�ͼ

    LCD_show_image(stx + 0, 0, myImage0_gray);
	  int sty = myImage0_gray->height;
    /*--------------------------�Ҷ�˫��ֵ��ֵ��--------------------------------------------------*/
    //���������ֵͼ��
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0_gray->width, myImage0_gray->height, CVBinaryType);//��ֵͼ
    CV_binary_param myParam = { .gray_double_t.thresh_min = 100 ,.gray_double_t.thresh_max = 150 };

    //������ֵ�任��Ⱦ��
    CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_DoubleThreshold);
    YMCV_BinaryKernel_Load(myRender1, &myParam);//���ز���������Ⱦ���á���ʼ��
    YMCV_Image_ThreshTo_Binary(myImage0_gray, myImage2, myRender1);//��Ŀ��ͼ��Ľ�����Ⱦ
    
    //��ʾԭͼ
    LCD_show_image(stx + 0, sty + 0, myImage2);
    
    //��Ϣ��ʾ
    sprintf(myInform, "To Binary ");
    LCD_ShowString(stx + 0, sty + 0, myInform);
		Delay(500);
    /*------------------------ ��ֵ������� ------------------------------*/
    //�����������100������
    YMCV_Binary_AreaFilter(myImage2, 100);
    LCD_show_image(stx + 0, sty + 0, myImage2);

    //��Ϣ��ʾ
    sprintf(myInform, "Area Save  ");
    LCD_ShowString(stx + 0, sty + 0, myInform);
		Delay(500);
		
    //�ͷ��ڴ�
    YMCV_Free_Binary_RenderFree(myRender1);
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage2);
}

