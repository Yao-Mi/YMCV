
/*******************************************************************************************
    *
    * ˵��  �������� ͼ��fftΪ����չʾ�˽�һ��ͼ����а�����˹��ͨ�˲��Ľ����
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
    //��ͣ1000ms
    Delay(1000);
		
    //Ƶ����Ⱦ��
    CVFFT2D_RENDER myKernel;
    YMCV_PADDING_VAL = 125;//�������ֵ

    /*--------------------------ͼ��fft�任��shift�任--------------------------------------------------*/

    //����������ͼ���ר�� fft�˲���
    myKernel = YMCV_Creat_FFT2D_Render_Creat(myImage0_gray->width, myImage0_gray->height);
    //ʹ���˲�������fft���ͼ�����ʹ���˲��������Ĳ���������,���ͱ���complex����
    //��Ȼ������ʹ���˲����Ŀ�Ⱥ͸߶Ƚ��д�����ͼ��Ҳ���Զ����¹滮��С���������˲�������
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0_gray->width, myImage0_gray->height, CVCGrayType);
    //����FFT�任
    YMCV_Gray_Image_FFT(myKernel, myImage0_gray, myImage1);//�������myImage1

    //fftshift�任
    YMCV_CGray_Image_FFTshift(myImage1);

    //��ʾƵ��ͼƬ2
    YMCV_CGray_Image_LogMag_AndPhase(myImage1);//��ת���Ⱥ���λ
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage1->width, myImage1->height, myImage0_gray->type);
    YMCV_Image_Pix_Change_Form(myImage1, myImage2, CV_CGrayToGray2);

    LCD_show_image(stx + 0, 16, myImage2);
    sprintf(myInform, "fft result -> shift"); //��Ϣ��ʾ
    LCD_ShowString(stx + 0, 0, myInform);

    //��ͣ800ms
    Delay(800);

    /*--------------------------Ƶ��ͼ���˲�--------------------------------------------------*/
    //����������˹��ͨ�˲���,������ʽΪ��������λ
    CVFREQ_RENDER freqKernl = YMCV_Creat_FreqKernel_Render_Creat(CV_ButterWorthHFilter, CV_RhoAndTheta);
    //�����˲�������
    CV_freq_1param myparam = { .butterworth.D0 = 15,.butterworth.n=2 };
    YMCV_Freq_Kernel_Load(freqKernl, myparam);
    //�Է��Ƚ����˲�
    YMCV_Gray_Image_Freq_Kernel_Render(myImage1, freqKernl);

    //��ʾ�˲���Ľ��
    YMCV_Image_Pix_Change_Form(myImage1, myImage2, CV_CGrayToGray2);

    LCD_show_image(stx + 0, 16, myImage2);
    sprintf(myInform, "butterworth high filter  "); //��Ϣ��ʾ
    LCD_ShowString(stx + 0, 0, myInform);

    //��ͣ800ms
    Delay(800);
    /*--------------------------Ƶ��ͼ��ifftshift�任��ifft�任--------------------------------------------------*/
    YMCV_CGray_Image_expMag_ToComplex(myImage1);//��Ƶͼת����ͼ
    //ifftshift�任 ����ż�����ͼ��shift��ishift������ͬ
    YMCV_CGray_Image_FFTshift(myImage1);


    //����ifft����Ľ��ͼ����ԭʼͼһ�¼���
    CVIMAGE myImage3 = YMCV_Creat_Img_Creat(myImage1->width, myImage1->height, myImage0_gray->type);
    //����iFFT�任
    YMCV_Gray_Image_IFFT(myKernel, myImage3, myImage1);//������myImage3
    //��ʾͼƬ3
    LCD_show_image(stx + 0, 16, myImage3);       
    sprintf(myInform, "ishift -> ifft result  "); //��Ϣ��ʾ
    LCD_ShowString(stx + 0, 0, myInform);
    //��ͣ500ms
    Delay(500);

    //�ͷ�FFT��Ⱦ��
    YMCV_Free_FFT2D_RenderFree(myKernel);
		YMCV_Free_FreqKernel_RenderFree(freqKernl);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_g);
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);

}








