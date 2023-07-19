
/*******************************************************************************************
    *
    * 说明  该例程以 二值图形态学滤波为例，展示了二值图像 的膨胀和腐蚀效果
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage0_gb = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_gb, CV_Rgb16ToGray);//rgb转灰度图
    //快速二值化
	  myImage0_gb->type = CVBinaryType;
    for (int i = 0,imax = myImage0_gb->width*myImage0_gb->height ; i < imax; i++)
    {
        myImage0_gb->data[i] = (myImage0_gb->data[i] > 100) ? 255 : 0;
    }
    
    //显示原图
    LCD_show_image(stx + 0, 0, myImage0_gb);


    //创建空间渲染器
    uint8 kernel_d = 0;
    CVFMAT myKernel;

    //由于缓存结果
    CVIMAGE myImage1 = NULL;

    /*--------------------------形态学膨胀滤波 修改滤波半径--------------------------------------------------*/
    for (kernel_d = 1; kernel_d < 10; kernel_d +=2)
    {
        //创建相应半径的形态学滤波核
        myKernel = YMCV_Creat_FMAT_Creat(kernel_d, kernel_d);

        //手动设置击中核结构
        CV_memset(myKernel->datap,0, myKernel->row * myKernel->col*sizeof(float));//全部初始为0
        //选择需要击中的部分记为1
        for (int i = 0; i < myKernel->row; i++)
        {
            for (int j = 0; j < myKernel->col; j++)
                myKernel->datap[i * myKernel->col + j] = 1.0f;
        }
        //计算形态学击中效果
        myImage1 = YMCV_Binary_Morph_Hit_Cal(myImage0_gb, myKernel,1);//输出击中率>1% 的结果，膨胀效果
       
        //显示图片1
        LCD_show_image(stx + 0,  myImage0_gb->height, myImage1);

        //信息显示
        sprintf(myInform, "morp Dilation radius: %02d   . ", kernel_d);
        LCD_ShowString(stx + 0,  myImage0_gb->height, myInform);
        //暂停300ms
		    Delay(300);

        //用释放图像，避免下次用的时候导致内存泄漏
        YMCV_Free_ImgFree(myImage1);
        //释放渲染核
        YMCV_Free_FMatFree(myKernel);
    }

    ///*--------------------------形态学腐蚀滤波 修改滤波半径--------------------------------------------------*/
    for (kernel_d = 1; kernel_d < 10; kernel_d +=2)
    {
        //创建相应半径的形态学滤波核
        myKernel = YMCV_Creat_FMAT_Creat(kernel_d, kernel_d);

        //手动设置击中核结构
        CV_memset(myKernel->datap, 0, myKernel->row * myKernel->col * sizeof(float));//全部初始为0
        //选择需要击中的部分记为1
        for (int i = 0; i < myKernel->row; i++)
        {
            for (int j = 0; j < myKernel->col; j++)
            {
                myKernel->datap[i * myKernel->col + j] = 1.0f;
            }
        }

        //计算形态学击中结果
        myImage1 = YMCV_Binary_Morph_Hit_Cal(myImage0_gb, myKernel,99);//输出击中率>99%的部分 ，腐蚀效果

        //显示图片1
        LCD_show_image(stx + 0,  myImage0_gb->height, myImage1);

        //信息显示
        sprintf(myInform, "morp Erosion radius: %02d   . ", kernel_d);
        LCD_ShowString(stx + 0,  myImage0_gb->height, myInform);
        //暂停300ms
		    Delay(300);

        //用释放图像，避免下次用的时候导致内存泄漏
        YMCV_Free_ImgFree(myImage1);
        //释放渲染核
        YMCV_Free_FMatFree(myKernel);
    }
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_gb);
}





