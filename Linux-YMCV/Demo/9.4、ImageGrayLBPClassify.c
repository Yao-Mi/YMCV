#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_MatchingCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 LBP 人脸分类为例，展示 人脸分类 结果
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

        //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    myData = CV_malloc1(sizeof(luojiuchuan_112x112_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, luojiuchuan_112x112_gray, sizeof(luojiuchuan_112x112_gray));
    CVIMAGE myImage1 = YMCV_Creat_DataToImg((uint8*)myData, 112, 112, CVGrayType);//灰度图0

    /*-------------------------- CasCade 人脸检测 --------------------------------------------------*/
    CV_cascade facedect;
    YMCV_Load_HaarCascade(&facedect, "frontface");//加载 正面(朝前) 人脸检测器

    CVVECTORS_U16 result = YMCV_HaarCascade_Detect_Objects(myImage0, &facedect, 0.8f);

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
    PAUSE();


    /*-------------------------人脸LBP直方图特征检测--------------------------------------------------*/

    //输入人脸图片限定最大为112×112，支持orl人脸数据集中92×112的大小
    CV_FACE_LBP_DESC lbpDescribe = YMCV_Face_UniformLBP_Describe(myImage1);

    LCD_Clear(0);
    LCD_show_image(112, 0, myImage1);//显示图片0
    LCDLbpHistShow(lbpDescribe, 0, 0);//显示LBP描述符
    //信息显示
    sprintf(myInform, " 模板人脸特征提取 ");
    LCD_ShowString(200, 420, myInform);//
    PAUSE();

    float32 mindis = 1.5f;//超过1.5假设不相关
    int  mini = -1;
    //提取照片中的人脸
    for (int i = 0; (result != NULL) && (i < result->dataNum); i++)
    {
        CVrect* recp = &result->datap[i * result->dim];
        //截取图片
        CVIMAGE faceimg = YMCV_Creat_ImgROICopyTo(myImage0, recp);

        //放缩到不超过112大小
        float scale_k = 112.0f / ((recp->w> recp->h)? recp->w: recp->h);//将长边缩放为112
        if (scale_k < 1.0f)
        {
            CV_geom_param myParam = { .stretch.Imgin_w = faceimg->width,
                                      .stretch.Imgin_h = faceimg->height,
                                      .stretch.out_w = scale_k * faceimg->width,
                                      .stretch.out_h = scale_k * faceimg->height, };
            //创建几何变换渲染器
            CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_StretchL2);//双线性插值
            YMCV_GeomKernel_Load(myRender1, &myParam);//使用参数进行几何渲染配置、初始化
            CVIMAGE myImage2 = YMCV_Image_Geom_RenderTo(faceimg, myRender1);//渲染

            //释放原来的图片内存
            YMCV_Free_ImgFree(faceimg);
            //将调整过大小的图片重新赋予
            faceimg = myImage2;
        }

        //LBP特征提取
        CV_FACE_LBP_DESC facelbp = YMCV_Face_UniformLBP_Describe(faceimg);

        LCD_show_image(230, 0, faceimg);//显示图片0
        LCDLbpHistShow(facelbp, 230+112, 0);//显示LBP描述符

        //特征直方图卡方检验匹配
        float32 dis = YMCV_Face_LBP_Hist_X2_Distance(lbpDescribe, facelbp);//特征比对
        sprintf(myInform, " 卡方检验距离 %5.2f ",dis);
        LCD_ShowString(200, 420, myInform);//

        //取最小距离作为匹配结果
        if (mindis > dis)
        {
            mindis = dis;
            mini = i;
        }

        //释放内存
        YMCV_Free_ImgFree(faceimg);
        YMCV_Free_FaceLbp_Hist_free(facelbp);
        PAUSE();
    }

    //显示匹配结果
    LCD_show_image(0, 0, myImage0);
    LCD_show_image(0, 0, myImage1);//模板
    if (mini != -1)
    {
        color24_t rgbval = { .R = 255 };
        LCD_Draw_Rect(0, 0, myImage1->width, myImage1->height, rgbval);
        
        //绘制结果矩形框
        CVrect* recp = &result->datap[mini * result->dim];
        color24_t rgbval2 = { .G = 255 };
        LCD_Draw_Rect(recp->x, recp->y, recp->w, recp->h, rgbval2);

        sprintf(myInform, "落九川");
        LCD_ShowString(recp->x - 16 + recp->w/2, recp->y - 16 + recp->h/2, myInform);
    } 
    sprintf(myInform, "    匹配结果展示    ");
    LCD_ShowString(200, 420, myInform);//

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_FaceLbp_Hist_free(lbpDescribe);//特征内存释放

    PAUSE();
    LCD_Destory();
    return 0;
}


