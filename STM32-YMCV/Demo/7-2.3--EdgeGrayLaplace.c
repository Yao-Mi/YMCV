
/*******************************************************************************************
    *
    * ˵��  �������� ������˹�˲���Ϊ����չʾ�˱�Ե���Ч����������ͨ�����ڶ�ֵͼ�ļ��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];
	
    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage0_g = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_g, CV_Rgb16ToGray);//rgbת�Ҷ�ͼ
		//��ʾͼƬ0
		LCD_show_image(stx + 0, 0, myImage0_g);
	
    /*--------------------------������˹��Ե���--------------------------------------------------*/
    //���������ֵͼ��
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0_g->width, myImage0_g->height, CVBinaryType);//��ֵͼ
    CV_edge_param myParam = { .EdgeType = CV_Laplace ,.Thresh.laplace.minT=30};

    YMCV_Gray_EdgeDetect(myImage0_g, myImage2, &myParam);

    //��ʾͼƬ2
    LCD_show_image(stx + 0, myImage0_g->height, myImage2);

    //��Ϣ��ʾ
    sprintf(myInform, "Laplace thresh:%3d  ", myParam.Thresh.sobel.minT);
    LCD_ShowString(stx + 0, myImage0_g->height - 8, myInform);
		
    //��ͣ300ms
    Delay(300);

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_g);
    YMCV_Free_ImgFree(myImage2);
}





