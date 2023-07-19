#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_FeatCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 边界跟踪 为例，展示了边界链码提取以及形状判别
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(640, 480);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(tuxin_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, tuxin_gray, sizeof(tuxin_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//灰度图0

    /*------------------------ 灰度色块提取 ------------------------------*/

    CVcolor minVal, maxVal;
    minVal.gray = 200;
    maxVal.gray = 255;

    CVVECTORS_U16 blobs = YMCV_Image_ColorBox_Find(myImage0, minVal, maxVal, 100, 1);//将提取结果输出到myImage0
    myImage0->type = CVBinaryType;//强制转为二值图
    LCD_show_image(300, 0, myImage0);
    //显示blob的box
    for (int i = 0; (blobs != NULL) && (i < blobs->dataNum); i++)
    {
        CVblob* blobp = blobs->datap + i * blobs->dim;//blobp= &data[i]
        color24_t thiscolor = { .G = 255 };
        //LCD_Draw_Rect(300 + blobp->box.x, blobp->box.y, blobp->box.w, blobp->box.h, thiscolor);
    }

    //信息显示
    sprintf(myInform, "灰度区域色块box图  ");
    LCD_ShowString(400, 310, myInform);
    PAUSE();

    /*-------------------------- 边界跟踪 --------------------------------------------------*/
    CVCHAINS_U16 edgeChains = YMCV_Image_Binary_Edge_Trace(myImage0, blobs);//得到各个blobs的边界链码

    //显示
    for (int i = 0; i < edgeChains->cNum; i++)//遍历边界链码
    {
        //绘制所有链码像素
        CVVECTORS_U16 pixles = edgeChains->chain[i];
        for (int pi = 0; pi < pixles->dataNum; pi++)//原始链码绘制
        {
            CVpoint* pxy = &pixles->datap[pi * pixles->dim];
            color24_t rgb = { .G = 255 };
            LCD_Fill_ConstRgbRect(pxy->x, pxy->y, 1, 1, rgb);
        }
    }
    //信息显示
    sprintf(myInform, "      二值图外边界跟踪     ");
    LCD_ShowString(100, 300, myInform);
    //暂停
    PAUSE();

    /*-------------------------- EDCorner凸角检测 --------------------------------------------------*/
    CVVECTORS_U16 corner = YMCV_Fast_EdgeDrawing_ChainsCornerNum(edgeChains, 20);//长度短于20不处理

    //显示
    for (int i = 0; i < corner->dataNum; i++)//遍历边界链码
    {
        //取得对应凸角信息
        CV_ed_corner* thisCorner = &corner->datap[i * corner->dim];
        
        //形状判别
        switch (thisCorner->CornerNum)//判断凸角个数
        {
        case 0:
            sprintf(myInform, "圆形");
            break;
        case 1:
            sprintf(myInform, "半圆形");
            break;
        case 2:
            sprintf(myInform, "椭圆形");
            break;
        default:
            sprintf(myInform, "%d角形 \n", thisCorner->CornerNum);
            break;
        }
        //在中心绘制文本
        LCD_ShowString(300 + thisCorner->center.x - 16, thisCorner->center.y-5, myInform);
    }
    //信息显示
    sprintf(myInform, "边界轮廓形状判别  ");
    LCD_ShowString(400, 310, myInform);


    //释放blob内存
    YMCV_Free_Vector_U16_Free(blobs);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    //YMCV_Free_ImgFree(myImage1);
    YMCV_Free_Chains_U16_Free(edgeChains);//2D链码存储清理

    PAUSE();
    LCD_Destory();
    return 0;
}





