#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_EdgeCal.h"
#include"../YMCV/CORE/YMCV_SegnmCal.h"

/*******************************************************************************************
    *
    * 说明  该例程以 区域生长为例，展示了mask二值图创建
    *
    ***************************************************************************************/

int main()
{
    char myInform[100];
    LCD_Init(1000, 450);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(mm00_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, mm00_gray, sizeof(mm00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//灰度图0

    /*--------------------------灰度区域生长--------------------------------------------------*/
    int x = 80, y = 80;

    for (int i = 0; i < 10; i++)
    {
        int thresh = 30;
        //区域生长 返回mask二值图
        CVIMAGE myImage2 = YMCV_Gray_Region_Growing8(myImage0, x, y, thresh);

        LCD_Clear(0);
        //显示图片0
        LCD_show_image(0, 0, myImage0);
        //绘制种子点位置
        color24_t cvl = { .R = 255,.G = 120,.B = 50 };
        LCD_Fill_ConstRgbRect(x - 1, y - 1, 3, 3, cvl);
        //显示图片2
        LCD_show_image(500, 0, myImage2);

        //信息显示
        sprintf(myInform, "区域生长 种子坐标 %d %d, 生长阈值：%d",x,y, thresh);
        LCD_ShowString(100, 310, myInform);
        //暂停
        PAUSE();
    
        x += 8;//移动种子点

        //释放图像内存
        YMCV_Free_ImgFree(myImage2);
    }

 

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);

    PAUSE();
    LCD_Destory();
    return 0;
}









