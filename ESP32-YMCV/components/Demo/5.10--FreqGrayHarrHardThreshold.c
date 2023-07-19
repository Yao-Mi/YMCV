
/*******************************************************************************************
    *
    * 说明  该例程以 图像dwt为例，展示了将一幅图像进行dwt变换及其逆变换的结果，
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
    //暂停500ms
    Delay(500);
		
    //频域渲染器
    CVDWT2D_RENDER myKernel;
    YMCV_PADDING_VAL = 125;//设置填充值

    /*--------------------------灰度图像dwt变换--------------------------------------------------*/

    //创建待处理图像的专属 dwt滤波核
    myKernel = YMCV_Creat_DWT2D_Render_Creat(myImage0_gray->width, myImage0_gray->height);
    //使用滤波器创建dwt输出图像，必须使用滤波器计算后的参数来创建,以满足滤波器需求,类型必须float类型
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myKernel->width, myKernel->height, CVFGrayType);
    //进行DWT变换
    YMCV_Gray_Image_DWT(myKernel, myImage0_gray, myImage1);//结果存在myImage1

    /*--------------------------变换图像结果展示--------------------------------------------------*/
    //显示频谱幅度图片2
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage1->width, myImage1->height, myImage0_gray->type);
    YMCV_Image_Pix_Change_Form(myImage1, myImage2, CV_FGrayToGray);

		LCD_show_image(stx + 0, 16, myImage2);
    sprintf(myInform, "dwt result  "); //信息显示
    LCD_ShowString(stx + 0, 0, myInform);

    //暂停800ms
    Delay(800);
    /*--------------------------频域图像滤波--------------------------------------------------*/
    //创建哈尔小波硬阈值波器
    CVFREQ_RENDER freqKernl = YMCV_Creat_FreqKernel_Render_Creat(CV_HarrThreshFilter, 0);
    //加载滤波器参数
    CV_freq_1param myparam = { .harr.threshold= 20 };
    YMCV_Freq_Kernel_Load(freqKernl, myparam);
    //对幅度进行滤波
    YMCV_Gray_Image_Freq_Kernel_Render(myImage1, freqKernl);

    /*--------------------------图像idwt变换--------------------------------------------------*/
     //创建idwt输出的结果图像，与原始图一致即可
    CVIMAGE myImage3 = YMCV_Creat_Img_Creat(myImage1->width, myImage1->height, myImage0_gray->type);
    //进行iDWT变换
    YMCV_Gray_Image_IDWT(myKernel, myImage3, myImage1);//结果存回myImage3
    //显示图片3
    LCD_show_image(stx + 0, 16, myImage3);       
    sprintf(myInform, "idwt filter result  "); //信息显示
    LCD_ShowString(stx + 0, 0, myInform);
    //暂停500ms
    Delay(500);


    //释放DWT渲染器
    YMCV_Free_DWT2D_RenderFree(myKernel);
    YMCV_Free_FreqKernel_RenderFree(freqKernl);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_g);
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);
}








