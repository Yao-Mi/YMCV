
/*******************************************************************************************
    *
    * 说明  该例程以 图像极坐标变换为例，展示了图片对数极坐标下的投影效果
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int sty = 16;
    char myInform[100];
	
    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage0_gray = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_gray, CV_Rgb16ToGray);//rgb转灰度图

		//显示图片0
    LCD_Clear(0);
		LCD_show_image(0, sty + 0, myImage0_gray);
		//暂停500ms
		Delay(500);
	
    /*--------------------------投影到对数极坐标下--------------------------------------------------*/
    CVIMAGE myImage2 = NULL, myImage3 = NULL;
    YMCV_PADDING_VAL = 50;//空白区填充值
    //加载图片0的渲染参数
    CV_geom_param myParam = { .polar.Imgin_w = myImage0_gray->width,
                              .polar.Imgin_h = myImage0_gray->height };
    //创建几何变换渲染器
    CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_LogPolar);

    //使用参数进行几何渲染配置、初始化
    YMCV_GeomKernel_Load(myRender1, &myParam);

    //将目标图像的进行渲染
    myImage2 = YMCV_Image_Geom_RenderTo(myImage0_gray, myRender1);

    //显示图片2
		{
			CV_geom_param stretchParam = {.stretch.Imgin_w= myImage2->width,
															 .stretch.Imgin_h = myImage2->height, 
															 .stretch.out_w = myImage2->width/2,  //缩放到一半大小
															 .stretch.out_h = myImage2->height/2, };
			//进行缩放后显示
			CVGEOM_RENDER stretchRender = YMCV_Creat_GeomRender_Creat(CV_Stretch);	
			YMCV_GeomKernel_Load(stretchRender, &stretchParam);
			CVIMAGE stretchImage = YMCV_Image_Geom_RenderTo(myImage2, stretchRender);														 
														
			LCD_show_image(0, sty + 0, stretchImage);
		  
			//释放临时内存
			YMCV_Free_GEOM_RenderFree(stretchRender);
			YMCV_Free_ImgFree(stretchImage);
		}

    //信息显示
    sprintf(myInform, "log polar   ");
    LCD_ShowString(0, 0, myInform);
    //暂停500ms
		Delay(500);
		
    /*--------------------------投影回笛卡尔坐标系--------------------------------------------------*/
    //加载图片2的渲染参数
    myParam.rotate.Imgin_w = myImage2->width;
    myParam.rotate.Imgin_h = myImage2->height;
    //创建几何变换渲染器
    CVGEOM_RENDER myRender2 = YMCV_Creat_GeomRender_Creat(CV_LogPolar_Inverse);

    //使用参数进行几何渲染配置、初始化
    YMCV_GeomKernel_Load(myRender2, &myParam);

    //将目标图像的进行渲染
    myImage3 = YMCV_Image_Geom_RenderTo(myImage2, myRender2);
    //结果图像必然是奇数的行列，因为必须有一个中心点

    //显示图片3
    LCD_show_image(0, sty + 0, myImage3);

    //信息显示
    sprintf(myInform, "invers log polar ");
    LCD_ShowString(0, 0, myInform);
		//暂停500ms
		Delay(500);
		
    //释放渲染器
    YMCV_Free_GEOM_RenderFree(myRender1);
    YMCV_Free_GEOM_RenderFree(myRender2);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);
    YMCV_Free_ImgFree(myImage0_gray);
}





