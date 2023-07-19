
/*******************************************************************************************
    *
    * 说明  该例程以 区域生长为例，展示了mask二值图创建
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];
	
    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage0_gray = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_gray, CV_Rgb16ToGray);//rgb转灰度图
		
    /*--------------------------灰度区域生长--------------------------------------------------*/
    int x = 40, y = 60;

    for (int i = 0; i < 10; i++)
    {
        int thresh = 30;
        //区域生长 返回mask二值图
        CVIMAGE myImage2 = YMCV_Gray_Region_Growing8(myImage0_gray, x, y, thresh);
			
				//显示图片0
				LCD_show_image(stx + 0, 0, myImage0_gray);
        //绘制种子点位置
        color24_t cvl = { .R = 255,.G = 120,.B = 50 };
        LCD_Fill_ConstRgbRect(stx + x - 1, y - 1, 3, 3, cvl);
        //显示图片2
        LCD_show_image(stx + 0, myImage0_gray->height, myImage2);

        //信息显示
        sprintf(myInform, "region growing: \nseed(%3d %3d)\ngrowThresh:%d",x,y, thresh);
				LCD_ShowString(0, myImage0_gray->height, myInform);
        //暂停300ms
        Delay(300);
    
        x += 8;//移动种子点

        //释放图像内存
        YMCV_Free_ImgFree(myImage2);
    }

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_gray);
}









