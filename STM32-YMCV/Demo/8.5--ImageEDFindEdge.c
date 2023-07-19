#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以 ED 为例，展示了边缘提取效果
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
    /*-------------------------ED边缘检测--------------------------------------------------*/
    //创建输出二值图像
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0_gray->width, myImage0_gray->height, CVBinaryType);//二值图

    CVCHAINS_U16 chains = YMCV_Fast_EdgeDrawing(myImage0_gray, CV_Sobel_Graident, 20, myImage2);

    LCD_show_image(stx + 0, sty + 0, myImage2);  //显示图片2
    //信息显示
    sprintf(myInform, "ED Find Edge ");
    LCD_ShowString(stx + 0, sty + 0, myInform);
    Delay(300);//暂停

    //在原图上绘制边缘链码
    for (int i = 0; i < chains->cNum; i++)
    {
        CVVECTORS_U16 pixles = chains->chain[i];

        //起点单独绘制
        CV_ed_point pxy;
        CV_memcpy(&pxy, &pixles->datap[0], sizeof(CV_ed_point));
        color24_t rgb = { .R = 255 };
        LCD_Fill_ConstRgbRect(stx + pxy.pos.x, pxy.pos.y, 2, 2, rgb);
        //剩余点
        for (int pi = 1; pi < pixles->dataNum; pi++)
        {
            CV_ed_point pxy;
            CV_memcpy(&pxy, &pixles->datap[pi * pixles->dim], sizeof(CVpoint));
            color24_t rgb = { .G = 255 };
            LCD_Fill_ConstRgbRect(stx + pxy.pos.x, pxy.pos.y, 1, 1, rgb);
        }
				//暂停
        Delay(200);
    }


    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_Chains_U16_Free(chains);//2D链码存储清理

    Delay(500);
}





