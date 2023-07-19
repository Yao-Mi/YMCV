
/*******************************************************************************************
    *
    * 说明  该例程以 拉普拉斯滤波器为例，展示了边缘检测效果，该算子通常用于二值图的检测
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];
	
    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage0_g = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_g, CV_Rgb16ToGray);//rgb转灰度图
		//显示图片0
		LCD_show_image(stx + 0, 0, myImage0_g);
	
    /*--------------------------拉普拉斯边缘检测--------------------------------------------------*/
    //创建输出二值图像
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0_g->width, myImage0_g->height, CVBinaryType);//二值图
    CV_edge_param myParam = { .EdgeType = CV_Laplace ,.Thresh.laplace.minT=30};

    YMCV_Gray_EdgeDetect(myImage0_g, myImage2, &myParam);

    //显示图片2
    LCD_show_image(stx + 0, myImage0_g->height, myImage2);

    //信息显示
    sprintf(myInform, "Laplace thresh:%3d  ", myParam.Thresh.sobel.minT);
    LCD_ShowString(stx + 0, myImage0_g->height - 8, myInform);
		
    //暂停300ms
    Delay(300);

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_g);
    YMCV_Free_ImgFree(myImage2);
}





