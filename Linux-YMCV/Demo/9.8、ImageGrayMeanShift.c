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
    * 说明  该例程以 meanshift跟踪 为例，展示了meanshift跟踪效果
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
    CVrect myroi = { .x = 310,.y = 100,.w = 150,.h = 150 },findroi;
    CVIMAGE faceImg = YMCV_Creat_ImgROICopyTo(myImage0, &myroi); //脸部区域

   

    /*---------------------------- 创建跟踪器 --------------------------------------------*/
    CV_TARGET_TRACK ms_track = YMCV_Creat_TargetTrack_Creat(myImage0, &myroi);//创建脸部跟踪器
    //显示图像
    LCD_show_image(0, 0, myImage0);
    LCD_show_image(750, 0, faceImg);
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
        if (i < 20) x_r += i * 2;//x轴旋转角
        else if (i < 40) y_r += (i - 20) * 2;//y轴旋转角
        else if (i < 60) z_r -= (i - 40) * 2;//z轴旋转角
        else if (i < 80) x_t -= (i - 60) * 5;//x轴平移
        else if (i < 100) y_t -= (i - 80) * 5;//y轴旋平移
        else if (i < 120) z_s -= (i - 100) * 0.02;//尺度

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
        
        //显示图片2
        LCD_show_image(500, 0, myImage2);

        //--------------------------- meanshift跟踪 ---------------------------------------
        if (i%20==0)//图像被重置，roi也进行重置
        {
            ms_track->roi = myroi;
        }
        //meanshift 匹配
        YMCV_Gray_MeanShift_Match(ms_track, myImage2, 20);

        //绘制矩形
        color24_t mycolor = { .R = 255 };
		LCD_Draw_Rect(500 + ms_track->roi.x, ms_track->roi.y, ms_track->roi.w, ms_track->roi.h, mycolor);

        //中心坐标提取
        int resx = ms_track->roi.x + ms_track->roi.w / 2;
        int resy = ms_track->roi.y + ms_track->roi.h / 2;
        //信息显示
        sprintf(myInform, " 透视变换  %g %g %g %g %g %g ,meanshift跟踪结果 x %d y %d    ", x_r, y_r, z_r, x_t, y_t, z_s,resx,resy);
        LCD_ShowString(0, 310, myInform);

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
    YMCV_Free_TargetTrack_Free(ms_track);

    LCD_Destory();
    return 0;
}


