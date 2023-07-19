
/*******************************************************************************************
    *
    * 说明  该例程以 二值图的逻辑运算为例，展示了逻辑 与或非 的 结果
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 60;
    char myInform[100];
	  static uint8_t clearlcd = 1;
    
	  if(clearlcd) 
			clearlcd = 0,
	    LCD_Clear(GRAY);
	
	  //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage0_g = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_g, CV_Rgb16ToGray);//rgb转灰度图
		//显示图片0
		LCD_show_image(stx + 0, 0, myImage0_g);
	
	  //由于屏幕太小，只取一部分来处理并显示
	  CVrect myroi = { .x = 80 - 30,.y = 60 - 30,.w = 60,.h = 60 };
    CVIMAGE myImage0_roi = YMCV_Creat_ImgROICopyTo(myImage0_g, &myroi);
		LCD_Draw_Rect(stx + myroi.x,myroi.y,myroi.w,myroi.h,(color24_t){.R = 255});
	  //暂停300ms
    Delay(300);
	
    //创建两个二值图像
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0_roi->width, myImage0_roi->height, CVBinaryType);//二值图
    CVIMAGE myImage3 = YMCV_Creat_Img_Creat(myImage0_roi->width, myImage0_roi->height, CVBinaryType);//二值图

    /*-------------------------- 二值化到 myImage2 --------------------------------------------------*/
    CV_binary_param myParam = { .gray_single_t.thresh = 150 };
    //创建二值变换渲染器
    CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_SingleThreshold);
    YMCV_BinaryKernel_Load(myRender1, &myParam);   //加载参数进行渲染配置、初始化

    //将目标图像的进行渲染
    YMCV_Image_ThreshTo_Binary(myImage0_roi, myImage2, myRender1);

    //显示图片2
    LCD_show_image(0, 0, myImage2);

    //信息显示
    sprintf(myInform, "1 thresh:%3d", myParam.gray_single_t.thresh);
    LCD_ShowString(stx + 0, 0, myInform);

		//释放渲染器内存
    YMCV_Free_Binary_RenderFree(myRender1);

    /*-------------------------- 二值化到 myImage3 --------------------------------------------------*/
    //设置新的参数
    myParam.gray_double_t.thresh_min = 100;
    myParam.gray_double_t.thresh_max = 180;
    //创建新的二值变换渲染器
    myRender1 = YMCV_Creat_BinaryRender_Creat(CV_DoubleThreshold);
    YMCV_BinaryKernel_Load(myRender1, &myParam);   //加载参数进行渲染配置、初始化

    //将目标图像的进行渲染
    YMCV_Image_ThreshTo_Binary(myImage0_roi, myImage3, myRender1);

    //显示图片3
    LCD_show_image(0, myImage2->height, myImage3);

    //信息显示
    sprintf(myInform, "2 thresh:[%3d,%3d]", myParam.gray_double_t.thresh_min,myParam.gray_double_t.thresh_max);
    LCD_ShowString(stx + 0,  myImage2->height, myInform);
    //暂停300ms
    Delay(300);
    
    //释放渲染器内存
    YMCV_Free_Binary_RenderFree(myRender1);
    /*-------------------------- myImage3 = myImage3 & myImage2 --------------------------------------------------*/
    CVIMAGE img3back1 = YMCV_Creat_Img_BackUp(myImage3);
		YMCV_Image_Binary_Logic_Cal(myImage2, img3back1, CV_And);//图2 逻辑与上 图3 ，输出结果放在图3

    //显示图片3
    LCD_show_image(20, myImage0->height + 40, img3back1);
    
    /*-------------------------- myImage3 = myImage2 | myImage3 --------------------------------------------------*/
    CVIMAGE img3back2 = YMCV_Creat_Img_BackUp(myImage3);
		YMCV_Image_Binary_Logic_Cal(myImage2, img3back2, CV_Or);//图2 逻辑或上 图3 ，输出结果放在图3

    //显示图片3
    LCD_show_image(90, myImage0->height + 40, img3back2);
		
    /*-------------------------- myImage3 = ! myImage2 --------------------------------------------------*/
    YMCV_Image_Binary_Logic_Cal(myImage2, img3back2, CV_Not);//图2 逻辑非  ，输出结果放在图3

    //显示图片3
    LCD_show_image(160, myImage0->height + 40, img3back2);
		
		
		//信息显示
    sprintf(myInform, "logic:\n   1 and 2  1 or 2    not 1");
    LCD_ShowString(0, myImage0->height, myInform);
    //暂停500ms
    Delay(500);


    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_g);
    YMCV_Free_ImgFree(myImage0_roi);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);
    YMCV_Free_ImgFree(img3back1);
    YMCV_Free_ImgFree(img3back2);

}





