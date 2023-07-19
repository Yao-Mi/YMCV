#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_EdgeCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 Sobel滤波器为例，展示了边缘检测效果
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


    /*--------------------------灰度Sobel边缘检测--------------------------------------------------*/
    //创建输出二值图像
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);//二值图
    CV_edge_param myParam = { .EdgeType = CV_Sobel ,.Thresh.sobel.minT=100 };

    YMCV_Gray_EdgeDetect(myImage0, myImage2, &myParam);
   
    LCD_Clear(0);
    //显示图片0
    LCD_show_image(0, 0, myImage0);
    //显示图片2
    LCD_show_image(500, 0, myImage2);

    //信息显示
    sprintf(myInform, "Sobel检测图像 ，阈值：%d  ", myParam.Thresh.sobel.minT);
    LCD_ShowString(100, 310, myInform);
    //暂停
    PAUSE();

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage2);

    PAUSE();
    LCD_Destory();
    return 0;
}





