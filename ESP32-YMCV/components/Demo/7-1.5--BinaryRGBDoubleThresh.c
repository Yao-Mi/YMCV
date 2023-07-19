
/*******************************************************************************************
    *
    * 说明  该例程以 RGB双阈值为例，展示了RGB二值化效果
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];

		//显示图片0
		LCD_show_image(stx + 0, 0, myImage0);
    /*--------------------------RGB双阈值二值化--------------------------------------------------*/
    //创建输出二值图像
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);//二值图
    
	  //初始化阈值参数
	  CV_binary_param myParam = { .rgb_double_t.thresh_Rmin = 0 ,.rgb_double_t.thresh_Rmax = 255 , 
     .rgb_double_t.thresh_Gmin = 0 ,.rgb_double_t.thresh_Gmax = 255 ,
     .rgb_double_t.thresh_Bmin = 0 ,.rgb_double_t.thresh_Bmax = 255 };

    for (int i = 1; i < 10; i++)
    {
        //设置参数
        myParam.rgb_double_t.thresh_Rmin = 20 * i;
        myParam.rgb_double_t.thresh_Rmax = 255;

        myParam.rgb_double_t.thresh_Gmin = 10 * i;
        myParam.rgb_double_t.thresh_Gmax = 255;

        myParam.rgb_double_t.thresh_Bmin = 15 * i;
        myParam.rgb_double_t.thresh_Bmax = 255;

        //创建二值变换渲染器
        CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_RGB2Threshold);

        //加载参数进行渲染配置、初始化
        YMCV_BinaryKernel_Load(myRender1, &myParam);

        //将目标图像的进行渲染
        YMCV_Image_ThreshTo_Binary(myImage0, myImage2, myRender1);

        //显示图片2
        LCD_show_image(stx + 0, myImage0->height, myImage2);

        //信息显示
        sprintf(myInform, "RGB thresh:\nR[%3d %3d]\nG[%d %d]\nB[%d %d]",
            myParam.rgb_double_t.thresh_Rmin,
            myParam.rgb_double_t.thresh_Rmax,
            myParam.rgb_double_t.thresh_Gmin,
            myParam.rgb_double_t.thresh_Gmax,
            myParam.rgb_double_t.thresh_Bmin,
            myParam.rgb_double_t.thresh_Bmax);

        LCD_ShowString(0, myImage0->height, myInform);
        //暂停300ms
				Delay(300);

        //释放渲染器，避免下次用的时候导致内存泄漏
        YMCV_Free_Binary_RenderFree(myRender1);
    }

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage2);

}





