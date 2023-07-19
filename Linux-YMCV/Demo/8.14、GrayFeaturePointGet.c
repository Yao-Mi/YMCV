#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_FeatCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 边界跟踪 为例，展示了边界链码提取以及形状判别
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(640, 480);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(mm00_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, mm00_gray, sizeof(mm00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//灰度图0

    /*------------------------ SUASAN特征点提取提取 ------------------------------*/
    CV_featpoint_param myparam;
    myparam.type = CV_FeatPoint_SUSAN;
    myparam.param.susan.thresh = 20;

    CVVECTORS_U16 featpoints = YMKV_FeaturePoint_Find(myImage0, &myparam);//特征点检测
    
    LCD_show_image(300, 0, myImage0);
    //显示特征点
    for (int i = 0; (featpoints != NULL) && (i < featpoints->dataNum); i++)
    {
        CVpoint* pp = featpoints->datap + i * featpoints->dim;//point= &data[i]
        color24_t thiscolor = { .G = 255 };
        LCD_Draw_Rect(300 + pp->x - 1, pp->y - 1, 2, 2, thiscolor);
        //LCD_Draw_Point(300+pp->x, pp->y, thiscolor);
    }

    //信息显示
    sprintf(myInform, "SUSAN特征点提取  ");
    LCD_ShowString(400, 310, myInform);

    //释放featpoints内存
    YMCV_Free_Vector_U16_Free(featpoints);
    PAUSE();

    /*------------------------ Fasts特征点提取 ------------------------------*/
    myparam.type = CV_FeatPoint_Fast;
    myparam.param.fast.thresh = 20;//差值

    featpoints = YMKV_FeaturePoint_Find(myImage0, &myparam);//特征点检测

    LCD_show_image(300, 0, myImage0);
    //显示特征点
    for (int i = 0; (featpoints != NULL) && (i < featpoints->dataNum); i++)
    {
        CVpoint* pp = featpoints->datap + i * featpoints->dim;//point= &data[i]
        color24_t thiscolor = { .G = 255 };
        LCD_Draw_Rect(300 + pp->x - 1, pp->y - 1, 2, 2, thiscolor);
    }

    
    //信息显示
    sprintf(myInform, "Fast9特征点提取  ");
    LCD_ShowString(400, 310, myInform);

    //内存释放
    YMCV_Free_Vector_U16_Free(featpoints);
    PAUSE();

    /*------------------------ AGAST 特征点提取 ------------------------------*/
    myparam.type = CV_FeatPoint_AGAST;
    myparam.param.agast.thresh = 20;//差值

    featpoints = YMKV_FeaturePoint_Find(myImage0, &myparam);//特征点检测

    LCD_show_image(300, 0, myImage0);
    //显示特征点
    for (int i = 0; (featpoints != NULL) && (i < featpoints->dataNum); i++)
    {
        CVpoint* pp = featpoints->datap + i * featpoints->dim;//point= &data[i]
        color24_t thiscolor = { .G = 255 };
        LCD_Draw_Rect(300 + pp->x - 1, pp->y - 1, 2, 2, thiscolor);
    }

    //信息显示
    sprintf(myInform, "AGAST 特征点提取  ");
    LCD_ShowString(400, 310, myInform);

    //内存释放
    YMCV_Free_Vector_U16_Free(featpoints);
    PAUSE();

    /*------------------------ Harris 特征点提取 ------------------------------*/
    myparam.type = CV_FeatPoint_Harris;
    myparam.param.harris.thresh = 5;//剔除百分比 5/255

    featpoints = YMKV_FeaturePoint_Find(myImage0, &myparam);//特征点检测

    LCD_show_image(300, 0, myImage0);
    //显示特征点
    for (int i = 0; (featpoints != NULL) && (i < featpoints->dataNum); i++)
    {
        CVpoint* pp = featpoints->datap + i * featpoints->dim;//point= &data[i]
        color24_t thiscolor = { .G = 255 };
        LCD_Draw_Rect(300 + pp->x - 1, pp->y - 1, 2, 2, thiscolor);
    }

    //信息显示
    sprintf(myInform, "Harris 特征点提取  ");
    LCD_ShowString(400, 310, myInform);
    PAUSE();

    //释放featpoints内存
    YMCV_Free_Vector_U16_Free(featpoints);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    //YMCV_Free_ImgFree(myImage1);

    PAUSE();
    LCD_Destory();
    return 0;
}





