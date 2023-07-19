#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_HistCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 众数和中点滤波为例，展示了一幅图像 的滤波效果
    *
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


    //创建空间渲染器
    uint8 kernel_r = 0;
    CVKERN_RENDER myKernel;
    YMCV_PADDING_VAL = 125;//设置填充值


    //加载渲染器参数，便于进行初始化
    CV_kernel_1param myParam;//均值滤波无需加载

    //由于缓存结果
    CVIMAGE myImage1 = NULL;

    /*--------------------------众数滤波 修改滤波半径--------------------------------------------------*/
    for (kernel_r = 1; kernel_r < 10; kernel_r++)
    {
        //创建相应半径的滤波核
        myKernel = YMCV_Creat_SpaceRender_Creat(CV_ModeFilter, PIXChannelNum1, kernel_r, OutPaddingConst);

        //使用参数进行渲染器通道配置、初始化
        YMCV_Space_SingleKernel_Load(myKernel, &myParam);//众数滤波无需加载，该步骤也可省略

        //使用空间渲染器进行渲染灰度图片
        myImage1 = YMCV_Image_Space_SigleKernel_Render(myImage0, myKernel);
        //显示图片1
        LCD_show_image(500, 0, myImage1);


        //信息显示
        sprintf(myInform, "众数滤波 滤波半径: %02d   . ", kernel_r);
        LCD_ShowString(100, 510, myInform);
        //暂停
        PAUSE();

        //用释放图像，避免下次用的时候导致内存泄漏
        YMCV_Free_ImgFree(myImage1);
        //释放空间渲染器
        YMCV_Free_KERN_RenderFree(myKernel);
    }

    /*--------------------------中点滤波 修改滤波半径--------------------------------------------------*/
    for (kernel_r = 1; kernel_r < 10; kernel_r++)
    {
        //创建相应半径的滤波核
        myKernel = YMCV_Creat_SpaceRender_Creat(CV_MidPointFilter, PIXChannelNum1, kernel_r, OutPaddingConst);

        //使用参数进行渲染器通道配置、初始化
        YMCV_Space_SingleKernel_Load(myKernel, &myParam);//中点滤波无需加载，该步骤也可省略

        //使用空间渲染器进行渲染灰度图片
        myImage1 = YMCV_Image_Space_SigleKernel_Render(myImage0, myKernel);
        //显示图片1
        LCD_show_image(500, 0, myImage1);


        //信息显示
        sprintf(myInform, "中点滤波 滤波半径: %02d   . ", kernel_r);
        LCD_ShowString(100, 510, myInform);
        //暂停
        PAUSE();

        //用释放图像，避免下次用的时候导致内存泄漏
        YMCV_Free_ImgFree(myImage1);
        //释放空间渲染器
        YMCV_Free_KERN_RenderFree(myKernel);
    }
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);

    PAUSE();
    LCD_Destory();
    return 0;
}








