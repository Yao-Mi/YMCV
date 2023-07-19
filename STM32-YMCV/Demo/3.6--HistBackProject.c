#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以 直方图反投影为例，展示了将一幅图像的直方图反投影给另一幅图使用的方法、直方图显示
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage1, CV_Rgb16ToGray);//rgb转灰度图
	
    CVrect myroi = { .x = 80 - 20,.y = 60 - 20,.w = 40,.h = 40 };
    CVIMAGE myImage2 = YMCV_Creat_ImgROICopyTo(myImage1, &myroi);
		
		LCD_show_image(stx + 0, 0, myImage1);
		LCD_Draw_Rect(stx + myroi.x,myroi.y,myroi.w,myroi.h,(color24_t){.R = 255});

    //直方图数组
    int32 myImage1Hist[256];

    /*--------------------------灰度图直方图正向与反向映射--------------------------------------------------*/

    //获取图像直方图
    YMCV_Image_Hist_Get(myImage2, myImage1Hist, 256, PIXChannelNum1);
    //显示image2及其直方图
    LCD_show_image(0, 0, myImage2);
    LCDHistShow(myImage1Hist, PIXChannelNum1, 170, 60, 40);

    //信息显示
    sprintf(myInform, "get roi hist     ");
    LCD_ShowString(stx + 0, 0,myInform);

    //暂停500ms
    Delay(500);
		
    //image1直方图反投影到image0 并显示
    YMCV_GrayHist_BackProject(myImage1Hist, 256, myImage1);
    LCD_show_image(stx + 0,  myImage1->height, myImage1);

    //信息显示
    sprintf(myInform, "grayHist backProj");
    LCD_ShowString(stx + 0, myImage1->height,myInform);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
}





