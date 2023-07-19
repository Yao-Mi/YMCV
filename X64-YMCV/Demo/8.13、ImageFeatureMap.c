#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_FeatCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 特征图提取 为例，展示了 LBP特征图的提取
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(640, 480);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(mm00_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, mm00_gray, sizeof(mm00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//灰度图0

    /*------------------------ LBP特征图提取 ------------------------------*/

    CVIMAGE myImage1 = YMCV_Image_FeatureMap_Detect(myImage0, CV_FeatMap_LBP);
    LCD_show_image(0, 0, myImage1);
    LCD_show_image(300, 0, myImage0);

    //信息显示
    sprintf(myInform, "LBP特征图  ");
    LCD_ShowString(400, 310, myInform);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage1);
    PAUSE();
    /*------------------------ 最小循环LBP特征图提取 ------------------------------*/

    myImage1 = YMCV_Image_FeatureMap_Detect(myImage0, CV_FeatMap_MRLBP);
    LCD_show_image(0, 0, myImage1);

    //信息显示
    sprintf(myInform, "最小循环LBP特征图  ");
    LCD_ShowString(400, 310, myInform);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage1);
    PAUSE();
    /*------------------------ USAN特征图提取 ------------------------------*/

    myImage1 = YMCV_Image_FeatureMap_Detect(myImage0, CV_FeatMap_USAN);
    LCD_show_image(0, 0, myImage1);

    //信息显示
    sprintf(myInform, "USAN特征图  ");
    LCD_ShowString(400, 310, myInform);

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);

    PAUSE();
    LCD_Destory();
    return 0;
}





