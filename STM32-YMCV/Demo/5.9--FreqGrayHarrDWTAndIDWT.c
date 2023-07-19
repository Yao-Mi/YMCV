
/*******************************************************************************************
    *
    * ˵��  �������� ͼ��dwtΪ����չʾ�˽�һ��ͼ�����dwt�任������任�Ľ����
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage0_g = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_g, CV_Rgb16ToGray);//rgbת�Ҷ�ͼ
	  
	  //ȡ120x120���м����򣬽��fft�任��Ϊ128x128��������240x240����Ļ����ʾ
	  CVrect myroi = { .x = 80 - 60,.y = 60 - 60,.w = 120,.h = 120 };
    CVIMAGE myImage0_gray = YMCV_Creat_ImgROICopyTo(myImage0_g, &myroi);		
    
		//��ʾԭͼ
		LCD_Clear(WHITE);
    LCD_show_image(stx + 0, 16, myImage0_gray);
    //��ͣ500ms
    Delay(500);
		
    //Ƶ����Ⱦ��
    CVDWT2D_RENDER myKernel;
    YMCV_PADDING_VAL = 125;//�������ֵ

    /*--------------------------�Ҷ�ͼ��dwt�任--------------------------------------------------*/

    //����������ͼ���ר�� dwt�˲���
    myKernel = YMCV_Creat_DWT2D_Render_Creat(myImage0_gray->width, myImage0_gray->height);
    //ʹ���˲�������dwt���ͼ�񣬱���ʹ���˲��������Ĳ���������,�������˲�������,���ͱ���float����
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myKernel->width, myKernel->height, CVFGrayType);
    //����DWT�任
    YMCV_Gray_Image_DWT(myKernel, myImage0_gray, myImage1);//�������myImage1

    /*--------------------------�任ͼ����չʾ--------------------------------------------------*/
    //��ʾƵ�׷���ͼƬ2
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage1->width, myImage1->height, myImage0_gray->type);
    YMCV_Image_Pix_Change_Form(myImage1, myImage2, CV_FGrayToGray);

		LCD_show_image(stx + 0, 16, myImage2);
    sprintf(myInform, "dwt result  "); //��Ϣ��ʾ
    LCD_ShowString(stx + 0, 0, myInform);

    //��ͣ800ms
    Delay(800);

    /*--------------------------ͼ��idwt�任--------------------------------------------------*/

     //����idwt����Ľ��ͼ����ԭʼͼһ�¼���
    CVIMAGE myImage3 = YMCV_Creat_Img_Creat(myImage1->width, myImage1->height, myImage0_gray->type);
    //����iDWT�任
    YMCV_Gray_Image_IDWT(myKernel, myImage3, myImage1);//������myImage3
    //��ʾͼƬ3
    LCD_show_image(stx + 0, 16, myImage3);       
    sprintf(myInform, "idwt result  "); //��Ϣ��ʾ
    LCD_ShowString(stx + 0, 0, myInform);
    //��ͣ500ms
    Delay(500);


    //�ͷ�DWT��Ⱦ��
    YMCV_Free_DWT2D_RenderFree(myKernel);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_g);
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);
}








