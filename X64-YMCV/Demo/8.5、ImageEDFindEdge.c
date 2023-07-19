#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_EdgeCal.h"
#include"../YMCV/CORE/YMCV_FeatCal.h"
#include"../YMCV/CORE/YMCV_BasicObjCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 ED 为例，展示了边缘提取效果
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 550);
    color24_t pencolor = { .R = 255 };
    LCD_Point_Color_Set(pencolor);//设置画笔颜色

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(tuxin_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, tuxin_gray, sizeof(tuxin_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//灰度图0
   
    /*-------------------------ED边缘检测--------------------------------------------------*/
    //创建输出二值图像
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);//二值图

    CVCHAINS_U16 chains = YMCV_Fast_EdgeDrawing(myImage0, CV_Sobel_Graident, 5, myImage2);

    LCD_Clear(0);
    LCD_show_image(0, 0, myImage0);//显示图片0
    LCD_show_image(300, 0, myImage2);  //显示图片2

    //信息显示
    sprintf(myInform, "ED 边缘检测图像 ");
    LCD_ShowString(100, 310, myInform);
    PAUSE();    //暂停

    //在原图上绘制边缘链码
    for (int i = 0; i < chains->cNum; i++)
    {
        CVVECTORS_U16 pixles = chains->chain[i];

        //起点单独绘制
        CV_ed_point pxy;
        CV_memcpy(&pxy, &pixles->datap[0], sizeof(CV_ed_point));
        color24_t rgb = { .R = 255 };
        LCD_Fill_ConstRgbRect(pxy.pos.x, pxy.pos.y, 2, 2, rgb);
        //剩余点
        for (int pi = 1; pi < pixles->dataNum; pi++)
        {
            CV_ed_point pxy;
            CV_memcpy(&pxy, &pixles->datap[pi * pixles->dim], sizeof(CVpoint));
            color24_t rgb = { .G = 255 };
            LCD_Fill_ConstRgbRect(pxy.pos.x, pxy.pos.y, 1, 1, rgb);
        }
        PAUSE();
    }


    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_Chains_U16_Free(chains);//2D链码存储清理

    PAUSE();
    LCD_Destory();
    return 0;
}





