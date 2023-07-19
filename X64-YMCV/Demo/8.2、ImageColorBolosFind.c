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
    * 说明  该例程以 多色块同时查找为例，展示了bolb提取的效果
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

    /*------------------------ 灰度色块提取 ------------------------------*/
    CVBLOB_SEG_RENDER blob_seg_render = YMCV_Creat_BlobSegRender_Creat(CV_DoubleThreshold);//创建二值化分割渲染器

    CV_binary_param myParam[8];
    //设置参数
    myParam[0].gray_double_t.thresh_min = 0; //第一组
    myParam[0].gray_double_t.thresh_max = 20;
    myParam[1].gray_double_t.thresh_min = 25;//第二组
    myParam[1].gray_double_t.thresh_max = 55;
    myParam[2].gray_double_t.thresh_min = 60;//第三组
    myParam[2].gray_double_t.thresh_max = 100;
    myParam[3].gray_double_t.thresh_min = 110;//第四组
    myParam[3].gray_double_t.thresh_max = 150;
    myParam[4].gray_double_t.thresh_min = 160;//第五组
    myParam[4].gray_double_t.thresh_max = 190;
    myParam[5].gray_double_t.thresh_min = 210;//第六组
    myParam[5].gray_double_t.thresh_max = 220;
    myParam[6].gray_double_t.thresh_min = 225;//第七组
    myParam[6].gray_double_t.thresh_max = 238;
    myParam[7].gray_double_t.thresh_min = 240;//第八组
    myParam[7].gray_double_t.thresh_max = 255;

    //对渲染器加载8组参数
    YMCV_BlobSegKernel_Load(blob_seg_render, myParam,8);

    //色块查找
    uint8 minArea = 50;
    uint8 showSegX = 2;//显示第二组数据的分割图
    CVVECTORS_U16 blobs = YMCV_Image_Find_ColorBlobs(myImage0, blob_seg_render, minArea, showSegX);

    LCD_show_image(300, 0, myImage0);
    //显示blob的box
    for (int i = 0; (blobs != NULL) && (i < blobs->dataNum); i++)
    {
        CVblob* blobp = blobs->datap + i * blobs->dim;//blobp= &data[i]
        uint8 label = blobp->label;//用作框的颜色标签
        //选择绘制部分box
        if((label+1 == showSegX) || (showSegX == 0))
        LCD_Draw_Rect(300 + blobp->box.x, blobp->box.y, blobp->box.w, blobp->box.h, YMCV_HeatPalette[label*36]);
    }

    //释放blob内存 和渲染器内存
    YMCV_Free_Vector_U16_Free(blobs);
    YMCV_Free_BlobSegRender_Free(blob_seg_render);
    //信息显示
    sprintf(myInform, "灰度区域色块box图  ");
    LCD_ShowString(400, 310, myInform);
    PAUSE();

    /*------------------------ RGB彩色色块提取 ------------------------------*/
    blob_seg_render = YMCV_Creat_BlobSegRender_Creat(CV_RGB2Threshold);//重新创建二值化分割渲染器
    CVIMAGE myImage3 = YMCV_Creat_Img_BackUp(myImage2);
    LCD_show_image(0, 0, myImage2);
    myParam[0].rgb_double_t.thresh_Rmin = 150; //第一组
    myParam[0].rgb_double_t.thresh_Rmax = 255;
    myParam[0].rgb_double_t.thresh_Gmin = 30;
    myParam[0].rgb_double_t.thresh_Gmax = 90;
    myParam[0].rgb_double_t.thresh_Bmin = 0;
    myParam[0].rgb_double_t.thresh_Bmax = 80;
    myParam[1].rgb_double_t.thresh_Rmin = 170; //第二组
    myParam[1].rgb_double_t.thresh_Rmax = 230;
    myParam[1].rgb_double_t.thresh_Gmin = 110;
    myParam[1].rgb_double_t.thresh_Gmax = 150;
    myParam[1].rgb_double_t.thresh_Bmin = 80;
    myParam[1].rgb_double_t.thresh_Bmax = 110;
    //对渲染器加载2组参数
    YMCV_BlobSegKernel_Load(blob_seg_render, myParam, 2);

    //色块查找
    minArea = 100;
    showSegX = 2;//显示第二组数据的分割图
    blobs = YMCV_Image_Find_ColorBlobs(myImage2, blob_seg_render, minArea, showSegX);
    LCD_show_image(600, 0, myImage2);
    //显示blob的box
    for (int i = 0; (blobs != NULL) && (i < blobs->dataNum); i++)
    {
        CVblob* blobp = blobs->datap + i * blobs->dim;//blobp= &data[i]
        uint8 label = blobp->label;//用作框的颜色标签
        //选择绘制部分box
        if ((label + 1 == showSegX) || (showSegX == 0))
            LCD_Draw_Rect(600 + blobp->box.x, blobp->box.y, blobp->box.w, blobp->box.h, YMCV_HeatPalette[label * 36]);
    }
    //信息显示
    sprintf(myInform, "彩色图RGB色块box图  ");
    LCD_ShowString(700, 310, myInform);
    //释放blob内存
    YMCV_Free_Vector_U16_Free(blobs);
    YMCV_Free_BlobSegRender_Free(blob_seg_render);

    PAUSE();
    /*------------------------ LAB彩色色块提取 ------------------------------*/
    blob_seg_render = YMCV_Creat_BlobSegRender_Creat(CV_LAB2Threshold);//重新创建二值化分割渲染器
    myParam[0].lab_double_t.thresh_Lmin = 75; //第一组
    myParam[0].lab_double_t.thresh_Lmax = 90;
    myParam[0].lab_double_t.thresh_Amin = 0;
    myParam[0].lab_double_t.thresh_Amax = 6;
    myParam[0].lab_double_t.thresh_Bmin = 12;
    myParam[0].lab_double_t.thresh_Bmax = 20;
    myParam[1].lab_double_t.thresh_Lmin = 60; //第二组
    myParam[1].lab_double_t.thresh_Lmax = 80;
    myParam[1].lab_double_t.thresh_Amin = 10;
    myParam[1].lab_double_t.thresh_Amax = 30;
    myParam[1].lab_double_t.thresh_Bmin = 10;
    myParam[1].lab_double_t.thresh_Bmax = 50;
    //对渲染器加载2组参数
    YMCV_BlobSegKernel_Load(blob_seg_render, myParam, 2);

    //色块查找
    minArea = 100;
    showSegX = 0;//不显示数据的分割图
    blobs = YMCV_Image_Find_ColorBlobs(myImage3, blob_seg_render, minArea, showSegX);
    LCD_show_image(600, 0, myImage3);
    //显示blob的box
    for (int i = 0; (blobs != NULL) && (i < blobs->dataNum); i++)
    {
        CVblob* blobp = blobs->datap + i * blobs->dim;//blobp= &data[i]
        uint8 label = blobp->label;//用作框的颜色标签
        //选择绘制部分box
        if ((label + 1 == showSegX) || (showSegX == 0))
            LCD_Draw_Rect(600 + blobp->box.x, blobp->box.y, blobp->box.w, blobp->box.h, YMCV_HeatPalette[label * 36]);
    }
    //信息显示
    sprintf(myInform, "彩色图LAB色块box图  ");
    LCD_ShowString(700, 310, myInform);
    //释放blob内存
    YMCV_Free_Vector_U16_Free(blobs);
    YMCV_Free_BlobSegRender_Free(blob_seg_render);

    //释放内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);

    PAUSE();
    LCD_Destory();
    return 0;
}

