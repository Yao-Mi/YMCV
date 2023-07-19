#include "../SDL_LCD/SDL_LCD.h"
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
    * 说明  该例程以 二值图霍夫变换为例，展示了霍夫空间的效果
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(640, 600);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(tuxin_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, tuxin_gray, sizeof(tuxin_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//灰度图0


        /*--------------------------灰度Sobel边缘检测--------------------------------------------------*/
    //创建输出二值图像
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);//二值图
    CV_edge_param myParam = { .EdgeType = CV_Sobel ,.Thresh.sobel.minT = 100 };
    YMCV_Gray_EdgeDetect(myImage0, myImage1, &myParam);

    //显示图片
    LCD_show_image(0, 0, myImage0);
    LCD_show_image(300, 0, myImage1);

    //信息显示
    sprintf(myInform, " 边缘检测");
    LCD_ShowString(100, 300, myInform);
    //暂停
    PAUSE();

    /*-------------------------- hough 空间计算--------------------------------------------------*/
    LCD_show_image(0, 0, myImage1);//原图显示在左边

    CVIMAGE houghImg = YMCV_Binary_To_Hough(myImage1, 0);//转为hough空间 角度分辨率为 (2^0)°

    //显示霍夫空间
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(houghImg->width, houghImg->height, CVGrayType);
    YMCV_Image_Pix_Change_Form(houghImg, myImage2, CV_IGrayToGray);
    LCD_show_image(300, 0, myImage2);//处理后显示在右边

        //信息显示
    sprintf(myInform, "  hough空间    ");
    LCD_ShowString(100, 300, myInform);


    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(houghImg);

    PAUSE();
    LCD_Destory();
    return 0;
}





