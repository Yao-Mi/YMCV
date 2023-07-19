#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_AreaFillCal.h"
#include <stdlib.h>

/*******************************************************************************************
    *
    * 说明  该例程以 扫描线种子点填充为例，展示了种子填充的效果
    *
    ***************************************************************************************/

int main()
{
    int i;
    char myInform[100];
    LCD_Init(1000, 450);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(mm00_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, mm00_gray, sizeof(mm00_gray));
    uint8* myData2 = CV_malloc1(sizeof(mm00));
    CV_memcpy(myData2, mm00, sizeof(mm00));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//灰度图0
    CVIMAGE myImage2 = YMCV_Creat_DataToImg((uint8*)myData2, 250, 140, CVRgb16Type);

    /*-------------------------- 种子点填充--------------------------------------------------*/
    LCD_Clear(0);
    //显示原图
    LCD_show_image(0, 0, myImage0);
    color24_t thiscolor = { .R = 255 };

    CVcolor fillval;
    fillval.gray =255;//填充值
    CVpoint seed = { .x = 130,.y = 45 };//种子点坐标
    int8 floaterror = 60;//种子点附近值，浮动误差为±50，即填充当前点颜色值±50的连通区域
    LCD_Fill_ConstRgbRect(seed.x - 1, seed.y - 1, 3, 3, thiscolor);

    //对灰度图进行种子填充
    YMCV_Image_ScanLine_SeedFill(myImage0, seed, floaterror, fillval);

    LCD_show_image(300, 0, myImage0);
    //信息显示
    sprintf(myInform, "种子区域填充图  ");
    LCD_ShowString(400, 310, myInform);
    PAUSE();

    fillval.rgb16 = 0xfe00;
    //对彩色图进行种子填充
    YMCV_Image_ScanLine_SeedFill(myImage2, seed, floaterror, fillval);

    LCD_show_image(600, 0, myImage2);
    //信息显示
    sprintf(myInform, "彩色图填充  ");
    LCD_ShowString(700, 310, myInform);

    //释放内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage2);

    PAUSE();
    LCD_Destory();
    return 0;
}

