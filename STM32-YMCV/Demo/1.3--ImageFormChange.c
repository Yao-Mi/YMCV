/*******************************************************************************************
	*
	* 说明  该例程展示了 图片创建，以及图片的格式转换
	*
	***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{

	//新图片创建，内容为空(默认不进行初始化，内容为随机值)
	CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
	CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);
	
	YMCV_Image_Pix_Change_Form(myImage0, myImage1, CV_Rgb16ToGray);//rgb转灰度图
	YMCV_Image_Pix_Change_Form(myImage1, myImage2, CV_GrayToDotBinary);//灰度图转二值点图
	
	//显示图片
	LCD_show_image(40, 0, myImage1);
	LCD_show_image(40, myImage1->height, myImage2);

	//使用完图片，要释放图片内存
	YMCV_Free_ImgFree(myImage1);
	YMCV_Free_ImgFree(myImage2);
}



