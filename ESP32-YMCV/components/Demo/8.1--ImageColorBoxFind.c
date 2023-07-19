
/*******************************************************************************************
    *
    * 说明  该例程以 色块查找为例，展示了bolb提取的效果
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage2, CV_Rgb16ToGray);//rgb转灰度图
	
    color24_t thiscolor = { .G = 255 };

    /*------------------------ 灰度色块提取 ------------------------------*/
    //显示原图
    LCD_show_image(stx + 0, 0, myImage2);

    CVcolor minVal,maxVal;
    minVal.gray = 100;
    maxVal.gray = 150;

    CVVECTORS_U16 blobs= YMCV_Image_ColorBox_Find(myImage2, minVal, maxVal, 50,1);
    LCD_show_image(stx + 0, myImage2->height, myImage2);
		
		int sty = myImage2->height;
		
    //显示blob的box
    for (int i = 0; (blobs!=NULL)&&(i < blobs->dataNum); i++)
    {
        CVblob* blobp = (CVblob*)(blobs->datap + i * blobs->dim);//blobp= &data[i]
        LCD_Draw_Rect(stx + blobp->box.x, blobp->box.y, blobp->box.w, blobp->box.h, thiscolor);
        LCD_Draw_Rect( stx + blobp->box.x,  sty + blobp->box.y, blobp->box.w, blobp->box.h, thiscolor);
    }

    //释放blob内存
    YMCV_Free_Vector_U16_Free(blobs);
		//信息显示
    sprintf(myInform, "Gray Color Box ");
    LCD_ShowString(stx + 0, sty + 0, myInform);
		Delay(800);

    /*------------------------ 彩色色块提取 ------------------------------*/
    LCD_show_image(stx + 0, 0, myImage0);
		sty = myImage0->height;
		
    minVal.rgb16 = YMCV_Rgb3ToRgb565(100, 60, 40);
    maxVal.rgb16 = YMCV_Rgb3ToRgb565(200, 150, 100);
    blobs = YMCV_Image_ColorBox_Find(myImage0, minVal, maxVal, 100,1);
		
		LCD_show_image(stx + 0, sty + 0, myImage0);
    //显示blob的box
    for (int i = 0; (blobs != NULL) && (i < blobs->dataNum); i++)
    {
        CVblob* blobp = (CVblob*)(blobs->datap + i * blobs->dim);//blobp= &data[i]
        LCD_Draw_Rect(stx + blobp->box.x, blobp->box.y, blobp->box.w, blobp->box.h, thiscolor);
        LCD_Draw_Rect(stx + blobp->box.x, sty + blobp->box.y, blobp->box.w, blobp->box.h, thiscolor);
    }
    //信息显示
    sprintf(myInform, "RGB Color Box  ");
    LCD_ShowString(stx + 0, sty + 0, myInform);
		Delay(800);
		
    //释放blob内存
    YMCV_Free_Vector_U16_Free(blobs);

    //释放内存
    YMCV_Free_ImgFree(myImage2);
		
}

