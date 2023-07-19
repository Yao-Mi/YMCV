#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_Special_Effect.h"
#include"../YMCV/CORE/YMCV_AreaFillCal.h"
#include <stdlib.h>

/*******************************************************************************************
    *
    * 说明  该例程以 inpaint为例，展示了自动修复的效果
    *
    ***************************************************************************************/

int main()
{
    int i;
    char myInform[100];
    LCD_Init(1000, 450);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(house_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, house_gray, sizeof(house_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 500, 270, CVGrayType);//灰度图0

    /*-------------------------- 手动创建mask图--------------------------------------------------*/
    LCD_Clear(0);
    //显示原图
    LCD_show_image(0, 0, myImage0);
    //sprintf(myInform, "手动绘制轮廓点 ");
    //LCD_ShowString(0, 310, myInform);

    color24_t thiscolor = { .R = 255,.G = 0 };
    LCD_Point_Color_Set(thiscolor);

    uint16 xpoint[] = { 423, 415, 455, 465, };
    uint16 ypoint[] = { 124, 157, 164, 124, };
    uint16 num = sizeof(xpoint) / 2;

    // char pflag = 0;
    // printf("准备绘制：\n'c':选择\n空格：结束绘制");
    // int px = 0, py = 0, w, h;

    // uint16 xpoint[100], ypoint[100], num = 0;

    // while (1)
    // {
    //    char mchr;
      
    //    if (pflag)
    //    {
    //        //从原图上绘制
    //        LCD_Fill_ConstRgbRect(px - 1, py - 1, 3, 3, thiscolor);
    //        pflag = 0;//画一次就可以了，不要一直画，不然后台SDL事件处理不过来会造成卡死或异常
    //    }

    //    if (LCD_GetChar(&mchr))
    //    {
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
    // }
    // printf("\n");
    // for (int i = 0; i < num; i++)
    //    printf("%d,", xpoint[i]);
    // printf("\n");
    // for (int i = 0; i < num; i++)
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

    CVIMAGE Masker = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);
    CVcolor fillval;
    fillval.gray = 0;
    YMCV_Image_FullFill(Masker, fillval);
    //对掩码分割进行区域填充
    fillval.gray = 255;
    YMCV_Img_Scanline_AreaFill(Masker, myPoint, num, fillval);
    LCD_show_image(500, 0, Masker);
    //信息显示
    sprintf(myInform, "原始mask图  ");
    LCD_ShowString(400, 310, myInform);
    PAUSE();

    YMCV_Image_MaskFill(myImage0, Masker, fillval);//原图的Mask显示
    LCD_show_image(0, 0, myImage0);

    //inpainting修复
    YMCV_Gray_Exemplar_Based_Inpainting(myImage0, Masker,5,11);

    //显示图片
    LCD_show_image(500, 0, myImage0);
    //信息显示
    sprintf(myInform, "Mask抠图及Inpainting修复结果图  ");
    LCD_ShowString(400, 310, myInform);

    //释放内存
    YMCV_Free_ImgFree(Masker);
    YMCV_Free_ImgFree(myImage0);

    PAUSE();
    LCD_Destory();
    return 0;
}

