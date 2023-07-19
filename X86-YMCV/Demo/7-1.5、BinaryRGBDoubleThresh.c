#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_BinaryCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 RGB双阈值为例，展示了RGB二值化效果
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 450);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(linglong00));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, linglong00, sizeof(linglong00));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 500, 255, CVRgb16Type);//彩色图0


    /*--------------------------RGB双阈值二值化--------------------------------------------------*/
    //创建输出二值图像
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);//二值图
    CV_binary_param myParam = { .rgb_double_t.thresh_Rmin = 0 ,.rgb_double_t.thresh_Rmax = 255 , 
     .rgb_double_t.thresh_Gmin = 0 ,.rgb_double_t.thresh_Gmax = 255 ,
     .rgb_double_t.thresh_Bmin = 0 ,.rgb_double_t.thresh_Bmax = 255 };

    for (int i = 1; i < 10; i++)
    {
        //设置参数
        myParam.rgb_double_t.thresh_Rmin = 20 * i;
        myParam.rgb_double_t.thresh_Rmax = 255;

        myParam.rgb_double_t.thresh_Gmin = 10 * i;
        myParam.rgb_double_t.thresh_Gmax = 255;

        myParam.rgb_double_t.thresh_Bmin = 15 * i;
        myParam.rgb_double_t.thresh_Bmax = 255;

        //创建二值变换渲染器
        CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_RGB2Threshold);

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
        sprintf(myInform, "二值图像 阈值: R[%d %d] G[%d %d] B[%d %d]    ",
            myParam.rgb_double_t.thresh_Rmin,
            myParam.rgb_double_t.thresh_Rmax,
            myParam.rgb_double_t.thresh_Gmin,
            myParam.rgb_double_t.thresh_Gmax,
            myParam.rgb_double_t.thresh_Bmin,
            myParam.rgb_double_t.thresh_Bmax);

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





