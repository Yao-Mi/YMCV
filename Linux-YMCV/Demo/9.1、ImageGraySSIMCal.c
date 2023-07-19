#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_MatchingCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以exp指数变换结果与原图进行结构相似计算，结构表明改变亮度并不改变图像结构
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(640, 480);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(mm00_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, mm00_gray, sizeof(mm00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//灰度图

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData2 = CV_malloc1(sizeof(tuxin_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData2, tuxin_gray, sizeof(tuxin_gray));
    CVIMAGE myImage_0 = YMCV_Creat_DataToImg((uint8*)myData2, 250, 140, CVGrayType);//灰度图

    /*--------------------------exp指数增强--------------------------------------------------*/

    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, myImage0->type);


    //重新创建一个通道渲染器
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Exp_Enhance, PIXChannelNum1);//指数增强
    CV_pixchannel_1param myParam1 = { .gray_exp_enhance._a = -542.0f,.gray_exp_enhance._b = 1.04f,.gray_exp_enhance._c = 0.18f };

    YMCV_SingleChannel_MapTo(myRender, &myParam1);//使用参数进行渲染器通道配置、初始化
    YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);

    //显示通道转换图
    LCDChannelTransformShow(myRender, 540, 60, 40);

    //显示图片
    LCD_show_image(0, 0, myImage0);
    LCD_show_image(250, 0, myImage1);

    //信息显示
    sprintf(myInform, "exp值数增强: a=%03.2f b= %01.3f c=%01.2f .", myParam1.gray_exp_enhance._a, myParam1.gray_exp_enhance._b, myParam1.gray_exp_enhance._c);
    LCD_ShowString(100, 300, myInform);
    //暂停
    PAUSE();
    //释放渲染器
    YMCV_Free_PIX_RenderFree(myRender);

    /*--------------------------结构相似度计算--------------------------------------------------*/
    float similar = YMCV_Image_Gray_SSIM_Cal(myImage0, myImage1);

    //信息显示
    sprintf(myInform, "     两幅图像相似度为      %f %%.             ", similar * 100);
    LCD_ShowString(100, 300, myInform);
    //暂停
    PAUSE();

    /*--------------------------结构相似度计算--------------------------------------------------*/
    similar = YMCV_Image_Gray_SSIM_Cal(myImage0, myImage_0);

    LCD_show_image(250, 0, myImage_0);
    //信息显示
    sprintf(myInform, "     两幅图像相似度为      %f %%.             ", similar * 100);
    LCD_ShowString(100, 300, myInform);
    //暂停
    PAUSE();
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage_0);
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);

    PAUSE();
    LCD_Destory();
    return 0;
}





