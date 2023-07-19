#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_HistCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 直方图规定化为例，展示了将一幅图像的直方图赋给另一幅图使用的方法、直方图显示
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 700);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(linglong00_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, linglong00_gray, sizeof(linglong00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 500, 255, CVGrayType);//灰度图1

    CVrect myroi = { .x = 310,.y = 100,.w = 150,.h = 150 };
    CVIMAGE myImage1 = YMCV_Creat_ImgROICopyTo(myImage0, &myroi);

    //直方图数组
    int32 myImage1Hist[256];

    /*--------------------------灰度图直方图正向与反向映射--------------------------------------------------*/

    //获取图像直方图
    YMCV_Image_Hist_Get(myImage1, myImage1Hist, 256, PIXChannelNum1);
    //显示image1及其直方图
    LCD_show_image(0, 0, myImage0);
    LCD_show_image(500, 0, myImage1);
    LCDHistShow(myImage1Hist, PIXChannelNum1, 550, 420, 128);

    //信息显示
    sprintf(myInform, "提取脸部灰度直方图");
    LCD_ShowString(100, 510, myInform);

    PAUSE();
    //image1直方图反投影到image0 并显示
    YMCV_GrayHist_BackProject(myImage1Hist, 256, myImage0);
    LCD_show_image(500, 0, myImage0);

    //信息显示
    sprintf(myInform, "灰度直方图反向投影");
    LCD_ShowString(100, 510, myInform);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);

    PAUSE();
    LCD_Destory();
    return 0;
}





