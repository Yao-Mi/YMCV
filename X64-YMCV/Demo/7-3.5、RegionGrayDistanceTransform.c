#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_EdgeCal.h"

#include"../YMCV/CORE/YMCV_SegnmCal.h"

/*******************************************************************************************
    *
    * 说明  该例程以 距离变换为例，展示了二值图变换效果
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

    /*--------------------------二值图距离变换--------------------------------------------------*/

    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);

    //创建二值化通道渲染器
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Thresh2ToBin, PIXChannelNum1);//单通道二值化，一个通道
    CV_pixchannel_1param myParam = { .gray_2tobin.leftThresh = 0,.gray_2tobin.rightThresh=150 };    //设置渲染器参数
    YMCV_SingleChannel_MapTo(myRender, &myParam);//使用参数进行渲染器通道配置、初始化  

    //图像二值化
    YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);//使用通道渲染器进行渲染图片


    //对二值图进行距离变换
    CVIMAGE myImage2 = YMCV_Binary_Distance_Transform(myImage1);


    LCD_Clear(0);
    //显示图片0
    LCD_show_image(0, 0, myImage1);
    //显示图片2
    LCD_show_image(500, 0, myImage2);

    //信息显示
    sprintf(myInform, "二值图距离变换  ");
    LCD_ShowString(100, 310, myInform);
    //暂停
    PAUSE();

    //释放渲染器
    YMCV_Free_PIX_RenderFree(myRender);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);

    PAUSE();
    LCD_Destory();
    return 0;
}









