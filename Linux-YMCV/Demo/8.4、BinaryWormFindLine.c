#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_EdgeCal.h"
#include"../YMCV/CORE/YMCV_FeatCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 以三节蠕虫算法为例，展示了直线检测效果，该算法为自研算法，商业需告知
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 450);
    color24_t pencolor = { .R = 255 };
    LCD_Point_Color_Set(pencolor);//设置画笔颜色

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(mm00_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, mm00_gray, sizeof(mm00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//灰度图0


    /*--------------------------Canny边缘检测--------------------------------------------------*/
    //创建输出二值图像
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);//二值图
    CV_edge_param myParam = { .EdgeType = CV_Canny,.Thresh.canny.StrongT = 100,.Thresh.canny.WeakT = 40 };

    YMCV_Gray_EdgeDetect(myImage0, myImage2, &myParam);//边缘检测

    LCD_Clear(0);
    LCD_show_image(0, 0, myImage0);//显示图片0
    LCD_show_image(500, 0, myImage2);  //显示图片2

    //信息显示
    sprintf(myInform, "Canny 边缘检测图像 ");
    LCD_ShowString(100, 310, myInform);
    PAUSE();    //暂停

    /*-------------------------- 蠕虫直线检测检测 --------------------------------------------------*/
    CVVECTORS_U16 lines = YMCV_WormFindLines(myImage2, 0.001, 10,1);
    if (lines != NULL)
    {
        for (int i = 0; i < lines->dataNum; i++)
        {
            CVline* thisline = lines->datap + i * lines->dim;
            LCD_Draw_Line(thisline->x1, thisline->y1, thisline->x2, thisline->y2);//绘制线段
        }
    }

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_Vector_U16_Free(lines);

    PAUSE();
    LCD_Destory();
    return 0;
}





