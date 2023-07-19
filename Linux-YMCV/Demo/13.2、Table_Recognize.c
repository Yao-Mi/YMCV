#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_SpaceCal.h"
#include "../YMCV/CORE/YMCV_FeatCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 表格识别为例，展示了 表格线提取效果
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1200, 700);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(tablerecg_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, tablerecg_gray, sizeof(tablerecg_gray));

    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 509, 668, CVBinaryType);//二值图0
    //显示原图
    LCD_show_image(0, 0, myImage0);


    uint8* myData2 = CV_malloc1(sizeof(tablerecg_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData2, tablerecg_gray, sizeof(tablerecg_gray));
    for (int i = 0; i < sizeof(tablerecg_gray); i++)
    {
        myData2[i] = (myData[i] > 220) ? 0 : 255;//快速二值化
    }
    CVIMAGE myImageb = YMCV_Creat_DataToImg((uint8*)myData2, 509, 668, CVBinaryType);//二值图0
    int pstrx = 510;
    LCD_show_image(pstrx, 0, myImageb);//处理后显示在右边
    PAUSE();

    ///*--------------------------形态学腐蚀滤波 提取垂直线段--------------------------------------------------*/
    //创建相应半径的形态学滤波核
    uint8 kernel_d = 3;
    CVFMAT myKernelH = YMCV_Creat_FMAT_Creat(kernel_d, 1);//3x1 提取垂直直线

    int morp_invpx = -YMCV_Ceil(myKernelH->col / 2.0f);//反向偏移量
    int morp_invpy = -YMCV_Ceil(myKernelH->row / 2.0f);//反向偏移量

    //手动设置击中核结构
    CV_memset(myKernelH->datap, 0, myKernelH->row * myKernelH->col * sizeof(float));//全部初始为0
    //选择需要击中的部分记为1
    for (int i = 0; i < myKernelH->row; i++)
    {
        for (int j = 0; j < myKernelH->col; j++)
        {
            myKernelH->datap[i * myKernelH->col + j] = 1.0f;
        }
    }

    //计算形态学击中结果
    CVIMAGE myImage1 = YMCV_Binary_Morph_Hit_Cal(myImageb, myKernelH, 99);//输出击中率>99%的部分 ，腐蚀效果
    //显示图片1
    LCD_show_image(pstrx, 0, myImage1);

    //信息显示
    sprintf(myInform, "腐蚀滤波 ");
    LCD_ShowString(pstrx, 0, myInform);
    //释放空间渲染器
    YMCV_Free_FMatFree(myKernelH);
    //暂停
    PAUSE();
    /*--------------------------骨架细化计算--------------------------------------------------*/
    YMCV_Image_Binary_Bone_Thin(myImage1);

    LCD_show_image(pstrx, 0, myImage1);//处理后显示在右边
    //信息显示
    sprintf(myInform, "骨架细化");
    LCD_ShowString(pstrx, 0, myInform);
    //暂停
    PAUSE();
    /*------------------------ 二值区域过滤 ------------------------------*/
    //保留面积大于12的区域
    YMCV_Binary_AreaFilter(myImage1, 30);
    LCD_show_image(pstrx, 0, myImage1);
    //信息显示
    sprintf(myInform, "区域过滤");
    LCD_ShowString(pstrx, 0, myInform);
    PAUSE();
    /*-------------------------- 蠕虫直线检测检测 --------------------------------------------------*/
    CVVECTORS_U16 Hlines = YMCV_WormFindLines(myImage1, 0.001, 10, 1);
    color24_t rgbt = { .R = 255 };
    LCD_Point_Color_Set(rgbt);
    if (Hlines != NULL)
    {
        for (int i = 0; i < Hlines->dataNum; i++)
        {
            CVline* thisline = Hlines->datap + i * Hlines->dim;
            LCD_Draw_Line(morp_invpx + thisline->x1, morp_invpy + thisline->y1, morp_invpx + thisline->x2, morp_invpy + thisline->y2);//绘制线段
        }
    }
    //信息显示
    sprintf(myInform, "线段检测");
    LCD_ShowString(pstrx, 0, myInform);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage1);
    PAUSE();

    ///*--------------------------形态学腐蚀滤波 提取水平线段--------------------------------------------------*/
    //创建相应半径的形态学滤波核
    kernel_d = 3;
    CVFMAT myKernelV = YMCV_Creat_FMAT_Creat(1, kernel_d);//1x3 提取水平直线

    morp_invpx = -YMCV_Ceil(myKernelV->col / 2.0f);//反向偏移量
    morp_invpy = -YMCV_Ceil(myKernelV->row / 2.0f);//反向偏移量

    //手动设置击中核结构
    CV_memset(myKernelV->datap, 0, myKernelV->row * myKernelV->col * sizeof(float));//全部初始为0
    //选择需要击中的部分记为1
    for (int i = 0; i < myKernelV->row; i++)
    {
        for (int j = 0; j < myKernelV->col; j++)
        {
            myKernelV->datap[i * myKernelV->col + j] = 1.0f;
        }
    }

    //计算形态学击中结果
    myImage1 = YMCV_Binary_Morph_Hit_Cal(myImageb, myKernelV, 99);//输出击中率>99%的部分 ，腐蚀效果
    //显示图片1
    LCD_show_image(pstrx, 0, myImage1);

    //信息显示
    sprintf(myInform, "腐蚀滤波 ");
    LCD_ShowString(pstrx, 0, myInform);
    //释放空间渲染器
    YMCV_Free_FMatFree(myKernelV);
    //暂停
    PAUSE();
    /*--------------------------骨架细化计算--------------------------------------------------*/
    YMCV_Image_Binary_Bone_Thin(myImage1);

    LCD_show_image(pstrx, 0, myImage1);//处理后显示在右边
    //信息显示
    sprintf(myInform, "骨架细化");
    LCD_ShowString(pstrx, 0, myInform);
    //暂停
    PAUSE();
    /*------------------------ 二值区域过滤 ------------------------------*/
    //保留面积大于12的区域
    YMCV_Binary_AreaFilter(myImage1, 40);
    LCD_show_image(pstrx, 0, myImage1);
    //信息显示
    sprintf(myInform, "区域过滤");
    LCD_ShowString(pstrx, 0, myInform);
    PAUSE();
    /*-------------------------- 蠕虫直线检测检测 --------------------------------------------------*/
    CVVECTORS_U16 Vlines = YMCV_WormFindLines(myImage1, 0.001, 10, 1);
    if (Vlines != NULL)
    {
        for (int i = 0; i < Vlines->dataNum; i++)
        {
            CVline* thisline = Vlines->datap + i * Vlines->dim;
            LCD_Draw_Line(morp_invpx + thisline->x1, morp_invpy + thisline->y1, morp_invpx + thisline->x2, morp_invpy + thisline->y2);//绘制线段
        }
    }
    //信息显示
    sprintf(myInform, "线段检测");
    LCD_ShowString(pstrx, 0, myInform);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage1);
    PAUSE();
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImageb);
    YMCV_Free_Vector_U16_Free(Vlines);
    YMCV_Free_Vector_U16_Free(Hlines);

    PAUSE();
    LCD_Destory();
    return 0;
}





