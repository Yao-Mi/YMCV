#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_GeomCal.h"
/*******************************************************************************************
    *
    * 说明  该例程以 图像投视变换为例，展示了图片变换效果
    *
    ***************************************************************************************/

int main()
{
    char myInform[100];
    LCD_Init(1000, 450);

    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = CV_malloc1(sizeof(mm00_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, mm00_gray, sizeof(mm00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//灰度图0


    /*--------------------------进行透视变换--------------------------------------------------*/
    CVIMAGE myImage2 = NULL;
    YMCV_PADDING_VAL = 100;//空白区填充值
    //加载图片0的渲染参数
    CV_geom_param myParam = { .perspect.Imgin_w = myImage0->width,
                              .perspect.Imgin_h = myImage0->height,
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
        myImage2 = YMCV_Image_Geom_RenderTo(myImage0, myRender1);
        
        LCD_Clear(0);
        //显示图片0
        LCD_show_image(0, 0, myImage0);
        //显示图片2
        LCD_show_image(500, 0, myImage2);

        //信息显示
        sprintf(myInform, "图像 透视变换  %g %g %g %g %g %g ",x_r, y_r, z_r,x_t,y_t,z_s);
        LCD_ShowString(100, 310, myInform);

        //释放渲染器,避免内存泄露
        YMCV_Free_GEOM_RenderFree(myRender1);
        //用释放图像
        YMCV_Free_ImgFree(myImage2);

        //Delay(50);
        PAUSE();
    }
 

    /*--------------------------进行透视变换--动态窗口------------------------------------------------*/
    YMCV_PADDING_VAL = 100;//空白区填充值
    //加载图片0的渲染参数
    myParam.perspect.Imgin_w = myImage0->width;
    myParam.perspect.Imgin_h = myImage0->height;
    myParam.perspect.wsize_type = CV_WinSize_Auto;//动态窗口
    myParam.perspect.scale = 1.0f;

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
        myImage2 = YMCV_Image_Geom_RenderTo(myImage0, myRender1);

        LCD_Clear(0);
        //显示图片0
        LCD_show_image(0, 0, myImage0);
        //显示图片2
        LCD_show_image(500, 0, myImage2);

        //信息显示
        sprintf(myInform, "图像动态窗口 透视变换  %g %g %g %g %g %g ", x_r, y_r, z_r, x_t, y_t, z_s);
        LCD_ShowString(100, 310, myInform);

        //释放渲染器,避免内存泄露
        YMCV_Free_GEOM_RenderFree(myRender1);
        //用释放图像
        YMCV_Free_ImgFree(myImage2);

        Delay(50);
        //PAUSE();
    }


    YMCV_Free_ImgFree(myImage0);

    PAUSE();
    LCD_Destory();
    return 0;
}





