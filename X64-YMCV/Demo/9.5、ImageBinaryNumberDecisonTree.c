#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_SegnmCal.h"
#include"../YMCV/CORE/YMCV_DecisionTree_ID3.h"
#include <stdlib.h>

/*******************************************************************************************
    *
    * 说明  该例程以 ID3 决策树分类为例，展示了数字识别的效果
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
    /*-------------------------- 决策树模型加载 --------------------------------------------------*/
    uint8 decision_model[] = "[0,0,0,12]LR[205,0,0,14]LR[205,0,0,10]LR[205,0,0,11]LR[205,0,1,8]LR[205,0,0,15]LR[205,0,0,13]LR[8,1,0,-12851]--[205,0,1,20]LR[0,1,0,-12851]--[6,1,0,-12851]--[4,1,0,-12851]--[1,1,0,-12851]--[9,1,0,-12851]--[205,0,1,15]LR[2,1,0,-12851]--[7,1,0,-12851]--[5,1,0,-12851]--[3,1,0,-12851]--";
    CV_DECISION_TREE myID3 = YMCV_DecisionTree_Creat();//创建ID3决策模型
    YMCV_DecisionTree_Load(myID3, decision_model);//模型加载

    /*-------------------------- y,x投影字符分割，并提取数字特征 --------------------------------------------------*/
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
            while ((ry < yhist->dataNum) && (yhist->datap[ry]))
            {
                ry++;
            }
            //截取横向图片
            CVIMAGE ySegImg = YMCV_Creat_ImgCopyTo(myImage2, 0, y, myImage2->width, ry - y);
            LCD_show_image(0, 0, ySegImg);
            PAUSE();
            //x轴投影分析
            CVVECTORS_U16 xhist = YMCV_Binary_XY_Axis_Proj(ySegImg, 0);//投影到x轴
            YMCV_Binary_Proj_Hist_Seg(xhist, 6, 5);//对投影直方图进行分割
            LCDProjHistShow(xhist, 0, 0, 80);//显示投影图
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
                    //PAUSE();
                    /*-------------------------- 数字特征提取--------------------------------------------------*/
                    //将图像变为16×16大小
                    CVIMAGE resultImg = YMCV_Binary_MaxPooling_ScaleSizeTo_MxM(xSegImg, 16);
                    LCD_show_image(500, 250, resultImg);
                    CVVECTORS_U16 numberhist = YMCV_Binary_Number16x16_ProjHist16(resultImg);//1x16
                    LCDProjHistShow(numberhist, 0, 530, 250);//显示投影特征图
                    sprintf(myInform, "16x1的特征向量");
                    LCD_ShowString(510, 300, myInform);
                    //强制转为16x1
                    numberhist->dim = numberhist->dataNum;
                    numberhist->dataNum = 1;
                    uint8 lable;

                    YMCV_DecisionTree_Classify(numberhist, &lable,myID3);//决策树判别
                    sprintf(myInform, "识别结果为 %d ", lable);
                    LCD_ShowString(550, 250, myInform);

                    YMCV_Free_ImgFree(resultImg);
                    YMCV_Free_Vector_U16_Free(numberhist);
                    PAUSE();
                    LCD_Clear(0);
                    /*-----------------------------------------------------------------------------------------------*/
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
