#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_EdgeCal.h"

#include"../YMCV/CORE/YMCV_SegnmCal.h"

/*******************************************************************************************
    *
    * 说明  该例程以 距离变换为例，展示了生成Trimap的效果
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

    /*--------------------------距离变换创建前景图--------------------------------------------------*/

    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);

    //创建二值化通道渲染器
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Thresh2ToBin, PIXChannelNum1);//单通道二值化，一个通道

    CV_pixchannel_1param myParam = { .gray_2tobin.leftThresh = 0,.gray_2tobin.rightThresh = 180 };    //设置渲染器参数
    YMCV_SingleChannel_MapTo(myRender, &myParam);//使用参数进行渲染器通道配置、初始化  

    //图像二值化
    YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);//使用通道渲染器进行渲染图片


    //对二值图进行距离变换
    CVIMAGE myImage2 = YMCV_Binary_Distance_Transform(myImage1);

    //对距离图二值化
    myParam.gray_2tobin.leftThresh = 20;
    myParam.gray_2tobin.rightThresh = 255;
    YMCV_SingleChannel_MapTo(myRender, &myParam);

    YMCV_Image_ChannelRenderTo(myImage2, myImage2, myRender);//二值化渲染


    LCD_Clear(0);
    //显示图片0
    LCD_show_image(0, 0, myImage1);
    //显示图片2
    LCD_show_image(300, 0, myImage2);

    //信息显示
    sprintf(myInform, "前景图  ");
    LCD_ShowString(400, 310, myInform);
    //暂停
    PAUSE();

    /*--------------------------距离变换创建背景图--------------------------------------------------*/

    //背景粗提取，前景粗背景取反
    YMCV_Image_Binary_Logic_Cal(myImage1, myImage1, CV_Not);//图1 逻辑非  ，输出结果放在图1

    //对二值图进行距离变换
    CVIMAGE myImage3 = YMCV_Binary_Distance_Transform(myImage1);


    //对距离图二值化
    myParam.gray_2tobin.leftThresh = 40;
    myParam.gray_2tobin.rightThresh = 255;
    YMCV_SingleChannel_MapTo(myRender, &myParam);

    YMCV_Image_ChannelRenderTo(myImage3, myImage3, myRender);//二值化渲染

     //显示图片2
    LCD_show_image(600, 0, myImage3);
    //信息显示
    sprintf(myInform, "背景图  ");
    LCD_ShowString(700, 310, myInform);

    /*-------------------------- TriMap图创建--------------------------------------------------*/
    //将灰度格式强制转为二值图
    myImage2->type = CVBinaryType;
    myImage3->type = CVBinaryType;
    //使用前景图和背景图，创建trimap图
    CVIMAGE TriMapImage = YMCV_BinaryTo_TriMap(myImage2, myImage3);

    //显示图片trimap
    LCD_show_image(0, 0, TriMapImage);

    //信息显示
    sprintf(myInform, "TriMap图  ");
    LCD_ShowString(100, 310, myInform);

    //释放渲染器
    YMCV_Free_PIX_RenderFree(myRender);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);
    YMCV_Free_ImgFree(TriMapImage);

    PAUSE();
    LCD_Destory();
    return 0;
}









