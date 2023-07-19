#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_SegnmCal.h"
#include"../YMCV/CORE/YMCV_AreaFillCal.h"
#include <stdlib.h>

/*******************************************************************************************
    *
    * 说明  该例程以 区域扫描线填充为例，展示了环形区域填充的效果
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
    uint8* myData2 = CV_malloc1(sizeof(mm00));
    CV_memcpy(myData2, mm00, sizeof(mm00));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//灰度图0
    CVIMAGE myImage2 = YMCV_Creat_DataToImg((uint8*)myData2, 250, 140, CVRgb16Type);

    /*-------------------------- 手动创建TriMap--------------------------------------------------*/
    LCD_Clear(0);
    //显示原图
    LCD_show_image(0, 0, myImage0);
    sprintf(myInform, "手动绘制轮廓点 ");
    LCD_ShowString(0, 310, myInform);

    char pflag = 0;
    printf("准备绘制：\n'c':选择\n空格：结束绘制");
    int px = 0, py = 0, w, h;

    uint16 xpoint[] = { 121, 171, 187, 174, 131, 88, 66, 65, 67, 84, 105, };
    uint16 ypoint[] = { 97, 86, 45, 22, 13, 18, 22, 38, 59, 78, 91, };
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
    LCD_Draw_Line(xpoint[i], ypoint[i], xpoint[0], ypoint[0]);

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
    LCD_show_image(300, 0, TriMapMarkers);
    //信息显示
    sprintf(myInform, "原始TriMap图  ");
    LCD_ShowString(400, 310, myInform);
    PAUSE();

    CVcolor fillval;
    fillval.gray = CVTriMapMaxVl - 10;//可能为前景的值

    //对Trimap进行区域填充
    YMCV_Img_Scanline_AreaFill(TriMapMarkers, myPoint, num, fillval);

    LCD_show_image(300, 0, TriMapMarkers);
    //信息显示
    sprintf(myInform, "环形区域填充图  ");
    LCD_ShowString(400, 310, myInform);
    PAUSE();

    fillval.rgb16 = 0xfe00;
    //对彩色图进行区域填充
    YMCV_Img_Scanline_AreaFill(myImage2, myPoint, num, fillval);
    LCD_show_image(600, 0, myImage2);
    //信息显示
    sprintf(myInform, "彩色图填充  ");
    LCD_ShowString(700, 310, myInform);

    //释放内存
    YMCV_Free_ImgFree(TriMapMarkers);
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage2);

    PAUSE();
    LCD_Destory();
    return 0;
}

