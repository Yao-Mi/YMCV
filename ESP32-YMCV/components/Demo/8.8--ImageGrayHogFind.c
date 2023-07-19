#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以 灰度图为例，展示了HOG描述符提取效果
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];
	
    color24_t pencolor = { .R = 255 };
    LCD_Point_Color_Set(pencolor);//设置画笔颜色

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(mm02_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, (void*)mm02_gray, sizeof(mm02_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVGrayType);//灰度图0

    /*-------------------------HOG特征检测--------------------------------------------------*/
    CVVECTORS_F32 hogDescribe= YMCV_Gray_Find_Hog_Describe(myImage0_gray);

    LCD_show_image(stx + 0, 0, myImage0_gray);//显示图片0
		
		//清屏
		LCD_Fill_ConstRgbRect(stx + 0, myImage0_gray->height,myImage0_gray->width,myImage0_gray->height,(color24_t){0,0,0});
		
		static uint8 showMode = 1;
		//showMode = (showMode)?0:1;//切换显示模式
    LCDHogHistShow(hogDescribe, stx + 0, myImage0_gray->height,showMode);//显示HOG描述符

    //信息显示
    sprintf(myInform, " HOG Hist ");
    LCD_ShowString(stx + 0, myImage0_gray->height-16, myInform);

    //暂停800ms
    Delay(800);
						
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_Vector_F32_Free(hogDescribe);//特征内存释放

}





