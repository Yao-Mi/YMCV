#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以 FishEye为例，展示了鱼眼模拟及矫正效果，当鱼眼图为正方形矫正效果才好
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(house2_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, (void*)house2_gray, sizeof(house2_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVGrayType);//灰度图0

    LCD_Clear(GRAY);
    //显示原图
    LCD_show_image(stx + 0, 0, myImage0_gray);
	  int sty = myImage0_gray->height;
    /*-------------------------- 鱼眼效果 --------------------------------------------------*/
   
    CVIMAGE feImg = YMCV_Gray_FishEye_Effect(myImage0_gray, myImage0_gray->width, myImage0_gray->height);
    LCD_show_image(stx + 0, sty, feImg);
    //信息显示
    sprintf(myInform, "FishEye Effect ");
    LCD_ShowString(stx + 0, sty, myInform);
    Delay(300);
    /*-------------------------- 透镜失真矫正 --------------------------------------------------*/
    YMCV_PADDING_VAL = 50;//空白处填充值
    CVIMAGE invfeImg = YMCV_Gray_invFishEye_Effect(feImg,4, 0.43, 0.85);
    LCD_show_image(stx + 0, sty, invfeImg);
    //信息显示
    sprintf(myInform, "inverse FishEye Effect ");
    LCD_ShowString(stx + 0, sty, myInform);
    Delay(300);

    //释放内存
    YMCV_Free_ImgFree(feImg);
    YMCV_Free_ImgFree(invfeImg);
    YMCV_Free_ImgFree(myImage0_gray);
}

