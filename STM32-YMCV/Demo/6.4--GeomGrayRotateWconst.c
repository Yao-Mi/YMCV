#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以 图像旋转变换为例，展示了图片旋转的固定窗口输出效果
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int sty = 16;
    char myInform[100];
	
    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage0_gray = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_gray, CV_Rgb16ToGray);//rgb转灰度图

    uint8* myData1 = CV_malloc1(sizeof(xgmm_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData1, (void*)xgmm_gray, sizeof(xgmm_gray));
    CVIMAGE myImage1 = YMCV_Creat_DataToImg((uint8*)myData1, 50, 75, CVGrayType);//灰度图1

		//显示图片0
		LCD_show_image(0, sty + 0, myImage0_gray);
		//暂停500ms
		Delay(500);

    /*--------------------------旋转自动缩放插值--------------------------------------------------*/
    CVIMAGE myImage2 = NULL;

    CV_geom_param myParam = { .rotate.Imgin_w = myImage0_gray->width,
                              .rotate.Imgin_h = myImage0_gray->height,
                              .rotate.angle = 30,//30度
                              .rotate.out_w= myImage0_gray->width ,//固定窗口需要设置输出大小
                              .rotate.out_h= myImage0_gray->height };
    float k = 1.0f;
    YMCV_PADDING_VAL = 50;//空白区填充值

    for (int i = 1; i < 10; i++)
    {
        //设置输出窗口大小
        myParam.rotate.out_w = (int)(k * myImage0_gray->width);
        myParam.rotate.out_h = (int)(k * myImage0_gray->height);

        //创建几何变换渲染器
        CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_RotateWconst);

        //使用参数进行几何渲染配置、初始化
        YMCV_GeomKernel_Load(myRender1, &myParam);

        //将目标图像的进行渲染
        myImage2 = YMCV_Image_Geom_RenderTo(myImage0_gray, myRender1);


        LCD_Clear(0);
        //显示图片2
        LCD_show_image(0, sty + 0, myImage2);

        //信息显示
			  sprintf(myInform, "outImage size:(%03d %03d) ", myParam.rotate.out_w, myParam.rotate.out_h);
        LCD_ShowString(0, 0, myInform);
        //暂停500ms
		    Delay(500);

        if (i < 4)
        {
            k -= 0.2f;
        }
        else
        {
            k += 0.2f;
        }
        //释放渲染器
        YMCV_Free_GEOM_RenderFree(myRender1);
        //用释放图像，避免下次用的时候导致内存泄漏
        YMCV_Free_ImgFree(myImage2);
    }

    k = 0.0f;
    //加载对图片1的渲染参数
    myParam.rotate.Imgin_w = myImage1->width;
    myParam.rotate.Imgin_h = myImage1->height;
    myParam.rotate.angle = 0;//0度
    //固定窗口需要设置输出大小
    myParam.rotate.out_w = 200;
    myParam.rotate.out_h = 200;

    for (int i = 1; i < 10; i++)
    {
        //设置旋转角度
        myParam.rotate.angle = k;
        //创建几何变换渲染器
        CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_RotateWconst);

        //使用参数进行几何渲染配置、初始化
        YMCV_GeomKernel_Load(myRender1, &myParam);

        //将目标图像的进行渲染
        myImage2 = YMCV_Image_Geom_RenderTo(myImage1, myRender1);

        LCD_Clear(0);
        //显示图片2
        LCD_show_image(0, sty + 0, myImage2);

        //信息显示
        sprintf(myInform, "constWin rotate:%02.1fdegree", k);
        LCD_ShowString(0, 0, myInform);
        //暂停500ms
		    Delay(500);

        if (i < 4)
        {
            k += 20;
        }
        else
        {
            k -= 20;
        }
        //释放渲染器
        YMCV_Free_GEOM_RenderFree(myRender1);
        //用释放图像，避免下次用的时候导致内存泄漏
        YMCV_Free_ImgFree(myImage2);
    }
    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage1);

}





