/*******************************************************************************************
    *
    * 说明  该例程以 扫描线种子点填充为例，展示了种子填充的效果
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

    /*-------------------------- 种子点填充--------------------------------------------------*/
    LCD_Clear(WHITE);
    LCD_show_image(stx, 0, myImage0);
    
    CVcolor fillval;
    fillval.rgb16 = 0xfe00;//填充值
    CVpoint seed = { .x = 60,.y = 45 };//种子点坐标
    int8 floaterror = 50;//种子点附近值，浮动误差为±50，即填充当前点颜色值±50的连通区域
		
    color24_t thiscolor = { .R = 255 };
    LCD_Fill_ConstRgbRect(stx + seed.x - 1, seed.y - 1, 3, 3, thiscolor);//绘制种子点
		
		
    //对彩色图进行种子填充
    YMCV_Image_ScanLine_SeedFill(myImage0, seed, floaterror, fillval);

    LCD_show_image(stx, myImage0->height, myImage0);
    //信息显示
    sprintf(myInform, "seed fill rgb ");
    LCD_ShowString(stx, myImage0->height, myInform);
}

