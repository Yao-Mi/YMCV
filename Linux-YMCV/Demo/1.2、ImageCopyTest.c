#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
/*******************************************************************************************
    *
    * 说明  该例程展示了 图片创建，以及图片的截取备份
    *
    ***************************************************************************************/
int main()
{
    LCD_Init(640, 480);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(linglong00));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, linglong00, sizeof(linglong00));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 500, 255, CVRgb16Type);

    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(200,200, CVRgb16Type);

    //截取灵笼图片中一个区域拷贝出来，转为一幅新的图像
    CVIMAGE myImage2 = YMCV_Creat_ImgCopyTo(myImage0, 100, 20, 200, 200);
    //采用ROI的方式截取
    CVrect myroi = { .x = 200,.y = 20,.w = 200,.h = 200 };
    CVIMAGE myImage3 = YMCV_Creat_ImgROICopyTo(myImage0, &myroi);


    //显示图片1
    LCD_show_image(0, 0, myImage1);
    //暂停
    PAUSE();

    //显示图片2
    LCD_show_image(0, 0, myImage2);
    //暂停
    PAUSE();

    //显示图片3
    LCD_show_image(0, 0, myImage3);
    //暂停
    PAUSE();

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);


    PAUSE();
    LCD_Destory();
    return 0;
}





