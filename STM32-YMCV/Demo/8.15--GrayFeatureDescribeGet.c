#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以 特征描述子 为例，展示了brief特征描述符提取 
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
    /*------------------------ AGAST 特征点提取 ------------------------------*/
    CV_featpoint_param myparam;
    myparam.type = CV_FeatPoint_AGAST;
    myparam.param.agast.thresh = 20;//差值
    CVVECTORS_U16 featpoints = YMKV_FeaturePoint_Find(myImage0_gray, &myparam);//特征点检测

    /*------------------------ rotate brief特征描述符提取 ------------------------------*/
    CVVECTORS_U16 featdesc = YMCV_FeatureDescribe_RotateBrief_Cal(myImage0_gray, featpoints);


    LCD_show_image(stx + 0, sty + 0, myImage0_gray);//显示图片
    //显示特征描述符
    for (int i = 0; (featpoints != NULL) && (i < featpoints->dataNum); i++)
    {
        CVpoint* pp = (CVpoint*)(featpoints->datap + i * featpoints->dim);//特征点
        CV_featdesc_type* desc = (CV_featdesc_type*)(featdesc->datap + i * featdesc->dim);//特征描述

        //绘制重心方向直线
        int r = 15;//由于模板选用r=15进行求描述符，这里直接用绘制r = 15的圆形
        int lendx = r * YMCV_Cos(desc->angle * YMCV_Deg2Rad);
        int lendy = r * YMCV_Sin(desc->angle * YMCV_Deg2Rad);
        color24_t thiscolor = { .R = 255 };
        LCD_Point_Color_Set(thiscolor);
        LCD_Draw_Line(stx + pp->x, sty + pp->y, stx + pp->x + lendx, sty + pp->y + lendy);
        //绘制点位置
        color24_t thiscolor2 = { .G = 255 };
        LCD_Point_Color_Set(thiscolor2);
        LCD_Draw_Circle(stx + pp->x, sty + pp->y, r);
    }

    //信息显示
    sprintf(myInform, "AGAST And Rotate BRIEF");
    LCD_ShowString(stx + 0, sty + 0, myInform);
    Delay(500);
		
    //释放featpoints内存
    YMCV_Free_Vector_U16_Free(featdesc);
    YMCV_Free_Vector_U16_Free(featpoints);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_gray);
    //YMCV_Free_ImgFree(myImage1);

    Delay(500);
}





