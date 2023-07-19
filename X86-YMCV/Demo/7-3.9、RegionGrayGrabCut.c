#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_EdgeCal.h"
#include"../YMCV/CORE/YMCV_MergeCal.h"
#include"../YMCV/CORE/YMCV_SegnmCal.h"
#include"../YMCV/CORE/YMCV_AreaFillCal.h"
#include <stdlib.h>

/*******************************************************************************************
    *
    * 说明  该例程以 GrabCut为例，展示了自动分割分割的效果
    *
    ***************************************************************************************/

int main()
{
    int i;
    char myInform[100];
    LCD_Init(1000, 450);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(mm00_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, mm00_gray, sizeof(mm00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//灰度图0

    /*-------------------------- 手动创建TriMap--------------------------------------------------*/
    LCD_Clear(0);
    //显示原图
    LCD_show_image(0, 0, myImage0);
    //sprintf(myInform, "手动绘制轮廓点 ");
    //LCD_ShowString(0, 310, myInform);

    color24_t thiscolor = { .R = 255,.G = 0 };
    LCD_Point_Color_Set(thiscolor);
    //char pflag = 0;
    //printf("准备绘制：\n'c':选择\n空格：结束绘制");
    //int px = 0, py = 0, w, h;

    uint16 xpoint[] = { 53, 53, 41, 40, 41, 41, 171, 177, 181, 181, 180, 175, 165, };
    uint16 ypoint[] = { 1, 24, 60, 88, 115, 138, 138, 122, 91, 65, 35, 14, 0, };
    uint16 num = sizeof(xpoint) / 2;

    //uint16 xpoint[100], ypoint[100], num = 0;

    //while (1)
    //{
    //    char mchr;
    //   
    //    if (pflag)
    //    {
    //        //从原图上绘制
    //        LCD_Fill_ConstRgbRect(px - 1, py - 1, 3, 3, thiscolor);
    //    }

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
    //printf("\n");
    //for (int i = 0; i < num; i++)
    //    printf("%d,", xpoint[i]);
    //printf("\n");
    //for (int i = 0; i < num; i++)
    //    printf("%d,", ypoint[i]);

    //绘制轮廓
    for (i = 0; i < num - 1; i++)
    {
        LCD_Draw_Line(xpoint[i], ypoint[i], xpoint[i + 1], ypoint[i + 1]);
    }
    LCD_Draw_Line(300 + xpoint[i], ypoint[i], 300 + xpoint[0], ypoint[0]);

    //创建为点集
    CVpoint* myPoint = (CVpoint*)CV_malloc1(num * sizeof(CVpoint));//点集
    for (int i = 0; i < num; i++)
    {
        CVpoint* thiscolor = myPoint + i;
        thiscolor->x = xpoint[i];
        thiscolor->y = ypoint[i];
    }

    CVIMAGE TriMapMarkers = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVTrimapType);
    YMCV_TriMap_Init(TriMapMarkers); //trimap初始化

    CVcolor fillval;
    //grabCut 使用的填充值：前景：[CVTriMapMinVl,CVTriMapMidle)，背景：(CVTriMapMidle,CVTriMapMaxVl]
    //其中，百分百为前景的，取CVTriMapMinVl，百分百背景的取CVTriMapMaxVl。
    fillval.gray = CVTriMapMaxVl - 10;//可能为前景的值

    //对Trimap进行区域填充
    YMCV_Img_Scanline_AreaFill(TriMapMarkers, myPoint, num, fillval);

    LCD_show_image(300, 0, TriMapMarkers);
    //信息显示
    sprintf(myInform, "原始TriMap图  ");
    LCD_ShowString(400, 310, myInform);
    PAUSE();


    /*-------------------------- grabCut分割 --------------------------------------------------*/
    YMCV_Gray_GrabCut(myImage0, TriMapMarkers, 2);//两次迭代

    //显示图片
    LCD_show_image(300, 0, TriMapMarkers);
    //信息显示
    sprintf(myInform, "TriMap结果图  ");
    LCD_ShowString(400, 310, myInform);


    CVIMAGE SegImage = YMCV_Image_Mask(TriMapMarkers, myImage0);//分割图当作掩码使用

    //显示图片分割结果
    LCD_show_image(600, 0, SegImage);

    //释放内存
    YMCV_Free_ImgFree(TriMapMarkers);
    YMCV_Free_ImgFree(SegImage);
    YMCV_Free_ImgFree(myImage0);

    PAUSE();
    LCD_Destory();
    return 0;
}

