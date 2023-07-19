
/*******************************************************************************************
    *
    * 说明  该例程以 Sobel滤波器为例，展示了边缘检测效果
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

    /*--------------------------灰度Sobel边缘检测--------------------------------------------------*/
    //创建输出二值图像
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0_g->width, myImage0_g->height, CVBinaryType);//二值图
    CV_edge_param myParam = { .EdgeType = CV_Sobel_Connect ,.Thresh.sobel_connect.minT = 150,
    .Thresh.sobel_connect.connect_r=2,//相似处理半径
    .Thresh.sobel_connect.valSimilarT=30,//幅度相似
    .Thresh.sobel_connect.angSimilarT=5};//角度相似

    YMCV_Gray_EdgeDetect(myImage0_g, myImage2, &myParam);

    //显示图片2
    LCD_show_image(stx + 0, myImage0_g->height, myImage2);

    //信息显示
    sprintf(myInform, "Sobel thresh:%d \nradius similar:%3d \nangle similar:%3d \nvalue similar:%3d ", myParam.Thresh.sobel_connect.minT,
        myParam.Thresh.sobel_connect.connect_r ,
        myParam.Thresh.sobel_connect.angSimilarT,
        myParam.Thresh.sobel_connect.valSimilarT);
    LCD_ShowString(stx + 0, myImage0_g->height - 16*2, myInform);
		
    //暂停300ms
    Delay(300);

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_g);
    YMCV_Free_ImgFree(myImage2);

}





