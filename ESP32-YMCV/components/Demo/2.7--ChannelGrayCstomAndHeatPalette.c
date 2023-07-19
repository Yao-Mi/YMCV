
/*******************************************************************************************
    *
    * 说明   该例程以自定义染色函数增强、调色板渲染为例，展示了使用自定义函数进行渲染与调参、灰度图转热力图显示、灰度图自定义颜色渲染
    *
    ***************************************************************************************/
int32 myFunction(int32 color);
float32 myFun_k = 1.0f;

void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
    CVIMAGE myImage3 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVRgb16Type);

		YMCV_Image_Pix_Change_Form(myImage0, myImage1, CV_Rgb16ToGray);//rgb转灰度图
	  //显示图片1
    LCD_Clear(0);
    LCD_show_image(stx + 0, 0, myImage1);
	
    /*-------------------------- 热力图颜色表显示 --------------------------------------------------*/
    color24_t* mycstomTable = (color24_t*)YMCV_HeatPalette;
    color24_t rectclv = { .R = 100,.G = 100,.B = 100 };
 
    uint16 rsx = stx+10, rsy = 50, rsw = 128, rsh = 20; //表长为rsw
    LCD_Draw_Rect(rsx - 1, rsy - 1, rsw + 1, rsh + 2, rectclv);
    LCD_Draw_Rect(rsx - 1, rsy - 3 - rsh, rsw + 1, rsh + 2, rectclv);
    for (int i = 0 ,imax = CVMin(256,rsw); i < rsw; i++)
    {
			  int curgrayv = i* 256 / imax;
        //灰度表绘制
        rectclv.R = rectclv.G = rectclv.B = curgrayv;// 0 -- 255
        LCD_Point_Color_Set(rectclv);
        LCD_Draw_Line(rsx + i, rsy, rsx + i, rsy + rsh);
        //热力图表绘制
        LCD_Point_Color_Set(mycstomTable[curgrayv]);
        LCD_Draw_Line(rsx + i, rsy - 2 - rsh, rsx + i, rsy - 2);
    }

    /*--------------------------自定义增强--------------------------------------------------*/

    //创建通道渲染器
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Channel_CustomTransform, PIXChannelNum1);//自定义渲染器
    CVPIX_RENDER myRender2 = YMCV_Creat_PixRender_Creat(CV_Channel_SingleToThree, PIXChannelNum3);//单通道转三通道

		myFun_k = 1.0f; //初始化k参数
    for (int i = 0; i < 10; i++)
    {
        //使用自己的函数初始化渲染通道
        YMCV_SingleChannel_CustomMapTo(myRender, myFunction);
        //使用通道渲染器进行渲染图片
        YMCV_Image_ChannelRenderTo(myImage1, myImage2, myRender);
        //显示图片1
        LCD_show_image(stx + 0, myImage1->height, myImage2);
			  //显示通道转换图
        LCDChannelTransformShow(myRender, 170, 220, 40);
        //信息显示
        sprintf(myInform, "cstom liner: k =%03.2f", myFun_k);
        LCD_ShowString(stx + 0, myImage1->height, myInform);
        //暂停300ms
        Delay(300);
			
        //转为热力图
        //使用热力图 调色板进行渲染器通道配置、初始化
        YMCV_ThreeChannel_PaletteMapTo(myRender2, mycstomTable);
        //使用通道渲染器进行渲染图片
        YMCV_Image_SingleToThree_ChannelRenderTo(myImage2, myImage3, myRender2);
        
        //显示图片3
        LCD_show_image(stx + 0, myImage1->height, myImage3);

        //暂停300ms
        Delay(300);

        //参数调整
        myFun_k += 1.0f;
    }
    //释放渲染器1
    YMCV_Free_PIX_RenderFree(myRender);


    /*--------------------------自定义调色板染色--------------------------------------------------*/

    //创建调色板
    color24_t myPalette[256] ={0};

    //使用RGB（220，150，100）对灰度图进行染色
    color24_t myColor = { .R = 250,.G = 100,.B = 50 };
    for (int i = 0; i < 256; i++)
    {
        myPalette[i].R = myColor.R * i / 255;
        myPalette[i].G = myColor.G * i / 255;
        myPalette[i].B = myColor.B * i / 255;
    }

    //使用调色板进行渲染器通道配置、初始化
    YMCV_ThreeChannel_PaletteMapTo(myRender2, myPalette);
    //使用通道渲染器进行渲染图片
    YMCV_Image_SingleToThree_ChannelRenderTo(myImage1, myImage3, myRender2);
    //显示图片1、3
    LCD_show_image(stx + 0, 0, myImage1);
    LCD_show_image(stx + 0, myImage1->height, myImage3);
    sprintf(myInform, "cstom:RGB=(%03d,%03d,%03d)  ", myColor.R, myColor.G, myColor.B);
    LCD_ShowString(0, myImage1->height, myInform);
    //暂停1000ms
    Delay(1000);

    //释放渲染器2
    YMCV_Free_PIX_RenderFree(myRender2);


    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);
}

//自定义线性函数
int32 myFunction(int32 color)
{
    return myFun_k * color;
}



