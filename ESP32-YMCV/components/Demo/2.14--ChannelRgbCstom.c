
/*******************************************************************************************
    *
    * 说明   该例程以自定义染色函数增强、调色板渲染为例，展示了使用自定义函数进行渲染与调参、灰度图转热力图显示、灰度图自定义颜色渲染
    *
    ***************************************************************************************/
int32 myRFunction(int32 color);
int32 myGFunction(int32 color);
int32 myBFunction(int32 color);
float32 myFun_Rk = 1.0f, myFun_Gk = 1.0f, myFun_Bk = 1.0f;

void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
	  myFun_Rk = 1.0f;
	  myFun_Gk = 1.0f;
	  myFun_Bk = 1.0f;
	
	  //显示图片0
    LCD_Clear(0);
    LCD_show_image(stx + 0, 0, myImage0);
	
    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVRgb16Type);

    /*--------------------------彩色图自定义渲染器--------------------------------------------------*/

    //创建通道渲染器
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Channel_CustomTransform, PIXChannelNum3);//自定义渲染器

    for (int i = 0; i < 10; i++)
    {
        //使用自己的函数初始化渲染通道
        YMCV_ThreeChannel_CustomMapTo(myRender, myRFunction, myGFunction, myBFunction);
        //使用通道渲染器进行渲染图片
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //显示图片1
        LCD_show_image(stx + 0, myImage0->height, myImage1);
        //显示通道转换图
        LCDChannelTransformShow(myRender, 170, 220, 40);
        //信息显示
        sprintf(myInform, "Cstom linerk:\n     R%03.2f G%03.2f B%03.2f ",myFun_Rk, myFun_Gk, myFun_Bk);
        LCD_ShowString(0, 0, myInform);

        //暂停300ms
        Delay(300);

        //参数调整
        myFun_Rk += 1.0f;
        myFun_Gk += 0.8f;
        myFun_Bk += 0.5f;
    }
    //释放渲染器1
    YMCV_Free_PIX_RenderFree(myRender);


    /*--------------------------彩色图调色板--------------------------------------------------*/

    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);//灰度图
    CVPIX_RENDER myRender2 = YMCV_Creat_PixRender_Creat(CV_Channel_ThreeToSingle, PIXChannelNum3);//三通道转单通道
    //创建调色板
    color24_t myPalette[256] = { 0 };

    //使用(0.3,0.6,0.1)系数转单通道
    float32 kr = 0.3, kg = 0.6, kb = 0.1;
    for (int i = 0; i < 256; i++)
    {
        myPalette[i].R = (_color8_t)(kr * i) ;
        myPalette[i].G = (_color8_t)(kg * i) ;
        myPalette[i].B = (_color8_t)(kb * i) ;
    }

    //使用调色板进行渲染器通道配置、初始化
    YMCV_ThreeChannel_PaletteMapTo(myRender2, myPalette);
    //使用通道渲染器进行渲染图片
    YMCV_Image_ThreeToSingle_ChannelRenderTo(myImage0, myImage2, myRender2);
    //显示图片2
    LCD_show_image(stx + 0, 0, myImage0);
    LCD_show_image(stx + 0, myImage0->height, myImage2);
		
    sprintf(myInform, "cstom k = (rk,bk,gk):\n        (%01.2f,%01.2f,%01.2f)    ",kr, kg, kb);
    LCD_ShowString(0, 0, myInform);
		//暂停300ms
		Delay(300);

    //释放渲染器2
    YMCV_Free_PIX_RenderFree(myRender2);

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
}

//自定义线性函数
int32 myRFunction(int32 color)
{
    return myFun_Rk * color;
}
int32 myGFunction(int32 color)
{
    return myFun_Gk * color;
}
int32 myBFunction(int32 color)
{
    return myFun_Bk * color;
}

