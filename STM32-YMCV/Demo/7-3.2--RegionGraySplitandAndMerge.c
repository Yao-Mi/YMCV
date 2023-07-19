
/*******************************************************************************************
    *
    * 说明  该例程以 分裂 合并为例，展示了一副图像分别进行分裂与合并的处理效果
    *           事实上这两个经常是串联使用，比如先对图像进行区域分裂，然后对结果图进行区域合并，从而得到比较规整的图像
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
    LCD_show_image(stx + 0, 0, myImage0_gray);


    /*--------------------------灰度 区域分裂--------------------------------------------------*/
    for (int i = 0; i < 8; i++)
    {
        int thresh = i * 20;

        CVIMAGE myImage2 = YMCV_Gray_Region_Splitand(myImage0_gray, thresh);//分裂

        //显示图片2
        LCD_show_image(stx + 0, myImage0_gray->height, myImage2);

        //信息显示
			  sprintf(myInform, "RegionSplit thresh:%3d ",thresh);
				LCD_ShowString(stx + 0, myImage0_gray->height, myInform);
        //暂停300ms
        Delay(300);

        //释放内存
        YMCV_Free_ImgFree(myImage2);
    }
   

    /*--------------------------灰度 区域合并--------------------------------------------------*/
    for (int i = 0; i < 8; i++)
    {
        int thresh = i;

        CVIMAGE myImage2 = YMCV_Gray_Region_Merge(myImage0_gray, thresh);//合并

        //显示图片2
        LCD_show_image(stx + 0, myImage0_gray->height, myImage2);

        //信息显示
			  sprintf(myInform, "RegionMerge thresh:%3d ",thresh);
				LCD_ShowString(stx + 0, myImage0_gray->height, myInform);
        //暂停300ms
        Delay(300);

        //释放内存
        YMCV_Free_ImgFree(myImage2);
    }

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_gray);

}









