#include "../SDL_LCD/SDL_LCD.h"
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
    * 说明  该例程展示了直线合并的效果
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 550);
    color24_t pencolor = { .R = 255 };
    LCD_Point_Color_Set(pencolor);//设置画笔颜色

    /*-------------------------- 直线合并 --------------------------------------------------*/
    CVVECTORS_U16 lines = YMCV_Creat_Vector_U16(sizeof(CVline) / sizeof(uint16), 8);
    CVline* lp = lines->datap;
    //横线
    lp[0].x1 = 0; lp[0].y1 = 10; lp[0].x2 = 15; lp[0].y2 = 10;
    lp[1].x1 = 16; lp[1].y1 = 12; lp[1].x2 = 30; lp[1].y2 = 12;
    lp[2].x1 = 10; lp[2].y1 = 10; lp[2].x2 = 21; lp[2].y2 = 12;
    lp[3].x1 = 10; lp[3].y1 = 11; lp[3].x2 = 20; lp[3].y2 = 11;
    //竖线
    lp[4].y1 = 0; lp[4].x1 = 10; lp[4].y2 = 15; lp[4].x2 = 10;
    lp[5].y1 = 16; lp[5].x1 = 12; lp[5].y2 = 30; lp[5].x2 = 12;
    lp[6].y1 = 10; lp[6].x1 = 10; lp[6].y2 = 21; lp[6].x2 = 12;
    lp[7].y1 = 10; lp[7].x1 = 11; lp[7].y2 = 20; lp[7].x2 = 11;
    
    //未合并时
    if (lines != NULL)
    {
        for (int i = 0; i < lines->dataNum; i++)
        {
            CVline* thisline = lines->datap + i * lines->dim;
            LCD_Draw_Line(300+thisline->x1, thisline->y1, 300 + thisline->x2, thisline->y2);//绘制线段
        }
    }
    //信息显示
    sprintf(myInform, "原始线段 ");
    LCD_ShowString(250, 100, myInform);
    //合并后
    if (lines != NULL)
    {
        YMCV_Merge_LineSegments(lines, 2, 12);//进行线段合并，误差12度，
        for (int i = 0; i < lines->dataNum; i++)
        {
            CVline* thisline = lines->datap + i * lines->dim;
            LCD_Draw_Line(600 + thisline->x1, thisline->y1, 600 + thisline->x2, thisline->y2);//绘制线段

            printf("i=%d  %d,%d -- %d,%d\n", i, thisline->x1, thisline->y1, thisline->x2, thisline->y2);
        }
    }
    //信息显示
    sprintf(myInform, "合并后的线段 ");
    LCD_ShowString(550, 100, myInform);

    //释放内存
    YMCV_Free_Vector_U16_Free(lines);

    PAUSE();
    LCD_Destory();
    return 0;
}





