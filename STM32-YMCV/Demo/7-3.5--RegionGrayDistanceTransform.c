
/*******************************************************************************************
    *
    * ˵��  �������� ����任Ϊ����չʾ�˶�ֵͼ�任Ч��
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
	
    /*--------------------------��ֵͼ����任--------------------------------------------------*/

    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0_gray->width, myImage0_gray->height, CVBinaryType);

    //������ֵ��ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Thresh2ToBin, PIXChannelNum1);//��ͨ����ֵ����һ��ͨ��
    CV_pixchannel_1param myParam = { .gray_2tobin.leftThresh = 150,.gray_2tobin.rightThresh=255 };    //������Ⱦ������
    YMCV_SingleChannel_MapTo(myRender, &myParam);//ʹ�ò���������Ⱦ��ͨ�����á���ʼ��  

    //ͼ���ֵ��
    YMCV_Image_ChannelRenderTo(myImage0_gray, myImage1, myRender);//ʹ��ͨ����Ⱦ��������ȾͼƬ

		//��ʾͼƬ1
		LCD_show_image(stx + 0, myImage0_gray->height, myImage1);
		//��Ϣ��ʾ
	  sprintf(myInform, "Binary thresh:[%3d,%3d] ",myParam.gray_2tobin.leftThresh,myParam.gray_2tobin.rightThresh);
		LCD_ShowString(0, myImage0_gray->height, myInform);
		//��ͣ300ms
		Delay(300);

    //�Զ�ֵͼ���о���任
    CVIMAGE myImage2 = YMCV_Binary_Distance_Transform(myImage1);

    //��ʾͼƬ2
		LCD_show_image(stx + 0, myImage0_gray->height, myImage2);
		//��Ϣ��ʾ
	  sprintf(myInform, "Binary distance Transform");
		LCD_ShowString(0, myImage0_gray->height, myInform);
		//��ͣ300ms
		Delay(300);

    //ͨ����Ⱦ���ڴ��ͷ�
    YMCV_Free_PIX_RenderFree(myRender);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
}









