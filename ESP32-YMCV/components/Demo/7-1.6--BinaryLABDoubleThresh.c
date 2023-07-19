
/*******************************************************************************************
    *
    * 说明  该例程以 LAB阈值分割为例，展示了LAB二值化效果
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];

		//显示图片0
		LCD_show_image(stx + 0, 0, myImage0);
    /*--------------------------彩色LAB双阈值二值化--------------------------------------------------*/
    //创建输出二值图像
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);//二值图
    //阈值参数初始化
  	CV_binary_param myParam = { .lab_double_t.thresh_Lmin = 0 ,.lab_double_t.thresh_Lmax = 100 ,
     .lab_double_t.thresh_Amin = -128 ,.lab_double_t.thresh_Amax = 127 ,
     .lab_double_t.thresh_Bmin = -128 ,.lab_double_t.thresh_Bmax = 127 };

    for (int i = 1; i < 10; i++)
    {
        //设置参数
        myParam.lab_double_t.thresh_Lmin = 6 * i;
        myParam.lab_double_t.thresh_Lmax = 100;

        myParam.lab_double_t.thresh_Amin = 14 * i - 128;
        myParam.lab_double_t.thresh_Amax = 127 - 12 * i;

        myParam.lab_double_t.thresh_Bmin = 14 * i - 128;
        myParam.lab_double_t.thresh_Bmax = 127 - 10 * i;

        //创建二值变换渲染器
        CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_LAB2Threshold);

        //加载参数进行渲染配置、初始化
        YMCV_BinaryKernel_Load(myRender1, &myParam);

        //将目标图像的进行渲染
        YMCV_Image_ThreshTo_Binary(myImage0, myImage2, myRender1);

        //显示图片2
        LCD_show_image(stx + 0, myImage0->height, myImage2);

        //信息显示
        sprintf(myInform, "LAB thresh:\nL[%d %d]\nA[%d %d]\nB[%d %d]",
            myParam.lab_double_t.thresh_Lmin,
            myParam.lab_double_t.thresh_Lmax,
            myParam.lab_double_t.thresh_Amin,
            myParam.lab_double_t.thresh_Amax,
            myParam.lab_double_t.thresh_Bmin,
            myParam.lab_double_t.thresh_Bmax);

        LCD_ShowString(0, myImage0->height, myInform);
        //暂停300ms
				Delay(300);

        //释放渲染器，避免下次用的时候导致内存泄漏
        YMCV_Free_Binary_RenderFree(myRender1);
    }

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage2);
}





