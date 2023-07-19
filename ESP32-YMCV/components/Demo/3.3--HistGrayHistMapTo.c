#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以 直方图映射为例，展示了将一幅图像的直方图赋给另一幅图使用的方法、直方图显示
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
	
    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(linglong00b_gray));//必须采用CV_malloc1为图片数据申请内存
	  if(myData == NULL) printf("cvmalloc1内存不足");
    CV_memcpy(myData, (void*)linglong00b_gray, sizeof(linglong00b_gray));
    CVIMAGE myImage2 = YMCV_Creat_DataToImg((uint8*)myData, 100, 87, CVGrayType);//灰度图0
	
    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage1, CV_Rgb16ToGray);//rgb转灰度图
	
		LCD_Clear(0);
    //直方图数组
    int32 myImage1Hist[256], myImage2Hist[256], myImage3Hist[256];

    /*--------------------------灰度图直方图映射--------------------------------------------------*/

    //获取图像直方图
    YMCV_Image_Hist_Get(myImage1, myImage1Hist, 256, PIXChannelNum1);
    //显示image1图片与直方图
    LCD_show_image(stx + 0, 0, myImage1);
    LCDHistShow(myImage1Hist, PIXChannelNum1, 170, 60, 40);

    //获取image2直方图与显示
    YMCV_Image_Hist_Get(myImage2, myImage2Hist, 256, PIXChannelNum1);
    LCD_show_image(stx + 0,myImage1->height, myImage2);
    LCDHistShow(myImage2Hist, PIXChannelNum1, 170, 220, 40);
		
    sprintf(myInform, "target Hist");
    LCD_ShowString(stx + 0, myImage1->height, myInform);
    //暂停500ms
    Delay(500);

    //把图片1备份一份
    CVIMAGE myImage3 = YMCV_Creat_Img_BackUp(myImage1);
    //将myImage2的直方图形状 规定为myImage3的形状
    YMCV_Image_Hist_MapTo(myImage3, myImage2);//image3的直方图重映射为image2直方图形状

    //获取图像直方图与显示
    YMCV_Image_Hist_Get(myImage3, myImage3Hist, 256, PIXChannelNum1);
    LCD_show_image(stx + 0,myImage1->height, myImage3);
    LCDHistShow(myImage3Hist, PIXChannelNum1, 170, 220, 40);

    //暂停500ms
    Delay(500);
		
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);
}





