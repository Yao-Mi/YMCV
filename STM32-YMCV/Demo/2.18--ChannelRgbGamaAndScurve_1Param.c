
/*******************************************************************************************
    *
    * 说明   该例程以gama、s-curve增强为例，展示了如何采用 单通道渲染器 渲染三通道图像
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

    /*--------------------------彩色图伽马曲线增强--------------------------------------------------*/

    //创建单通道渲染器
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Gama_Enhance, PIXChannelNum1);//gama曲线增强
    //设置渲染器参数 ，参数粗调节推荐步幅为     c（5）    ，  gama（0.1）
    CV_pixchannel_1param myParam = { .gray_gama_enhance._c = 309.0f,.gray_gama_enhance._gama = 1.6f };

    for (int i = 0; i < 10; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_SingleChannel_MapTo(myRender, &myParam);
        //使用通道渲染器进行渲染彩色图片
        YMCV_Image_ChannelRenderTo_UGray(myImage0, myImage1, myRender);
        //显示图片1
        LCD_show_image(stx + 0, myImage0->height, myImage1);
        //显示通道转换图
        LCDChannelTransformShow(myRender, 170, 220, 40);
        //信息显示
			  sprintf(myInform, "gama:  c=%03.2f gama= %01.3f  ", myParam.gray_gama_enhance._c, myParam.gray_gama_enhance._gama);
        LCD_ShowString(0, myImage0->height, myInform);

        //暂停200ms
        Delay(200);

        //参数调整
        myParam.gray_gama_enhance._c += 20.0f;
        //myParam.gray_gama_enhance._gama += 0.1f;
    }
    //释放渲染器
    YMCV_Free_PIX_RenderFree(myRender);


    /*--------------------------彩色图S曲线增强--------------------------------------------------*/

    //重新创建一个通道渲染器
    myRender = YMCV_Creat_PixRender_Creat(CV_Scurve_Enhance, PIXChannelNum1);//S曲线增强
    //设置渲染器参数 ，参数粗调节推荐步幅为     a（10）    ，  b（0.001）    ，   c（0.01）
    CV_pixchannel_1param myParam1 = { .gray_scurve_enhance._m = 132.0f,.gray_scurve_enhance._E = -5.0f };

    for (int i = 0; i < 20; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_SingleChannel_MapTo(myRender, &myParam1);

        //使用通道渲染器进行渲染图片
        YMCV_Image_ChannelRenderTo_UGray(myImage0, myImage1, myRender);
        //显示图片1
        LCD_show_image(stx + 0, myImage0->height, myImage1);
        //显示通道转换图
        LCDChannelTransformShow(myRender, 170, 220, 40);

        //信息显示
			  sprintf(myInform, "Scure: m = %03.2f  E = %02.2f  ", myParam1.gray_scurve_enhance._m, myParam1.gray_scurve_enhance._E);
        LCD_ShowString(0, myImage0->height, myInform);
        //暂停200ms
        Delay(200);

        //参数调整
        //myParam1.gray_scurve_enhance._m += 10.0f;
        myParam1.gray_scurve_enhance._E += 0.5f;
    }
    //释放渲染器
    YMCV_Free_PIX_RenderFree(myRender);

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage1);
}





