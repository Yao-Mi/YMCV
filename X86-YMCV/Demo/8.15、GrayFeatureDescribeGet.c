#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_FeatCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 特征描述子 为例，展示了brief特征描述符提取 
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(640, 480);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(tuxin_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, tuxin_gray, sizeof(tuxin_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//灰度图0

    /*------------------------ AGAST 特征点提取 ------------------------------*/
    CV_featpoint_param myparam;
    myparam.type = CV_FeatPoint_AGAST;
    myparam.param.agast.thresh = 20;//差值
    CVVECTORS_U16 featpoints = YMKV_FeaturePoint_Find(myImage0, &myparam);//特征点检测

    LCD_show_image(0, 0, myImage0);//显示图片

    /*------------------------ rotate brief特征描述符提取 ------------------------------*/
    CVVECTORS_U16 featdesc = YMCV_FeatureDescribe_RotateBrief_Cal(myImage0, featpoints);


    LCD_show_image(0, 0, myImage0);//显示图片
    //显示特征描述符
    for (int i = 0; (featpoints != NULL) && (i < featpoints->dataNum); i++)
    {
        CVpoint* pp = featpoints->datap + i * featpoints->dim;//特征点
        CV_featdesc_type* desc = featdesc->datap + i * featdesc->dim;//特征描述

        //绘制重心方向直线
        int r = 15;//由于模板选用r=15进行求描述符，这里直接用绘制r = 15的圆形
        int lendx = r * YMCV_Cos(desc->angle * YMCV_Deg2Rad);
        int lendy = r * YMCV_Sin(desc->angle * YMCV_Deg2Rad);
        color24_t thiscolor = { .R = 255 };
        LCD_Point_Color_Set(thiscolor);
        LCD_Draw_Line(pp->x, pp->y, pp->x + lendx, pp->y + lendy);
        //绘制点位置
        color24_t thiscolor2 = { .G = 255 };
        LCD_Point_Color_Set(thiscolor2);
        LCD_Draw_Circle(pp->x, pp->y, r);
    }

    //信息显示
    sprintf(myInform, "AGAST 特征点与RotateBRIEF描述符提取  ");
    LCD_ShowString(200, 310, myInform);
    PAUSE();
    //释放featpoints内存
    YMCV_Free_Vector_U16_Free(featdesc);
    YMCV_Free_Vector_U16_Free(featpoints);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    //YMCV_Free_ImgFree(myImage1);

    PAUSE();
    LCD_Destory();
    return 0;
}





