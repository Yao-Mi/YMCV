#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以 inpaint为例，展示了自动修复的效果
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(house2_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, (void*)house2_gray, sizeof(house2_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVGrayType);//灰度图0

    /*-------------------------- 手动创建mask图--------------------------------------------------*/
    LCD_Clear(GRAY);
    //显示原图
    LCD_show_image(stx + 0, 0, myImage0_gray);
    int sty = myImage0_gray->height;
	  Delay(300);
    color24_t thiscolor = { .R = 255,.G = 0 };
    LCD_Point_Color_Set(thiscolor);

    uint16 xpoint[] = { 135, 150, 150, 135, };
    uint16 ypoint[] = { 55, 55, 70, 70, };
    uint16 num = sizeof(xpoint) / 2;
    //利用鼠标绘制轮廓请参考PC版本的Demo
    int i = 0;
    //绘制轮廓
    for (i = 0; i < num - 1; i++)
    {
        LCD_Draw_Line(stx + xpoint[i], ypoint[i], stx + xpoint[i + 1], ypoint[i + 1]);
    }
    LCD_Draw_Line(stx + xpoint[i], ypoint[i], stx + xpoint[0], ypoint[0]);

    //创建为点集
    CVpoint* myPoint = (CVpoint*)CV_malloc1(num * sizeof(CVpoint));//点集
    for (int i = 0; i < num; i++)
    {
        CVpoint* thiscolor = myPoint + i;
        thiscolor->x = xpoint[i];
        thiscolor->y = ypoint[i];
    }

    CVIMAGE Masker = YMCV_Creat_Img_Creat(myImage0_gray->width, myImage0_gray->height, CVBinaryType);
    CVcolor fillval;
    fillval.gray = 0;
    YMCV_Image_FullFill(Masker, fillval);
    //对掩码分割进行区域填充
    fillval.gray = 255;
    YMCV_Img_Scanline_AreaFill(Masker, myPoint, num, fillval);
    LCD_show_image(stx + 0, sty+0, Masker);

    YMCV_Image_MaskFill(myImage0_gray, Masker, fillval);//原图的Mask显示
    LCD_show_image(stx + 0, 0, myImage0_gray);
    //信息显示
    sprintf(myInform, "mask image ");
    LCD_ShowString(stx + 0, 0, myInform);
    Delay(300);
		
    //inpainting修复
    YMCV_Gray_Exemplar_Based_Inpainting(myImage0_gray, Masker,4,9);

    //显示图片
    LCD_show_image(stx + 0, sty + 0, myImage0_gray);
    //信息显示
    sprintf(myInform, "Inpainting Result");
    LCD_ShowString(stx + 0, sty + 0, myInform);
    Delay(500);
    //释放内存
    YMCV_Free_ImgFree(Masker);
    YMCV_Free_ImgFree(myImage0_gray);

}

