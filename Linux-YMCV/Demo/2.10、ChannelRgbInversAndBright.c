#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_ChannelCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以反色、亮度增强为例，展示了 通道渲染器的使用，以及参数取值 、增长步幅
    *
    ***************************************************************************************/
int main()
{
    char myInform[50];
    LCD_Init(640, 480);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(linglong00));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, linglong00, sizeof(linglong00));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 500, 255, CVRgb16Type);//彩色图


    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, myImage0->type);

    /*--------------------------彩色图亮度增强--------------------------------------------------*/

    //创建通道渲染器
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Bright, PIXChannelNum3);//图像亮度增强
    //设置渲染器参数
    CV_pixchannel_3param myParam = { .rgb_bright.R_k = 0.0f ,.rgb_bright.G_k = 0.0f ,.rgb_bright.B_k = 0.0f };
    for (int i = 0; i < 10; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_ThreeChannel_MapTo(myRender, &myParam);

        //使用通道渲染器进行渲染图片
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //显示图片1
        LCD_show_image(0, 0, myImage1);

        //信息显示
        sprintf(myInform, "亮度增强: R%01.2f G%01.2f B%01.2f .", myParam.rgb_bright.R_k, myParam.rgb_bright.G_k, myParam.rgb_bright.B_k);
        LCD_ShowString(100, 300, myInform);
        //暂停
        PAUSE();

        //参数调整
        myParam.rgb_bright.R_k += 0.5f;
        myParam.rgb_bright.G_k += 0.5f;
        myParam.rgb_bright.B_k += 0.5f;
    }
    //释放渲染器
    YMCV_Free_PIX_RenderFree(myRender);

    /*--------------------------彩色图反色增强--------------------------------------------------*/

    //重新创建一个通道渲染器
    myRender = YMCV_Creat_PixRender_Creat(CV_Invers, PIXChannelNum3);//反色
    //设置渲染器参数 取值在[0.0 -- 1.0]区间 或者 [0 --- 255]区间
    CV_pixchannel_3param myParam1 = { .rgb_invers.R_k = 0.0f,.rgb_invers.G_k = 0.0f,.rgb_invers.B_k = 0.0f };
    for (int i = 0; i < 10; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_ThreeChannel_MapTo(myRender, &myParam1);

        //使用通道渲染器进行渲染图片
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //显示图片1
        LCD_show_image(0, 0, myImage1);

        //信息显示
        sprintf(myInform, "反色增强: R%01.2f G%01.2f B%01.2f .", myParam1.rgb_invers.R_k, myParam1.rgb_invers.G_k, myParam1.rgb_invers.B_k);
        LCD_ShowString(100, 300, myInform);
        //暂停
        PAUSE();

        //参数调整
        myParam1.rgb_invers.R_k += 0.50f;//[0.0 -- 1.0]
        myParam1.rgb_invers.G_k += 0.50f;
        myParam1.rgb_invers.B_k += 0.50f;
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





