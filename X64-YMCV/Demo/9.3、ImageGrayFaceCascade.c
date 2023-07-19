#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_MatchingCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 Harr CasCade为例，展示人脸检测结果
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(740, 480);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(luojiuchuan_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, luojiuchuan_gray, sizeof(luojiuchuan_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 740, 400, CVGrayType);//灰度图

    /*-------------------------- CasCade 人脸检测 --------------------------------------------------*/
    CV_cascade facedect;
    YMCV_Load_HaarCascade(&facedect, "frontface");//加载 正面(朝前) 人脸检测器

    CVVECTORS_U16 result = YMCV_HaarCascade_Detect_Objects(myImage0, &facedect, 1.0f);

    //显示图片
    LCD_show_image(0, 0, myImage0);
    //绘制人脸框
    for (int i = 0; (result != NULL) && (i < result->dataNum); i++)
    {
        CVrect* recp = &result->datap[i * result->dim];
        //绘制矩形框
        color24_t rgbval = { .R = 255 };
        LCD_Draw_Rect(recp->x, recp->y, recp->w, recp->h, rgbval);
    }
    //信息显示
    sprintf(myInform, " Haar Cascade 检测 正面人脸 ");
    LCD_ShowString(200, 420, myInform);
    
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_Vector_U16_Free(result);
    PAUSE();
    LCD_Destory();
    return 0;
}





