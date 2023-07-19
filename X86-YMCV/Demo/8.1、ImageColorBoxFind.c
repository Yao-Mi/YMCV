#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_FeatCal.h"
#include"../YMCV/CORE/YMCV_PixConv.h"
#include <stdlib.h>

/*******************************************************************************************
    *
    * 说明  该例程以 色块查找为例，展示了bolb提取的效果
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


    /*-------------------------- 种子点填充--------------------------------------------------*/
    LCD_Clear(0);

    //显示原图
    LCD_show_image(0, 0, myImage0);
    color24_t thiscolor = { .G = 255 };

    /*------------------------ 灰度色块提取 ------------------------------*/

    CVcolor minVal,maxVal;
    minVal.gray = 100;
    maxVal.gray = 150;

    CVVECTORS_U16 blobs= YMCV_Image_ColorBox_Find(myImage0, minVal, maxVal, 100,1);
    LCD_show_image(300, 0, myImage0);
    //显示blob的box
    for (int i = 0; (blobs!=NULL)&&(i < blobs->dataNum); i++)
    {
        CVblob* blobp = blobs->datap + i * blobs->dim;//blobp= &data[i]
        LCD_Draw_Rect(300+blobp->box.x, blobp->box.y, blobp->box.w, blobp->box.h, thiscolor);
    }

    //释放blob内存
    YMCV_Free_Vector_U16_Free(blobs);
    
    //信息显示
    sprintf(myInform, "灰度区域色块box图  ");
    LCD_ShowString(400, 310, myInform);
    PAUSE();

    /*------------------------ 彩色色块提取 ------------------------------*/
    minVal.rgb16 = YMCV_Rgb3ToRgb565(100, 60, 40);
    maxVal.rgb16 = YMCV_Rgb3ToRgb565(200, 150, 100);
    blobs = YMCV_Image_ColorBox_Find(myImage2, minVal, maxVal, 100,1);
    LCD_show_image(600, 0, myImage2);
    //显示blob的box
    for (int i = 0; (blobs != NULL) && (i < blobs->dataNum); i++)
    {
        CVblob* blobp = blobs->datap + i * blobs->dim;//blobp= &data[i]
        LCD_Draw_Rect(600 + blobp->box.x, blobp->box.y, blobp->box.w, blobp->box.h, thiscolor);
    }
    //信息显示
    sprintf(myInform, "彩色图色块box图  ");
    LCD_ShowString(700, 310, myInform);
    //释放blob内存
    YMCV_Free_Vector_U16_Free(blobs);

    //释放内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage2);

    PAUSE();
    LCD_Destory();
    return 0;
}

