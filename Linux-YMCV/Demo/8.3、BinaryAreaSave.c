#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_FeatCal.h"
#include"../YMCV/CORE/YMCV_PixConv.h"
#include <stdlib.h>

/*******************************************************************************************
    *
    * 说明  该例程以 区域保留为例，展示了 保留较大面积区域的效果
    *
    ***************************************************************************************/
int main()
{
    int i;
    char myInform[100];
    LCD_Init(1000, 450);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(mm00_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, mm00_gray, sizeof(mm00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//灰度图0


    LCD_Clear(0);
    LCD_show_image(0, 0, myImage0);
    /*--------------------------灰度双阈值二值化--------------------------------------------------*/
    //创建输出二值图像
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);//二值图
    CV_binary_param myParam = { .gray_double_t.thresh_min = 100 ,.gray_double_t.thresh_max = 150 };

    //创建二值变换渲染器
    CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_DoubleThreshold);
    YMCV_BinaryKernel_Load(myRender1, &myParam);//加载参数进行渲染配置、初始化
    YMCV_Image_ThreshTo_Binary(myImage0, myImage2, myRender1);//将目标图像的进行渲染
    
    //显示原图
    LCD_show_image(300, 0, myImage2);
    PAUSE();
    /*------------------------ 二值区域过滤 ------------------------------*/
    //保留面积大于100的区域
    YMCV_Binary_AreaFilter(myImage2, 100);
    LCD_show_image(300, 0, myImage2);

    //信息显示
    sprintf(myInform, "保留图  ");
    LCD_ShowString(400, 310, myInform);
    //释放内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_Binary_RenderFree(myRender1);

    PAUSE();
    LCD_Destory();
    return 0;
}

