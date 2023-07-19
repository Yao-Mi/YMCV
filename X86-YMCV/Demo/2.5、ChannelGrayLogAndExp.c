#include "../EGE_LCD/EGE_LCD.h"
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
    uint8* myData = CV_malloc1(sizeof(linglong00_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, linglong00_gray, sizeof(linglong00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 500, 255, CVGrayType);//灰度图


    /*--------------------------log对数增强--------------------------------------------------*/

    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, myImage0->type);


    //创建通道渲染器
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Log_Enhance, PIXChannelNum1);//图像对比度增强/减弱（雾化）
    //设置渲染器参数 ，参数粗调节推荐步幅为     a（5）    ，  b（0.005）    ，   c（0.01）
    CV_pixchannel_1param myParam = { .gray_log_enhance._a = -156.0f,.gray_log_enhance._b = 0.040f,.gray_log_enhance._c = 1.42f };
    for (int i = 0; i < 10; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_SingleChannel_MapTo(myRender, &myParam);
        //使用通道渲染器进行渲染图片
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //显示图片1
        LCD_show_image(0, 0, myImage1);
        //显示通道转换图
        LCDChannelTransformShow(myRender, 540, 60, 40);
        //信息显示
        sprintf(myInform, "log对数增强: a=%03.2f b= %01.3f c=%01.2f .", myParam.gray_log_enhance._a, myParam.gray_log_enhance._b, myParam.gray_log_enhance._c);
        LCD_ShowString(100, 300, myInform);

        //暂停
        PAUSE();

        //参数调整
        myParam.gray_log_enhance._a += 5.0f;
        //myParam.gray_log_enhance._b += 0.005f;
        //myParam.gray_log_enhance._c += 0.01f;
    }
    //释放渲染器
    YMCV_Free_PIX_RenderFree(myRender);


    /*--------------------------exp指数增强--------------------------------------------------*/

    //重新创建一个通道渲染器
    myRender = YMCV_Creat_PixRender_Creat(CV_Exp_Enhance, PIXChannelNum1);//指数增强
    //设置渲染器参数 ，参数粗调节推荐步幅为     a（10）    ，  b（0.001）    ，   c（0.01）
    CV_pixchannel_1param myParam1 = { .gray_exp_enhance._a = -542.0f,.gray_exp_enhance._b = 1.04f,.gray_exp_enhance._c = 0.18f };
    for (int i = 0; i < 10; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_SingleChannel_MapTo(myRender, &myParam1);

        //使用通道渲染器进行渲染图片
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //显示通道转换图
        LCDChannelTransformShow(myRender, 540, 60, 40);
        //显示图片1
        LCD_show_image(0, 0, myImage1);

        //信息显示
        sprintf(myInform, "exp值数增强: a=%03.2f b= %01.3f c=%01.2f .", myParam1.gray_exp_enhance._a, myParam1.gray_exp_enhance._b, myParam1.gray_exp_enhance._c);
        LCD_ShowString(100, 300, myInform);
        //暂停
        PAUSE();

        //参数调整
        myParam1.gray_exp_enhance._a += 10.0f;
        //myParam1.gray_exp_enhance._b += 0.001f;
        //myParam1.gray_exp_enhance._c += 0.01f;
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





