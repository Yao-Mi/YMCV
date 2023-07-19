#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_MatchingCal.h"
#include"../YMCV/CORE/YMCV_AreaFillCal.h"

/*******************************************************************************************
    *
    * 说明  该例程以 相位相关匹配为例，展示图像平移后的匹配效果
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 480);
    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = (uint8*)CV_malloc1(sizeof(linglong00_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, linglong00_gray, sizeof(linglong00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg(myData, 500, 255, CVGrayType);//灰度图

        //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData2 = (uint8*)CV_malloc1(sizeof(linglong00b_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData2, linglong00b_gray, sizeof(linglong00b_gray));
    CVIMAGE myImage_0 = YMCV_Creat_DataToImg(myData2, 100, 87, CVGrayType);//灰度图

    /*---------------------------- 图像平移 --------------------------------------------*/
    //加载图片0的渲染参数
    CV_geom_param myParam = { .perspect.Imgin_w = myImage0->width,
                              .perspect.Imgin_h = myImage0->height,
                              .perspect.wsize_type = CV_WinSize_Const,//固定窗口
                              .perspect.scale = 1.0f };
    //修改渲染参数
    myParam.perspect.x_angle = 0;
    myParam.perspect.y_angle = 0;
    myParam.perspect.z_angle = 0;
    //单次偏移量需保持在[±宽 / 2，±高 / 2]范围内，
    myParam.perspect.x_move = 100; //平移变换
    myParam.perspect.y_move = 50;
    myParam.perspect.scale = 1;
    //创建几何变换渲染器
    CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_Perspective);

    //使用参数进行几何渲染配置、初始化
    YMCV_GeomKernel_Load(myRender1, &myParam);

    //将目标图像的进行渲染
    CVIMAGE myImage1 = YMCV_Image_Geom_RenderTo(myImage0, myRender1);

    LCD_show_image(0, 0, myImage0);
    LCD_show_image(500, 0, myImage1);
    //信息显示
    sprintf(myInform, " 图像平移 ");
    LCD_ShowString(100, 300, myInform);
    PAUSE();

    /*-------------------------- 相位相关匹配 --------------------------------------------------*/
    float32 moveX, moveY;
    YMCV_Image_Gray_PhaseCorrelate(myImage0, myImage1, &moveX, &moveY);
    //显示图片
    LCD_show_image(0, 0, myImage0);

    //信息显示
    sprintf(myInform, "相位相关匹配 结果偏移：x%.2f y%.2f ", moveX, moveY);
    LCD_ShowString(100, 300, myInform);
    PAUSE();

    //释放内存
    YMCV_Free_GEOM_RenderFree(myRender1);
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage_0);
    YMCV_Free_ImgFree(myImage1);

    LCD_Destory();
    return 0;
}


