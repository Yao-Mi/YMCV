#include "../SDL_LCD/SDL_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_MatchingCal.h"
#include"../YMCV/CORE/YMCV_AreaFillCal.h"

/*******************************************************************************************
    *
    * 说明  该例程以 粒子滤波跟踪 为例，展示反投影图和粒子滤波跟踪效果
    *
    ***************************************************************************************/

int main()
{
    char myInform[100];
    LCD_Init(1000, 480);
    //为了不破坏原始数据，重新申请数据内存,并将数据封装为图片格式 
    uint8* myData = (uint8*)CV_malloc1(sizeof(linglong00_gray));//必须采用CV_malloc1为图片数据申请内存
    CV_memcpy(myData, linglong00_gray, sizeof(linglong00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg(myData, 500, 255, CVGrayType);//灰度图
    //------------------ 确定待跟踪区域-------------------
    CVrect myroi = { .x = 310,.y = 100,.w = 150,.h = 150 }, findroi;
    CVIMAGE faceImg = YMCV_Creat_ImgROICopyTo(myImage0, &myroi); //脸部区域
    //直方图数组
    int32 myImage1Hist[256];
    //获取图像直方图
    YMCV_Image_Hist_Get(faceImg, myImage1Hist, 256, PIXChannelNum1);
    CVIMAGE myImage0bk = YMCV_Creat_Img_BackUp(myImage0);
    //image1直方图反投影到image0 并显示
    YMCV_GrayHist_BackProject(myImage1Hist, 256, myImage0bk);

    /*---------------------------- 创建跟踪器 --------------------------------------------*/
    CV_PARTICLE_TRACK pt_track = YMCV_Creat_ParticleTrack_Creat(myImage0bk, &myroi,100,10,0.1f);//创建脸部跟踪器，粒子太少效果不好
    //显示图像
    LCD_show_image(0, 0, myImage0);
    LCD_show_image(500, 0, myImage0bk);

    color24_t mycolorc = { .R = 255 };
    LCD_Draw_Rect( myroi.x, myroi.y, myroi.w, myroi.h, mycolorc);
    //信息显示
    sprintf(myInform, "跟踪器选中脸部区域");
    LCD_ShowString(500, 0, myInform);
    PAUSE();
    /*--------------------------进行透视变换--静态窗口------------------------------------------------*/
    //加载图片0的渲染参数
    CV_geom_param myParam = { .perspect.Imgin_w = myImage0->width,
                              .perspect.Imgin_h = myImage0->height,
                              .perspect.wsize_type = CV_WinSize_Const,//固定窗口
                              .perspect.scale = 1.0f };

    CVIMAGE myImage2 = NULL;
    YMCV_PADDING_VAL = 100;//空白区填充值
    //加载图片0的渲染参数
    myParam.perspect.Imgin_w = myImage0->width;
    myParam.perspect.Imgin_h = myImage0->height;
    myParam.perspect.wsize_type = CV_WinSize_Const;//静态窗口
    myParam.perspect.scale = 1.0f;

    for (int i = 0; i < 120; i++)
    {

        //参数设置
        float x_r = 0, y_r = 0, z_r = 0, x_t = 0, y_t = 0, z_s = 1.0f;
        if (i < 10) x_r += i * 2;//x轴旋转角
        else if (i < 20) x_r += (20 - i) * 2;
        else if (i < 30) y_r += (i - 20) * 2;//y轴旋转角
        else if (i < 40) y_r += (40 - i) * 2;
        else if (i < 50) z_r -= (i - 40) * 2;//z轴旋转角
        else if (i < 60) z_r -= (60 - i) * 2;
        else if (i < 70) x_t -= (i - 60) * 5;//x轴平移
        else if (i < 80) x_t -= (80 - i) * 5;
        else if (i < 90) y_t -= (i - 80) * 5;//y轴旋平移
        else if (i < 100) y_t -= (100 - i) * 5;
        else if (i < 120) z_s -= (i - 100) * 0.02;//尺度
        else if (i < 120) z_s -= (120 - i) * 0.02;

        //修改渲染参数
        myParam.perspect.x_angle = x_r;
        myParam.perspect.y_angle = y_r;
        myParam.perspect.z_angle = z_r;
        myParam.perspect.x_move = x_t;
        myParam.perspect.y_move = y_t;
        myParam.perspect.scale = z_s;


        CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_Perspective);//创建几何变换渲染器
        YMCV_GeomKernel_Load(myRender1, &myParam);//使用参数进行几何渲染配置、初始化
        myImage2 = YMCV_Image_Geom_RenderTo(myImage0, myRender1);//将目标图像的进行渲染

        //直方图反投影
        YMCV_GrayHist_BackProject(myImage1Hist, 256, myImage2);
        //显示图片2
        LCD_show_image(500, 0, myImage2);

        //--------------------------- 粒子滤波跟踪 ---------------------------------------
  
        //跟踪成功
        if (YMCV_GrayParticleTracking(pt_track, myImage2))
        {
            //绘制矩形
            color24_t mycolor = { .R = 255 };
            LCD_Draw_Rect(500 + pt_track->result.x, pt_track->result.y, pt_track->result.w, pt_track->result.h, mycolor);

            //中心坐标提取
            int resx = pt_track->result.x + pt_track->result.w / 2;
            int resy = pt_track->result.y + pt_track->result.h / 2;

            //信息显示
            sprintf(myInform, " 透视变换  %g %g %g %g %g %g ,粒子跟踪结果 x %d y %d    ", x_r, y_r, z_r, x_t, y_t, z_s, resx, resy);
        }
        else
        {
            //信息显示
            sprintf(myInform, " 透视变换  %g %g %g %g %g %g ,粒子跟踪结果：     失败      ", x_r, y_r, z_r, x_t, y_t, z_s);
        }
        LCD_ShowString(0, 310, myInform);

        //绘制每个粒子
        for (int ik = 0; ik < pt_track->particleNum; ik++)
        { 
           //绘制矩形
           color24_t mycolor = { .G = 255 };
           LCD_Draw_Rect(500 + pt_track->states[ik].x - pt_track->states[ik].Hx, pt_track->states[ik].y - pt_track->states[ik].Hy,
               2* pt_track->states[ik].Hx+1, 2 * pt_track->states[ik].Hy + 1, mycolor);
        }

        //释放渲染器,避免内存泄露
        YMCV_Free_GEOM_RenderFree(myRender1);
        //用释放图像
        YMCV_Free_ImgFree(myImage2);

        Delay(60);
        //PAUSE();
    }
    //释放内存
    YMCV_Free_ImgFree(faceImg);
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ParticleTrack_Free(pt_track);

    LCD_Destory();
    return 0;
}


