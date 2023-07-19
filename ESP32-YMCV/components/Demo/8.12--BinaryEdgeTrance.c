#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以 边界跟踪 为例，展示了边界链码提取以及形状判别
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
    color24_t pencolor = { .R = 255 };
    LCD_Point_Color_Set(pencolor);//设置画笔颜色

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(tuxin02_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, (void*)tuxin02_gray, sizeof(tuxin02_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVGrayType);//灰度图0
   
    LCD_show_image(stx + 0, 0, myImage0_gray);//显示图片0
		int sty = myImage0_gray->height;
    /*------------------------ 灰度色块提取 ------------------------------*/

    CVcolor minVal, maxVal;
    minVal.gray = 200;
    maxVal.gray = 255;

    CVVECTORS_U16 blobs = YMCV_Image_ColorBox_Find(myImage0_gray, minVal, maxVal, 100, 1);//将提取结果输出到myImage0
    myImage0_gray->type = CVBinaryType;//强制转为二值图
    LCD_show_image(stx + 0, 0, myImage0_gray);
    //显示blob的box
    for (int i = 0; (blobs != NULL) && (i < blobs->dataNum); i++)
    {
        CVblob* blobp = (CVblob*)(blobs->datap + i * blobs->dim);//blobp= &data[i]
        color24_t thiscolor = { .G = 255 };
        //LCD_Draw_Rect(300 + blobp->box.x, blobp->box.y, blobp->box.w, blobp->box.h, thiscolor);
    }

    //信息显示
    sprintf(myInform, "gray Box ");
    LCD_ShowString(stx + 0, 0, myInform);
    Delay(500);

    /*-------------------------- 边界跟踪 --------------------------------------------------*/
    CVCHAINS_U16 edgeChains = YMCV_Image_Binary_Edge_Trace(myImage0_gray, blobs);//得到各个blobs的边界链码

		//清屏
		LCD_Fill_ConstRgbRect(stx + 0, myImage0_gray->height,myImage0_gray->width,myImage0_gray->height,(color24_t){0,0,0});
		
    //显示
    for (int i = 0; i < edgeChains->cNum; i++)//遍历边界链码
    {
        //绘制所有链码像素
        CVVECTORS_U16 pixles = edgeChains->chain[i];
        for (int pi = 0; pi < pixles->dataNum; pi++)//原始链码绘制
        {
            CVpoint* pxy = (CVpoint*)&pixles->datap[pi * pixles->dim];
            color24_t rgb = { .G = 255 };
            LCD_Fill_ConstRgbRect(stx + pxy->x, sty + pxy->y, 1, 1, rgb);
        }
    }
    //信息显示
    sprintf(myInform, "Binary Edge Trance");
    LCD_ShowString(stx + 0, sty, myInform);
    //暂停
    Delay(500);

    /*-------------------------- EDCorner凸角检测 --------------------------------------------------*/
    CVVECTORS_U16 corner = YMCV_Fast_EdgeDrawing_ChainsCornerNum(edgeChains, 20);//长度短于20不处理

    //显示
    for (int i = 0; i < corner->dataNum; i++)//遍历边界链码
    {
        //取得对应凸角信息
        CV_ed_corner* thisCorner = (CV_ed_corner*)&corner->datap[i * corner->dim];
        
        //形状判别
        switch (thisCorner->CornerNum)//判断凸角个数
        {
        case 0:
            sprintf(myInform, "circle");//圆
            break;
        case 1:
            sprintf(myInform, "halfcr");//半圆形
            break;
        case 2:
            sprintf(myInform, "ballcr");//椭圆形或很小的锐角三角形
            break;
        default:
					  sprintf(myInform, "Ang:%2d", thisCorner->CornerNum);
            break;
        }
        //在中心绘制文本
        LCD_ShowString(stx + thisCorner->center.x - 16, sty + thisCorner->center.y-5, myInform);
				//暂停200ms
				Delay(200);
    }
    //信息显示
    sprintf(myInform, "Edge Shape");
    LCD_ShowString(stx+0, sty, myInform);


    //释放blob内存
    YMCV_Free_Vector_U16_Free(blobs);
    YMCV_Free_Vector_U16_Free(corner);
    YMCV_Free_Chains_U16_Free(edgeChains);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_gray);
    //YMCV_Free_ImgFree(myImage1);

    Delay(500);
}





