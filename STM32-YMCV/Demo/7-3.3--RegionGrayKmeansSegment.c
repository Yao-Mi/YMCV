
/*******************************************************************************************
    *
    * 说明  该例程以 Kmeans聚类为例，展示了图像的聚类效果
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];
	
    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage0_gray = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_gray, CV_Rgb16ToGray);//rgb转灰度图
		
    /*-------------------------- KMeans 均值聚类 --------------------------------------------------*/
    int classnum = 4;//设置聚类数目
    int maxItera=100;//最多迭代100次
    CVIMAGE myImage2 = YMCV_Gray_KMeans_Clustering(myImage0_gray, classnum, maxItera);

		//显示图片0
		LCD_show_image(stx + 0, 0, myImage0_gray);
		//显示图片2
		LCD_show_image(stx + 0, myImage0_gray->height, myImage2);

		//信息显示
	  sprintf(myInform, "Kmeans classNum:%3d\nmax iterTimes:%3d",classnum,maxItera);
		LCD_ShowString(0, myImage0_gray->height, myInform);
		//暂停300ms
		Delay(300);
		
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage2);
}









