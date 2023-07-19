#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以 人脸灰度图为例，展示了LBP直方图描述符提取效果
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];
	
    color24_t pencolor = { .R = 255 };
    LCD_Point_Color_Set(pencolor);//设置画笔颜色

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(luojiuchuan_112x112_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, (void*)luojiuchuan_112x112_gray, sizeof(luojiuchuan_112x112_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 112, 112, CVGrayType);//灰度图0

    /*-------------------------人脸LBP直方图特征检测--------------------------------------------------*/

    //输入人脸图片限定最大为112×112，支持orl人脸数据集中92×112的大小
    CV_FACE_LBP_DESC lbpDescribe = YMCV_Face_UniformLBP_Describe(myImage0_gray);

    
    LCD_show_image(stx + 0, 0, myImage0_gray);//显示图片0
		
    LCDLbpHistShow(lbpDescribe, stx + 0, myImage0_gray->height);//显示LBP描述符

    //信息显示
    sprintf(myInform, " LBP Hist ");
    LCD_ShowString(stx + 0, myImage0_gray->height, myInform);
    
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_FaceLbp_Hist_free(lbpDescribe);//特征内存释放

    Delay(800);
}





