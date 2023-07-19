#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以 直方图均衡化、直方图规定化为例，展示了直方图的使用及显示
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
    CVIMAGE myImage01 = YMCV_Creat_DataToImg((uint8*)myData, 100, 87, CVGrayType);//灰度图0
	
    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage1, CV_Rgb16ToGray);//rgb转灰度图
	
    //直方图数组
    int32 myImage1Hist[256], myImage2Hist[256];

    /*--------------------------灰度图直方图均衡化--------------------------------------------------*/

    //获取图像直方图
    YMCV_Image_Hist_Get(myImage1, myImage1Hist, 256, PIXChannelNum1);
    //显示image1图片与直方图
    LCD_show_image(stx + 0, 0, myImage1);
    LCDHistShow(myImage1Hist, PIXChannelNum1, 170, 60, 40);


    //把image1备份一份
    CVIMAGE myImage2 = YMCV_Creat_Img_BackUp(myImage1);
    //将myImage2的进行直方图均衡化操作
    YMCV_Image_Hist_Equalize(myImage2, myImage2);//输出覆盖自身

    //获取图像直方图与显示
    YMCV_Image_Hist_Get(myImage2, myImage2Hist, 256, PIXChannelNum1);
    LCD_show_image(stx + 0, myImage1->height, myImage2);
    LCDHistShow(myImage2Hist, PIXChannelNum1, 170, 220, 40);//由于大小放缩到40，经过转换形状可能不真实，可以用256观察真实直方图

    sprintf(myInform, "1 channel equalize"); //3通道同时做均衡化（合并式均衡化）
    LCD_ShowString(stx + 0,  myImage1->height, myInform);
    //暂停1000ms
    Delay(1000);


    /*--------------------------灰度图直方图规定化--------------------------------------------------*/

    ////手动将直方图规定为某个形状
    //for (int i = 0; i < 256; i++)
    //{
    //    myImage1Hist[i] = i + 50;//线性函数，即直方图为梯形角形
    //}
    //从图像01获取直方图与显示
    YMCV_Image_Hist_Get(myImage01, myImage2Hist, 256, PIXChannelNum1);
    LCD_show_image(stx + 0, 0, myImage01);
    LCDHistShow(myImage2Hist, PIXChannelNum1, 170, 60, 40);
		
    sprintf(myInform, "target Hist");
    LCD_ShowString(stx + 0, 0, myInform);
    //暂停500ms
    Delay(500);

    //对图像1进行直方图规定化
    YMCV_Image_Hist_To(myImage1, myImage2, myImage2Hist, 256, PIXChannelNum1);
    //获取图像直方图与显示
    YMCV_Image_Hist_Get(myImage2, myImage2Hist, 256, PIXChannelNum1);
    LCD_show_image(stx + 0, myImage1->height, myImage2);
    LCDHistShow(myImage2Hist, PIXChannelNum1, 170, 220, 40);//由于大小放缩到40，经过转换形状可能不真实，可以用256观察真实直方图
    
		sprintf(myInform, "hist specification");
    LCD_ShowString(stx + 0, myImage1->height, myInform);
    //暂停1000ms
    Delay(1000);
		
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage01);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);

}



