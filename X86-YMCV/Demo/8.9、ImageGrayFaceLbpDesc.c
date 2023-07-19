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
    * 说明  该例程以 人脸灰度图为例，展示了LBP直方图描述符提取效果
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 550);
    color24_t pencolor = { .R = 255 };
    LCD_Point_Color_Set(pencolor);//设置画笔颜色

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(luojiuchuan_112x112_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, luojiuchuan_112x112_gray, sizeof(luojiuchuan_112x112_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 112, 112, CVGrayType);//灰度图0

    /*-------------------------人脸LBP直方图特征检测--------------------------------------------------*/

    //输入人脸图片限定最大为112×112，支持orl人脸数据集中92×112的大小
    CV_FACE_LBP_DESC lbpDescribe = YMCV_Face_UniformLBP_Describe(myImage0);

    
    LCD_Clear(0);
    LCD_show_image(0, 0, myImage0);//显示图片0
    LCDLbpHistShow(lbpDescribe, 300, 0);//显示LBP描述符

    //信息显示
    sprintf(myInform, " LBP直方图  ");
    LCD_ShowString(100, 310, myInform);
    
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_FaceLbp_Hist_free(lbpDescribe);//特征内存释放

    PAUSE();
    LCD_Destory();
    return 0;
}





