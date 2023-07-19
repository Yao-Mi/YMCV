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
    * 说明  该例程以 Canny滤波器为例，展示了边缘检测效果
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
    sprintf(myInform, "ED 直线检测 ");
    LCD_ShowString(100, 310, myInform);

    /*-------------------------- EDLines直线检测 --------------------------------------------------*/
    CVVECTORS_U16 lines = YMCV_Fast_EdgeDrawing_Lines(chains, 0.25f, 9);

    YMCV_Merge_LineSegments(lines, 3, 20);//尝试进行合并
    for (size_t i = 0; i < lines->dataNum; i++)
    {
	    color24_t rgb = { .G = 255 };
	    LCD_Point_Color_Set(rgb);
	    CV_ed_line* point_p = lines->datap + i * lines->dim;
	    LCD_Draw_Line(point_p->start_end.x1, point_p->start_end.y1, point_p->start_end.x2, point_p->start_end.y2);
       // PAUSE(); //暂停   
    }

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_Chains_U16_Free(chains);//2D链码存储清理
    YMCV_Free_Vector_U16_Free(lines);//直线内存清理

    PAUSE();
    LCD_Destory();
    return 0;
}





