#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以 特征点 为例，展示了susan、fast-9、agast、harris特征点提取
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(mm02_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, (void*)mm02_gray, sizeof(mm02_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVGrayType);//灰度图0

    LCD_show_image(stx + 0, 0, myImage0_gray);
	  int sty =  myImage0_gray->height;
    /*------------------------ SUASAN特征点提取提取 ------------------------------*/
    CV_featpoint_param myparam;
    myparam.type = CV_FeatPoint_SUSAN;
    myparam.param.susan.thresh = 20;

    CVVECTORS_U16 featpoints = YMKV_FeaturePoint_Find(myImage0_gray, &myparam);//特征点检测
    
    LCD_show_image(stx + 0, sty + 0, myImage0_gray);
    //显示特征点
    for (int i = 0; (featpoints != NULL) && (i < featpoints->dataNum); i++)
    {
        CVpoint* pp = (CVpoint*)(featpoints->datap + i * featpoints->dim);//point= &data[i]
        color24_t thiscolor = { .G = 255 };
        LCD_Draw_Rect(stx + pp->x - 1, sty + pp->y - 1, 2, 2, thiscolor);
        //LCD_Draw_Point(stx+pp->x, sty + pp->y, thiscolor);
    }

    //信息显示
    sprintf(myInform, "SUSAN FeatPoint ");
    LCD_ShowString(stx + 0, sty + 0, myInform);

    //释放featpoints内存
    YMCV_Free_Vector_U16_Free(featpoints);
    Delay(500);

    /*------------------------ Fasts特征点提取 ------------------------------*/
    myparam.type = CV_FeatPoint_Fast;
    myparam.param.fast.thresh = 20;//差值

    featpoints = YMKV_FeaturePoint_Find(myImage0_gray, &myparam);//特征点检测

    LCD_show_image(stx + 0, sty + 0, myImage0_gray);
    //显示特征点
    for (int i = 0; (featpoints != NULL) && (i < featpoints->dataNum); i++)
    {
        CVpoint* pp = (CVpoint*)(featpoints->datap + i * featpoints->dim);//point= &data[i]
        color24_t thiscolor = { .G = 255 };
        LCD_Draw_Rect(stx + pp->x - 1, sty + pp->y - 1, 2, 2, thiscolor);
    }

    
    //信息显示
    sprintf(myInform, "Fast9 FeatPoint  ");
    LCD_ShowString(stx + 0, sty + 0, myInform);

    //内存释放
    YMCV_Free_Vector_U16_Free(featpoints);
    Delay(500);

    /*------------------------ AGAST 特征点提取 ------------------------------*/
    myparam.type = CV_FeatPoint_AGAST;
    myparam.param.agast.thresh = 20;//差值

    featpoints = YMKV_FeaturePoint_Find(myImage0_gray, &myparam);//特征点检测

    LCD_show_image(stx + 0, sty + 0, myImage0_gray);
    //显示特征点
    for (int i = 0; (featpoints != NULL) && (i < featpoints->dataNum); i++)
    {
        CVpoint* pp = (CVpoint*)(featpoints->datap + i * featpoints->dim);//point= &data[i]
        color24_t thiscolor = { .G = 255 };
        LCD_Draw_Rect(stx + pp->x - 1, sty + pp->y - 1, 2, 2, thiscolor);
    }

    //信息显示
    sprintf(myInform, "AGAST FeatPoint  ");
    LCD_ShowString(stx + 0, sty + 0, myInform);

    //内存释放
    YMCV_Free_Vector_U16_Free(featpoints);
    Delay(500);

    /*------------------------ Harris 特征点提取 ------------------------------*/
    myparam.type = CV_FeatPoint_Harris;
    myparam.param.harris.thresh = 5;//剔除百分比 5/255

    featpoints = YMKV_FeaturePoint_Find(myImage0_gray, &myparam);//特征点检测

    LCD_show_image(stx + 0, sty + 0, myImage0_gray);
    //显示特征点
    for (int i = 0; (featpoints != NULL) && (i < featpoints->dataNum); i++)
    {
        CVpoint* pp = (CVpoint*)(featpoints->datap + i * featpoints->dim);//point= &data[i]
        color24_t thiscolor = { .G = 255 };
        LCD_Draw_Rect(stx + pp->x - 1, sty + pp->y - 1, 2, 2, thiscolor);
    }

    //信息显示
    sprintf(myInform, "Harris FeatPoint  ");
    LCD_ShowString(stx + 0, sty + 0, myInform);
    Delay(500);

    //释放featpoints内存
    YMCV_Free_Vector_U16_Free(featpoints);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_gray);
    //YMCV_Free_ImgFree(myImage1);
}





