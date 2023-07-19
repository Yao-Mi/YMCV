
/*******************************************************************************************
    *
    * 说明  该例程以 二值图的掩码运算为例，展示了掩码保留 的 结果
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
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
	  //暂停300ms
		Delay(300);
		
    //创建两个二值图像
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0_g->width, myImage0_g->height, CVBinaryType);//二值图

    /*-------------------------- 二值化到 myImage2 --------------------------------------------------*/
    CV_binary_param myParam = { .gray_single_t.thresh = 180 };
    //创建二值变换渲染器
    CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_SingleThreshold);
    YMCV_BinaryKernel_Load(myRender1, &myParam);   //加载参数进行渲染配置、初始化

    //将目标图像的进行渲染
    YMCV_Image_ThreshTo_Binary(myImage0_g, myImage2, myRender1);

    //快速翻转一下
    for (int i = 0; i < myImage2->width* myImage2->height; i++)
    {
        myImage2->data[i] = 255 - myImage2->data[i];
    }

    //显示图片
    LCD_show_image(stx + 0, myImage0_g->height, myImage2);

    //信息显示
    sprintf(myInform, "binary thresh: %3d ", myParam.gray_single_t.thresh);
    LCD_ShowString(stx + 0, myImage0_g->height, myInform);
    //暂停500ms
    Delay(500);
    //释放渲染器内存
    YMCV_Free_Binary_RenderFree(myRender1);

    /*-------------------------- 掩码保留 myImage0 = myImage0 & Mask --------------------------------------------------*/
    YMCV_Image_BinaryMask_Cal(myImage0_g, myImage2);//掩码保留，输出结果放在图0

    //显示图片
    LCD_show_image(stx + 0, myImage0_g->height, myImage0_g);
    //信息显示
    sprintf(myInform, " image mask  saved  ");
    LCD_ShowString(stx + 0, myImage0_g->height, myInform);
    //暂停800ms
    Delay(800);

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_g);
    YMCV_Free_ImgFree(myImage2);
}






