#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_EdgeCal.h"
#include"../YMCV/CORE/YMCV_MergeCal.h"
#include"../YMCV/CORE/YMCV_SegnmCal.h"
#include"../YMCV/CORE/YMCV_AreaFillCal.h"
#include <stdlib.h>

/*******************************************************************************************
    *
    * 说明  该例程以 xy轴投影分割为例，展示了二值图分割的效果
    *
    ***************************************************************************************/

int main()
{
    char myInform[100];
    LCD_Init(1000, 500);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(number0_9_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, number0_9_gray, sizeof(number0_9_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 400, 252, CVGrayType);//灰度图0

    /*-------------------------- 二值化 --------------------------------------------------*/
     //创建输出二值图像
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);//二值图
    CV_binary_param myParam = { .gray_double_t.thresh_min = 0 ,.gray_double_t.thresh_max = 50 };
    
    CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_DoubleThreshold);//创建二值变换渲染器
    YMCV_BinaryKernel_Load(myRender1, &myParam);//加载参数进行渲染配置、初始化
    YMCV_Image_ThreshTo_Binary(myImage0, myImage2, myRender1);//将目标图像的进行渲染

    LCD_Clear(0);
    //显示原图
    LCD_show_image(0, 0, myImage0);
    LCD_show_image(400, 0, myImage2);
    sprintf(myInform, " 二值化 ");
    LCD_ShowString(0, 310, myInform);
    PAUSE();
    
    /*-------------------------- y,x投影字符分割 --------------------------------------------------*/
    CVVECTORS_U16 yhist = YMCV_Binary_XY_Axis_Proj(myImage2, 1);//先按y轴投影
    YMCV_Binary_Proj_Hist_Seg(yhist, 20, 5);//投影值统计小于20，长度小于5的区域都为0
    LCDProjHistShow(yhist, 1, 0, 250);//显示投影图
    //按y方向分析
    for (int y = 0; y < yhist->dataNum; y++)
    {
        //取非0区间的位置
        if (yhist->datap[y] > 0)
        {
            int ry = y;
            while ((ry < yhist->dataNum)&&(yhist->datap[ry]))//直到碰到下一个0，可获得非0片段
            {
                ry++; 
            }
            //截取横向图片
            CVIMAGE ySegImg = YMCV_Creat_ImgCopyTo(myImage2, 0, y, myImage2->width, ry - y);
            LCD_show_image(0, 0, ySegImg);

            sprintf(myInform, " y投影分割 ");
            LCD_ShowString(200, 310, myInform);
            PAUSE();
            //x轴投影分析
            CVVECTORS_U16 xhist = YMCV_Binary_XY_Axis_Proj(ySegImg, 0);//投影到x轴
            YMCV_Binary_Proj_Hist_Seg(xhist, 6, 5);//对投影直方图进行分割
            LCDProjHistShow(xhist, 0, 0, 80);//显示投影图

            sprintf(myInform, " x投影分割 ");
            LCD_ShowString(200, 310, myInform);
            PAUSE();
            //按x方向分析
            for (int x = 0; x < xhist->dataNum; x++)
            {
                //取非0区间的位置
                if (xhist->datap[x] > 0)
                {
                    int rx = x;
                    while ((rx < xhist->dataNum) && (xhist->datap[rx]))
                    {
                        rx++;
                    }
                    //截取纵向图片
                    CVIMAGE xSegImg = YMCV_Creat_ImgCopyTo(ySegImg, x, 0, rx - x, ySegImg->height);
                    LCD_show_image(400, 250, xSegImg);
                    PAUSE();
                    //释放内存
                    YMCV_Free_ImgFree(xSegImg);
                    //跳过已经取过的区间
                    x = rx;
                }
            }
            //释放截图内存和投影图
            YMCV_Free_ImgFree(ySegImg);
            YMCV_Free_Vector_U16_Free(xhist);
            //跳过已经取过的区间
            y = ry;
        }
    }
    //释放内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_Vector_U16_Free(yhist);

    PAUSE();
    LCD_Destory();
    return 0;
}

