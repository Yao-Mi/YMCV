
/*******************************************************************************************
    *
    * 说明  该例程以 高斯滤波 为例，展示了将一幅图像的  显示
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage1, CV_Rgb16ToGray);//rgb转灰度图
	
    //显示原图
    LCD_show_image(stx + 0, 0, myImage1);

    //创建空间渲染器
    uint8 kernel_r = 5;
    CVKERN_RENDER myKernel = YMCV_Creat_SpaceRender_Creat(CV_GaussFilter, PIXChannelNum1, kernel_r, OutPaddingConst);
    YMCV_PADDING_VAL = 125;//设置填充值


    //加载渲染器参数，进行初始化
    CV_kernel_1param myParam = { .gauss.sigma = 1.0 };

    //由于缓存结果
    CVIMAGE myImage2 = NULL;

    /*--------------------------高斯滤波修改高斯方差---------------------------------------------*/
    for (int i = 0; i < 10; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_Space_SingleKernel_Load(myKernel, &myParam);

        //使用空间渲染器进行渲染灰度图片
        myImage2 = YMCV_Image_Space_SigleKernel_Render(myImage1, myKernel);
        //显示图片2
        LCD_show_image(stx + 0,  myImage1->height, myImage2);

        //用完就释放图像，避免下次用的时候导致内存泄漏
        YMCV_Free_ImgFree(myImage2);

        //信息显示
        sprintf(myInform, "gauss sigma: %01.2f ", myParam.gauss.sigma);
        LCD_ShowString(stx + 0, myImage1->height, myInform);
        //暂停300ms
        Delay(300);

        //参数调整
        myParam.gauss.sigma += 0.5f;
    }
    //释放渲染器
    YMCV_Free_KERN_RenderFree(myKernel);
    
    /*--------------------------高斯滤波修改滤波半径--------------------------------------------------*/
    for (kernel_r = 0; kernel_r < 10; kernel_r++)
    {
        //创建相应半径的滤波核
        myKernel = YMCV_Creat_SpaceRender_Creat(CV_GaussFilter, PIXChannelNum1, kernel_r, OutPaddingConst);

        //使用参数进行渲染器通道配置、初始化
        YMCV_Space_SingleKernel_Load(myKernel, &myParam);

        //使用空间渲染器进行渲染灰度图片
        myImage2 = YMCV_Image_Space_SigleKernel_Render(myImage1, myKernel);
        //显示图片2
        LCD_show_image(stx + 0,  myImage1->height, myImage2);


        //信息显示
        sprintf(myInform, "gauss radius: %02d      ", kernel_r);
        LCD_ShowString(stx + 0,  myImage1->height, myInform);
        //暂停300ms
        Delay(300);

        //用释放图像，避免下次用的时候导致内存泄漏
        YMCV_Free_ImgFree(myImage2);
        //释放空间渲染器
        YMCV_Free_KERN_RenderFree(myKernel);
    }

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage1);

}





