#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_BinaryCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 灰度双阈值分割为例，展示了二值化的效果
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


    /*--------------------------灰度双阈值二值化--------------------------------------------------*/
    //创建输出二值图像
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);//二值图
    CV_binary_param myParam = { .gray_double_t.thresh_min = 0 ,.gray_double_t.thresh_max = 255};

    for (int i = 1; i < 10; i++)
    {

        //设置参数
        myParam.gray_double_t.thresh_min = 0;
        myParam.gray_double_t.thresh_max = 255 - 20 * i;


        //创建二值变换渲染器
        CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_DoubleThreshold);

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
        sprintf(myInform, "二值图像 阈值: [%d %d] ", myParam.gray_double_t.thresh_min,
                                                     myParam.gray_double_t.thresh_max);
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





