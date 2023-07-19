/*******************************************************************************************
    *
    * 说明  该例程以 区域扫描线填充为例，展示了环形区域填充的效果
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40,i;
    char myInform[100];
	
    /*-------------------------- 手动创建TriMap--------------------------------------------------*/
    LCD_Clear(WHITE);
    //显示原图
    LCD_show_image(40, myImage0->height, myImage0);

    uint16 xpoint[] = { 121, 141, 159, 151, 131, 88, 66, 65, 67, 84, 105, };
    uint16 ypoint[] = { 97, 86, 45, 22, 13, 18, 22, 38, 59, 78, 91, };
    uint16 num = sizeof(xpoint) / 2;

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

    CVIMAGE TriMapMarkers = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVTrimapType);
    YMCV_TriMap_Init(TriMapMarkers); //trimap初始化
    LCD_show_image(stx, 0, TriMapMarkers);
    //信息显示
    sprintf(myInform, "old - TriMap");
    LCD_ShowString(stx, 0, myInform);

		Delay(300);
		/*----------------------------环形区域填充----------------------------*/
		
    CVcolor fillval;
    fillval.gray = CVTriMapMaxVl - 10;//可能为前景的值

    //对Trimap进行区域填充
    YMCV_Img_Scanline_AreaFill(TriMapMarkers, myPoint, num, fillval);

    LCD_show_image(stx, 0, TriMapMarkers);
    //信息显示
    sprintf(myInform, "ring fill  ");
    LCD_ShowString(stx, 0, myInform);
    Delay(300);

		/*----------------------------彩色图填充----------------------------*/
		
    fillval.rgb16 = 0xfe00;
    //对彩色图进行区域填充
    YMCV_Img_Scanline_AreaFill(myImage0, myPoint, num, fillval);
    LCD_show_image(stx, 0, myImage0);
    //信息显示
    sprintf(myInform, "rgb fill  ");
    LCD_ShowString(stx, 0, myInform);

    //释放内存
    YMCV_Free_ImgFree(TriMapMarkers);

    Delay(300);
}

