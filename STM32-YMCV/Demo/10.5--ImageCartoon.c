#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以 卡通特效为例，展示了卡通画效果
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(mm02_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, (void*)mm02_gray, sizeof(mm02_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVGrayType);//灰度图

    LCD_Clear(GRAY);
    //显示原图
    LCD_show_image(stx + 0, 0, myImage0_gray);
    /*-------------------------- 卡通效果 --------------------------------------------------*/

    CVIMAGE feImg = YMCV_Gray_Cartoon_Effect(myImage0_gray,1);
    LCD_show_image(stx, myImage0_gray->height, feImg);
    //信息显示
    sprintf(myInform, "Cartoon Effect ");
    LCD_ShowString(stx, myImage0_gray->height, myInform);

    //释放内存
    YMCV_Free_ImgFree(feImg);
    YMCV_Free_ImgFree(myImage0_gray);

    Delay(500);
}

