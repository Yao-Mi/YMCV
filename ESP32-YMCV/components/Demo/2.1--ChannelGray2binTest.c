
/*******************************************************************************************
    *
    * 说明  该例程以单、双阈值为例，展示了 通道渲染器创建，渲染器参数设置与调整，渲染器初始化以及渲染图片并显示
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

	
    /*--------------------------单阈值二值化--------------------------------------------------*/

    //创建通道渲染器
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_ThreshToBin, PIXChannelNum1);//单通道二值化，一个通道
    //设置渲染器参数
    CV_pixchannel_1param myParam = { .gray_tobin.Thresh = 20 };
    for (int i = 0; i < 10; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_SingleChannel_MapTo(myRender,&myParam);

        //使用通道渲染器进行渲染图片
        YMCV_Image_ChannelRenderTo(myImage1, myImage2, myRender);
        //显示图片2
        LCD_show_image(stx + 0, myImage1->height, myImage2);

        //信息显示
        sprintf(myInform, "1 - 1 thresh: %03d .       ", myParam.gray_tobin.Thresh);
        LCD_ShowString(stx + 0, myImage1->height, myInform);
        //暂停200ms
        Delay(200);

        //参数调整
        myParam.gray_tobin.Thresh += 10;
    }
    //释放渲染器
    YMCV_Free_PIX_RenderFree(myRender);


    /*--------------------------双阈值二值化--------------------------------------------------*/

    //重新创建一个通道渲染器
    myRender = YMCV_Creat_PixRender_Creat(CV_Thresh2ToBin, PIXChannelNum1);//单通道二值化，一个通道
    //设置渲染器参数
    CV_pixchannel_1param myParam1 = { .gray_2tobin.leftThresh = 0 ,.gray_2tobin.rightThresh = 20 };
    for (int i = 0; i < 10; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_SingleChannel_MapTo(myRender, &myParam1);

        //使用通道渲染器进行渲染图片
        YMCV_Image_ChannelRenderTo(myImage1, myImage2, myRender);
        //显示图片2
        LCD_show_image(stx + 0, myImage1->height, myImage2);

        //信息显示
        sprintf(myInform, "1 - 2 thresh: %03d -- %03d .", myParam1.gray_2tobin.leftThresh, myParam1.gray_2tobin.rightThresh);
        LCD_ShowString(stx + 0, myImage1->height, myInform);
        //暂停200ms
        Delay(200);

        //参数调整
        myParam1.gray_2tobin.leftThresh += 10;
        myParam1.gray_2tobin.rightThresh += 10;
    }
    //释放渲染器
    YMCV_Free_PIX_RenderFree(myRender);

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
}





