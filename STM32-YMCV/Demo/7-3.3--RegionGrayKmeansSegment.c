
/*******************************************************************************************
    *
    * ˵��  �������� Kmeans����Ϊ����չʾ��ͼ��ľ���Ч��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];
	
    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage0_gray = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_gray, CV_Rgb16ToGray);//rgbת�Ҷ�ͼ
		
    /*-------------------------- KMeans ��ֵ���� --------------------------------------------------*/
    int classnum = 4;//���þ�����Ŀ
    int maxItera=100;//������100��
    CVIMAGE myImage2 = YMCV_Gray_KMeans_Clustering(myImage0_gray, classnum, maxItera);

		//��ʾͼƬ0
		LCD_show_image(stx + 0, 0, myImage0_gray);
		//��ʾͼƬ2
		LCD_show_image(stx + 0, myImage0_gray->height, myImage2);

		//��Ϣ��ʾ
	  sprintf(myInform, "Kmeans classNum:%3d\nmax iterTimes:%3d",classnum,maxItera);
		LCD_ShowString(0, myImage0_gray->height, myInform);
		//��ͣ300ms
		Delay(300);
		
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage2);
}









