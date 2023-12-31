#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程展示了 条形码解码
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    char myInform[100];
    //信息显示
    sprintf(myInform, "BarCode Decode  ");
    LCD_ShowString(0, 200, myInform);
    /*----------------------------------- 图像旋转 ------------------------------------------*/
	  //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(bar_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, (void*)bar_gray, sizeof(bar_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVGrayType);//灰度图0
	
    //显示图片
    LCD_show_image(0, 0, myImage0_gray);

    // QR码识别
    CVVECTORS_U16 result = YMCV_Gray_Find_BarCode(myImage0_gray);

    //显示结果
    for (int i = 0; (result != NULL) && i < result->dataNum; i++)
    {
        CV_barcodes_data* tda = (CV_barcodes_data*)&result->datap[i * result->dim];

        color24_t rgbcv = { .G = 255 }; LCD_Point_Color_Set(rgbcv);
        //绘制矩形框
        LCD_Draw_Line(tda->corners[0].x, tda->corners[0].y, tda->corners[1].x, tda->corners[1].y);// 0 -> 1
        LCD_Draw_Line(tda->corners[1].x, tda->corners[1].y, tda->corners[2].x, tda->corners[2].y);// 1 -> 2
        LCD_Draw_Line(tda->corners[2].x, tda->corners[2].y, tda->corners[3].x, tda->corners[3].y);// 2 -> 3
        LCD_Draw_Line(tda->corners[3].x, tda->corners[3].y, tda->corners[0].x, tda->corners[0].y);// 3 -> 0

        // 0 -> 1 -> 2 -> 3 : 黑，灰（褐），浅红，红
        rgbcv = (color24_t){ .R = 0 }; LCD_Fill_ConstRgbRect(tda->corners[0].x - 2, tda->corners[0].y - 2, 5, 5, rgbcv);
        rgbcv = (color24_t){ .R = 100 }; LCD_Fill_ConstRgbRect(tda->corners[1].x - 2, tda->corners[1].y - 2, 5, 5, rgbcv);
        rgbcv = (color24_t){ .R = 180 }; LCD_Fill_ConstRgbRect(tda->corners[2].x - 2, tda->corners[2].y - 2, 5, 5, rgbcv);
        rgbcv = (color24_t){ .R = 255 }; LCD_Fill_ConstRgbRect(tda->corners[3].x - 2, tda->corners[3].y - 2, 5, 5, rgbcv);

        //绘制中心 
        rgbcv = (color24_t){ .G = 150 };
        int centerx = tda->rect.x + tda->rect.w / 2; //外接矩形框的中心
        int centery = tda->rect.y + tda->rect.h / 2;
        LCD_Fill_ConstRgbRect(centerx - 2, centery - 2, 5, 5, rgbcv);
        LCD_Draw_Rect(tda->rect.x, tda->rect.y, tda->rect.w, tda->rect.h, rgbcv);//外接矩形框

        // DM码的内容
        printf("QR码内容:%s\n长度为%d个字符（含结束符\\0） ", tda->payload, tda->payload_len);

        printf("\n类型：");
        //条码类型
        switch (tda->bartype)
        {
        case    CV_BarCode_UNKNOW://未知类型
            printf("未知");
            break;
        case    CV_BarCode_EAN2: printf("EAN2");
            break; 
        case CV_BarCode_EAN5: printf("EAN5");
            break;
        case    CV_BarCode_EAN8: printf("EAN8");
            break;
        case    CV_BarCode_UPCE: printf("UPCE");
            break;
        case    CV_BarCode_ISBN10: printf("ISBN10");
            break;
        case    CV_BarCode_UPCA: printf("UPCA");
            break;
        case    CV_BarCode_EAN13: printf("EAN13");
            break;
        case    CV_BarCode_ISBN13: printf("ISBN13");
            break;
        case    CV_BarCode_I25: printf("I25");
            break;
        case    CV_BarCode_DATABAR: printf("DATABAR");
            break;
        case    CV_BarCode_DATABAR_EXP: printf("DATABAR-EXP");
            break;
        case    CV_BarCode_CODABAR: printf("CODABAR");
            break;
        case    CV_BarCode_CODE39: printf("CODE39");
            break;
        case    CV_BarCode_PDF417: printf("PDF417");
            break;
        case    CV_BarCode_CODE93: printf("CODE93");
            break;
        case    CV_BarCode_CODE128: printf("CODE128");
            break;
        }
        //旋转角度
        printf("角度：%d", tda->rotation);
    }

    YMCV_Free_Vector_U16_Free(result);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_gray);

    Delay(300);
}


