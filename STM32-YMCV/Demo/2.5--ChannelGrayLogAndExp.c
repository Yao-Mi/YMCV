
/*******************************************************************************************
    *
    * 说明  该例程以log对数、exp指数变换为例，展示了参数取值 、增长步幅，以及通道转换示意图的显示
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
    /*--------------------------log对数增强--------------------------------------------------*/

    //创建通道渲染器
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Log_Enhance, PIXChannelNum1);//图像对比度增强/减弱（雾化）
    //设置渲染器参数 ，参数粗调节推荐步幅为     a（5）    ，  b（0.005）    ，   c（0.01）
    CV_pixchannel_1param myParam = { .gray_log_enhance._a = -156.0f,.gray_log_enhance._b = 0.041f,.gray_log_enhance._c = 1.42f };
    for (int i = 0; i < 10; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_SingleChannel_MapTo(myRender, &myParam);
        //使用通道渲染器进行渲染图片
        YMCV_Image_ChannelRenderTo(myImage1, myImage2, myRender);
        //显示图片2
        LCD_show_image(stx + 0, myImage1->height, myImage2);
			  //显示通道转换图
        LCDChannelTransformShow(myRender, 170, 220, 40);
			
        //信息显示
        sprintf(myInform, "log: a=%03.2f b=%01.3f c=%01.2f", myParam.gray_log_enhance._a, myParam.gray_log_enhance._b, myParam.gray_log_enhance._c);
        LCD_ShowString(0, myImage1->height, myInform);

        //暂停200ms
        Delay(200);

        //参数调整
        myParam.gray_log_enhance._a += 5.0f;
        //myParam.gray_log_enhance._b += 0.005f;
        //myParam.gray_log_enhance._c += 0.01f;
    }
    //释放渲染器
    YMCV_Free_PIX_RenderFree(myRender);


    /*--------------------------exp指数增强--------------------------------------------------*/

    //重新创建一个通道渲染器
    myRender = YMCV_Creat_PixRender_Creat(CV_Exp_Enhance, PIXChannelNum1);//指数增强
    //设置渲染器参数 ，参数粗调节推荐步幅为     a（10）    ，  b（0.001）    ，   c（0.01）
    CV_pixchannel_1param myParam1 = { .gray_exp_enhance._a = -542.0f,.gray_exp_enhance._b = 1.04f,.gray_exp_enhance._c = 0.18f };
    for (int i = 0; i < 10; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_SingleChannel_MapTo(myRender, &myParam1);

        //使用通道渲染器进行渲染图片
        YMCV_Image_ChannelRenderTo(myImage1, myImage2, myRender);
			  //显示图片2
        LCD_show_image(stx + 0, myImage1->height, myImage2);
			  //显示通道转换图
        LCDChannelTransformShow(myRender, 170, 220, 40);

        //信息显示
        sprintf(myInform, "exp: a=%03.2f b=%01.3f c=%01.2f", myParam1.gray_exp_enhance._a, myParam1.gray_exp_enhance._b, myParam1.gray_exp_enhance._c);
        LCD_ShowString(0, myImage1->height, myInform);
        //暂停200ms
        Delay(200);

        //参数调整
        myParam1.gray_exp_enhance._a += 10.0f;
        //myParam1.gray_exp_enhance._b += 0.001f;
        //myParam1.gray_exp_enhance._c += 0.01f;
    }
    //释放渲染器
    YMCV_Free_PIX_RenderFree(myRender);


    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
}





