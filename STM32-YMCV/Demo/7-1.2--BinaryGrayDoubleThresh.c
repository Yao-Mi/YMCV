
/*******************************************************************************************
    *
    * 说明  该例程以 灰度双阈值分割为例，展示了二值化的效果
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];
	  
    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage0_gray = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_gray, CV_Rgb16ToGray);//rgb转灰度图

		//显示图片0
    LCD_Clear(0);
		LCD_show_image(stx + 0, 0, myImage0_gray);
		//暂停500ms
		Delay(500);

    /*--------------------------灰度双阈值二值化--------------------------------------------------*/
    //创建输出二值图像
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0_gray->width, myImage0_gray->height, CVBinaryType);//二值图
    CV_binary_param myParam = { .gray_double_t.thresh_min = 0 ,.gray_double_t.thresh_max = 255};

    for (int i = 1; i < 10; i++)
    {

        //设置参数
        myParam.gray_double_t.thresh_min = 10 * i;
        myParam.gray_double_t.thresh_max = 255- 2 * i;


        //创建二值变换渲染器
        CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_DoubleThreshold);

        //加载参数进行渲染配置、初始化
        YMCV_BinaryKernel_Load(myRender1, &myParam);

        //将目标图像的进行渲染
        YMCV_Image_ThreshTo_Binary(myImage0_gray, myImage2, myRender1);

        //显示图片2
        LCD_show_image(stx + 0, myImage0_gray->height, myImage2);

        //信息显示
        sprintf(myInform, "binary thresh: [%3d %3d]", myParam.gray_double_t.thresh_min,
                                                     myParam.gray_double_t.thresh_max);
        LCD_ShowString(stx + 0, myImage0_gray->height - 16, myInform);
        //暂停300ms
		    Delay(300);

        //释放渲染器，避免下次用的时候导致内存泄漏
        YMCV_Free_Binary_RenderFree(myRender1);

    }

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage2);
}





