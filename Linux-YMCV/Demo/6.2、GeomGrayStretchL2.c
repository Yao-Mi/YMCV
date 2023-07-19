#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_GeomCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 图像伸缩变换为例，展示了线性插值的效果
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

    uint8* myData1 = CV_malloc1(sizeof(xgmm_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData1, xgmm_gray, sizeof(xgmm_gray));
    CVIMAGE myImage1 = YMCV_Creat_DataToImg((uint8*)myData1, 50, 75, CVGrayType);//灰度图1


    /*--------------------------双线性插值--------------------------------------------------*/
    CVIMAGE myImage2 = NULL;
    CV_geom_param myParam = { .stretch.Imgin_w = myImage0->width,
                              .stretch.Imgin_h = myImage0->height,
                              .stretch.out_w = myImage0->width,
                              .stretch.out_h = myImage0->height, };
    float k = 1.0;
    for (int i = 1; i < 10; i++)
    {
        //初始化参数
        myParam.stretch.out_w = (int)(k * myImage0->width);
        myParam.stretch.out_h = (int)(k * myImage0->height);

        //创建几何变换渲染器
        CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_StretchL2);

        //使用参数进行几何渲染配置、初始化
        YMCV_GeomKernel_Load(myRender1, &myParam);

        //将目标图像的进行渲染
        myImage2 = YMCV_Image_Geom_RenderTo(myImage0, myRender1);

        LCD_Clear(0);
        //显示图片0
        LCD_show_image(0, 0, myImage0);
        //显示图片2
        LCD_show_image(500, 0, myImage2);

        //信息显示
        sprintf(myInform, "双线性插值伸缩图像 k: %02.1f   . ", k);
        LCD_ShowString(100, 310, myInform);
        //暂停
        PAUSE();


        if (i < 4)
        {
            k -= 0.2;
        }
        else
        {
            k += 0.2;
        }
        //释放渲染器
        YMCV_Free_GEOM_RenderFree(myRender1);
        //用释放图像，避免下次用的时候导致内存泄漏
        YMCV_Free_ImgFree(myImage2);
    }

    //通用参数初始化
    myParam.stretch.Imgin_w = myImage1->width;
    myParam.stretch.Imgin_h = myImage1->height;
    k = 1.0;
    for (int i = 1; i < 10; i++)
    {
        //初始化参数
        myParam.stretch.out_w = (int)(k * myImage1->width);
        myParam.stretch.out_h = (int)(k * myImage1->height);

        //创建几何变换渲染器
        CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_StretchL2);

        //使用参数进行几何渲染配置、初始化
        YMCV_GeomKernel_Load(myRender1, &myParam);

        //将目标图像的进行渲染
        myImage2 = YMCV_Image_Geom_RenderTo(myImage1, myRender1);


        LCD_Clear(0);
        //显示图片0
        LCD_show_image(0, 0, myImage1);
        //显示图片2
        LCD_show_image(500, 0, myImage2);

        //信息显示
        sprintf(myInform, "双线性插值伸缩图像 k: %02.2f   . ", k);
        LCD_ShowString(100, 310, myInform);
        //暂停
        PAUSE();

        if (i < 4)
        {
            k *= 0.8;
        }
        else
        {
            k *= 1.6;
        }

        //释放渲染器
        YMCV_Free_GEOM_RenderFree(myRender1);
        //用释放图像，避免下次用的时候导致内存泄漏
        YMCV_Free_ImgFree(myImage2);
    }
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);

    PAUSE();
    LCD_Destory();
    return 0;
}





