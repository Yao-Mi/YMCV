
/*******************************************************************************************
    *
    * 说明  该例程以反色、亮度增强为例，展示了 通道渲染器的使用，以及参数取值 、增长步幅
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

	  //显示图片0
    LCD_show_image(stx + 0, 0, myImage0);
	
    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVRgb16Type);

    /*--------------------------彩色图亮度增强--------------------------------------------------*/

    //创建通道渲染器
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Bright, PIXChannelNum3);//图像亮度增强
    //设置渲染器参数
    CV_pixchannel_3param myParam = { .rgb_bright.R_k = 0.0f ,.rgb_bright.G_k = 0.0f ,.rgb_bright.B_k = 0.0f };
    for (int i = 0; i < 10; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_ThreeChannel_MapTo(myRender, &myParam);

        //使用通道渲染器进行渲染图片
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //显示图片1
        LCD_show_image(stx + 0, myImage0->height, myImage1);

        //信息显示
        sprintf(myInform, "bright: R%01.2f G%01.2f B%01.2f .", myParam.rgb_bright.R_k, myParam.rgb_bright.G_k, myParam.rgb_bright.B_k);
        LCD_ShowString(0, myImage1->height, myInform);
			
        //暂停200ms
        Delay(200);

        //参数调整
        myParam.rgb_bright.R_k += 0.5f;
        myParam.rgb_bright.G_k += 0.5f;
        myParam.rgb_bright.B_k += 0.5f;
    }
    //释放渲染器
    YMCV_Free_PIX_RenderFree(myRender);

    /*--------------------------彩色图反色增强--------------------------------------------------*/

    //重新创建一个通道渲染器
    myRender = YMCV_Creat_PixRender_Creat(CV_Invers, PIXChannelNum3);//反色
    //设置渲染器参数 取值在[0.0 -- 1.0]区间 或者 [0 --- 255]区间
    CV_pixchannel_3param myParam1 = { .rgb_invers.R_k = 0.0f,.rgb_invers.G_k = 0.0f,.rgb_invers.B_k = 0.0f };
    for (int i = 0; i < 10; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_ThreeChannel_MapTo(myRender, &myParam1);

        //使用通道渲染器进行渲染图片
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //显示图片1
        LCD_show_image(stx + 0, myImage0->height, myImage1);

        //信息显示
        sprintf(myInform, "invers: R%01.2f G%01.2f B%01.2f  ", myParam1.rgb_invers.R_k, myParam1.rgb_invers.G_k, myParam1.rgb_invers.B_k);
        LCD_ShowString(0, myImage1->height, myInform);
			
        //暂停200ms
        Delay(200);

        //参数调整
        myParam1.rgb_invers.R_k += 0.50f;//[0.0 -- 1.0]
        myParam1.rgb_invers.G_k += 0.50f;
        myParam1.rgb_invers.B_k += 0.50f;
    }
    //释放渲染器
    YMCV_Free_PIX_RenderFree(myRender);


    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage1);
}





