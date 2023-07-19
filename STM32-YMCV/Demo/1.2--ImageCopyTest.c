/*******************************************************************************************
    *
    * ˵��  ������չʾ�� ͼƬ�������Լ�ͼƬ�Ľ�ȡ����
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(50,50, CVRgb16Type);

    //��ȡ����ͼƬ��һ�����򿽱�������תΪһ���µ�ͼ��
    CVIMAGE myImage2 = YMCV_Creat_ImgCopyTo(myImage0, 30, 20, 70, 70);
    //����ROI�ķ�ʽ��ȡ
    CVrect myroi = { .x = 50,.y = 50,.w = 40,.h = 40 };
    CVIMAGE myImage3 = YMCV_Creat_ImgROICopyTo(myImage0, &myroi);


    //��ʾͼƬ1
    LCD_show_image(10, 60, myImage1);

    //��ʾͼƬ2
    LCD_show_image(myImage1->width + 20, 60, myImage2);

    //��ʾͼƬ3
    LCD_show_image(myImage1->width + myImage2->width + 30, 60, myImage3);


    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);
}





