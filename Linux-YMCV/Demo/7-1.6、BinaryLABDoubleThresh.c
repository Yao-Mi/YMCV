#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_BinaryCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 LAB阈值分割为例，展示了LAB二值化效果
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 450);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(linglong00));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, linglong00, sizeof(linglong00));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 500, 255, CVRgb16Type);//彩色图0


    /*--------------------------彩色LAB双阈值二值化--------------------------------------------------*/
    //创建输出二值图像
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);//二值图
    CV_binary_param myParam = { .lab_double_t.thresh_Lmin = 0 ,.lab_double_t.thresh_Lmax = 100 ,
     .lab_double_t.thresh_Amin = -128 ,.lab_double_t.thresh_Amax = 127 ,
     .lab_double_t.thresh_Bmin = -128 ,.lab_double_t.thresh_Bmax = 127 };

    for (int i = 1; i < 10; i++)
    {
        //设置参数
        myParam.lab_double_t.thresh_Lmin = 6 * i;
        myParam.lab_double_t.thresh_Lmax = 100;

        myParam.lab_double_t.thresh_Amin = 14 * i - 128;
        myParam.lab_double_t.thresh_Amax = 127 - 12 * i;

        myParam.lab_double_t.thresh_Bmin = 14 * i - 128;
        myParam.lab_double_t.thresh_Bmax = 127 - 10 * i;

        //创建二值变换渲染器
        CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_LAB2Threshold);

        //加载参数进行渲染配置、初始化
        YMCV_BinaryKernel_Load(myRender1, &myParam);

        //将目标图像的进行渲染
        YMCV_Image_ThreshTo_Binary(myImage0, myImage2, myRender1);

        LCD_Clear(0);
        //显示图片0
        LCD_show_image(0, 0, myImage0);
        //显示图片2
        LCD_show_image(500, 0, myImage2);

        //信息显示
        sprintf(myInform, "二值图像 阈值: L[%d %d] A[%d %d] B[%d %d]    ",
            myParam.lab_double_t.thresh_Lmin,
            myParam.lab_double_t.thresh_Lmax,
            myParam.lab_double_t.thresh_Amin,
            myParam.lab_double_t.thresh_Amax,
            myParam.lab_double_t.thresh_Bmin,
            myParam.lab_double_t.thresh_Bmax);

        LCD_ShowString(100, 310, myInform);
        //暂停
        PAUSE();

        //释放渲染器，避免下次用的时候导致内存泄漏
        YMCV_Free_Binary_RenderFree(myRender1);
    }

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage2);

    PAUSE();
    LCD_Destory();
    return 0;
}





