
/*******************************************************************************************
    *
    * 说明  该例程以 导向滤波为例，展示了将一幅图像 的引导滤波效果，
    *       通过一张引导图G，对目标图像P（输入图像）进行滤波处理，
    *       使得最后的输出图像大体上与目标图像P相似，但是纹理部分与引导图G相似
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

    //创建半径 = 6的最大值滤波核
    myKernel = YMCV_Creat_SpaceRender_Creat(CV_MaxFilter, PIXChannelNum1, 5, OutPaddingConst);
    //使用最大值滤波得到较优目标图像
    CVIMAGE myImage1 = YMCV_Image_Space_SigleKernel_Render(myImage0_gray, myKernel);
    //显示图片1
    LCD_show_image(stx + 0,  myImage0_gray->height, myImage1);

		//信息显示
		sprintf(myInform, "guidef image");
		LCD_ShowString(stx + 0,  myImage0_gray->height, myInform);
		//暂停1000ms
		Delay(1000);

    CVIMAGE myImage2 = NULL;
    /*--------------------------导向滤波 修改滤波半径--------------------------------------------------*/
    for (kernel_r = 1; kernel_r < 10; kernel_r++)
    {
        //将目标图像的纹理引导回原图，使用原图作为纹理引导图像进行引导滤波
        myImage2 = YMCV_Gray_Image_GuideFilter(myImage1, myImage0_gray, OutPaddingConst, kernel_r,0.001);
        //显示图片1
        LCD_show_image(stx + 0,  myImage0_gray->height, myImage2);

        //信息显示
        sprintf(myInform, "guidef radius: %02d   ", kernel_r);
        LCD_ShowString(stx + 0,  myImage0_gray->height, myInform);
        //暂停300ms
        Delay(300);

        //用释放图像，避免下次用的时候导致内存泄漏
        YMCV_Free_ImgFree(myImage2);
    }


    //释放空间渲染器
    YMCV_Free_KERN_RenderFree(myKernel);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage1);

}








