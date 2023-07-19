
/*******************************************************************************************
    *
    * 说明  该例程以 表面模糊滤波为例，展示了将一幅图像进行滤波的滤波效果、以及参数的使用
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
    uint8 kernel_r = 1;
    uint8 T_v = 20;
    YMCV_PADDING_VAL = 50;//输出边界填充值

    //由于缓存结果
    CVIMAGE myImage1 = NULL;

    /*--------------------------表面模糊滤波 修改滤波半径--------------------------------------------------*/
    for (kernel_r = 1; kernel_r < 10; kernel_r++)
    {
        //使用表面模糊进行滤波
        myImage1 = YMCV_Gray_SurfaceBlur(myImage0_gray, kernel_r, T_v, OutPaddingConst);
        //显示图片1
        LCD_show_image(stx + 0,  myImage0_gray->height, myImage1);

        //信息显示
        sprintf(myInform, "surfaceBL raadius: %02d\n      thresh:%d  ", kernel_r, T_v);
        LCD_ShowString(stx +0,  myImage0_gray->height, myInform);
        //暂停300ms
        Delay(300);

        //用释放图像，避免下次用的时候导致内存泄漏
        YMCV_Free_ImgFree(myImage1);
    }


    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_gray);

}








