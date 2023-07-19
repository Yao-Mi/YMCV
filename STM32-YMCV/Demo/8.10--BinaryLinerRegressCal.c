#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以 二值图像素回归为例，展示了线性回归效果，
    *  该方法通过将图像重采样为不超过25x25大小的图像，进行回归分析后再将其变换回原图，可能会出现与原图实际直线不完全重合的情况
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];
	
    color24_t pencolor = { .R = 255 };
    LCD_Point_Color_Set(pencolor);//设置画笔颜色

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(xunxian03_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, (void*)xunxian03_gray, sizeof(xunxian03_gray));
    
    for (int i = 0; i < sizeof(xunxian03_gray); i++)
    {
        myData[i] = (myData[i] < 127) ? 255 : 0; //快速二值化
    }
    CVIMAGE myImage0b = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVBinaryType);//灰度图0

    /*-------------------------最小二乘 直线回归拟合--------------------------------------------------*/
    CVline imgline = YMCV_BinaryMax25x25_Regression_Line(myImage0b, CVRegress_LSE);

    LCD_show_image(stx + 0, 0, myImage0b);//显示图片0
    LCD_Draw_Line(stx + imgline.x1, imgline.y1, stx + imgline.x2, imgline.y2);//绘制直线

    //信息显示
    sprintf(myInform, "LES regress ");
    LCD_ShowString(stx + 0, 0, myInform);
    Delay(500);


    /*-------------------------泰尔森 直线回归拟合--------------------------------------------------*/
    imgline = YMCV_BinaryMax25x25_Regression_Line(myImage0b, CVRegress_TSE);

    LCD_show_image(stx + 0, 0, myImage0b);//显示图片0
    LCD_Draw_Line(stx + imgline.x1, imgline.y1, stx + imgline.x2, imgline.y2);//绘制直线
    //信息显示
    sprintf(myInform, "TSE regress ");
    LCD_ShowString(stx + 0, 0, myInform);
    Delay(500);


    /*-------------------------RANSAC 直线回归拟合--------------------------------------------------*/
    imgline = YMCV_BinaryMax25x25_Regression_Line(myImage0b, CVRegress_RANSAC);

    LCD_show_image(stx + 0, 0, myImage0b);//显示图片0
    LCD_Draw_Line(stx + imgline.x1, imgline.y1, stx + imgline.x2, imgline.y2);//绘制直线
    //信息显示
    sprintf(myInform, "RANSAC regress");
    LCD_ShowString(stx + 0, 0, myInform);
    Delay(500);


    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0b);

}





