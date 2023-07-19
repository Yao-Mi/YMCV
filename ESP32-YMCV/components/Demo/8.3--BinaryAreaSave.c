
/*******************************************************************************************
    *
    * 说明  该例程以 区域保留为例，展示了 保留较大面积区域的效果
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
    /*--------------------------灰度双阈值二值化--------------------------------------------------*/
    //创建输出二值图像
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0_gray->width, myImage0_gray->height, CVBinaryType);//二值图
    CV_binary_param myParam = { .gray_double_t.thresh_min = 100 ,.gray_double_t.thresh_max = 150 };

    //创建二值变换渲染器
    CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_DoubleThreshold);
    YMCV_BinaryKernel_Load(myRender1, &myParam);//加载参数进行渲染配置、初始化
    YMCV_Image_ThreshTo_Binary(myImage0_gray, myImage2, myRender1);//将目标图像的进行渲染
    
    //显示原图
    LCD_show_image(stx + 0, sty + 0, myImage2);
    
    //信息显示
    sprintf(myInform, "To Binary ");
    LCD_ShowString(stx + 0, sty + 0, myInform);
		Delay(500);
    /*------------------------ 二值区域过滤 ------------------------------*/
    //保留面积大于100的区域
    YMCV_Binary_AreaFilter(myImage2, 100);
    LCD_show_image(stx + 0, sty + 0, myImage2);

    //信息显示
    sprintf(myInform, "Area Save  ");
    LCD_ShowString(stx + 0, sty + 0, myInform);
		Delay(500);
		
    //释放内存
    YMCV_Free_Binary_RenderFree(myRender1);
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage2);
}

