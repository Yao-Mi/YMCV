/*******************************************************************************************
    *
    * 说明  该例程展示了 在不破坏原始的情况下，将数据封装为图片，以及图片的备份，与图片显示
    * 
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    //重新拷贝一份该图
    CVIMAGE myImage1 = YMCV_Creat_Img_BackUp(myImage0);
    //显示拷贝图片
    LCD_show_image(10,60, myImage1);
    
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage1);
}





