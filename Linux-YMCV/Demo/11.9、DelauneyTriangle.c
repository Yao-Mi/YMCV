#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_Special_Effect.h"
#include"../YMCV/CORE/YMCV_BasicObjCal.h"
#include <stdlib.h>

/*******************************************************************************************
    *
    * 说明  该例程以 Delauney三角生成为例，展示了在顶点集中创建三角网格
    *
    ***************************************************************************************/
int main()
{
    int i;
    char myInform[100];
    LCD_Init(500, 450);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(mm00_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, mm00_gray, sizeof(mm00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//灰度图0

    LCD_Clear(0);
    //显示原图
    LCD_show_image(0, 0, myImage0);
    /*-------------------------- 网格效果 --------------------------------------------------*/

    //信息显示
    sprintf(myInform, "网格效果  ");
    LCD_ShowString(0, 310, myInform);

    // 确定网格点
    color24_t thiscolor = { .R = 255,.G = 0 };

    uint16 xpoint[] = { 89, 100, 110, 127, 140, 126, 103, 99, 121, 112, 117 };
    uint16 ypoint[] = { 34, 31, 28, 20, 51, 74, 65, 41, 32, 53, 62 };
    uint16 num = sizeof(xpoint) / 2;

    //uint16 xpoint[100], ypoint[100], num = 0;
    // 
    //char pflag = 0;
    //printf("准备绘制：\n'c':选择\n空格：结束绘制");
    //int px = 0, py = 0, w, h;
    //while (1)
    //{
    //    char mchr;
    //   
    //    if (pflag)
    //    {
    //        //从原图上绘制
    //        LCD_Fill_ConstRgbRect(px - 1, py - 1, 3, 3, thiscolor);
    //        pflag = 0;//画一次就可以了，不要一直画，不然后台SDL事件处理不过来会造成卡死或异常
    //    }
    // 
    //    if (LCD_GetChar(&mchr))
    //    {
    //        
    //        if (mchr == 'c')
    //        {
    //            pflag = 1;
    //            if (LCD_GetXY(0, 0, myImage0->width, myImage0->height, &px, &py))
    //            {
    //                xpoint[num] = px;
    //                ypoint[num] = py;
    //                num++;
    //                printf("选择点：%d , %d\n", px, py);
    //            }
    //        }
    //        if (mchr == ' ') { printf("结束绘制\n"); break; }
    //    }
    //    if (num >= 100) break;
    //}
    //for (int i = 0; i < num; i++)
    //{
    //    printf("%d,",xpoint[i]);
    //}
    //printf("\n");
    //for (int i = 0; i < num; i++)
    //{
    //    printf("%d,",ypoint[i]);
    //}
    //printf("\n");

    CVVECTORS_F32 fpointList = YMCV_Creat_Vector_F32(sizeof(CVfpoint) / sizeof(float32), num);
    for (i = 0; i < num; i++)
    {
        CVfpoint* fpthis = fpointList->datap + i * fpointList->dim;
        fpthis->x = xpoint[i];
        fpthis->y = ypoint[i];
    }

    //Delauney创建三角形
    CVVECTORS_F32 myTriangles = YMCV_DelauneyTriangle_Construct(fpointList);

    LCD_Point_Color_Set(thiscolor);
    for (i = 0; (myTriangles != NULL) && (i < myTriangles->dataNum); i++)
    {
        CVftriangle* fpthis = myTriangles->datap + i * myTriangles->dim;

        LCD_Draw_Line(fpthis->A.x, fpthis->A.y, fpthis->B.x, fpthis->B.y);//AB
        LCD_Draw_Line(fpthis->B.x, fpthis->B.y, fpthis->C.x, fpthis->C.y);//BC
        LCD_Draw_Line(fpthis->C.x, fpthis->C.y, fpthis->A.x, fpthis->A.y);//CA
    }
    //信息显示
    sprintf(myInform, "三角形网格  ");
    LCD_ShowString(0, 310, myInform);
    
    //释放内存
    YMCV_Free_ImgFree(myImage0);

    PAUSE();
    LCD_Destory();
    return 0;
}