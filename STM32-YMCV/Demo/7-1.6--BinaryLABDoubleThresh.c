
/*******************************************************************************************
    *
    * ˵��  �������� LAB��ֵ�ָ�Ϊ����չʾ��LAB��ֵ��Ч��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];

		//��ʾͼƬ0
		LCD_show_image(stx + 0, 0, myImage0);
    /*--------------------------��ɫLAB˫��ֵ��ֵ��--------------------------------------------------*/
    //���������ֵͼ��
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);//��ֵͼ
    //��ֵ������ʼ��
  	CV_binary_param myParam = { .lab_double_t.thresh_Lmin = 0 ,.lab_double_t.thresh_Lmax = 100 ,
     .lab_double_t.thresh_Amin = -128 ,.lab_double_t.thresh_Amax = 127 ,
     .lab_double_t.thresh_Bmin = -128 ,.lab_double_t.thresh_Bmax = 127 };

    for (int i = 1; i < 10; i++)
    {
        //���ò���
        myParam.lab_double_t.thresh_Lmin = 6 * i;
        myParam.lab_double_t.thresh_Lmax = 100;

        myParam.lab_double_t.thresh_Amin = 14 * i - 128;
        myParam.lab_double_t.thresh_Amax = 127 - 12 * i;

        myParam.lab_double_t.thresh_Bmin = 14 * i - 128;
        myParam.lab_double_t.thresh_Bmax = 127 - 10 * i;

        //������ֵ�任��Ⱦ��
        CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_LAB2Threshold);

        //���ز���������Ⱦ���á���ʼ��
        YMCV_BinaryKernel_Load(myRender1, &myParam);

        //��Ŀ��ͼ��Ľ�����Ⱦ
        YMCV_Image_ThreshTo_Binary(myImage0, myImage2, myRender1);

        //��ʾͼƬ2
        LCD_show_image(stx + 0, myImage0->height, myImage2);

        //��Ϣ��ʾ
        sprintf(myInform, "LAB thresh:\nL[%d %d]\nA[%d %d]\nB[%d %d]",
            myParam.lab_double_t.thresh_Lmin,
            myParam.lab_double_t.thresh_Lmax,
            myParam.lab_double_t.thresh_Amin,
            myParam.lab_double_t.thresh_Amax,
            myParam.lab_double_t.thresh_Bmin,
            myParam.lab_double_t.thresh_Bmax);

        LCD_ShowString(0, myImage0->height, myInform);
        //��ͣ300ms
				Delay(300);

        //�ͷ���Ⱦ���������´��õ�ʱ�����ڴ�й©
        YMCV_Free_Binary_RenderFree(myRender1);
    }

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage2);
}





