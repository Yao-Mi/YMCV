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

    uint8* myData1 = CV_malloc1(sizeof(linglong01_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData1, linglong01_gray, sizeof(linglong01_gray));
    CVIMAGE myImage1 = YMCV_Creat_DataToImg((uint8*)myData1, 500, 255, CVGrayType);//灰度图1

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

    PAUSE();


    /*--------------------------灰度图直方图规定化--------------------------------------------------*/

    ////手动将直方图规定为某个形状
    //for (int i = 0; i < 256; i++)
    //{
    //    myImage1Hist[i] = i + 50;//线性函数，即直方图为梯形角形
    //}
    //从图像1获取直方图与显示
    YMCV_Image_Hist_Get(myImage1, myImage1Hist, 256, PIXChannelNum1);
    LCD_show_image(500, 0, myImage1);
    LCDHistShow(myImage1Hist, PIXChannelNum1, 550, 420, 128);
    PAUSE();

    //对图像0进行直方图规定化
    YMCV_Image_Hist_To(myImage0, myImage2, myImage1Hist, 256, PIXChannelNum1);
    //获取图像直方图与显示
    YMCV_Image_Hist_Get(myImage2, myImage1Hist, 256, PIXChannelNum1);
    LCD_show_image(500, 0, myImage2);
    LCDHistShow(myImage1Hist, PIXChannelNum1, 550, 420, 128);//由于大小放缩到128，经过转换形状可能不真实，可以用256观察真实直方图

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);

    PAUSE();
    LCD_Destory();
    return 0;
}





