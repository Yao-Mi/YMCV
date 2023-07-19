
/*******************************************************************************************
    *
    * ˵��  �������� ��ֵͼ��̬ѧ�˲�Ϊ����չʾ�˶�ֵͼ�� �����ͺ͸�ʴЧ��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage0_gb = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_gb, CV_Rgb16ToGray);//rgbת�Ҷ�ͼ
    //���ٶ�ֵ��
	  myImage0_gb->type = CVBinaryType;
    for (int i = 0,imax = myImage0_gb->width*myImage0_gb->height ; i < imax; i++)
    {
        myImage0_gb->data[i] = (myImage0_gb->data[i] > 100) ? 255 : 0;
    }
    
    //��ʾԭͼ
    LCD_show_image(stx + 0, 0, myImage0_gb);


    //�����ռ���Ⱦ��
    uint8 kernel_d = 0;
    CVFMAT myKernel;

    //���ڻ�����
    CVIMAGE myImage1 = NULL;

    /*--------------------------��̬ѧ�����˲� �޸��˲��뾶--------------------------------------------------*/
    for (kernel_d = 1; kernel_d < 10; kernel_d +=2)
    {
        //������Ӧ�뾶����̬ѧ�˲���
        myKernel = YMCV_Creat_FMAT_Creat(kernel_d, kernel_d);

        //�ֶ����û��к˽ṹ
        CV_memset(myKernel->datap,0, myKernel->row * myKernel->col*sizeof(float));//ȫ����ʼΪ0
        //ѡ����Ҫ���еĲ��ּ�Ϊ1
        for (int i = 0; i < myKernel->row; i++)
        {
            for (int j = 0; j < myKernel->col; j++)
                myKernel->datap[i * myKernel->col + j] = 1.0f;
        }
        //������̬ѧ����Ч��
        myImage1 = YMCV_Binary_Morph_Hit_Cal(myImage0_gb, myKernel,1);//���������>1% �Ľ��������Ч��
       
        //��ʾͼƬ1
        LCD_show_image(stx + 0,  myImage0_gb->height, myImage1);

        //��Ϣ��ʾ
        sprintf(myInform, "morp Dilation radius: %02d   . ", kernel_d);
        LCD_ShowString(stx + 0,  myImage0_gb->height, myInform);
        //��ͣ300ms
		    Delay(300);

        //���ͷ�ͼ�񣬱����´��õ�ʱ�����ڴ�й©
        YMCV_Free_ImgFree(myImage1);
        //�ͷ���Ⱦ��
        YMCV_Free_FMatFree(myKernel);
    }

    ///*--------------------------��̬ѧ��ʴ�˲� �޸��˲��뾶--------------------------------------------------*/
    for (kernel_d = 1; kernel_d < 10; kernel_d +=2)
    {
        //������Ӧ�뾶����̬ѧ�˲���
        myKernel = YMCV_Creat_FMAT_Creat(kernel_d, kernel_d);

        //�ֶ����û��к˽ṹ
        CV_memset(myKernel->datap, 0, myKernel->row * myKernel->col * sizeof(float));//ȫ����ʼΪ0
        //ѡ����Ҫ���еĲ��ּ�Ϊ1
        for (int i = 0; i < myKernel->row; i++)
        {
            for (int j = 0; j < myKernel->col; j++)
            {
                myKernel->datap[i * myKernel->col + j] = 1.0f;
            }
        }

        //������̬ѧ���н��
        myImage1 = YMCV_Binary_Morph_Hit_Cal(myImage0_gb, myKernel,99);//���������>99%�Ĳ��� ����ʴЧ��

        //��ʾͼƬ1
        LCD_show_image(stx + 0,  myImage0_gb->height, myImage1);

        //��Ϣ��ʾ
        sprintf(myInform, "morp Erosion radius: %02d   . ", kernel_d);
        LCD_ShowString(stx + 0,  myImage0_gb->height, myInform);
        //��ͣ300ms
		    Delay(300);

        //���ͷ�ͼ�񣬱����´��õ�ʱ�����ڴ�й©
        YMCV_Free_ImgFree(myImage1);
        //�ͷ���Ⱦ��
        YMCV_Free_FMatFree(myKernel);
    }
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gb);
}





