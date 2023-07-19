#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * 说明  该例程以 图像投视变换为例，展示了图片变换效果
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int sty = 16*4;
    char myInform[100];
	  
    //新图片创建，内容为空(默认不进行初始化，内容为随机值)
    CVIMAGE myImage0_gray = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_gray, CV_Rgb16ToGray);//rgb转灰度图

		//显示图片0
    LCD_Clear(0);
		LCD_show_image(0, sty + 0, myImage0_gray);
		//暂停500ms
		Delay(500);
	
    /*--------------------------进行透视变换--------------------------------------------------*/
    CVIMAGE myImage2 = NULL;
    YMCV_PADDING_VAL = 100;//空白区填充值
    //加载图片0的渲染参数
    CV_geom_param myParam = { .perspect.Imgin_w = myImage0_gray->width,
                              .perspect.Imgin_h = myImage0_gray->height,
                              .perspect.wsize_type = CV_WinSize_Const,//固定窗口
                              .perspect.scale=1.0f };

    for (int i = 0; i < 120; i++)
    {   
        //参数设置
        float x_r = 0, y_r = 0, z_r = 0, x_t = 0, y_t = 0, z_s = 0.5;
        if (i < 20) x_r += i * 5;//x轴旋转角
        else if (i < 40) y_r += (i - 20) * 5;//y轴旋转角
        else if (i < 60) z_r += (i - 40) * 5;//z轴旋转角
        else if (i < 80) x_t += (i - 60) * 2;//x轴平移
        else if (i < 100) y_t += (i - 80) * 2;//y轴旋平移
        else if (i < 120) z_s += (i - 100) * 0.1;//尺度

        //修改渲染参数
        myParam.perspect.x_angle = x_r;
        myParam.perspect.y_angle = y_r;
        myParam.perspect.z_angle = z_r;
        myParam.perspect.x_move = x_t;
        myParam.perspect.y_move = y_t;
        myParam.perspect.scale = z_s;

        //创建几何变换渲染器
        CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_Perspective);

        //使用参数进行几何渲染配置、初始化
        YMCV_GeomKernel_Load(myRender1, &myParam);

        //将目标图像的进行渲染
        myImage2 = YMCV_Image_Geom_RenderTo(myImage0_gray, myRender1);
        
        //显示图片2
        LCD_show_image(0, sty + 0, myImage2);

        //信息显示
        sprintf(myInform, "constWin perspective:\n   rotate:%g %g %g   \n   move:%g %g\n   size:%g  ",
				                  x_r, y_r, z_r,x_t,y_t,z_s);
        LCD_ShowString(0, 0, myInform);


        Delay(50);
				
        //释放渲染器,避免内存泄露
        YMCV_Free_GEOM_RenderFree(myRender1);
        //用释放图像
        YMCV_Free_ImgFree(myImage2);
    }
 

    /*--------------------------进行透视变换--动态窗口------------------------------------------------*/

		YMCV_PADDING_VAL = 100;//空白区填充值
    //加载图片0的渲染参数
    myParam.perspect.Imgin_w = myImage0_gray->width;
    myParam.perspect.Imgin_h = myImage0_gray->height;
    myParam.perspect.wsize_type = CV_WinSize_Auto;//动态窗口
    myParam.perspect.scale = 1.0f;

    LCD_Clear(0);//清屏
    for (int i = 0; i < 120; i++)
    {
        //参数设置
        float x_r = 0, y_r = 0, z_r = 0, x_t = 0, y_t = 0, z_s = 0.5;
        if (i < 20) x_r += i * 5;//x轴旋转角
        else if (i < 40) y_r += (i - 20) * 5;//y轴旋转角
        else if (i < 60) z_r += (i - 40) * 5;//z轴旋转角
        else if (i < 80) x_t += (i - 60) * 2;//x轴平移
        else if (i < 100) y_t += (i - 80) * 2;//y轴旋平移
        else if (i < 120) z_s += (i - 100) * 0.05;//尺度

        //修改渲染参数
        myParam.perspect.x_angle = x_r;
        myParam.perspect.y_angle = y_r;
        myParam.perspect.z_angle = z_r;
        myParam.perspect.x_move = x_t;
        myParam.perspect.y_move = y_t;
        myParam.perspect.scale = z_s;

        //创建几何变换渲染器
        CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_Perspective);

        //使用参数进行几何渲染配置、初始化
        YMCV_GeomKernel_Load(myRender1, &myParam);

        //将目标图像的进行渲染
        myImage2 = YMCV_Image_Geom_RenderTo(myImage0_gray, myRender1);

        //显示图片2
        LCD_show_image(0, sty + 0, myImage2);

        //信息显示
        sprintf(myInform, "autoWin perspective:\n   rotate:%g %g %g   \n   move:%g %g\n   size:%g  ",
				                  x_r, y_r, z_r,x_t,y_t,z_s);
        LCD_ShowString(0, 0, myInform);
				
        Delay(50);
				//清屏
				LCD_Fill_ConstRgbRect(0,sty + 0,myImage2->width,myImage2->height,(color24_t){0});
        //释放渲染器,避免内存泄露
        YMCV_Free_GEOM_RenderFree(myRender1);
        //用释放图像
        YMCV_Free_ImgFree(myImage2);

    }

    YMCV_Free_ImgFree(myImage0_gray);
}





