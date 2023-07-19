#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_SpaceCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 二值图形态学滤波为例，展示了二值图像 的膨胀和腐蚀效果
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(500, 300);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(tuxin_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, tuxin_gray, sizeof(tuxin_gray));

    for (int i = 0; i < sizeof(tuxin_gray); i++)
    {
        myData[i] = (myData[i] > 127) ? 0 : 255;//快速二值化
    }
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVBinaryType);//二值图0
    //显示原图
    LCD_show_image(0, 0, myImage0);


    //创建空间渲染器
    uint8 kernel_d = 0;
    CVFMAT myKernel;

    //由于缓存结果
    CVIMAGE myImage1 = NULL;

    /*--------------------------形态学膨胀滤波 修改滤波半径--------------------------------------------------*/
    for (kernel_d = 1; kernel_d < 10; kernel_d +=2)
    {
        //创建相应半径的形态学滤波核
        myKernel = YMCV_Creat_FMAT_Creat(kernel_d, kernel_d);

        //手动设置击中核结构
        CV_memset(myKernel->datap,0, myKernel->row * myKernel->col*sizeof(float));//全部初始为0
        //选择需要击中的部分记为1
        for (int i = 0; i < myKernel->row; i++)
        {
            for (int j = 0; j < myKernel->col; j++)
                myKernel->datap[i * myKernel->col + j] = 1.0f;
        }
        //计算形态学击中效果
        myImage1 = YMCV_Binary_Morph_Hit_Cal(myImage0, myKernel,1);//输出击中率>1% 的结果，膨胀效果
       
        //显示图片1
        LCD_show_image(250, 0, myImage1);

        //信息显示
        sprintf(myInform, "膨胀滤波 滤波直径: %02d   . ", kernel_d);
        LCD_ShowString(100, 200, myInform);
        //暂停
        PAUSE();

        //用释放图像，避免下次用的时候导致内存泄漏
        YMCV_Free_ImgFree(myImage1);
        //释放空间渲染器
        YMCV_Free_FMatFree(myKernel);
    }

    ///*--------------------------形态学腐蚀滤波 修改滤波半径--------------------------------------------------*/
    for (kernel_d = 1; kernel_d < 10; kernel_d +=2)
    {
        //创建相应半径的形态学滤波核
        myKernel = YMCV_Creat_FMAT_Creat(kernel_d, kernel_d);

        //手动设置击中核结构
        CV_memset(myKernel->datap, 0, myKernel->row * myKernel->col * sizeof(float));//全部初始为0
        //选择需要击中的部分记为1
        for (int i = 0; i < myKernel->row; i++)
        {
            for (int j = 0; j < myKernel->col; j++)
            {
                myKernel->datap[i * myKernel->col + j] = 1.0f;
            }
        }

        //计算形态学击中结果
        myImage1 = YMCV_Binary_Morph_Hit_Cal(myImage0, myKernel,99);//输出击中率>99%的部分 ，腐蚀效果

        //显示图片1
        LCD_show_image(250, 0, myImage1);

        //信息显示
        sprintf(myInform, "腐蚀滤波 滤波直径: %02d   . ", kernel_d);
        LCD_ShowString(100, 200, myInform);
        //暂停
        PAUSE();

        //用释放图像，避免下次用的时候导致内存泄漏
        YMCV_Free_ImgFree(myImage1);
        //释放空间渲染器
        YMCV_Free_FMatFree(myKernel);
    }
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);

    PAUSE();
    LCD_Destory();
    return 0;
}





