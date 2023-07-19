
/*******************************************************************************************
    *
    * ˵��  �������� ����������㷨Ϊ����չʾ��ֱ�߼��Ч�������㷨Ϊ�����㷨����ҵ���֪
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
	
    color24_t pencolor = { .R = 255 };
    LCD_Point_Color_Set(pencolor);//���û�����ɫ

    /*--------------------------Canny��Ե���--------------------------------------------------*/
    //���������ֵͼ��
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0_gray->width, myImage0_gray->height, CVBinaryType);//��ֵͼ
    CV_edge_param myParam = { .EdgeType = CV_Canny,.Thresh.canny.StrongT = 100,.Thresh.canny.WeakT = 40 };

    YMCV_Gray_EdgeDetect(myImage0_gray, myImage2, &myParam);//��Ե���

    LCD_show_image(stx + 0, sty + 0, myImage2);  //��ʾͼƬ2

    //��Ϣ��ʾ
    sprintf(myInform, "Canny Edge ");
    LCD_ShowString(stx + 0, sty + 0, myInform);
    Delay(300);    //��ͣ

    /*-------------------------- ���ֱ�߼���� --------------------------------------------------*/
    CVVECTORS_U16 lines = YMCV_WormFindLines(myImage2, 0.001, 10,1);
    if (lines != NULL)
    {
        for (int i = 0; i < lines->dataNum; i++)
        {
            CVline* thisline = (CVline*)(lines->datap + i * lines->dim);
            LCD_Draw_Line(stx + thisline->x1, thisline->y1, stx + thisline->x2, thisline->y2);//�����߶�
        }
    }
    //��Ϣ��ʾ
    sprintf(myInform, "Worm Lines ");
    LCD_ShowString(stx + 0, sty + 0, myInform);
		Delay(500);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_Vector_U16_Free(lines);
}





