
/*******************************************************************************************
    *
    * ˵��  �������� ����任Ϊ����չʾ������Trimap��Ч��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];
	
    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage0_gray = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_gray, CV_Rgb16ToGray);//rgbת�Ҷ�ͼ
		//��ʾͼƬ0
		LCD_show_image(stx + 0, 0, myImage0_gray);
    /*--------------------------����任����ǰ��ͼ--------------------------------------------------*/

    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0_gray->width, myImage0_gray->height, CVBinaryType);

    //������ֵ��ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Thresh2ToBin, PIXChannelNum1);//��ͨ����ֵ����һ��ͨ��

    CV_pixchannel_1param myParam = { .gray_2tobin.leftThresh = 130,.gray_2tobin.rightThresh = 255 };    //������Ⱦ������
    YMCV_SingleChannel_MapTo(myRender, &myParam);//ʹ�ò���������Ⱦ��ͨ�����á���ʼ��  

    //ͼ���ֵ��
    YMCV_Image_ChannelRenderTo(myImage0_gray, myImage1, myRender);//ʹ��ͨ����Ⱦ��������ȾͼƬ

    //�Զ�ֵͼ���о���任
    CVIMAGE myImage2 = YMCV_Binary_Distance_Transform(myImage1);
		
    //��ʾͼƬ2
		LCD_show_image(stx + 0, myImage0_gray->height, myImage2);
		//��Ϣ��ʾ
	  sprintf(myInform, "front distance Transform");
		LCD_ShowString(0, myImage0_gray->height, myInform);
		//��ͣ300ms
		Delay(300);
		

    //�Ծ���ͼ��ֵ��
    myParam.gray_2tobin.leftThresh = 20;
    myParam.gray_2tobin.rightThresh = 255;
    YMCV_SingleChannel_MapTo(myRender, &myParam);

    YMCV_Image_ChannelRenderTo(myImage2, myImage2, myRender);//��ֵ����Ⱦ

		//��ʾͼƬ2
		LCD_show_image(stx + 0, myImage0_gray->height, myImage2);
		//��Ϣ��ʾ
	  sprintf(myInform, "front thresh:[%3d,%3d] ",myParam.gray_2tobin.leftThresh,myParam.gray_2tobin.rightThresh);
		LCD_ShowString(0, myImage0_gray->height, myInform);
		//��ͣ300ms
		Delay(300);

    /*--------------------------����任��������ͼ--------------------------------------------------*/

    //��������ȡ��ǰ���ֱ���ȡ��
    YMCV_Image_Binary_Logic_Cal(myImage1, myImage1, CV_Not);//ͼ1 �߼��� ������������ͼ1

    //�Զ�ֵͼ���о���任
    CVIMAGE myImage3 = YMCV_Binary_Distance_Transform(myImage1);
		
		//��ʾͼƬ3
		LCD_show_image(stx + 0, myImage0_gray->height, myImage2);
		//��Ϣ��ʾ
	  sprintf(myInform, "behind distance Transform");
		LCD_ShowString(0, myImage0_gray->height, myInform);
		//��ͣ300ms
		Delay(300);

    //�Ծ���ͼ��ֵ��
    myParam.gray_2tobin.leftThresh = 40;
    myParam.gray_2tobin.rightThresh = 255;
    YMCV_SingleChannel_MapTo(myRender, &myParam);

    YMCV_Image_ChannelRenderTo(myImage3, myImage3, myRender);//��ֵ����Ⱦ

		//��ʾͼƬ3
		LCD_show_image(stx + 0, myImage0_gray->height, myImage3);
		//��Ϣ��ʾ
	  sprintf(myInform, "behind thresh:[%3d,%3d] ",myParam.gray_2tobin.leftThresh,myParam.gray_2tobin.rightThresh);
		LCD_ShowString(0, myImage0_gray->height, myInform);
		//��ͣ300ms
		Delay(300);

    /*-------------------------- TriMapͼ����--------------------------------------------------*/
    //���Ҷȸ�ʽǿ��תΪ��ֵͼ
    myImage2->type = CVBinaryType;
    myImage3->type = CVBinaryType;
    //ʹ��ǰ��ͼ�ͱ���ͼ������trimapͼ
    CVIMAGE TriMapImage = YMCV_BinaryTo_TriMap(myImage2, myImage3);

    //��ʾͼƬtrimap
		LCD_show_image(stx + 0, myImage0_gray->height, TriMapImage);

    //��Ϣ��ʾ
    sprintf(myInform, "TriMap image creat        ");
    LCD_ShowString(0, myImage0_gray->height, myInform);
		//��ͣ800ms
		Delay(800);
		
    //ͨ����Ⱦ���ڴ��ͷ�
    YMCV_Free_PIX_RenderFree(myRender);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);
    YMCV_Free_ImgFree(TriMapImage);
}









