//fun demo
void runYMCV_Demo(CVIMAGE myImage0)
{
    color24_t pointcolor = { .R=255,.G = 150 ,.B=0};

    LCD_Point_Color_Set(pointcolor);
    LCD_Draw_Circle(160, 120, 40);	// 画圆，圆心为(320, 240)，半径为100
}

