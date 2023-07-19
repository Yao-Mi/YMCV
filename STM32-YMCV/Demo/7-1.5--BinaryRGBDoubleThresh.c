
/*******************************************************************************************
    *
    * ˵��  �������� RGB˫��ֵΪ����չʾ��RGB��ֵ��Ч��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];

		//��ʾͼƬ0
		LCD_show_image(stx + 0, 0, myImage0);
    /*--------------------------RGB˫��ֵ��ֵ��--------------------------------------------------*/
    //���������ֵͼ��
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);//��ֵͼ
    
	  //��ʼ����ֵ����
	  CV_binary_param myParam = { .rgb_double_t.thresh_Rmin = 0 ,.rgb_double_t.thresh_Rmax = 255 , 
     .rgb_double_t.thresh_Gmin = 0 ,.rgb_double_t.thresh_Gmax = 255 ,
     .rgb_double_t.thresh_Bmin = 0 ,.rgb_double_t.thresh_Bmax = 255 };

    for (int i = 1; i < 10; i++)
    {
        //���ò���
        myParam.rgb_double_t.thresh_Rmin = 20 * i;
        myParam.rgb_double_t.thresh_Rmax = 255;

        myParam.rgb_double_t.thresh_Gmin = 10 * i;
        myParam.rgb_double_t.thresh_Gmax = 255;

        myParam.rgb_double_t.thresh_Bmin = 15 * i;
        myParam.rgb_double_t.thresh_Bmax = 255;

        //������ֵ�任��Ⱦ��
        CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_RGB2Threshold);

        //���ز���������Ⱦ���á���ʼ��
        YMCV_BinaryKernel_Load(myRender1, &myParam);

        //��Ŀ��ͼ��Ľ�����Ⱦ
        YMCV_Image_ThreshTo_Binary(myImage0, myImage2, myRender1);

        //��ʾͼƬ2
        LCD_show_image(stx + 0, myImage0->height, myImage2);

        //��Ϣ��ʾ
        sprintf(myInform, "RGB thresh:\nR[%3d %3d]\nG[%d %d]\nB[%d %d]",
            myParam.rgb_double_t.thresh_Rmin,
            myParam.rgb_double_t.thresh_Rmax,
            myParam.rgb_double_t.thresh_Gmin,
            myParam.rgb_double_t.thresh_Gmax,
            myParam.rgb_double_t.thresh_Bmin,
            myParam.rgb_double_t.thresh_Bmax);

        LCD_ShowString(0, myImage0->height, myInform);
        //��ͣ300ms
				Delay(300);

        //�ͷ���Ⱦ���������´��õ�ʱ�����ڴ�й©
        YMCV_Free_Binary_RenderFree(myRender1);
    }

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage2);

}





