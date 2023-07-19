#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以 Canny滤波器为例，展示了边缘检测效果
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
    sprintf(myInform, "ED Find Lines ");
    LCD_ShowString(stx + 0, sty + 0, myInform);
    Delay(300);//暂停

    /*-------------------------- EDLines直线检测 --------------------------------------------------*/
    CVVECTORS_U16 lines = YMCV_Fast_EdgeDrawing_Lines(chains, 0.25f, 6);

    YMCV_Merge_LineSegments(lines, 3, 20);//尝试进行合并
    for (size_t i = 0; i < lines->dataNum; i++)
    {
	    color24_t rgb = { .R = 255 };
	    LCD_Point_Color_Set(rgb);
	    CV_ed_line* point_p = (CV_ed_line*)(lines->datap + i * lines->dim);
	    LCD_Draw_Line(stx + point_p->start_end.x1, point_p->start_end.y1, stx + point_p->start_end.x2, point_p->start_end.y2);  
    }

		Delay(300); //暂停 
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_Chains_U16_Free(chains);//2D链码存储清理
    YMCV_Free_Vector_U16_Free(lines);//直线内存清理
}





