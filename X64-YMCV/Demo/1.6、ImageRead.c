#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/IOFILE/YMCV_IOFile.h"

/*******************************************************************************************
    *
    * ˵��  ������չʾ�� jpg��bmp��gif��avi(mjpeg����)��ͼ���ȡ
    *
    ***************************************************************************************/
int main()
{
    LCD_Init(1000, 480);
    char myInform[100];
    //��Ϣ��ʾ
    sprintf(myInform, "JPGͼƬ����  ");
    LCD_ShowString(400, 410, myInform);
    /*----------------------------------- jpg���� ------------------------------------------*/

    CVIMAGE myImage1 = YMCV_Image_Read("D:/Documents/1012_YMCV/ͼƬ�ز�/xgmm01.jpg", CVRgb16Type);
    CVIMAGE myImage0 = YMCV_Image_Read("D:/Documents/1012_YMCV/ͼƬ�ز�/xgmm01.jpg", CVGrayType);


    //��ʾͼƬ
    LCD_show_image(0, 0, myImage0);
    LCD_show_image(myImage0->width, 0, myImage1);

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);
    //��ͣ
    PAUSE();

    LCD_Clear(0);
    //��Ϣ��ʾ
    sprintf(myInform, "BMPͼƬ����  ");
    LCD_ShowString(400, 410, myInform);
    /*----------------------------------- bmp���� ------------------------------------------*/

    myImage1 = YMCV_Image_Read("D:/Documents/1012_YMCV/ͼƬ�ز�/linglong_00.bmp", CVRgb16Type);
    myImage0 = YMCV_Image_Read("D:/Documents/1012_YMCV/ͼƬ�ز�/linglong_00.bmp", CVGrayType);

    //��ʾͼƬ
    LCD_show_image(0, 0, myImage0);
    LCD_show_image(myImage0->width, 0, myImage1);

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);
    PAUSE();

    LCD_Clear(0);
    //��Ϣ��ʾ
    sprintf(myInform, "GIFͼƬ����  ");
    LCD_ShowString(400, 410, myInform);
    /*----------------------------------- gif���� ------------------------------------------*/

    //׼���������ļ� ��gifdec.h��Ĭ������Ϊrgb565����
    gd_GIF* mygif = gd_prepare_from_file("D:/Documents/1012_YMCV/ͼƬ�ز�/jntm.gif");
    //������������ӳ��Ϊͼ��
    CVimage gifimg;
    gifimg.data = mygif->canvas;
    gifimg.width = mygif->width;
    gifimg.height = mygif->height;
    gifimg.type = CVRgb16Type;//rgb565��ʽ,�����gif������ʽͳһ

    while (1)
    {
        //��ȡ��һ֡ͼ��
        gd_next_frame(mygif);
        //��ʾͼ��
        LCD_show_image(0, 0, &gifimg);
        //��ʱ
        Delay(mygif->gce.delay * 10);
        //������ֱ���˳����ټ���
        if (mygif->is_finished)
            break;
    }
    //�˳�������
    gd_quit(mygif);
    PAUSE();

    LCD_Clear(0);
    //��Ϣ��ʾ
    sprintf(myInform, "AVI��Ƶ - MJPEG����  ");
    LCD_ShowString(400, 410, myInform);
    /*----------------------------------- avi���� ------------------------------------------*/
    //��avi�ļ�����ȡ�ļ���Ϣ
    avi_t* myavi = AVI_open_input_file("D:/Documents/1012_YMCV/ͼƬ�ز�/ymtec.avi", 1);
    //������ʾ��ͼ��
    CVIMAGE aviImg = YMCV_Creat_Img_Creat(myavi->width, myavi->height, CVRgb16Type);//��ʾ��ͼ��

     //��֡��
    long tolnum = AVI_video_frames(myavi);
    //֡�� = ֡��/1��
    float fps = AVI_frame_rate(myavi);

    //��ȡ����ʼλ��
    int isfailed = AVI_set_video_position(myavi, 0);//������0λ�ô�

    if (!isfailed)
    {
        //��֡��ȡ
        for (int pos = 0; pos < tolnum; pos++)
        {
            //��ȡ��һ֡
            int keyFrame = YMCV_AVI_Decode_Next_Jpeg_Frame(myavi, aviImg);
            //��ʾͼƬ
            LCD_show_image(0, 0, aviImg);

            Delay((1000 / fps) - 50);//֡��10����ʱ 100ms �������Ҫ50ms

            if (keyFrame == 1)
            {
                //�ؼ�֡
            }
            else if (keyFrame == 0)
            {
                //�ǹؼ�֡����ͨ֡��
            }
            else if (keyFrame == -1)
            {
                //��֡����ȡ��ϣ��޷�������ȡ��
                break;
            }
        }
    }
    //�ر��ļ�
    AVI_close(myavi);

    PAUSE();
    LCD_Destory();
    return 0;
}




