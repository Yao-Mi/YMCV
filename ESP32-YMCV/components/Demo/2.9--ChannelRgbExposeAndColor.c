
/*******************************************************************************************
    *
    * 说明  该例程以曝光阈值、灰度染色为例，展示了 渲染器的使用，以及灰度染色的两种参数取值法
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

    /*--------------------------彩色图阈值曝光--------------------------------------------------*/

    //创建通道渲染器
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Expose, PIXChannelNum3);//图像曝光
    //设置渲染器参数
    CV_pixchannel_3param myParam = { .rgb_expose.R_thresh = 0,.rgb_expose.G_thresh = 0,.rgb_expose.B_thresh = 0 };
    for (int i = 0; i < 10; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_ThreeChannel_MapTo(myRender, &myParam);

        //使用通道渲染器进行渲染图片
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //显示图片1
        LCD_show_image(stx + 0, myImage0->height, myImage1);

        //信息显示
        sprintf(myInform, "Expose Thresh: R%03d G%03d B%03d ", myParam.rgb_expose.R_thresh, myParam.rgb_expose.G_thresh, myParam.rgb_expose.B_thresh);
        LCD_ShowString(0, myImage1->height, myInform);
        //暂停200ms
        Delay(200);

        //参数调整
        myParam.rgb_expose.R_thresh += 10;
        myParam.rgb_expose.G_thresh += 10;
        myParam.rgb_expose.B_thresh += 10;
    }
    //释放渲染器
    YMCV_Free_PIX_RenderFree(myRender);


    /*--------------------------彩色图染色法1--------------------------------------------------*/

    //重新创建一个通道渲染器
    myRender = YMCV_Creat_PixRender_Creat(CV_ColorTo, PIXChannelNum3);//彩色三通道染色
    //设置渲染器参数 取值在[0.0 -- 1.0]区间 或者 [0 --- 255]区间
    CV_pixchannel_3param myParam1 = { .rgb_colorto.R_color = 0.0 , .rgb_colorto.G_color = 0.0 , .rgb_colorto.B_color = 0.0 };
    for (int i = 0; i < 10; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_ThreeChannel_MapTo(myRender, &myParam1);

        //使用通道渲染器进行渲染图片
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //显示图片1
        LCD_show_image(stx + 0, myImage0->height, myImage1);

        //信息显示
        sprintf(myInform, "colorTo: R%03.2f%% G%03.2f%% B%03.2f%%  ", myParam1.rgb_colorto.R_color, myParam1.rgb_colorto.G_color, myParam1.rgb_colorto.B_color);
        LCD_ShowString(0, myImage1->height, myInform);
        //暂停200ms
        Delay(200);

        //参数调整
        myParam1.rgb_colorto.R_color +=  0.11f;//[0.0 -- 1.0]
        myParam1.rgb_colorto.G_color +=  0.08f;
        myParam1.rgb_colorto.B_color +=  0.07f;
    }

    /*--------------------------彩色图染色法2--------------------------------------------------*/

    //重新初始化染色参数
    myParam1.rgb_colorto.R_color = 0.0f;
    myParam1.rgb_colorto.G_color = 0.0f;
    myParam1.rgb_colorto.B_color = 0.0f;

    for (int i = 0; i < 10; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_ThreeChannel_MapTo(myRender, &myParam1);

        //使用通道渲染器进行渲染图片
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //显示图片1
        LCD_show_image(stx + 0, myImage0->height, myImage1);

        //信息显示
        sprintf(myInform, "colorTo: R%03.1f G%03.1f B%03.1f   ", myParam1.rgb_colorto.R_color, myParam1.rgb_colorto.G_color, myParam1.rgb_colorto.B_color);
        LCD_ShowString(0, myImage1->height, myInform);
        //暂停200ms
        Delay(200);

        //参数调整
        myParam1.rgb_colorto.R_color += 13.0f;//取值在 [0 -- 255]区间
        myParam1.rgb_colorto.G_color += 19.0f;
        myParam1.rgb_colorto.B_color += 26.0f;
    }
    //释放渲染器
    YMCV_Free_PIX_RenderFree(myRender);



    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage1);
}


