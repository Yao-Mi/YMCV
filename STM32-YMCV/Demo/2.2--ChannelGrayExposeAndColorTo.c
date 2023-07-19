
/*******************************************************************************************
    *
    * 说明  该例程以曝光阈值、灰度染色为例，展示了 渲染器的使用，以及灰度染色的两种参数取值法
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
	
    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage1->width, myImage1->height, CVGrayType);

		YMCV_Image_Pix_Change_Form(myImage0, myImage1, CV_Rgb16ToGray);//rgb转灰度图
	  
	  //显示图片1
    LCD_show_image(stx + 0, 0, myImage1);

    /*--------------------------阈值曝光--------------------------------------------------*/

    //创建通道渲染器
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Expose, PIXChannelNum1);//图像曝光
    //设置渲染器参数
    CV_pixchannel_1param myParam = { .gray_expose.thresh = 0 };
    for (int i = 0; i < 10; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_SingleChannel_MapTo(myRender, &myParam);

        //使用通道渲染器进行渲染图片
        YMCV_Image_ChannelRenderTo(myImage1, myImage2, myRender);

        //显示图片2
        LCD_show_image(stx + 0, myImage1->height, myImage2);

        //信息显示
        sprintf(myInform, "Expose Thresh: %03d .", myParam.gray_expose.thresh);
        LCD_ShowString(stx + 0, myImage1->height, myInform);
        //暂停200ms
        Delay(200);

        //参数调整
        myParam.gray_expose.thresh += 10;
    }
    //释放渲染器
    YMCV_Free_PIX_RenderFree(myRender);

    /*--------------------------灰度图染色--------------------------------------------------*/

    //重新创建一个通道渲染器
    myRender = YMCV_Creat_PixRender_Creat(CV_ColorTo, PIXChannelNum1);//灰度染色
    //设置渲染器参数 取值在[0.0 -- 1.0]区间 或者 [0 --- 255]区间
    CV_pixchannel_1param myParam1 = { .gray_colorto.graycolor = 0.0 };
    for (int i = 0; i < 10; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_SingleChannel_MapTo(myRender, &myParam1);

        //使用通道渲染器进行渲染图片
        YMCV_Image_ChannelRenderTo(myImage1, myImage2, myRender);

        //显示图片2
        LCD_show_image(stx + 0, myImage1->height, myImage2);

        //信息显示
        sprintf(myInform, "gray color: %03.2f %", myParam1.gray_colorto.graycolor);
        LCD_ShowString(stx + 0, myImage1->height, myInform);
        //暂停200ms
        Delay(200);

        //参数调整
        myParam1.gray_colorto.graycolor = myParam1.gray_colorto.graycolor + 0.10f;//[0.0 -- 1.0]
    }
    
    //重新初始化染色参数
    myParam1.gray_colorto.graycolor = 0.0f;
    for (int i = 0; i < 10; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_SingleChannel_MapTo(myRender, &myParam1);

        //使用通道渲染器进行渲染图片
        YMCV_Image_ChannelRenderTo(myImage1, myImage2, myRender);

        //显示图片2
        LCD_show_image(stx + 0, myImage1->height, myImage2);

        //信息显示
        sprintf(myInform, "gray color: %03.2f", myParam1.gray_colorto.graycolor);
        LCD_ShowString(stx + 0, myImage1->height, myInform);
        //暂停200ms
        Delay(200);

        //参数调整
        myParam1.gray_colorto.graycolor = myParam1.gray_colorto.graycolor + 10.00f;//取值在 [0 -- 255]区间
    }
    //释放渲染器
    YMCV_Free_PIX_RenderFree(myRender);



    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);

}





