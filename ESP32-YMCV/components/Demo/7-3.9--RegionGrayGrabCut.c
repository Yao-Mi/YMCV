#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以 GrabCut为例，展示了自动分割分割的效果
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];
	
//    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
//    CVIMAGE myImage0_g = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
//		YMCV_Image_Pix_Change_Form(myImage0, myImage0_g, CV_Rgb16ToGray);//rgb转灰度图
	
    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(mm02_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, (void*)mm02_gray, sizeof(mm02_gray));
    CVIMAGE myImage0_g = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVGrayType);//灰度图0
	
	  //显示图片0
		LCD_show_image(stx + 0, 0, myImage0_g);
	  int sty = myImage0_g->height;
		//原图太大了，缩小一半，到80x60进行处理
		CV_geom_param myParam = {.stretch.Imgin_w= myImage0_g->width,
													 .stretch.Imgin_h = myImage0_g->height, 
													 .stretch.out_w = myImage0_g->width/2, 
													 .stretch.out_h = myImage0_g->height/2, };

		CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_Stretch);//创建几何变换渲染器
		YMCV_GeomKernel_Load(myRender1, &myParam);//使用参数进行几何渲染配置、初始化
		CVIMAGE myImage0_gray = YMCV_Image_Geom_RenderTo(myImage0_g,myRender1);//进行几何变换渲染
    
		//显示图片0
		LCD_show_image(0, 0, myImage0_gray);
		//信息显示
    sprintf(myInform, "half size");
    LCD_ShowString(myImage0_gray->width, 0, myInform);

		//释放渲染器
		YMCV_Free_GEOM_RenderFree(myRender1);
    /*-------------------------- 手动创建TriMap--------------------------------------------------*/
	
    color24_t thiscolor = { .R = 255,.G = 0 };
    LCD_Point_Color_Set(thiscolor);

    CVpoint myPoint[]={{23,4},{23,24},{31,60},{30,88},{31,95},{31,115},{101,115},{107,118},{121,91},{121,65},{120,35},{115,14},{118,0},};
    uint16 num = sizeof(myPoint) / sizeof(CVpoint);
		
		int i;
		//原图缩小了一半进行处理，轮廓也缩小一半
		for(int i=0;i<num;i++)
		{
			myPoint[i].x /=2;
			myPoint[i].y /=2;
		}
			
		//绘制轮廓
    for (i = 0; i < num - 1; i++)
    {
        LCD_Draw_Line(myPoint[i].x, myPoint[i].y, myPoint[i + 1].x, myPoint[i + 1].y);
    }
    LCD_Draw_Line(myPoint[i].x, myPoint[i].y, myPoint[0].x, myPoint[0].y);
		

    CVIMAGE TriMapMarkers = YMCV_Creat_Img_Creat(myImage0_gray->width, myImage0_gray->height, CVTrimapType);
    YMCV_TriMap_Init(TriMapMarkers); //trimap初始化

    CVcolor fillval;
    //grabCut 使用的填充值：前景：[CVTriMapMinVl,CVTriMapMidle)，背景：(CVTriMapMidle,CVTriMapMaxVl]
    //其中，百分百为前景的，取CVTriMapMinVl，百分百背景的取CVTriMapMaxVl。
    fillval.gray = CVTriMapMaxVl - 10;//可能为前景的值

    //对Trimap进行区域填充
    YMCV_Img_Scanline_AreaFill(TriMapMarkers, myPoint, num, fillval);

    //显示图片trimap
		LCD_show_image(0, myImage0_gray->height, TriMapMarkers);
    //信息显示
    sprintf(myInform, "TriMap creat");
    LCD_ShowString(myImage0_gray->width, myImage0_gray->height, myInform);
//		//暂停800ms
//		Delay(800);

    /*-------------------------- grabCut分割 --------------------------------------------------*/
    YMCV_Gray_GrabCut(myImage0_gray, TriMapMarkers, 2);//两次迭代

    //显示图片
		LCD_show_image(0, myImage0_gray->height, TriMapMarkers);
    //信息显示
    sprintf(myInform, "GrabCut Result");
    LCD_ShowString(myImage0_gray->width, myImage0_gray->height, myInform);
		//暂停500ms
		Delay(500);

    /*-------------------------- 结果展示 --------------------------------------------------*/
    //将分割结果图x2还原，重新变成160x120大小
		CV_geom_param myParam2 = {.stretch.Imgin_w= TriMapMarkers->width,
													 .stretch.Imgin_h = TriMapMarkers->height, 
													 .stretch.out_w = myImage0_g->width, 
													 .stretch.out_h = myImage0_g->height, };

		CVGEOM_RENDER myRender2 = YMCV_Creat_GeomRender_Creat(CV_StretchL2);//创建几何变换渲染器
		YMCV_GeomKernel_Load(myRender2, &myParam2);//使用参数进行几何渲染配置、初始化
		CVIMAGE myImage0_mask = YMCV_Image_Geom_RenderTo(TriMapMarkers,myRender2);//进行几何变换渲染
    
		//显示图片0
		LCD_show_image(stx + 0, sty, myImage0_mask);
		//暂停1000ms
		Delay(1000);

    //
    CVIMAGE SegImage = YMCV_Image_Mask(myImage0_mask, myImage0_g);//分割图当作掩码使用

    //显示图片
		LCD_show_image(stx + 0, sty, SegImage);
    sprintf(myInform, "GrabCut Result Mask");//信息显示
    LCD_ShowString(stx + 0, sty, myInform);
		//暂停1000ms
		Delay(1000);

		//释放渲染器
		YMCV_Free_GEOM_RenderFree(myRender2);
		
    //释放内存
    YMCV_Free_ImgFree(myImage0_g);
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage0_mask);
    YMCV_Free_ImgFree(TriMapMarkers);
    YMCV_Free_ImgFree(SegImage);

}

