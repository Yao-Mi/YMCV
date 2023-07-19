
/*******************************************************************************************
    *
    * 说明  该例程以 均值和中值滤波为例，展示了一幅图像 的滤波效果
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage0_gray = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_gray, CV_Rgb16ToGray);//rgb转灰度图
	
    //显示原图
    LCD_show_image(stx + 0, 0, myImage0_gray);

    //创建空间渲染器
    uint8 kernel_r = 0;
    CVKERN_RENDER myKernel;
    YMCV_PADDING_VAL = 125;//设置填充值


    //加载渲染器参数，便于进行初始化
    CV_kernel_1param myParam ;//均值滤波无需加载

    //由于缓存结果
    CVIMAGE myImage1 = NULL;
 
    /*--------------------------均值滤波 修改滤波半径--------------------------------------------------*/
    for (kernel_r = 1; kernel_r < 10; kernel_r++)
    {
        //创建相应半径的滤波核
        myKernel = YMCV_Creat_SpaceRender_Creat(CV_AverFilter, PIXChannelNum1, kernel_r, OutPaddingConst);

        //使用参数进行渲染器通道配置、初始化
        YMCV_Space_SingleKernel_Load(myKernel, &myParam);//均值滤波无需加载，该步骤也可省略

        //使用空间渲染器进行渲染灰度图片
        myImage1 = YMCV_Image_Space_SigleKernel_Render(myImage0_gray, myKernel);
        //显示图片1
        LCD_show_image(stx + 0,  myImage0_gray->height, myImage1);


        //信息显示
        sprintf(myInform, "averf radius: %02d   . ", kernel_r);
        LCD_ShowString(stx + 0,  myImage0_gray->height, myInform);
        //暂停300ms
        Delay(300);

        //用释放图像，避免下次用的时候导致内存泄漏
        YMCV_Free_ImgFree(myImage1);
        //释放空间渲染器
        YMCV_Free_KERN_RenderFree(myKernel);
    }

    /*--------------------------中值滤波 修改滤波半径--------------------------------------------------*/
    for (kernel_r = 1; kernel_r < 10; kernel_r++)
    {
        //创建相应半径的滤波核
        myKernel = YMCV_Creat_SpaceRender_Creat(CV_MedFilter, PIXChannelNum1, kernel_r, OutPaddingConst);

        //使用参数进行渲染器通道配置、初始化
        YMCV_Space_SingleKernel_Load(myKernel, &myParam);//中值滤波无需加载，该步骤也可省略

        //使用空间渲染器进行渲染灰度图片
        myImage1 = YMCV_Image_Space_SigleKernel_Render(myImage0_gray, myKernel);
        //显示图片1
        LCD_show_image(stx + 0,  myImage0_gray->height, myImage1);


        //信息显示
        sprintf(myInform, "medf radius: %02d   . ", kernel_r);
        LCD_ShowString(stx + 0,  myImage0_gray->height, myInform);
        //暂停300ms
        Delay(300);

        //用释放图像，避免下次用的时候导致内存泄漏
        YMCV_Free_ImgFree(myImage1);
        //释放空间渲染器
        YMCV_Free_KERN_RenderFree(myKernel);
    }
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_gray);
}





