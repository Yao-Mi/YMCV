#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以 图像刚性变形为例，展示了在控制点作用下的变形效果
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 30;
    char myInform[100];
    int i = 0, j = 0;

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(huochairen_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, (void*)huochairen_gray, sizeof(huochairen_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 181, 240, CVGrayType);//灰度图0

    //显示原图
    LCD_show_image(stx + 0, 0, myImage0_gray);
        // 确定控制点
    color24_t thiscolor = { .R = 255,.G = 0 };

    uint16 xpoint[] = {36, 61, 150, 128, 47, 70, 140, 114, /*91,*/ 91, 92};
    uint16 ypoint[] = {126, 82, 128, 86, 232, 176, 231, 174, /*7,*/ 57, 126};
    uint16 xpoint2[] = { 11, 66, 141, 136, 69, 65, 172, 136, /*72,*/ 104, 93 };
    uint16 ypoint2[] = { 98, 94, 131, 82, 228, 170, 200, 156, /*10,*/ 56, 126 };
    uint16 num = sizeof(xpoint) / 2;
    //用鼠标绘制控制点请参考PC版本的Demo

    CVVECTORS_F32 pList = YMCV_Creat_Vector_F32(sizeof(CVfpoint) / sizeof(float32), num);
    CVVECTORS_F32 qList = YMCV_Creat_Vector_F32(sizeof(CVfpoint) / sizeof(float32), num);
    for (i = 0; i < num; i++)
    {
        //P
        CVfpoint* fpthis = (CVfpoint*)(pList->datap + i * pList->dim);
        fpthis->x = xpoint[i];
        fpthis->y = ypoint[i]; 
        LCD_Fill_ConstRgbRect(stx + fpthis->x - 1, fpthis->y - 1, 3, 3, thiscolor);
        
        //Q
        fpthis = (CVfpoint*)(qList->datap + i * qList->dim);
        fpthis->x = xpoint2[i];
        fpthis->y = ypoint2[i];
    }
    YMCV_PADDING_VAL = 0;//空白区填充0

    //刚性变换：P是原始控制点，Q是变换后的控制点
    CVIMAGE myImage1 = YMCV_Gray_RigidDeformation_Effect(myImage0_gray, pList, qList);
    LCD_show_image(stx + 0, 0, myImage1);

    //从原图上绘制
    for (i = 0; i < num; i++)
    {
        //Q控制点
        CVfpoint* fpthis = (CVfpoint*)(qList->datap + i * qList->dim);
			
        LCD_Fill_ConstRgbRect(stx+fpthis->x - 1, fpthis->y - 1, 3, 3, thiscolor);
    }
    Delay(300);
		
		//释放内存
    YMCV_Free_Vector_F32_Free(pList);
    YMCV_Free_Vector_F32_Free(qList);
		//释放内存
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage0_gray);
}
