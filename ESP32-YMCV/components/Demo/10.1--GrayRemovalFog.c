#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程 暗通道去雾，实现 除雾效果
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = (uint8*)CV_malloc1(sizeof(wutu2_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, (void*)wutu2_gray, sizeof(wutu2_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg(myData, 160, 120, CVGrayType);//灰度图
    //显示原图
    LCD_show_image(stx + 0, 0, myImage0_gray);
	  int sty = myImage0_gray->height;

    //创建空间渲染器
    uint8 kernel_r = 1;
    YMCV_PADDING_VAL = 125;//设置填充值

    //由于缓存结果
    CVIMAGE myImage1 = NULL, myImage2 = NULL;

    //获得暗通道图
    myImage1 = YMCV_Gray_Image_GetDarkChannel(myImage0_gray, kernel_r);
    //计算大气光照A
    uint8 AtmosValue = YMCV_Gray_Image_GetAtmosphericlighting(myImage0_gray, myImage1);

    /*--------------------------暗通道去雾 修改滤波半径--------------------------------------------------*/
    for (float32 k = 0.1; k < 1.0; k+=0.1)
    {
        //设置出雾率 k，得到去雾图像
        myImage2 = YMCV_Gray_Image_RemovalFog(myImage0_gray, myImage1, AtmosValue,k);
        //如果可以的话，再使用通道渲染器拉伸一下图像整体的亮度，效果会比较好
        
        //这里显示图片2
        LCD_show_image(stx + 0, sty + 0, myImage2);

        //信息显示
        sprintf(myInform, "fog remove: %.2f  ", k);
        LCD_ShowString(stx + 0, sty + 0, myInform);
        //暂停
        Delay(300);

        //用释放图像，避免下次用的时候导致内存泄漏
        YMCV_Free_ImgFree(myImage2);
    }

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage1);
}








