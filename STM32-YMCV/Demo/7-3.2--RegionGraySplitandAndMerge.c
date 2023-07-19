
/*******************************************************************************************
    *
    * ˵��  �������� ���� �ϲ�Ϊ����չʾ��һ��ͼ��ֱ���з�����ϲ��Ĵ���Ч��
    *           ��ʵ�������������Ǵ���ʹ�ã������ȶ�ͼ�����������ѣ�Ȼ��Խ��ͼ��������ϲ����Ӷ��õ��ȽϹ�����ͼ��
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


    /*--------------------------�Ҷ� �������--------------------------------------------------*/
    for (int i = 0; i < 8; i++)
    {
        int thresh = i * 20;

        CVIMAGE myImage2 = YMCV_Gray_Region_Splitand(myImage0_gray, thresh);//����

        //��ʾͼƬ2
        LCD_show_image(stx + 0, myImage0_gray->height, myImage2);

        //��Ϣ��ʾ
			  sprintf(myInform, "RegionSplit thresh:%3d ",thresh);
				LCD_ShowString(stx + 0, myImage0_gray->height, myInform);
        //��ͣ300ms
        Delay(300);

        //�ͷ��ڴ�
        YMCV_Free_ImgFree(myImage2);
    }
   

    /*--------------------------�Ҷ� ����ϲ�--------------------------------------------------*/
    for (int i = 0; i < 8; i++)
    {
        int thresh = i;

        CVIMAGE myImage2 = YMCV_Gray_Region_Merge(myImage0_gray, thresh);//�ϲ�

        //��ʾͼƬ2
        LCD_show_image(stx + 0, myImage0_gray->height, myImage2);

        //��Ϣ��ʾ
			  sprintf(myInform, "RegionMerge thresh:%3d ",thresh);
				LCD_ShowString(stx + 0, myImage0_gray->height, myInform);
        //��ͣ300ms
        Delay(300);

        //�ͷ��ڴ�
        YMCV_Free_ImgFree(myImage2);
    }

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);

}









