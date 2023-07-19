#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以 相位相关匹配为例，展示图像平移后的匹配效果
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(mm02_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, (void*)mm02_gray, sizeof(mm02_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVGrayType);//灰度图0

    /*---------------------------- 图像平移 --------------------------------------------*/
    //加载图片0的渲染参数
    CV_geom_param myParam = { .perspect.Imgin_w = myImage0->width,
                              .perspect.Imgin_h = myImage0->height,
                              .perspect.wsize_type = CV_WinSize_Const,//固定窗口
                              .perspect.scale = 1.0f };
    //修改渲染参数
    myParam.perspect.x_angle = 0;
    myParam.perspect.y_angle = 0;
    myParam.perspect.z_angle = 0;
    //单次偏移量需保持在[±宽 / 2，±高 / 2]范围内，
    myParam.perspect.x_move = 90; //平移变换
    myParam.perspect.y_move = 50;
    myParam.perspect.scale = 1;
    //创建几何变换渲染器
    CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_Perspective);

    //使用参数进行几何渲染配置、初始化
    YMCV_GeomKernel_Load(myRender1, &myParam);

    //将目标图像的进行渲染
    CVIMAGE myImage1 = YMCV_Image_Geom_RenderTo(myImage0_gray, myRender1);

		LCD_Clear(GRAY);
    LCD_show_image(stx + 0, 0, myImage0_gray);
		int sty = myImage0_gray->height;
    LCD_show_image(stx + 0, sty + 0, myImage1);
    //信息显示
    sprintf(myInform, "image move ");
    LCD_ShowString(stx + 0, sty + 0, myInform);
    Delay(500);

    /*-------------------------- 相位相关匹配 --------------------------------------------------*/
    float32 moveX, moveY;
    YMCV_Image_Gray_PhaseCorrelate(myImage0_gray, myImage1, &moveX, &moveY);//计算偏移
//    //显示图片
//    LCD_show_image(0, 0, myImage0_gray);

    //信息显示
    sprintf(myInform, "found move:x%.2f y%.2f ", moveX, moveY);
    LCD_ShowString(0, sty, myInform);
    Delay(500);

    //释放内存
    YMCV_Free_GEOM_RenderFree(myRender1);
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage1);

}


