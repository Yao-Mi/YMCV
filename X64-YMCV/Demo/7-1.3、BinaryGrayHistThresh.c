#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_BinaryCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 Otsu阈值分割为例，展示了灰度图分割的效果
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 450);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(mm01_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, mm01_gray, sizeof(mm01_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 375, 500, CVGrayType);//灰度图

       //直方图数组
    int32 myImage0Hist[256];
    /*--------------------------灰度图直方图--------------------------------------------------*/
    //获取图像直方图
    YMCV_Image_Hist_Get(myImage0, myImage0Hist, 256, PIXChannelNum1);
    //显示image0图片与直方图
    LCD_show_image(0, 0, myImage0);
    LCDHistShow(myImage0Hist, PIXChannelNum1, 350, 140, 128);

    /*--------------------------灰度Otsu阈值二值化--------------------------------------------------*/
    //创建输出二值图像
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);//二值图
    CV_binary_param myParam = {.hist_auto_t.thresh_type= CV_HistOtsu };//初始化为otsu阈值

    //创建二值变换渲染器
    CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_HistThreshold);

    //设置为otsu阈值法
    myParam.hist_auto_t.thresh_type = CV_HistOtsu;//阈值类型有多种可选，若使用p-tile则需设置先验百分比参数
    //myParam.hist_auto_t.p_tile_val = 40;//设置先验百分比参数
    
    //加载参数进行配置
    YMCV_BinaryKernel_Load(myRender1, &myParam);

    //将目标图像的进行渲染
    YMCV_Image_ThreshTo_Binary(myImage0, myImage2, myRender1);

    //显示图片2
    LCD_show_image(500, 0, myImage2);

    //信息显示
    sprintf(myInform, "二值图像 Otsu阈值  ");
    LCD_ShowString(100, 310, myInform);
    //暂停
    PAUSE();

    //释放渲染器，避免下次用的时候导致内存泄漏
    YMCV_Free_Binary_RenderFree(myRender1);

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage2);

    PAUSE();
    LCD_Destory();
    return 0;
}





