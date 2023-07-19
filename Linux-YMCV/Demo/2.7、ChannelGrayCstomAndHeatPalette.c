#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_ChannelCal.h"
#include "../YMCV/CORE/YMCV_PubData.h"
/*******************************************************************************************
    *
    * 说明   该例程以自定义染色函数增强、调色板渲染为例，展示了使用自定义函数进行渲染与调参、灰度图转热力图显示、灰度图自定义颜色渲染
    *
    ***************************************************************************************/
int32 myFunction(int32 color);
float32 myFun_k = 1.0f;
int main()
{
    char myInform[100];
    LCD_Init(1000, 480);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(linglong00_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, linglong00_gray, sizeof(linglong00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 500, 255, CVGrayType);//灰度图

    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, myImage0->type);
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVRgb16Type);


    /*-------------------------- 热力图颜色表显示 --------------------------------------------------*/
    color24_t* mycstomTable = YMCV_HeatPalette;
    color24_t rectclv = { .R = 100,.G = 100,.B = 100 };
 
    uint16 rsx = 500, rsy = 400, rsw = 256, rsh = 20;
    LCD_Draw_Rect(rsx - 1, rsy - 1, rsw + 1, rsh + 2, rectclv);
    LCD_Draw_Rect(rsx - 1, rsy - 3 - rsh, rsw + 1, rsh + 2, rectclv);
    for (int i = 0 ,imax = CVMin(256,rsw); i < rsw; i++)
    {
        //灰度表绘制
        rectclv.R = rectclv.G = rectclv.B = i;// 0 -- 255
        LCD_Point_Color_Set(rectclv);
        LCD_Draw_Line(rsx + i, rsy, rsx + i, rsy + rsh);
        //热力图表绘制
        LCD_Point_Color_Set(mycstomTable[i]);
        LCD_Draw_Line(rsx + i, rsy - 2 - rsh, rsx + i, rsy - 2);
    }

    /*--------------------------自定义增强--------------------------------------------------*/

    //创建通道渲染器
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Channel_CustomTransform, PIXChannelNum1);//自定义渲染器
    CVPIX_RENDER myRender2 = YMCV_Creat_PixRender_Creat(CV_Channel_SingleToThree, PIXChannelNum3);//单通道转三通道

    for (int i = 0; i < 10; i++)
    {
        //使用自己的函数初始化渲染通道
        YMCV_SingleChannel_CustomMapTo(myRender, myFunction);
        //使用通道渲染器进行渲染图片
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //显示图片1
        LCD_show_image(0, 0, myImage1);
        //显示通道转换图
        LCDChannelTransformShow(myRender, 30, 320, 40);
        //信息显示
        sprintf(myInform, "自定义线性增强: k =%03.2f .", myFun_k);
        LCD_ShowString(100, 300, myInform);

        //转为热力图
        //使用热力图 调色板进行渲染器通道配置、初始化
        YMCV_ThreeChannel_PaletteMapTo(myRender2, mycstomTable);
        //使用通道渲染器进行渲染图片
        YMCV_Image_SingleToThree_ChannelRenderTo(myImage1, myImage2, myRender2);
        //显示图片1
        LCD_show_image(500, 0, myImage2);

        //暂停
        PAUSE();

        //参数调整
        myFun_k += 1.0f;
    }
    //释放渲染器1
    YMCV_Free_PIX_RenderFree(myRender);


    /*--------------------------自定义调色板染色--------------------------------------------------*/

    //创建调色板
    color24_t myPalette[256] ={0};

    //使用RGB（220，150，100）对灰度图进行染色
    color24_t myColor = { .R = 220,.G = 150,.B = 100 };
    for (int i = 0; i < 256; i++)
    {
        myPalette[i].R = myColor.R * i / 255;
        myPalette[i].G = myColor.G * i / 255;
        myPalette[i].B = myColor.B * i / 255;
    }

    //使用调色板进行渲染器通道配置、初始化
    YMCV_ThreeChannel_PaletteMapTo(myRender2, myPalette);
    //使用通道渲染器进行渲染图片
    YMCV_Image_SingleToThree_ChannelRenderTo(myImage0, myImage2, myRender2);
    //显示图片1
    LCD_Clear(0);
    LCD_show_image(0, 0, myImage0);
    LCD_show_image(500, 0, myImage2);
    sprintf(myInform, "自定义颜色渲染:RGB=（%03d,%03d,%03d）", myColor.R, myColor.G, myColor.B);
    LCD_ShowString(300, 300, myInform);
    //暂停
    PAUSE();

    //释放渲染器2
    YMCV_Free_PIX_RenderFree(myRender2);


    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);

    PAUSE();
    LCD_Destory();
    return 0;
}

//自定义线性函数
int32 myFunction(int32 color)
{
    return myFun_k * color;
}



