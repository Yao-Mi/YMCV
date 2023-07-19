
/*******************************************************************************************
    *
    * 说明  该例程以 直方图均衡化、直方图规定化为例，展示了直方图的使用及显示
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage1, CV_Rgb16ToGray);//rgb转灰度图
	  //显示图片1
    LCD_show_image(stx + 0, 0, myImage1);
	
    //直方图数组
    int32 myImage1Hist[256], myImage2Hist[256];

    /*--------------------------灰度图直方图均衡化--------------------------------------------------*/

    //获取图像直方图
    YMCV_Image_Hist_Get(myImage1, myImage1Hist, 256, PIXChannelNum1);
    //显示image1图片与直方图
    LCD_show_image(stx + 0, 0, myImage1);
    LCDHistShow(myImage1Hist, PIXChannelNum1, 170, 60, 40);

    //把image1备份一份
    CVIMAGE myImage2 = YMCV_Creat_Img_BackUp(myImage1);
    //将myImage2的进行直方图均衡化操作
    YMCV_Image_Hist_Equalize(myImage2, myImage2);//输出覆盖自身

    //获取图像直方图与显示
    YMCV_Image_Hist_Get(myImage2, myImage2Hist, 256, PIXChannelNum1);
    LCD_show_image(stx + 0, myImage1->height, myImage2);
    LCDHistShow(myImage2Hist, PIXChannelNum1, 170, 220, 40);//由于大小放缩到40，经过转换形状可能不真实，可以用256观察真实直方图

    //用完释放内存
    YMCV_Free_ImgFree(myImage2);
		//局部直方图去雾耗时较长，这里不开延时
//    //暂停500ms
//    Delay(500);

    /*--------------------------局部直方图去雾 修改滤波半径--------------------------------------------------*/
    uint8 ker_r = 60;
    //先Padding填充，扩充图像边界,扩充半径为r
    CVIMAGE myImage3 = YMCV_Image_Image_Padding(myImage1, ker_r, ker_r, CV_PaddingAutoSpace);
    //LCD_show_image(0, 0, myImage3);

    //局部直方图均衡
    myImage2 = YMCV_Gray_Local_Hist_Equalize(myImage3, ker_r, NOPadding);//image2内存之前已经释放了
    //获取图像直方图与显示
    YMCV_Image_Hist_Get(myImage2, myImage2Hist, 256, PIXChannelNum1);
    LCD_show_image(stx + 0, myImage1->height, myImage2);
    LCDHistShow(myImage2Hist, PIXChannelNum1, 170, 220, 40);//由于大小放缩到40，经过转换形状可能不真实，可以用256观察真实直方图


    //信息显示
    sprintf(myInform, "local HistEqRadius:%d  ", ker_r);
    LCD_ShowString(stx + 0,myImage1->height , myInform);
		
		//暂停1000ms
    Delay(1000);
		
    //释放内存
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);
}





