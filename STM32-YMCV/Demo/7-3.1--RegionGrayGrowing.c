
/*******************************************************************************************
    *
    * ˵��  �������� ��������Ϊ����չʾ��mask��ֵͼ����
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];
	
    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage0_gray = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_gray, CV_Rgb16ToGray);//rgbת�Ҷ�ͼ
		
    /*--------------------------�Ҷ���������--------------------------------------------------*/
    int x = 40, y = 60;

    for (int i = 0; i < 10; i++)
    {
        int thresh = 30;
        //�������� ����mask��ֵͼ
        CVIMAGE myImage2 = YMCV_Gray_Region_Growing8(myImage0_gray, x, y, thresh);
			
				//��ʾͼƬ0
				LCD_show_image(stx + 0, 0, myImage0_gray);
        //�������ӵ�λ��
        color24_t cvl = { .R = 255,.G = 120,.B = 50 };
        LCD_Fill_ConstRgbRect(stx + x - 1, y - 1, 3, 3, cvl);
        //��ʾͼƬ2
        LCD_show_image(stx + 0, myImage0_gray->height, myImage2);

        //��Ϣ��ʾ
        sprintf(myInform, "region growing: \nseed(%3d %3d)\ngrowThresh:%d",x,y, thresh);
				LCD_ShowString(0, myImage0_gray->height, myInform);
        //��ͣ300ms
        Delay(300);
    
        x += 8;//�ƶ����ӵ�

        //�ͷ�ͼ���ڴ�
        YMCV_Free_ImgFree(myImage2);
    }

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
}









