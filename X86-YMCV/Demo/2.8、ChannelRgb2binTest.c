#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_ChannelCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以单、双阈值为例，展示了 三通道渲染器创建，渲染器参数设置与调整，渲染器初始化以及渲染图片并显示
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 480);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(linglong00));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, linglong00, sizeof(linglong00));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 500, 255, CVRgb16Type);//彩色图
    //显示图片0
    LCD_show_image(0, 0, myImage0);

    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, myImage0->type);

    /*--------------------------彩色图单阈值二值化--------------------------------------------------*/

    //创建通道渲染器
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_ThreshToBin, PIXChannelNum3);//单通道二值化，一个通道
    //设置渲染器参数
    CV_pixchannel_3param myParam = { .rgb_tobin.R_Thresh = 40 , .rgb_tobin.G_Thresh = 40 , .rgb_tobin.B_Thresh = 40 };
    for (int i = 0; i < 22; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_ThreeChannel_MapTo(myRender, &myParam);

        //使用通道渲染器进行渲染图片
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //显示图片1
        LCD_show_image(500, 0, myImage1);

        //信息显示
        sprintf(myInform, "三通道单阈值:R %03d,G %03d,B %03d .", myParam.rgb_tobin.R_Thresh, myParam.rgb_tobin.G_Thresh, myParam.rgb_tobin.B_Thresh);
        LCD_ShowString(100, 300, myInform);
        //暂停
        PAUSE();

        //参数调整
        if (i>10)
        {
            if (i > 17)
            {
                myParam.rgb_tobin.G_Thresh += 15;
            }
            else
            {
               myParam.rgb_tobin.R_Thresh += 10;
            }
        }
        else
        {
            myParam.rgb_tobin.B_Thresh += 10;
        }
    }
    //释放渲染器
    YMCV_Free_PIX_RenderFree(myRender);

    /*--------------------------彩色图双阈值二值化--------------------------------------------------*/

    //重新创建一个通道渲染器
    myRender = YMCV_Creat_PixRender_Creat(CV_Thresh2ToBin, PIXChannelNum3);//单通道二值化，一个通道
    //设置渲染器参数
    CV_pixchannel_3param myParam1 = {   .rgb_2tobin.R_leftThresh = 0 ,.rgb_2tobin.R_rightThresh = 40,
                                        .rgb_2tobin.G_leftThresh = 0 ,.rgb_2tobin.G_rightThresh = 40 ,
                                        .rgb_2tobin.B_leftThresh = 0 ,.rgb_2tobin.B_rightThresh = 40 };
    for (int i = 0; i < 22; i++)
    {
        //使用参数进行渲染器通道配置、初始化
        YMCV_ThreeChannel_MapTo(myRender, &myParam1);

        //使用通道渲染器进行渲染图片
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //显示图片1
        LCD_show_image(500, 0, myImage1);

        //信息显示
        sprintf(myInform, "三通道双阈值: R %03d--%03d  G %03d--%03d  B %03d--%03d .", myParam1.rgb_2tobin.R_leftThresh, myParam1.rgb_2tobin.R_rightThresh, 
                                                                                    myParam1.rgb_2tobin.G_leftThresh, myParam1.rgb_2tobin.G_rightThresh, 
                                                                                    myParam1.rgb_2tobin.B_leftThresh, myParam1.rgb_2tobin.B_rightThresh );
        LCD_ShowString(100, 300, myInform);
        //暂停
        PAUSE();

        //参数调整
        if (i > 10)
        {
            if (i > 17)
            {
                myParam1.rgb_2tobin.G_leftThresh += 25;
                myParam1.rgb_2tobin.G_rightThresh += 25;
            }
            else
            {
                myParam1.rgb_2tobin.R_leftThresh += 10;
                myParam1.rgb_2tobin.R_rightThresh += 10;
            }
        }
        else
        {
            myParam1.rgb_2tobin.B_leftThresh += 10;
            myParam1.rgb_2tobin.B_rightThresh += 10;
        }
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





