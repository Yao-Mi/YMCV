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
    * 说明  该例程以 GMM聚类为例，展示了图像的聚类效果
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

    /*-------------------------- GMM 聚类 --------------------------------------------------*/
    int classnum =3;//设置聚类数目
    int maxItera = 100;//最多迭代100次
    CVIMAGE myImage2 = YMCV_Gray_GMM_Clustering(myImage0, classnum, maxItera);

    LCD_Clear(0);
    //显示图片0
    LCD_show_image(0, 0, myImage0);
    //显示图片2
    LCD_show_image(500, 0, myImage2);

    //信息显示
    sprintf(myInform, "GMM  聚类：%d个 ， 最大迭代：%d次", classnum, maxItera);
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









