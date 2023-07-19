#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_ChannelCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以log对数、exp指数变换为例，展示了参数取值 、增长步幅，以及通道转换示意图的显示
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


    /*--------------------------彩色图log对数增强--------------------------------------------------*/

    //创建通道渲染器
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Log_Enhance, PIXChannelNum3);//图像对数增强
    //设置渲染器参数 ，参数粗调节推荐步幅为     a（5）    ，  b（0.005）    ，   c（0.01）
    CV_pixchannel_3param myParam = { .rgb_log_enhance.R_a = -156.0f,.rgb_log_enhance.R_b = 0.040f,.rgb_log_enhance.R_c = 1.42f,
                                     .rgb_log_enhance.G_a = -156.0f,.rgb_log_enhance.G_b = 0.040f,.rgb_log_enhance.G_c = 1.42f,
                                     .rgb_log_enhance.B_a = -156.0f,.rgb_log_enhance.B_b = 0.040f,.rgb_log_enhance.B_c = 1.42f };
    for (int i = 0; i < 10; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_ThreeChannel_MapTo(myRender, &myParam);
        //使用通道渲染器进行渲染图片
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //显示图片1
        LCD_show_image(0, 0, myImage1);
        //显示通道转换图
        LCDChannelTransformShow(myRender, 540, 70, 60);
        //信息显示
        sprintf(myInform, "log对数增强 R: a=%03.2f b= %01.3f c=%01.2f .  ", myParam.rgb_log_enhance.R_a, myParam.rgb_log_enhance.R_b, myParam.rgb_log_enhance.R_c);
        LCD_ShowString(100, 300, myInform);
        sprintf(myInform, "log对数增强 G: a=%03.2f b= %01.3f c=%01.2f .  ", myParam.rgb_log_enhance.G_a, myParam.rgb_log_enhance.G_b, myParam.rgb_log_enhance.G_c);
        LCD_ShowString(100, 320, myInform);
        sprintf(myInform, "log对数增强 B: a=%03.2f b= %01.3f c=%01.2f .  ", myParam.rgb_log_enhance.B_a, myParam.rgb_log_enhance.B_b, myParam.rgb_log_enhance.B_c);
        LCD_ShowString(100, 340, myInform);

        //暂停
        PAUSE();

        //参数调整
        myParam.rgb_log_enhance.R_a += 5.0f;
        //myParam.gray_log_enhance._b += 0.005f;
        //myParam.gray_log_enhance._c += 0.01f;
        myParam.rgb_log_enhance.G_a += 3.0f;
        myParam.rgb_log_enhance.B_a += 1.5f;
    }
    //释放渲染器
    YMCV_Free_PIX_RenderFree(myRender);

    /*--------------------------彩色图exp指数增强--------------------------------------------------*/

    //重新创建一个通道渲染器
    myRender = YMCV_Creat_PixRender_Creat(CV_Exp_Enhance, PIXChannelNum3);//指数增强
    //设置渲染器参数 ，参数粗调节推荐步幅为     a（10）    ，  b（0.001）    ，   c（0.01）
    CV_pixchannel_3param myParam1 = { .rgb_exp_enhance.R_a = -542.0f,.rgb_exp_enhance.R_b = 1.04f,.rgb_exp_enhance.R_c = 0.18f,
                                      .rgb_exp_enhance.G_a = -542.0f,.rgb_exp_enhance.G_b = 1.04f,.rgb_exp_enhance.G_c = 0.18f,
                                      .rgb_exp_enhance.B_a = -542.0f,.rgb_exp_enhance.B_b = 1.04f,.rgb_exp_enhance.B_c = 0.18f };
    for (int i = 0; i < 10; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_ThreeChannel_MapTo(myRender, &myParam1);

        //使用通道渲染器进行渲染图片
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //显示通道转换图
        LCDChannelTransformShow(myRender, 540, 70, 60);
        //显示图片1
        LCD_show_image(0, 0, myImage1);

        //信息显示
        sprintf(myInform, "exp值数增强  R: a=%03.2f b= %01.3f c=%01.2f .   ", myParam1.rgb_exp_enhance.R_a, myParam1.rgb_exp_enhance.R_b, myParam1.rgb_exp_enhance.R_c);
        LCD_ShowString(100, 300, myInform);
        sprintf(myInform, "exp值数增强  G: a=%03.2f b= %01.3f c=%01.2f .   ", myParam1.rgb_exp_enhance.G_a, myParam1.rgb_exp_enhance.G_b, myParam1.rgb_exp_enhance.G_c);
        LCD_ShowString(100, 320, myInform);
        sprintf(myInform, "exp值数增强  B: a=%03.2f b= %01.3f c=%01.2f .   ", myParam1.rgb_exp_enhance.B_a, myParam1.rgb_exp_enhance.B_b, myParam1.rgb_exp_enhance.B_c);
        LCD_ShowString(100, 340, myInform);
        //暂停
        PAUSE();

        //参数调整
        myParam1.rgb_exp_enhance.R_a += 2.0f;
        //myParam1.gray_exp_enhance._b += 0.001f;
        //myParam1.gray_exp_enhance._c += 0.01f;
        myParam1.rgb_exp_enhance.G_a += 5.0f;
        myParam1.rgb_exp_enhance.B_a += 10.0f;
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





