#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/IOFILE/YMCV_IOFile.h"

/*******************************************************************************************
    *
    * 说明  该例程展示了 jpg、bmp、gif、avi(mjpeg编码)的图像读取
    *
    ***************************************************************************************/
int main()
{
    LCD_Init(1000, 480);
    char myInform[100];
    //信息显示
    sprintf(myInform, "JPG图片解码  ");
    LCD_ShowString(400, 410, myInform);
    /*----------------------------------- jpg解码 ------------------------------------------*/

    CVIMAGE myImage1 = YMCV_Image_Read("D:/Documents/1012_YMCV/图片素材/xgmm01.jpg", CVRgb16Type);
    CVIMAGE myImage0 = YMCV_Image_Read("D:/Documents/1012_YMCV/图片素材/xgmm01.jpg", CVGrayType);


    //显示图片
    LCD_show_image(0, 0, myImage0);
    LCD_show_image(myImage0->width, 0, myImage1);

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);
    //暂停
    PAUSE();

    LCD_Clear(0);
    //信息显示
    sprintf(myInform, "BMP图片解码  ");
    LCD_ShowString(400, 410, myInform);
    /*----------------------------------- bmp解码 ------------------------------------------*/

    myImage1 = YMCV_Image_Read("D:/Documents/1012_YMCV/图片素材/linglong_00.bmp", CVRgb16Type);
    myImage0 = YMCV_Image_Read("D:/Documents/1012_YMCV/图片素材/linglong_00.bmp", CVGrayType);

    //显示图片
    LCD_show_image(0, 0, myImage0);
    LCD_show_image(myImage0->width, 0, myImage1);

    //使用完图片，要释放图片内存
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);
    PAUSE();

    LCD_Clear(0);
    //信息显示
    sprintf(myInform, "GIF图片解码  ");
    LCD_ShowString(400, 410, myInform);
    /*----------------------------------- gif解码 ------------------------------------------*/

    //准备解析该文件 在gifdec.h中默认设置为rgb565解析
    gd_GIF* mygif = gd_prepare_from_file("D:/Documents/1012_YMCV/图片素材/jntm.gif");
    //将解析器画布映射为图像
    CVimage gifimg;
    gifimg.data = mygif->canvas;
    gifimg.width = mygif->width;
    gifimg.height = mygif->height;
    gifimg.type = CVRgb16Type;//rgb565格式,必须和gif画布格式统一

    while (1)
    {
        //获取下一帧图像
        gd_next_frame(mygif);
        //显示图像
        LCD_show_image(0, 0, &gifimg);
        //延时
        Delay(mygif->gce.delay * 10);
        //结束，直接退出不再继续
        if (mygif->is_finished)
            break;
    }
    //退出解析器
    gd_quit(mygif);
    PAUSE();

    LCD_Clear(0);
    //信息显示
    sprintf(myInform, "AVI视频 - MJPEG解码  ");
    LCD_ShowString(400, 410, myInform);
    /*----------------------------------- avi解码 ------------------------------------------*/
    //打开avi文件，读取文件信息
    avi_t* myavi = AVI_open_input_file("D:/Documents/1012_YMCV/图片素材/ymtec.avi", 1);
    //创建显示的图像
    CVIMAGE aviImg = YMCV_Creat_Img_Creat(myavi->width, myavi->height, CVRgb16Type);//显示的图像

     //总帧数
    long tolnum = AVI_video_frames(myavi);
    //帧率 = 帧数/1秒
    float fps = AVI_frame_rate(myavi);

    //读取的起始位置
    int isfailed = AVI_set_video_position(myavi, 0);//设置在0位置处

    if (!isfailed)
    {
        //逐帧读取
        for (int pos = 0; pos < tolnum; pos++)
        {
            //获取下一帧
            int keyFrame = YMCV_AVI_Decode_Next_Jpeg_Frame(myavi, aviImg);
            //显示图片
            LCD_show_image(0, 0, aviImg);

            Delay((1000 / fps) - 50);//帧率10则延时 100ms 假设解码要50ms

            if (keyFrame == 1)
            {
                //关键帧
            }
            else if (keyFrame == 0)
            {
                //非关键帧（普通帧）
            }
            else if (keyFrame == -1)
            {
                //非帧（读取完毕，无法继续读取）
                break;
            }
        }
    }
    //关闭文件
    AVI_close(myavi);

    PAUSE();
    LCD_Destory();
    return 0;
}




