#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_GeomCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 图像极坐标变换为例，展示了图片对数极坐标下的投影效果
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


    /*--------------------------投影到对数极坐标下--------------------------------------------------*/
    CVIMAGE myImage2 = NULL, myImage3 = NULL;
    YMCV_PADDING_VAL = 50;//空白区填充值
    //加载图片0的渲染参数
    CV_geom_param myParam = { .polar.Imgin_w = myImage0->width,
                              .polar.Imgin_h = myImage0->height };
    //创建几何变换渲染器
    CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_LogPolar);

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
    sprintf(myInform, "图像 对数极坐标变换  . ");
    LCD_ShowString(100, 310, myInform);
    //暂停
    PAUSE();

    /*--------------------------投影回笛卡尔坐标系--------------------------------------------------*/
    //加载图片2的渲染参数
    myParam.rotate.Imgin_w = myImage2->width;
    myParam.rotate.Imgin_h = myImage2->height;
    //创建几何变换渲染器
    CVGEOM_RENDER myRender2 = YMCV_Creat_GeomRender_Creat(CV_LogPolar_Inverse);

    //使用参数进行几何渲染配置、初始化
    YMCV_GeomKernel_Load(myRender2, &myParam);

    //将目标图像的进行渲染
    myImage3 = YMCV_Image_Geom_RenderTo(myImage2, myRender2);
    //结果图像必然是奇数的行列，因为必须有一个中心点

    LCD_Clear(0);
    //显示图片0
    LCD_show_image(0, 0, myImage2);
    //显示图片2
    LCD_show_image(500, 0, myImage3);


    //信息显示
    sprintf(myInform, "图像 对数极坐标逆变换  . ");
    LCD_ShowString(200, 310, myInform);

    //释放渲染器
    YMCV_Free_GEOM_RenderFree(myRender1);
    YMCV_Free_GEOM_RenderFree(myRender2);
    //用释放图像，避免下次用的时候导致内存泄漏
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);

    PAUSE();
    LCD_Destory();
    return 0;
}





