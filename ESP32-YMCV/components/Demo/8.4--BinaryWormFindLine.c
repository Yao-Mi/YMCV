
/*******************************************************************************************
    *
    * 说明  该例程以 以三节蠕虫算法为例，展示了直线检测效果，该算法为自研算法，商业需告知
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage0_gray = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_gray, CV_Rgb16ToGray);//rgb转灰度图

    LCD_show_image(stx + 0, 0, myImage0_gray);
	  int sty = myImage0_gray->height;
	
    color24_t pencolor = { .R = 255 };
    LCD_Point_Color_Set(pencolor);//设置画笔颜色

    /*--------------------------Canny边缘检测--------------------------------------------------*/
    //创建输出二值图像
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0_gray->width, myImage0_gray->height, CVBinaryType);//二值图
    CV_edge_param myParam = { .EdgeType = CV_Canny,.Thresh.canny.StrongT = 100,.Thresh.canny.WeakT = 40 };

    YMCV_Gray_EdgeDetect(myImage0_gray, myImage2, &myParam);//边缘检测

    LCD_show_image(stx + 0, sty + 0, myImage2);  //显示图片2

    //信息显示
    sprintf(myInform, "Canny Edge ");
    LCD_ShowString(stx + 0, sty + 0, myInform);
    Delay(300);    //暂停

    /*-------------------------- 蠕虫直线检测检测 --------------------------------------------------*/
    CVVECTORS_U16 lines = YMCV_WormFindLines(myImage2, 0.001, 10,1);
    if (lines != NULL)
    {
        for (int i = 0; i < lines->dataNum; i++)
        {
            CVline* thisline = (CVline*)(lines->datap + i * lines->dim);
            LCD_Draw_Line(stx + thisline->x1, thisline->y1, stx + thisline->x2, thisline->y2);//绘制线段
        }
    }
    //信息显示
    sprintf(myInform, "Worm Lines ");
    LCD_ShowString(stx + 0, sty + 0, myInform);
		Delay(500);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_Vector_U16_Free(lines);
}





