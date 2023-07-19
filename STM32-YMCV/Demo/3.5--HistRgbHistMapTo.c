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
    uint8* myData = CV_malloc1(sizeof(linglong00b));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, (void*)linglong00b, sizeof(linglong00b));
    CVIMAGE myImage1 = YMCV_Creat_DataToImg((uint8*)myData, 100, 87, CVRgb16Type);//彩色图1

	  LCD_Clear(0);
    //直方图数组，由于不是static，所以测试该demo要把启动文件中的栈大小调整到3000左右
    int32 myImage0Hist[3*256], myImage1Hist[3*256], myImage2Hist[3*256];

    /*--------------------------彩色图直方图重映射--------------------------------------------------*/

    //获取图像直方图
    YMCV_Image_Hist_Get(myImage0, myImage0Hist, 3*256, PIXChannelNum3);
    //显示图片与直方图
    LCD_show_image(stx + 0, 0, myImage0);
    LCDHistShow(myImage0Hist, PIXChannelNum3, 170, 60, 40);

    //获取图像直方图与显示
    YMCV_Image_Hist_Get(myImage1, myImage1Hist, 3*256, PIXChannelNum3);
    LCD_show_image(stx + 0, myImage0->height, myImage1);
    LCDHistShow(myImage1Hist, PIXChannelNum3, 170, 220, 40);
		
    sprintf(myInform, "target Hist");
    LCD_ShowString(stx + 0, myImage0->height, myInform);
    //暂停500ms
    Delay(500);

    //把图片0备份一份
    CVIMAGE myImage2 = YMCV_Creat_Img_BackUp(myImage0);
//    //将myImage0的直方图形状 规定为myImage1的形状
//    YMCV_Image_Hist_MapTo(myImage2, myImage1);

//    //获取图像直方图与显示
//    YMCV_Image_Hist_Get(myImage2, myImage2Hist, 3*256, PIXChannelNum3);
//    LCD_show_image(stx + 0, myImage0->height, myImage2);
//    LCDHistShow(myImage2Hist, PIXChannelNum3,170, 220, 40);

    //暂停500ms
    Delay(500);

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);

}





