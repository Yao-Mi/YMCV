/*******************************************************************************************
    *
    * 说明  该例程展示了 图片创建，以及图片的截取备份
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(50,50, CVRgb16Type);

    //截取灵笼图片中一个区域拷贝出来，转为一幅新的图像
    CVIMAGE myImage2 = YMCV_Creat_ImgCopyTo(myImage0, 30, 20, 70, 70);
    //采用ROI的方式截取
    CVrect myroi = { .x = 50,.y = 50,.w = 40,.h = 40 };
    CVIMAGE myImage3 = YMCV_Creat_ImgROICopyTo(myImage0, &myroi);


    //显示图片1
    LCD_show_image(10, 60, myImage1);

    //显示图片2
    LCD_show_image(myImage1->width + 20, 60, myImage2);

    //显示图片3
    LCD_show_image(myImage1->width + myImage2->width + 30, 60, myImage3);


    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);
}





