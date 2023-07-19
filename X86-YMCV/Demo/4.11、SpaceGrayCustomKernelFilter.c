#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_HistCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 自定义滤波为例，展示了将一幅图像使用自定义滤波核进行浮雕显示的效果
    ***************************************************************************************/
    int main()
{
    char myInform[100];
    LCD_Init(1000, 700);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(mm01_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, mm01_gray, sizeof(mm01_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 375, 500, CVGrayType);//灰度图
    //显示原图
    LCD_show_image(0, 0, myImage0);


    //创建自定义空间渲染器
    uint8 kernel_r = 1;
    uint8 Kernel_N = (2 * kernel_r + 1) * (2 * kernel_r + 1);
    //注意：卷积模板大小取为正方形NxN，如r=1，则N=3。
    CVKERN_RENDER myKernel = YMCV_Creat_SpaceRender_Creat(CV_CustomFilter, PIXChannelNum1, kernel_r, OutPaddingConst);
    YMCV_PADDING_VAL = 125;//设置填充值

    //由于缓存结果
    CVIMAGE myImage1 = NULL;
    float32 FDkernel0[9] = { -1,-1, 0,  //浮雕滤波核
                           -1, 0, 1,
                           0, 1, 1 };
    float32 FDkernel1[9] = { -1,-1, 0,  //浮雕效果
                             -1, 1, 1,
                              0, 1, 1 };
    float32 FDkernel2[9] = { 2, 0, 0,
                             0,-1, 0,
                             0, 0,-1 };
    /*--------------------------浮雕滤波---------------------------------------------*/

        //手动对参数进行渲染器通道配置、初始化
    CV_memcpy(myKernel->data,FDkernel1, Kernel_N*sizeof(float32));

        //使用自定义空间渲染器进行渲染灰度图片
        myImage1 = YMCV_Image_Space_CustomKernel_Render(myImage0, myKernel);
        //显示图片1
        LCD_show_image(500, 0, myImage1);

        //用完就释放图像，避免下次用的时候导致内存泄漏
        YMCV_Free_ImgFree(myImage1);

        //信息显示
        sprintf(myInform, "自定义滤波核 .");
        LCD_ShowString(100, 510, myInform);
        //暂停
        PAUSE();

    
    //释放渲染器
    YMCV_Free_KERN_RenderFree(myKernel);
  
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);

    PAUSE();
    LCD_Destory();
    return 0;
}





