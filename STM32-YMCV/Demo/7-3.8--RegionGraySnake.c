
/*******************************************************************************************
    *
    * 说明  该例程以 Snake轮廓模型为例，展示了外轮廓提取效果
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];
	
    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage0_gray = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_gray, CV_Rgb16ToGray);//rgb转灰度图
	
	  for (int i = 0,imax = myImage0_gray->width*myImage0_gray->height; i < imax; i++)
    {
        myImage0_gray->data[i] = (myImage0_gray->data[i] >> 7) << 7;
    }
	  
		//显示图片0
		LCD_show_image(stx + 0, 0, myImage0_gray);
	  
    color24_t thiscolor = { .R = 255,.G = 0 };
    /*-------------------------- 手动创建轮廓--------------------------------------------------*/
    
		//由于单片机未接键盘和鼠标，绘制这部分被删除了，直接采用固定点，更详细的使用参考PC上的Demo
		
    uint16 xpoint[] = { 143, 143, 142, 140, 135, 131, 128, 123, 120, 112, 105, 97, 93, 91, 91, 89, 87, 87, 88, 90, 91, 93, 95, 95, 97, 98, 101, 105, 109, 114, 119, 123, 128, 135, 139, 139, 143, 143, };
    uint16 ypoint[] = { 55, 49, 44, 32, 24, 18, 16, 11, 5, 2, 2, 3, 6, 10, 15, 21, 29, 35, 38, 42, 48, 51, 54, 58, 59, 63, 68, 70, 74, 74, 78, 80, 80, 76, 70, 68, 65, 59, }; 
    uint16 num = sizeof(xpoint)/2;

		int i;
		//点集太靠边了，这里移到中间区域
		for (i = 0; i < num; i++)
		{
			xpoint[i] -= 20;
			ypoint[i] += 20;
		}
		
    LCD_Point_Color_Set(thiscolor);;
    int spy = 0;
    for (i = 0; i < num - 1; i++)
    {
        LCD_Draw_Line(stx + xpoint[i], ypoint[i]+spy, stx + xpoint[i + 1], ypoint[i + 1]+spy);
    }
    LCD_Draw_Line(stx + xpoint[i], ypoint[i]+spy, stx + xpoint[0], ypoint[0]+spy);
		
    //信息显示
    sprintf(myInform, "contour  init ");
    LCD_ShowString(0, 0, myInform);

    /*-------------------------- snake轮廓分割 --------------------------------------------------*/
    int search_r = 3;
    //α，β，γ取值范围[-1,1]
    float alpha = 0.3;//迭代变化率，变化步长，刚度
    float beta = 0.35;//正数代表向内收缩，值控制圆滑程度
    float gamma = 1.0;//图像能量

    //kapa取值[-1,1]
    float imageKpa = 0.1;//正数代表向黑色靠拢，负数向白色靠拢
    float gradientKpa = 1.0;//向梯度高的方向靠拢
    YMCV_Gray_Snake(myImage0_gray, xpoint, ypoint,num, search_r,500,alpha,beta,gamma, imageKpa, gradientKpa);//将Trimap图像分割清晰
     //显示图片
    LCD_show_image(stx + 0, myImage0_gray->height, myImage0_gray);
    //信息显示
    sprintf(myInform, "snake  contour");
    LCD_ShowString(0, myImage0_gray->height, myInform);
		
    spy = myImage0_gray->height;
    for (i = 0; i < num - 1; i++)
    {
        //printf("点：%d , %d\n", xpoint[i], ypoint[i]);
        LCD_Draw_Line(stx + xpoint[i], ypoint[i]+spy, stx + xpoint[i + 1], ypoint[i + 1]+spy);
    }
    LCD_Draw_Line(stx + xpoint[i], ypoint[i]+spy, stx + xpoint[0], ypoint[0]+spy);
    //暂停500ms
		Delay(500);
		//释放内存
    YMCV_Free_ImgFree(myImage0_gray);
}


