#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以 Harr CasCade为例，展示人脸检测结果
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(luojiuchuan2_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, (void*)luojiuchuan2_gray, sizeof(luojiuchuan2_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVGrayType);//灰度图

    /*-------------------------- CasCade 人脸检测 --------------------------------------------------*/
    CV_cascade facedect;
    YMCV_Load_HaarCascade(&facedect, "frontface");//加载 正面(朝前) 人脸检测器

    CVVECTORS_U16 result = YMCV_HaarCascade_Detect_Objects(myImage0_gray, &facedect, 0.2f);//保留率越低速度越快

    //显示图片
    LCD_show_image(stx + 0, 16, myImage0_gray);
    //绘制人脸框
    for (int i = 0; (result != NULL) && (i < result->dataNum); i++)
    {
        CVrect* recp = (CVrect*)&result->datap[i * result->dim];
        //绘制矩形框
        color24_t rgbval = { .R = 255 };
        LCD_Draw_Rect(stx + recp->x, 16+recp->y, recp->w, recp->h, rgbval);
    }
    //信息显示
    sprintf(myInform, " Haar Cascade front face ");
    LCD_ShowString(0, 0, myInform);
    //暂停
    Delay(500);
		
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_Vector_U16_Free(result);

}





