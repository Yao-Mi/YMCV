
/*******************************************************************************************
    *
    * 说明  该例程以 图像fft为例，展示了将一幅图像进行巴特沃斯高通滤波的结果，
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage0_g = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_g, CV_Rgb16ToGray);//rgb转灰度图
	  
	  //取120x120的中间区域，结果fft变换后为128x128，可以在240x240的屏幕上显示
	  CVrect myroi = { .x = 80 - 60,.y = 60 - 60,.w = 120,.h = 120 };
    CVIMAGE myImage0_gray = YMCV_Creat_ImgROICopyTo(myImage0_g, &myroi);		
    
		//显示原图
		LCD_Clear(WHITE);
    LCD_show_image(stx + 0, 16, myImage0_gray);
    //暂停1000ms
    Delay(1000);
		
    //频域渲染器
    CVFFT2D_RENDER myKernel;
    YMCV_PADDING_VAL = 125;//设置填充值

    /*--------------------------图像fft变换及shift变换--------------------------------------------------*/

    //创建待处理图像的专属 fft滤波核
    myKernel = YMCV_Creat_FFT2D_Render_Creat(myImage0_gray->width, myImage0_gray->height);
    //使用滤波器创建fft输出图像，最好使用滤波器计算后的参数来创建,类型必须complex类型
    //当然，若不使用滤波器的宽度和高度进行创建，图像也会自动重新规划大小，以满足滤波器需求
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0_gray->width, myImage0_gray->height, CVCGrayType);
    //进行FFT变换
    YMCV_Gray_Image_FFT(myKernel, myImage0_gray, myImage1);//结果存在myImage1

    //fftshift变换
    YMCV_CGray_Image_FFTshift(myImage1);

    //显示频谱图片2
    YMCV_CGray_Image_LogMag_AndPhase(myImage1);//先转幅度和相位
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage1->width, myImage1->height, myImage0_gray->type);
    YMCV_Image_Pix_Change_Form(myImage1, myImage2, CV_CGrayToGray2);

    LCD_show_image(stx + 0, 16, myImage2);
    sprintf(myInform, "fft result -> shift"); //信息显示
    LCD_ShowString(stx + 0, 0, myInform);

    //暂停800ms
    Delay(800);

    /*--------------------------频域图像滤波--------------------------------------------------*/
    //创建巴特沃斯高通滤波器,复数格式为幅度与相位
    CVFREQ_RENDER freqKernl = YMCV_Creat_FreqKernel_Render_Creat(CV_ButterWorthHFilter, CV_RhoAndTheta);
    //加载滤波器参数
    CV_freq_1param myparam = { .butterworth.D0 = 15,.butterworth.n=2 };
    YMCV_Freq_Kernel_Load(freqKernl, myparam);
    //对幅度进行滤波
    YMCV_Gray_Image_Freq_Kernel_Render(myImage1, freqKernl);

    //显示滤波后的结果
    YMCV_Image_Pix_Change_Form(myImage1, myImage2, CV_CGrayToGray2);

    LCD_show_image(stx + 0, 16, myImage2);
    sprintf(myInform, "butterworth high filter  "); //信息显示
    LCD_ShowString(stx + 0, 0, myInform);

    //暂停800ms
    Delay(800);
    /*--------------------------频域图像ifftshift变换与ifft变换--------------------------------------------------*/
    YMCV_CGray_Image_expMag_ToComplex(myImage1);//幅频图转复数图
    //ifftshift变换 对于偶数宽高图像，shift与ishift操作相同
    YMCV_CGray_Image_FFTshift(myImage1);


    //创建ifft输出的结果图像，与原始图一致即可
    CVIMAGE myImage3 = YMCV_Creat_Img_Creat(myImage1->width, myImage1->height, myImage0_gray->type);
    //进行iFFT变换
    YMCV_Gray_Image_IFFT(myKernel, myImage3, myImage1);//结果存回myImage3
    //显示图片3
    LCD_show_image(stx + 0, 16, myImage3);       
    sprintf(myInform, "ishift -> ifft result  "); //信息显示
    LCD_ShowString(stx + 0, 0, myInform);
    //暂停500ms
    Delay(500);

    //释放FFT渲染器
    YMCV_Free_FFT2D_RenderFree(myKernel);
		YMCV_Free_FreqKernel_RenderFree(freqKernl);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_g);
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);

}








