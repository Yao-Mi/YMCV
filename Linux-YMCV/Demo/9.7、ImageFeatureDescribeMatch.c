#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_FeatCal.h"
#include"../YMCV/CORE/YMCV_MatchingCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 特征描述子匹配 为例，展示了brief特征描述符匹配 及单应矩阵计算并进行图像矫正
    *
    ***************************************************************************************/

void show_brief_discribe(int rx, int ry, CVVECTORS_U16 featpoints2, CVVECTORS_U16 featdesc2);

int main()
{
    char myInform[100];
    LCD_Init(640, 480);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(tuxin_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, tuxin_gray, sizeof(tuxin_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//灰度图0

    /*------------------------ AGAST 特征点提取 ------------------------------*/
    CV_featpoint_param myparam;
    myparam.type = CV_FeatPoint_AGAST;
    myparam.param.agast.thresh = 20;//差值
    CVVECTORS_U16 featpoints = YMKV_FeaturePoint_Find(myImage0, &myparam);//特征点检测

    LCD_show_image(0, 0, myImage0);//显示图片

    /*------------------------ rotate brief特征描述符提取 ------------------------------*/
    CVVECTORS_U16 featdesc = YMCV_FeatureDescribe_RotateBrief_Cal(myImage0, featpoints);


    LCD_show_image(0, 0, myImage0);//显示图片
    //显示特征描述符
    show_brief_discribe(0, 0, featpoints, featdesc);

    //信息显示
    sprintf(myInform, "AGAST 特征点与RotateBRIEF描述符提取  ");
    LCD_ShowString(200, 310, myInform);
    PAUSE();

    //----------------------------- 图像几何变换 -----------------------------------
     //加载图片0的渲染参数
    CV_geom_param geoParam = { .perspect.Imgin_w = myImage0->width,
                              .perspect.Imgin_h = myImage0->height,
                              .perspect.wsize_type = CV_WinSize_Const,//固定窗口
                              .perspect.scale = 1.0f };
    //修改渲染参数
    geoParam.perspect.x_angle = 0;
    geoParam.perspect.y_angle = 0;
    geoParam.perspect.z_angle = 30;
    geoParam.perspect.x_move = 60; //平移变换
    geoParam.perspect.y_move = 40;

    //创建几何变换渲染器
    CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_Perspective);

    //使用参数进行几何渲染配置、初始化
    YMCV_GeomKernel_Load(myRender1, &geoParam);

    //将目标图像的进行渲染
    CVIMAGE myImage1 = YMCV_Image_Geom_RenderTo(myImage0, myRender1);

    int rx = 260, ry = 0;
    LCD_show_image(rx, ry, myImage1);//显示图片

    /*------------------------ AGAST 特征点 及 rotate brief特征描述符提取提取 ------------------------------*/
    CVVECTORS_U16 featpoints2 = YMKV_FeaturePoint_Find(myImage1, &myparam);//特征点检测
    CVVECTORS_U16 featdesc2 = YMCV_FeatureDescribe_RotateBrief_Cal(myImage1, featpoints2);
    show_brief_discribe(rx, ry, featpoints2, featdesc2);

    PAUSE();
    //---------------------------- 特征匹配 --------------------------------------
    uint16* resultID = CV_malloc1(featdesc2->dataNum * sizeof(uint16));
    uint8* matched = CV_malloc1(featdesc2->dataNum * sizeof(uint8));
    uint8 thresh = 80;//80%
    YMCV_FeatureDescribe_Match(featdesc, featdesc2, resultID, matched, thresh);

    //显示匹配结果
    int matchnum = 0;
    for (int i = 0; i < featdesc2->dataNum; i++)
    {
        if (matched[i])
        {
            matchnum++;
            int id1 = resultID[i];
            //获取特征点坐标
            CVpoint* pp1 = featpoints->datap + id1 * featpoints->dim;//特征点1
            CVpoint* pp2 = featpoints2->datap + i * featpoints2->dim;//特征点2

            //连线
            color24_t thiscolor = { .R = 255, };
            LCD_Point_Color_Set(thiscolor);
            LCD_Draw_Line(pp1->x, pp1->y, rx + pp2->x, ry + pp2->y);
        }
    }        //信息显示
    sprintf(myInform, " 进行 特征描述匹配                     ");
    LCD_ShowString(200, 310, myInform);
    PAUSE();
    //---------------------------------- 求取变换参数 --------------------------------

    CVVECTORS_U16 X1 = YMCV_Creat_Vector_U16(sizeof(CVpoint) / sizeof(uint16), matchnum);
    CVVECTORS_U16 X2 = YMCV_Creat_Vector_U16(sizeof(CVpoint) / sizeof(uint16), matchnum);
    //提取有效的匹配点对
    for (int i = 0, matchi = 0; i < featdesc2->dataNum; i++)
    {
        if (matched[i])
        {
            int id1 = resultID[i];
            //获取特征点坐标
            CVpoint* pp1 = featpoints->datap + id1 * featpoints->dim;//特征点1
            CVpoint* pp2 = featpoints2->datap + i * featpoints2->dim;//特征点2

            //记录
            CVpoint* pax1 = X1->datap + matchi * X1->dim;//特征点缓存
            CVpoint* pax2 = X2->datap + matchi * X2->dim;
            matchi++;

            pax1->x = pp1->x; pax1->y = pp1->y;
            pax2->x = pp2->x; pax2->y = pp2->y;

        }
    }
    //由于匹配成功的只有7个点对，实际正确的只有前4个，我们这边手动设置只保留前4个点对
    X1->dataNum = 4; 
    X2->dataNum = 4;
    //计算坐标变换参数 ：X1 <-- X2
    CVFMAT myHmat = YMCV_FindHomography_Ransac(X1, X2,0.99,25);

    //矩阵转为几何变换渲染器
    CVGEOM_RENDER geoHrender = YMCV_HomographyMat_ToPerspective_GeomRender(myHmat, myImage1->width, myImage1->height, CV_WinSize_Const);//固定窗口

    //将目标图像的进行渲染
    CVIMAGE myImage2 = YMCV_Image_Geom_RenderTo(myImage1, geoHrender);

    LCD_show_image(0, 0, myImage0);//显示图片
    LCD_show_image(rx, ry, myImage2);//显示矫正图片
        //信息显示  
    sprintf(myInform, " 尝试 矫正图像1    ");
    LCD_ShowString(200, 310, myInform);
    PAUSE();

    //释放featpoints内存
    YMCV_Free_Vector_U16_Free(featdesc);
    YMCV_Free_Vector_U16_Free(featpoints);
    YMCV_Free_Vector_U16_Free(featdesc2);
    YMCV_Free_Vector_U16_Free(featpoints2);
    YMCV_Free_Vector_U16_Free(X1);
    YMCV_Free_Vector_U16_Free(X2);
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);
    //其他内存
    YMCV_Free_GEOM_RenderFree(myRender1);
    YMCV_Free_GEOM_RenderFree(geoHrender);
    YMCV_Free_FMatFree(myHmat);
    CV_free1(resultID);
    CV_free1(matched);

    PAUSE();
    LCD_Destory();
    return 0;
}


void show_brief_discribe(int rx,int ry,CVVECTORS_U16 featpoints2, CVVECTORS_U16 featdesc2)
{
    color24_t oldcolor = LCD_Pen_Color_Get();
    //显示特征描述符
    for (int i = 0; (featpoints2 != NULL) && (i < featpoints2->dataNum); i++)
    {
        CVpoint* pp = featpoints2->datap + i * featpoints2->dim;//特征点
        CV_featdesc_type* desc = featdesc2->datap + i * featdesc2->dim;//特征描述

        //绘制重心方向直线
        int r = 15;//由于模板选用r=15进行求描述符，这里直接用绘制r = 15的圆形
        int lendx = r * YMCV_Cos(desc->angle * YMCV_Deg2Rad);
        int lendy = r * YMCV_Sin(desc->angle * YMCV_Deg2Rad);
        color24_t thiscolor = { .B = 255 };
        LCD_Point_Color_Set(thiscolor);
        LCD_Draw_Line(rx + pp->x, ry + pp->y, rx + pp->x + lendx, ry + pp->y + lendy);
        //绘制点位置
        color24_t thiscolor2 = { .G = 255 };
        LCD_Point_Color_Set(thiscolor2);
        LCD_Draw_Circle(rx + pp->x, ry + pp->y, r);
    }

    LCD_Point_Color_Set(oldcolor);
}


