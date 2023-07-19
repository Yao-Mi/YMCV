#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_EdgeCal.h"

#include"../YMCV/CORE/YMCV_SegnmCal.h"

/*******************************************************************************************
    *
    * 说明  该例程以 分裂 合并为例，展示了一副图像分别进行分裂与合并的处理效果
    *           事实上这两个经常是串联使用，比如先对图像进行区域分裂，然后对结果图进行区域合并，从而得到比较规整的图像
    *
    ***************************************************************************************/

int main()
{
    char myInform[100];
    LCD_Init(1000, 450);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(mm00_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, mm00_gray, sizeof(mm00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//灰度图0

    LCD_Clear(0);
    //显示图片0
    LCD_show_image(0, 0, myImage0);


    /*--------------------------灰度 区域分裂--------------------------------------------------*/
    for (int i = 0; i < 8; i++)
    {
        int thresh = i * 20;

        CVIMAGE myImage2 = YMCV_Gray_Region_Splitand(myImage0, thresh);//分裂

        //显示图片2
        LCD_show_image(500, 0, myImage2);

        //信息显示
        sprintf(myInform, "区域分裂阈值：%d ",thresh);
        LCD_ShowString(100, 310, myInform);
        //暂停
        PAUSE();

        //释放内存
        YMCV_Free_ImgFree(myImage2);
    }
   

    /*--------------------------灰度 区域合并--------------------------------------------------*/
    for (int i = 0; i < 8; i++)
    {
        int thresh = i;

        CVIMAGE myImage2 = YMCV_Gray_Region_Merge(myImage0, thresh);//合并

        //显示图片2
        LCD_show_image(500, 0, myImage2);

        //信息显示
        sprintf(myInform, "区域合并阈值：%d     ", thresh);
        LCD_ShowString(100, 310, myInform);
        //暂停
        PAUSE();

        //释放内存
        YMCV_Free_ImgFree(myImage2);
    }


    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);

    PAUSE();
    LCD_Destory();
    return 0;
}









