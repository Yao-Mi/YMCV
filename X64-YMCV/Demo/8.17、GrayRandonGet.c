#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_FeatCal.h"
#include "../YMCV/CORE/YMCV_EdgeCal.h"
#include "../YMCV/CORE/YMCV_FormCge.h"
/*******************************************************************************************
    *
    * 说明  该例程以 拉东变换为例，展示了拉东变换效果及其逆变换
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(640, 600);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(ctcut00_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, ctcut00_gray, sizeof(ctcut00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 250, CVGrayType);//灰度图0

    /*-------------------------- RADON 变换计算--------------------------------------------------*/
    LCD_show_image(0, 0, myImage0);//原图显示在左边
    uint8 theta_shift_step = 0; //角度分辨率为(2 ^ 0)°

    CVIMAGE radonImg = YMCV_Gray_Radon_Convert(myImage0, theta_shift_step, 0);//进行拉东变换

    //显示霍夫空间
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(radonImg->width, radonImg->height, CVGrayType);
    YMCV_Image_Pix_Change_Form(radonImg, myImage2, CV_IGrayToGray2);
    LCD_show_image(300, 0, myImage2);//处理后显示在右边

        //信息显示
    sprintf(myInform, "  拉东变换    ");
    LCD_ShowString(100, 300, myInform);
    PAUSE();
    /*-------------------------- RADON 逆变换计算--------------------------------------------------*/
    CVIMAGE irimg = YMCV_Gray_Radon_IConvert(radonImg, theta_shift_step);

    CVIMAGE myImage3 = YMCV_Creat_Img_Creat(irimg->width, irimg->height, CVGrayType);
    YMCV_Image_Pix_Change_Form(irimg, myImage3, CV_IGrayToGray2);
    LCD_show_image(0, 0, myImage3);//处理后显示在右边

        //信息显示
    sprintf(myInform, "  拉东反变换    ");
    LCD_ShowString(100, 300, myInform);

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);
    YMCV_Free_ImgFree(radonImg);

    PAUSE();
    LCD_Destory();
    return 0;
}





