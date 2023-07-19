#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_BinaryCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 二值图的逻辑运算为例，展示了逻辑 与或非 的 结果
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


    //创建两个二值图像
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);//二值图
    CVIMAGE myImage3 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);//二值图


    /*-------------------------- 二值化到 myImage2 --------------------------------------------------*/
    CV_binary_param myParam = { .gray_single_t.thresh = 120 };
    //创建二值变换渲染器
    CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_SingleThreshold);
    YMCV_BinaryKernel_Load(myRender1, &myParam);   //加载参数进行渲染配置、初始化

    //将目标图像的进行渲染
    YMCV_Image_ThreshTo_Binary(myImage0, myImage2, myRender1);

    //释放渲染器内存
    YMCV_Free_Binary_RenderFree(myRender1);

    //显示图片0
    LCD_Clear(0);
    LCD_show_image(500, 0, myImage0);
    LCD_show_image(0, 0, myImage2);

    //信息显示
    sprintf(myInform, "二值图像2 阈值: %d  ", myParam.gray_single_t.thresh);
    LCD_ShowString(100, 310, myInform);
    //暂停
    PAUSE();

    /*-------------------------- 二值化到 myImage3 --------------------------------------------------*/
    //设置新的参数
    myParam.gray_double_t.thresh_min = 50;
    myParam.gray_double_t.thresh_max = 150;
    //创建新的二值变换渲染器
    myRender1 = YMCV_Creat_BinaryRender_Creat(CV_DoubleThreshold);
    YMCV_BinaryKernel_Load(myRender1, &myParam);   //加载参数进行渲染配置、初始化

    //将目标图像的进行渲染
    YMCV_Image_ThreshTo_Binary(myImage0, myImage3, myRender1);

    //显示图片3
    LCD_show_image(500, 0, myImage3);

    //信息显示
    sprintf(myInform, "二值图像3 阈值: %d ，%d", myParam.gray_double_t.thresh_min,myParam.gray_double_t.thresh_max);
    LCD_ShowString(600, 310, myInform);
    //暂停
    PAUSE();
    

    /*-------------------------- myImage3 = myImage3 & myImage2 --------------------------------------------------*/
    YMCV_Image_Binary_Logic_Cal(myImage2, myImage3, CV_And);//图2 逻辑与上 图3 ，输出结果放在图3

    //显示图片3
    LCD_show_image(250, 150, myImage3);
    //信息显示
    sprintf(myInform, "二值图像 图2 & 图3 ");
    LCD_ShowString(300, 310, myInform);
    //暂停
    PAUSE();

    LCD_show_image(500, 0, myImage3);//更新图3
    /*-------------------------- myImage3 = myImage2 | myImage3 --------------------------------------------------*/
    YMCV_Image_Binary_Logic_Cal(myImage2, myImage3, CV_Or);//图2 逻辑或上 图3 ，输出结果放在图3

    //显示图片3
    LCD_show_image(250, 150, myImage3);
    //信息显示
    sprintf(myInform, "二值图像 图2 | 图3 ");
    LCD_ShowString(300, 310, myInform);
    //暂停
    PAUSE();
    /*-------------------------- myImage3 = ! myImage2 --------------------------------------------------*/
    YMCV_Image_Binary_Logic_Cal(myImage2, myImage3, CV_Not);//图2 逻辑非  ，输出结果放在图3

    //显示图片3
    LCD_show_image(250, 150, myImage3);
    //信息显示
    sprintf(myInform, "二值图像 ！图2     ");
    LCD_ShowString(300, 310, myInform);
    //暂停
    PAUSE();


    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage2);

    PAUSE();
    LCD_Destory();
    return 0;
}





