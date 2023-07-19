#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_HistCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 直方图均衡化、直方图规定化为例，展示了直方图的使用及显示
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 450);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(linglong00_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, linglong00_gray, sizeof(linglong00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 500, 255, CVGrayType);//灰度图0


    //直方图数组
    int32 myImage0Hist[256], myImage1Hist[256];

    /*--------------------------灰度图直方图均衡化--------------------------------------------------*/

    //获取图像直方图
    YMCV_Image_Hist_Get(myImage0, myImage0Hist, 256, PIXChannelNum1);
    //显示image0图片与直方图
    LCD_show_image(0, 0, myImage0);
    LCDHistShow(myImage0Hist, PIXChannelNum1, 50, 420, 128);

    //把image0备份一份
    CVIMAGE myImage2 = YMCV_Creat_Img_BackUp(myImage0);
    //将myImage2的进行直方图均衡化操作
    YMCV_Image_Hist_Equalize(myImage2, myImage2);//输出覆盖自身

    //获取图像直方图与显示
    YMCV_Image_Hist_Get(myImage2, myImage1Hist, 256, PIXChannelNum1);
    LCD_show_image(500, 0, myImage2);
    LCDHistShow(myImage1Hist, PIXChannelNum1, 550, 420, 128);//由于大小放缩到128，经过转换形状可能不真实，可以用256观察真实直方图

    //用完释放内存
    YMCV_Free_ImgFree(myImage2);
    PAUSE();

    /*--------------------------局部直方图去雾 修改滤波半径--------------------------------------------------*/
    uint8 ker_r = 60;
    //先Padding填充，扩充图像边界,扩充半径为r
    CVIMAGE myImage1 = YMCV_Image_Image_Padding(myImage0, ker_r, ker_r, CV_PaddingAutoSpace);
    //LCD_show_image(0, 0, myImage1);

    //局部直方图均衡
    myImage2 = YMCV_Gray_Local_Hist_Equalize(myImage1, ker_r, NOPadding);
    //获取图像直方图与显示
    YMCV_Image_Hist_Get(myImage2, myImage1Hist, 256, PIXChannelNum1);
    LCD_show_image(500, 0, myImage2);
    LCDHistShow(myImage1Hist, PIXChannelNum1, 550, 420, 128);//由于大小放缩到128，经过转换形状可能不真实，可以用256观察真实直方图


    //信息显示
    sprintf(myInform, "局部均衡半径: %d  ", ker_r);
    LCD_ShowString(100, 510, myInform);
    //释放内存
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage1);
    PAUSE();

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);

    PAUSE();
    LCD_Destory();
    return 0;
}





