
/*******************************************************************************************
    *
    * 说明  该例程以反色、亮度增强为例，展示了使用 单通道渲染器 渲染彩色图像
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

	  //显示图片0
    LCD_Clear(0);
    LCD_show_image(stx + 0, 0, myImage0);

    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVRgb16Type);


    /*--------------------------彩色图亮度增强--------------------------------------------------*/

    //创建通道渲染器
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Bright, PIXChannelNum1);//图像亮度增强
    //设置渲染器参数
    CV_pixchannel_1param myParam = { .gray_bright._k = 0.0f };
    for (int i = 0; i < 10; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_SingleChannel_MapTo(myRender, &myParam);

        //使用通道渲染器进行渲染彩色图片
        YMCV_Image_ChannelRenderTo_UGray(myImage0, myImage1, myRender);
        //显示图片1
        LCD_show_image(stx + 0, myImage0->height, myImage1);

        //信息显示
        sprintf(myInform, "bright: %01.2f .", myParam.gray_bright._k);
        LCD_ShowString(stx + 0, myImage1->height, myInform);
			
        //暂停200ms
        Delay(200);

        //参数调整
        myParam.gray_bright._k += 0.5f;
    }
    //释放渲染器
    YMCV_Free_PIX_RenderFree(myRender);


    /*--------------------------彩色图反色--------------------------------------------------*/

    //重新创建一个通道渲染器
    myRender = YMCV_Creat_PixRender_Creat(CV_Invers, PIXChannelNum1);//灰度反色
    //设置渲染器参数 取值在[0.0 -- 1.0]区间 或者 [0 --- 255]区间
    CV_pixchannel_1param myParam1 = { .gray_invers._k = 0.0f };
    for (int i = 0; i < 10; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_SingleChannel_MapTo(myRender, &myParam1);

        //使用通道渲染器进行渲染彩色图片
        YMCV_Image_ChannelRenderTo_UGray(myImage0, myImage1, myRender);
        //显示图片1
        LCD_show_image(stx + 0, myImage0->height, myImage1);

        //信息显示
        sprintf(myInform, "invers: %01.2f .", myParam1.gray_invers._k);
        LCD_ShowString(stx + 0, myImage1->height, myInform);
			
        //暂停200ms
        Delay(200);

        //参数调整
        myParam1.gray_invers._k += 0.50f;//[0.0 -- 1.0]
    }
    //释放渲染器
    YMCV_Free_PIX_RenderFree(myRender);


    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage1);

}





