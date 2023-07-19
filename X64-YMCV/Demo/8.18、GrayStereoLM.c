#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_FeatCal.h"
#include "../YMCV/CORE/YMCV_EdgeCal.h"
#include "../YMCV/CORE/YMCV_FormCge.h"
#include "../YMCV/CORE/YMCV_PubData.h"
#include<time.h>
/*******************************************************************************************
    *
    * 说明  该例程以 立体匹配LM算法为例，展示了视差图计算效果
    *
    ***************************************************************************************/
CVIMAGE YMCV_computerSAD(CVIMAGE left, CVIMAGE right);
int main()
{
    char myInform[100];
    LCD_Init(1300, 600);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(sleft_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, sleft_gray, sizeof(sleft_gray));
    CVIMAGE myImageL = YMCV_Creat_DataToImg((uint8*)myData, 425, 425, CVGrayType);//灰度图0

    myData = CV_malloc1(sizeof(sright_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, sright_gray, sizeof(sright_gray));
    CVIMAGE myImageR = YMCV_Creat_DataToImg((uint8*)myData, 425, 425, CVGrayType);//灰度图0
     
    /*-------------------------- 视差图计算--------------------------------------------------*/

    LCD_show_image(0, 0, myImageL);//原图显示在左边
    LCD_show_image(myImageL->width, 0, myImageR);//原图显示在右边

    CVIMAGE dImg = YMCV_Gray_Stereo_LineMatching(myImageL, myImageR, 20, 5);//进行视差计算

    //CVPIX_RENDER myRender2 = YMCV_Creat_PixRender_Creat(CV_Channel_SingleToThree, PIXChannelNum3);//单通道转三通道
    //CVIMAGE myImage3 = YMCV_Creat_Img_Creat(dImg->width, dImg->height, CVRgb16Type);
    ////使用热力图 调色板进行渲染器通道配置、初始化
    //YMCV_ThreeChannel_PaletteMapTo(myRender2, &YMCV_HeatPalette);
    ////使用通道渲染器进行渲染图片
    //YMCV_Image_SingleToThree_ChannelRenderTo(dImg, myImage3, myRender2);
   
    LCD_show_image(2 * myImageL->width, 0, dImg);//处理后显示在右边

        //信息显示
    sprintf(myInform, "  视差计算    ");
    LCD_ShowString(100, 500, myInform);
    PAUSE();

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImageL);
    YMCV_Free_ImgFree(myImageR);
    //YMCV_Free_ImgFree(myImage3);
    YMCV_Free_ImgFree(dImg);

    PAUSE();
    LCD_Destory();
    return 0;
}





