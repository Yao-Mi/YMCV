#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_HistCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 表面模糊滤波为例，展示了将一幅图像进行滤波的滤波效果、以及参数的使用
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 700);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(mm01_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, mm01_gray, sizeof(mm01_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 375, 500, CVGrayType);//灰度图
    //显示原图
    LCD_show_image(0, 0, myImage0);


    //创建空间渲染器
    uint8 kernel_r = 1;
    uint8 T_v = 20;
    YMCV_PADDING_VAL = 50;//输出边界填充值

    //由于缓存结果
    CVIMAGE myImage1 = NULL;

    /*--------------------------表面模糊滤波 修改滤波半径--------------------------------------------------*/
    for (kernel_r = 1; kernel_r < 10; kernel_r++)
    {
        //使用表面模糊进行滤波
        myImage1 = YMCV_Gray_SurfaceBlur(myImage0, kernel_r, T_v, OutPaddingConst);
        //显示图片1
        LCD_show_image(500, 0, myImage1);

        //信息显示
        sprintf(myInform, "表面模糊 处理半径: %02d 阈值:%d  . ", kernel_r, T_v);
        LCD_ShowString(100, 510, myInform);
        //暂停
        PAUSE();

        //用释放图像，避免下次用的时候导致内存泄漏
        YMCV_Free_ImgFree(myImage1);
    }


    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);

    PAUSE();
    LCD_Destory();
    return 0;
}








