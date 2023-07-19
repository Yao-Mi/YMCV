#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以 拉东变换为例，展示了拉东变换效果及其逆变换
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(ctcut01_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, (void*)ctcut01_gray, sizeof(ctcut01_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 112, 112, CVGrayType);//灰度图0
	
    LCD_show_image(stx + 0, 0, myImage0_gray);//原图显示
	  int sty = myImage0_gray->height;
    /*-------------------------- RADON 变换计算--------------------------------------------------*/
    uint8 theta_shift_step = 0; //角度分辨率为(2 ^ 0)°

    CVIMAGE radonImg = YMCV_Gray_Radon_Convert(myImage0_gray, theta_shift_step, 0);//进行拉东变换

    //显示霍夫空间
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(radonImg->width, radonImg->height, CVGrayType);
    YMCV_Image_Pix_Change_Form(radonImg, myImage2, CV_IGrayToGray2);
    LCD_show_image(stx + 0, sty + 0, myImage2);//处理后显示在右边

        //信息显示
    sprintf(myInform, "radon Transform");
    LCD_ShowString(stx + 0, sty + 0, myInform);
    Delay(500);
    /*-------------------------- RADON 逆变换计算--------------------------------------------------*/
    CVIMAGE irimg = YMCV_Gray_Radon_IConvert(radonImg, theta_shift_step);

    CVIMAGE myImage3 = YMCV_Creat_Img_Creat(irimg->width, irimg->height, CVGrayType);
    YMCV_Image_Pix_Change_Form(irimg, myImage3, CV_IGrayToGray2);
    LCD_show_image(stx + 0, sty + 0, myImage3);//处理后显示在右边

        //信息显示
    sprintf(myInform, "radon invers   ");
    LCD_ShowString(stx + 0, sty + 0, myInform);

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);
    YMCV_Free_ImgFree(irimg);
    YMCV_Free_ImgFree(radonImg);

    Delay(500);
}





