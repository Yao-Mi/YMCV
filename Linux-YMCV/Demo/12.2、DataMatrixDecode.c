#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/IOFILE/YMCV_IOFile.h"
#include"../YMCV/QRCODE/YMCV_Codes.h"
/*******************************************************************************************
    *
    * 说明  该例程展示了 DM码解码
    *
    ***************************************************************************************/
int main()
{
    LCD_Init(1000, 480);
    char myInform[100];
    //信息显示
    sprintf(myInform, "DataMatrix二维码解码  ");
    LCD_ShowString(400, 410, myInform);
    /*----------------------------------- 图像旋转 ------------------------------------------*/

    CVIMAGE myImage0 = YMCV_Image_Read("../../图片素材/DataMatrix/ymcv-test.jpg", CVGrayType);

    CV_geom_param myParam = { .rotate.Imgin_w = myImage0->width,
                            .rotate.Imgin_h = myImage0->height,
                            .rotate.angle = -20,//20度
                            .rotate.out_w = myImage0->width ,//固定窗口需要设置输出大小
                            .rotate.out_h = myImage0->height };
    YMCV_PADDING_VAL = 100;//空白区填充值

    CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_RotateWconst);//创建几何变换渲染器
    YMCV_GeomKernel_Load(myRender1, &myParam);//使用参数进行几何渲染配置、初始化
    CVIMAGE myImage1 = YMCV_Image_Geom_RenderTo(myImage0, myRender1);//将目标图像的进行渲染

    //显示图片
    LCD_show_image(0, 0, myImage1);

    // DataMatrix识别
    CVVECTORS_U16 result = YMCV_Gray_Find_DataMatrices(myImage1, 200);

    //显示结果
    for (int i = 0; (result != NULL) && i < result->dataNum; i++)
    {
        CV_datamatrices_data* tda = &result->datap[i * result->dim];

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
        LCD_Draw_Rect(tda->rect.x,tda->rect.y, tda->rect.w, tda->rect.h, rgbcv);//外接矩形框

        // DM码的内容
        printf("DM码内容:%s\n长度为%d个字符（含结束符\\0） ", tda->payload, tda->payload_len);

        //角度参数
        printf(" 角度 %d ", tda->rotation);
    }

    YMCV_Free_Vector_U16_Free(result);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);

    PAUSE();
    LCD_Destory();
    return 0;
}


