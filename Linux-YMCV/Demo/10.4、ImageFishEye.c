#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_Special_Effect.h"
#include"../YMCV/CORE/YMCV_AreaFillCal.h"
#include <stdlib.h>

/*******************************************************************************************
    *
    * 说明  该例程以 FishEye为例，展示了鱼眼模拟及矫正效果
    *
    ***************************************************************************************/

int main()
{
    int i;
    char myInform[100];
    LCD_Init(1100, 450);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(house_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, house_gray, sizeof(house_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 500, 270, CVGrayType);//灰度图0

    LCD_Clear(0);
    //显示原图
    LCD_show_image(0, 0, myImage0);
    /*-------------------------- 鱼眼效果 --------------------------------------------------*/
   
    CVIMAGE feImg = YMCV_Gray_FishEye_Effect(myImage0, myImage0->width, myImage0->height);
    LCD_show_image(500, 0, feImg);
    //信息显示
    sprintf(myInform, "鱼眼效果图  ");
    LCD_ShowString(400, 310, myInform);
    PAUSE();
    /*-------------------------- 透镜失真矫正 --------------------------------------------------*/
    YMCV_PADDING_VAL = 100;//空白处填充值
    CVIMAGE invfeImg = YMCV_Gray_invFishEye_Effect(feImg,4, 0.43, 0.85);
    LCD_show_image(0, 0, invfeImg);
    //信息显示
    sprintf(myInform, "鱼眼失真矫正图  ");
    LCD_ShowString(400, 310, myInform);

    //释放内存
    YMCV_Free_ImgFree(feImg);
    YMCV_Free_ImgFree(myImage0);

    PAUSE();
    LCD_Destory();
    return 0;
}

