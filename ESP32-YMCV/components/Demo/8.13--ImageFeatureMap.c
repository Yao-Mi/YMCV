#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以 特征图提取 为例，展示了 LBP特征图的提取
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(mm02_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, (void*)mm02_gray, sizeof(mm02_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVGrayType);//灰度图0

    LCD_show_image(stx + 0, 0, myImage0_gray);
	  int sty =  myImage0_gray->height;
    /*------------------------ LBP特征图提取 ------------------------------*/

    CVIMAGE myImage1 = YMCV_Image_FeatureMap_Detect(myImage0_gray, CV_FeatMap_LBP);
    LCD_show_image(stx + 0, sty + 0, myImage1);

    //信息显示
    sprintf(myInform, "LBP featMap");
    LCD_ShowString(stx + 0, sty + 0, myInform);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage1);
    Delay(500);
    /*------------------------ 最小循环LBP特征图提取 ------------------------------*/

    myImage1 = YMCV_Image_FeatureMap_Detect(myImage0_gray, CV_FeatMap_MRLBP);
    LCD_show_image(stx + 0, sty + 0, myImage1);

    //信息显示
    sprintf(myInform, "MRLBP featMap");
    LCD_ShowString(stx + 0, sty + 0, myInform);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage1);
    Delay(500);
    /*------------------------ USAN特征图提取 ------------------------------*/

    myImage1 = YMCV_Image_FeatureMap_Detect(myImage0_gray, CV_FeatMap_USAN);
    LCD_show_image(stx + 0, sty + 0, myImage1);

    //信息显示
    sprintf(myInform, "USAN  featMap ");
    LCD_ShowString(stx + 0, sty + 0, myInform);
    Delay(500);

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage1);
}





