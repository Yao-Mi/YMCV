#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以exp指数变换结果与原图进行结构相似计算，结构表明改变亮度并不改变图像结构
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(mm02_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, (void*)mm02_gray, sizeof(mm02_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVGrayType);//灰度图

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData2 = CV_malloc1(sizeof(tuxin02_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData2, (void*)tuxin02_gray, sizeof(tuxin02_gray));
    CVIMAGE myImage1_gray = YMCV_Creat_DataToImg((uint8*)myData2, 160, 120, CVGrayType);//灰度图

    LCD_show_image(stx + 0, 0, myImage0_gray);
	  int sty = myImage0_gray->height;
    /*--------------------------exp指数增强--------------------------------------------------*/

    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage_0 = YMCV_Creat_Img_Creat(myImage0_gray->width, myImage0_gray->height, myImage0_gray->type);


    //重新创建一个通道渲染器
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Exp_Enhance, PIXChannelNum1);//指数增强
    CV_pixchannel_1param myParam1 = { .gray_exp_enhance._a = -542.0f,.gray_exp_enhance._b = 1.04f,.gray_exp_enhance._c = 0.17f };

    YMCV_SingleChannel_MapTo(myRender, &myParam1);//使用参数进行渲染器通道配置、初始化
    YMCV_Image_ChannelRenderTo(myImage0_gray, myImage_0, myRender);

    //显示图片
    LCD_show_image(stx + 0, sty, myImage_0);
    //显示通道转换图
    LCDChannelTransformShow(myRender, 170, 220, 40);

    //信息显示
    sprintf(myInform, "exp:a%03.2f b%01.3f c%01.2f", myParam1.gray_exp_enhance._a, myParam1.gray_exp_enhance._b, myParam1.gray_exp_enhance._c);
    LCD_ShowString(0, sty, myInform);
    //暂停
    Delay(500);
    //释放渲染器
    YMCV_Free_PIX_RenderFree(myRender);

    /*--------------------------结构相似度计算--------------------------------------------------*/
    float similar = YMCV_Image_Gray_SSIM_Cal(myImage0_gray, myImage_0);

    //显示图片
    LCD_show_image(stx + 0, sty, myImage_0);
    //信息显示
    sprintf(myInform, "similar = %f %%    ", similar * 100);
    LCD_ShowString(0, sty, myInform);
    //暂停
    Delay(500);

    /*--------------------------结构相似度计算--------------------------------------------------*/
    similar = YMCV_Image_Gray_SSIM_Cal(myImage0_gray, myImage1_gray);

    LCD_show_image(stx + 0, sty, myImage1_gray);
    //信息显示
    sprintf(myInform, "similar = %f %%    ", similar * 100);
    LCD_ShowString(0, sty, myInform);
    //暂停
    Delay(500);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage1_gray);
    YMCV_Free_ImgFree(myImage_0);

}





