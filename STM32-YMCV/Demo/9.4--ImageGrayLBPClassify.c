#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以 LBP 人脸分类为例，展示 人脸分类 结果
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

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    myData = CV_malloc1(sizeof(luojiuchuan_112x112_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, (void*)luojiuchuan_112x112_gray, sizeof(luojiuchuan_112x112_gray));
    CVIMAGE myImage1 = YMCV_Creat_DataToImg((uint8*)myData, 112, 112, CVGrayType);//灰度图0

    /*-------------------------- CasCade 人脸检测 --------------------------------------------------*/
    CV_cascade facedect;
    YMCV_Load_HaarCascade(&facedect, "frontface");//加载 正面(朝前) 人脸检测器

    CVVECTORS_U16 result = YMCV_HaarCascade_Detect_Objects(myImage0_gray, &facedect, 1.0f);

    //显示图片
	  LCD_Clear(GRAY);
    LCD_show_image(stx + 0, 16, myImage0_gray);
    //绘制人脸框
    for (int i = 0; (result != NULL) && (i < result->dataNum); i++)
    {
        CVrect* recp = (CVrect*)(&result->datap[i * result->dim]);
        //绘制矩形框
        color24_t rgbval = { .R = 255 };
        LCD_Draw_Rect(stx + recp->x, 16+recp->y, recp->w, recp->h, rgbval);
    }
    //信息显示
    sprintf(myInform, " Haar Cascade front face ");
    LCD_ShowString(0, 0, myInform);
    //暂停
    Delay(500);


    /*-------------------------人脸LBP直方图特征检测--------------------------------------------------*/

    //输入人脸图片限定最大为112×112，支持orl人脸数据集中92×112的大小
    CV_FACE_LBP_DESC lbpDescribe = YMCV_Face_UniformLBP_Describe(myImage1);

    LCD_Clear(GRAY);
    LCD_show_image(0, 0, myImage1);//显示图片0
    LCDLbpHistShow(lbpDescribe, 112, 0);//显示LBP描述符
		int sty = myImage1->height;
    //信息显示
    sprintf(myInform, " face LBP feature");
    LCD_ShowString(0, 0, myInform);//
    Delay(300);

    //float32 mindis = 1.5f;//超过1.5假设不相关
		float32 mindis = 30.5f;//这里为了效果设置大一些
    int  mini = -1;
    //提取照片中的人脸
    for (int i = 0; (result != NULL) && (i < result->dataNum); i++)
    {
        CVrect* recp = (CVrect*)(&result->datap[i * result->dim]);
        //截取图片
        CVIMAGE faceimg = YMCV_Creat_ImgROICopyTo(myImage0_gray, recp);

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

        LCD_show_image(0, sty, faceimg);//显示图片0
				Delay(300);
        LCDLbpHistShow(facelbp, 120,sty);//显示LBP描述符

        //特征直方图卡方检验匹配
        float32 dis = YMCV_Face_LBP_Hist_X2_Distance(lbpDescribe, facelbp);//特征比对
        sprintf(myInform, " chi-squared dis %5.2f ",dis);//卡方检验距离
        LCD_ShowString(0, sty, myInform);//

        //取最小距离作为匹配结果
        if (mindis > dis)
        {
            mindis = dis;
            mini = i;
        }

        //释放内存
        YMCV_Free_ImgFree(faceimg);
        YMCV_Free_FaceLbp_Hist_free(facelbp);
        Delay(500);
    }

    //显示匹配结果
		LCD_Clear(GRAY);
    LCD_show_image(0, 0, myImage1);//模板
    LCD_show_image(stx+0, sty, myImage0_gray);
    if (mini != -1)
    {
        color24_t rgbval = { .R = 255 };
        LCD_Draw_Rect(0, 0, myImage1->width, myImage1->height, rgbval);
        
        //绘制结果矩形框
        CVrect* recp = (CVrect*)(&result->datap[mini * result->dim]);
        color24_t rgbval2 = { .G = 255 };
        LCD_Draw_Rect(stx+recp->x, sty+recp->y, recp->w, recp->h, rgbval2);


        LCD_Point_Color_Set(rgbval);
        sprintf(myInform, "LuoJiuC");
        LCD_ShowString(stx+recp->x - 16 + recp->w/2, sty+recp->y - 16 + recp->h/2, myInform);
    } 
//    sprintf(myInform, "match result");
//    LCD_ShowString(200, 420, myInform);//

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_Vector_U16_Free(result);
    YMCV_Free_FaceLbp_Hist_free(lbpDescribe);//特征内存释放

    Delay(500);
}


