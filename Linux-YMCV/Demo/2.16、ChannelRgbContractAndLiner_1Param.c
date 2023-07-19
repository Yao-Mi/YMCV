#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_ChannelCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以对比度、分段线性增强为例，展示了使用 单通道渲染器 渲染彩色图像
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(640, 480);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(linglong00));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, linglong00, sizeof(linglong00));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 500, 255, CVRgb16Type);//彩色图


    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, myImage0->type);

    /*--------------------------彩色图对比度增强--------------------------------------------------*/

    //创建通道渲染器
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Contract, PIXChannelNum1);//图像对比度增强/减弱（雾化）
    //设置渲染器参数
    CV_pixchannel_1param myParam = { .gray_contract._k = -100.0f };
    for (int i = 0; i < 10; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_SingleChannel_MapTo(myRender, &myParam);
        //使用通道渲染器进行渲染彩色图片
        YMCV_Image_ChannelRenderTo_UGray(myImage0, myImage1, myRender);
        //显示图片1
        LCD_show_image(0, 0, myImage1);
        //显示通道转换图
        LCDChannelTransformShow(myRender, 540, 60, 40);
        //信息显示
        sprintf(myInform, "对比度增强: %01.2f .", myParam.gray_contract._k);
        LCD_ShowString(100, 300, myInform);

        //暂停
        PAUSE();

        //参数调整
        myParam.gray_contract._k += 20.0f;
    }
    //释放渲染器
    YMCV_Free_PIX_RenderFree(myRender);

    /*--------------------------彩色图单阈值二值化分段线性增强--------------------------------------------------*/

    //重新创建一个通道渲染器
    myRender = YMCV_Creat_PixRender_Creat(CV_Liner_Enhance, PIXChannelNum1);//分段线性增强
    //设置渲染器参数 将[0,f_a][f_a,f_b][f_b,f_c][f_c,255] 变换为 [0,t_a][t_a,t_b][t_b,t_c][t_c,255] 
    CV_pixchannel_1param myParam1 = { .gray_liner_enhance.from_a = 20,.gray_liner_enhance.from_b = 100,.gray_liner_enhance.from_c = 200,
                                      .gray_liner_enhance.to_a = 30, .gray_liner_enhance.to_b = 30, .gray_liner_enhance.to_c = 230 };
    for (int i = 0; i < 10; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_SingleChannel_MapTo(myRender, &myParam1);

        //使用通道渲染器进行渲染彩色图片
        YMCV_Image_ChannelRenderTo_UGray(myImage0, myImage1, myRender);
        //显示通道转换图
        LCDChannelTransformShow(myRender, 540, 60, 40);
        //显示图片1
        LCD_show_image(0, 0, myImage1);

        //信息显示
        sprintf(myInform, "分段线性增强:[ %03d %03d %03d]-> [ %03d %03d %03d]",
            myParam1.gray_liner_enhance.from_a, myParam1.gray_liner_enhance.from_b, myParam1.gray_liner_enhance.from_c, \
            myParam1.gray_liner_enhance.to_a, myParam1.gray_liner_enhance.to_b, myParam1.gray_liner_enhance.to_c); \

        LCD_ShowString(100, 300, myInform);
        //暂停
        PAUSE();

        //参数调整 一般选择好固定的from参数后，调节to参数，所有参数范围是[0 -- 255]
        myParam1.gray_liner_enhance.to_b += 20;//[to_a -- to_c]
    }
    //释放渲染器
    YMCV_Free_PIX_RenderFree(myRender);


    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);

    PAUSE();
    LCD_Destory();
    return 0;
}





