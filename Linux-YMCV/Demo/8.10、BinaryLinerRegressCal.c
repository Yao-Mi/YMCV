#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_EdgeCal.h"
#include"../YMCV/CORE/YMCV_FeatCal.h"
#include"../YMCV/CORE/YMCV_BasicObjCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 二值图像素回归为例，展示了线性回归效果，
    *  该方法通过将图像重采样为不超过25x25大小的图像，进行回归分析后再将其变换回原图，可能会出现与原图实际直线不完全重合的情况
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 550);
    color24_t pencolor = { .R = 255 };
    LCD_Point_Color_Set(pencolor);//设置画笔颜色

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(xunxian02_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, xunxian02_gray, sizeof(xunxian02_gray));
    
    for (int i = 0; i < sizeof(xunxian02_gray); i++)
    {
        myData[i] = (myData[i] < 127) ? 255 : 0; //快速二值化
    }
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVBinaryType);//灰度图0

    /*-------------------------最小二乘 直线回归拟合--------------------------------------------------*/
    CVline imgline = YMCV_BinaryMax25x25_Regression_Line(myImage0, CVRegress_LSE);
    LCD_Clear(0);
    LCD_show_image(0, 0, myImage0);//显示图片0
    LCD_Draw_Line(imgline.x1, imgline.y1, imgline.x2, imgline.y2);//绘制直线

    //信息显示
    sprintf(myInform, "最小二乘 直线回归 ");
    LCD_ShowString(100, 310, myInform);
    PAUSE();


    /*-------------------------泰尔森 直线回归拟合--------------------------------------------------*/
    imgline = YMCV_BinaryMax25x25_Regression_Line(myImage0, CVRegress_TSE);
    LCD_Clear(0);
    LCD_show_image(0, 0, myImage0);//显示图片0
    LCD_Draw_Line(imgline.x1, imgline.y1, imgline.x2, imgline.y2);//绘制直线
    //信息显示
    sprintf(myInform, "泰尔森 直线回归 ");
    LCD_ShowString(100, 310, myInform);
    PAUSE();


    /*-------------------------RANSAC 直线回归拟合--------------------------------------------------*/
    imgline = YMCV_BinaryMax25x25_Regression_Line(myImage0, CVRegress_RANSAC);
    LCD_Clear(0);
    LCD_show_image(0, 0, myImage0);//显示图片0
    LCD_Draw_Line(imgline.x1, imgline.y1, imgline.x2, imgline.y2);//绘制直线
    //信息显示
    sprintf(myInform, "RANSAC 直线回归 ");
    LCD_ShowString(100, 310, myInform);
    PAUSE();


    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);

    PAUSE();
    LCD_Destory();
    return 0;
}





