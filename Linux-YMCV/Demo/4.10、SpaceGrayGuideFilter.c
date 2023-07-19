#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_HistCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 导向滤波为例，展示了将一幅图像 的引导滤波效果，
    *       通过一张引导图G，对目标图像P（输入图像）进行滤波处理，
    *       使得最后的输出图像大体上与目标图像P相似，但是纹理部分与引导图G相似
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

    //创建半径 = 6的最大值滤波核
    myKernel = YMCV_Creat_SpaceRender_Creat(CV_MaxFilter, PIXChannelNum1, 5, OutPaddingConst);
    //使用最大值滤波得到较优目标图像
    CVIMAGE myImage1 = YMCV_Image_Space_SigleKernel_Render(myImage0, myKernel);


    CVIMAGE myImage2 = NULL;
    /*--------------------------导向滤波 修改滤波半径--------------------------------------------------*/
    for (kernel_r = 1; kernel_r < 10; kernel_r++)
    {
        //将目标图像的纹理引导回原图，使用原图作为纹理引导图像进行引导滤波
        myImage2 = YMCV_Gray_Image_GuideFilter(myImage1, myImage0, OutPaddingConst, kernel_r,0.001);
        //显示图片1
        LCD_show_image(500, 0, myImage2);

        //信息显示
        sprintf(myInform, "引导滤波 滤波半径: %02d   . ", kernel_r);
        LCD_ShowString(100, 510, myInform);
        //暂停
        PAUSE();

        //用释放图像，避免下次用的时候导致内存泄漏
        YMCV_Free_ImgFree(myImage2);
    }


    //释放空间渲染器
    YMCV_Free_KERN_RenderFree(myKernel);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);

    PAUSE();
    LCD_Destory();
    return 0;
}








