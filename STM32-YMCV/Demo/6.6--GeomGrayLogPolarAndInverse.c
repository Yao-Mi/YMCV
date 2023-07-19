
/*******************************************************************************************
    *
    * ˵��  �������� ͼ������任Ϊ����չʾ��ͼƬ�����������µ�ͶӰЧ��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int sty = 16;
    char myInform[100];
	
    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage0_gray = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_gray, CV_Rgb16ToGray);//rgbת�Ҷ�ͼ

		//��ʾͼƬ0
    LCD_Clear(0);
		LCD_show_image(0, sty + 0, myImage0_gray);
		//��ͣ500ms
		Delay(500);
	
    /*--------------------------ͶӰ��������������--------------------------------------------------*/
    CVIMAGE myImage2 = NULL, myImage3 = NULL;
    YMCV_PADDING_VAL = 50;//�հ������ֵ
    //����ͼƬ0����Ⱦ����
    CV_geom_param myParam = { .polar.Imgin_w = myImage0_gray->width,
                              .polar.Imgin_h = myImage0_gray->height };
    //�������α任��Ⱦ��
    CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_LogPolar);

    //ʹ�ò������м�����Ⱦ���á���ʼ��
    YMCV_GeomKernel_Load(myRender1, &myParam);

    //��Ŀ��ͼ��Ľ�����Ⱦ
    myImage2 = YMCV_Image_Geom_RenderTo(myImage0_gray, myRender1);

    //��ʾͼƬ2
		{
			CV_geom_param stretchParam = {.stretch.Imgin_w= myImage2->width,
															 .stretch.Imgin_h = myImage2->height, 
															 .stretch.out_w = myImage2->width/2,  //���ŵ�һ���С
															 .stretch.out_h = myImage2->height/2, };
			//�������ź���ʾ
			CVGEOM_RENDER stretchRender = YMCV_Creat_GeomRender_Creat(CV_Stretch);	
			YMCV_GeomKernel_Load(stretchRender, &stretchParam);
			CVIMAGE stretchImage = YMCV_Image_Geom_RenderTo(myImage2, stretchRender);														 
														
			LCD_show_image(0, sty + 0, stretchImage);
		  
			//�ͷ���ʱ�ڴ�
			YMCV_Free_GEOM_RenderFree(stretchRender);
			YMCV_Free_ImgFree(stretchImage);
		}

    //��Ϣ��ʾ
    sprintf(myInform, "log polar   ");
    LCD_ShowString(0, 0, myInform);
    //��ͣ500ms
		Delay(500);
		
    /*--------------------------ͶӰ�صѿ�������ϵ--------------------------------------------------*/
    //����ͼƬ2����Ⱦ����
    myParam.rotate.Imgin_w = myImage2->width;
    myParam.rotate.Imgin_h = myImage2->height;
    //�������α任��Ⱦ��
    CVGEOM_RENDER myRender2 = YMCV_Creat_GeomRender_Creat(CV_LogPolar_Inverse);

    //ʹ�ò������м�����Ⱦ���á���ʼ��
    YMCV_GeomKernel_Load(myRender2, &myParam);

    //��Ŀ��ͼ��Ľ�����Ⱦ
    myImage3 = YMCV_Image_Geom_RenderTo(myImage2, myRender2);
    //���ͼ���Ȼ�����������У���Ϊ������һ�����ĵ�

    //��ʾͼƬ3
    LCD_show_image(0, sty + 0, myImage3);

    //��Ϣ��ʾ
    sprintf(myInform, "invers log polar ");
    LCD_ShowString(0, 0, myInform);
		//��ͣ500ms
		Delay(500);
		
    //�ͷ���Ⱦ��
    YMCV_Free_GEOM_RenderFree(myRender1);
    YMCV_Free_GEOM_RenderFree(myRender2);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);
    YMCV_Free_ImgFree(myImage0_gray);
}





