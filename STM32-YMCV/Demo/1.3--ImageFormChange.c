/*******************************************************************************************
	*
	* ˵��  ������չʾ�� ͼƬ�������Լ�ͼƬ�ĸ�ʽת��
	*
	***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{

	//��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
	CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
	CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);
	
	YMCV_Image_Pix_Change_Form(myImage0, myImage1, CV_Rgb16ToGray);//rgbת�Ҷ�ͼ
	YMCV_Image_Pix_Change_Form(myImage1, myImage2, CV_GrayToDotBinary);//�Ҷ�ͼת��ֵ��ͼ
	
	//��ʾͼƬ
	LCD_show_image(40, 0, myImage1);
	LCD_show_image(40, myImage1->height, myImage2);

	//ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
	YMCV_Free_ImgFree(myImage1);
	YMCV_Free_ImgFree(myImage2);
}



