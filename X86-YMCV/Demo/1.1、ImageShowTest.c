#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
/*******************************************************************************************
    *
    * 说明  该例程展示了 在不破坏原始的情况下，将数据封装为图片，以及图片的备份，与图片显示
    * 
    ***************************************************************************************/
int main()
{
    LCD_Init(640, 480);
 
    //为了不破坏原始数据，重新申请数据内存
    uint8* myData = CV_malloc1(sizeof(linglong00));//必须采用CV_malloc1为图片数据申请内存
    //把数据拷贝到申请区
    CV_memcpy(myData, linglong00, sizeof(linglong00));
   //将数据封装为图片格式 
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8 *)myData, 500, 255, CVRgb16Type);


    //重新拷贝一份该图
    CVIMAGE myImage1 = YMCV_Creat_Img_BackUp(myImage0);
    //显示图片
    LCD_show_image(0,0, myImage1);
    //暂停
    PAUSE();

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);


    PAUSE();
    LCD_Destory();
    return 0;
}





