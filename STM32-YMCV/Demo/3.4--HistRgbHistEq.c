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
    uint8* myData = CV_malloc1(sizeof(linglong00b));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, (void*)linglong00b, sizeof(linglong00b));
    CVIMAGE myImage1 = YMCV_Creat_DataToImg((uint8*)myData, 100, 87, CVRgb16Type);//彩色图1

		LCD_Clear(0);
    //直方图数组
    int32 myImage0Hist[3*256], myImage1Hist[3*256];

    /*--------------------------彩色图直方图均衡化--------------------------------------------------*/

    //获取图像直方图
    YMCV_Image_Hist_Get(myImage0, myImage0Hist, 3 * 256, PIXChannelNum3);
    //显示图片与直方图
    LCD_show_image(stx + 0, 0, myImage0);
    LCDHistShow(myImage0Hist, PIXChannelNum3, 150, 80, 60);

    //把图片0备份一份
    CVIMAGE myImage2 = YMCV_Creat_Img_BackUp(myImage0);
    //将myImage2的进行直方图均衡化操作
    YMCV_Image_Hist_Equalize(myImage2, myImage2);//输出覆盖自身

    //获取图像直方图与显示
    YMCV_Image_Hist_Get(myImage2, myImage1Hist, 3 * 256, PIXChannelNum3);
    LCD_show_image(stx + 0, myImage1->height, myImage2);
    LCDHistShow(myImage1Hist, PIXChannelNum3, 150, 220, 60);//由于大小放缩到60，经过转换形状可能不真实，可以用256观察真实直方图

    sprintf(myInform, "3 channel equalize indepen"); //3通道独立做均衡化（分离式均衡化）
    LCD_ShowString(0,  myImage1->height, myInform);
    //暂停300ms
    Delay(300);

    //将myImage0的进行直方图均衡化操作
    YMCV_Image_Hist_Equalize_UGray(myImage0, myImage2);

    //获取图像直方图与显示
    YMCV_Image_Hist_Get(myImage2, myImage1Hist, 3 * 256, PIXChannelNum3);
		LCD_show_image(stx + 0, myImage1->height, myImage2);
    LCDHistShow(myImage1Hist, PIXChannelNum3, 150, 220, 60);//由于大小放缩到60，经过转换形状可能不真实，可以用256观察真实直方图

    sprintf(myInform, "3 channel equalize by gray"); //3通道同时做均衡化（合并式均衡化）
    LCD_ShowString(0,  myImage1->height, myInform);
    //暂停1000ms
    Delay(1000);
		

    /*--------------------------彩色图直方图规定化--------------------------------------------------*/

    ////手动将直方图规定为某个形状
    //for (int i = 0; i < 256; i++)
    //{
    //    myImage1Hist[i] = i + 50;//线性函数，即直方图为梯形角形
    //}
    //从图像1获取直方图与显示
    YMCV_Image_Hist_Get(myImage1, myImage1Hist, 3*256, PIXChannelNum3);
		LCD_show_image(stx + 0, 0, myImage1);
    LCDHistShow(myImage1Hist, PIXChannelNum3, 150, 80, 60);

    sprintf(myInform, "target Hist");
    LCD_ShowString(stx + 0, 0, myInform);
    //暂停500ms
    Delay(500);

    //对图像0进行直方图规定化
    YMCV_Image_Hist_To(myImage0, myImage2, myImage1Hist, 3*256, PIXChannelNum3);
    //获取图像直方图与显示
    YMCV_Image_Hist_Get(myImage2, myImage1Hist, 3*256, PIXChannelNum3);
    LCD_show_image(stx + 0, myImage1->height, myImage2);
    LCDHistShow(myImage1Hist, PIXChannelNum3, 150, 220, 60);//由于大小放缩到60，经过转换形状可能不真实，可以用256观察真实直方图

    sprintf(myInform, "hist specification    ");
    LCD_ShowString(stx + 0, myImage1->height, myInform);
		
		//暂停1000ms
    Delay(1000);
		
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
}





