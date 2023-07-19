#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_MatchingCal.h"
#include"../YMCV/CORE/YMCV_AreaFillCal.h"

/*******************************************************************************************
    *
    * ˵��  �������� meanshift���� Ϊ����չʾ��meanshift����Ч��
    *
    ***************************************************************************************/

int main()
{
    char myInform[100];
    LCD_Init(1000, 480);
    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = (uint8*)CV_malloc1(sizeof(linglong00_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, linglong00_gray, sizeof(linglong00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg(myData, 500, 255, CVGrayType);//�Ҷ�ͼ
    //------------------ ȷ������������-------------------
    CVrect myroi = { .x = 310,.y = 100,.w = 150,.h = 150 },findroi;
    CVIMAGE faceImg = YMCV_Creat_ImgROICopyTo(myImage0, &myroi); //��������

   

    /*---------------------------- ���������� --------------------------------------------*/
    CV_TARGET_TRACK ms_track = YMCV_Creat_TargetTrack_Creat(myImage0, &myroi);//��������������
    //��ʾͼ��
    LCD_show_image(0, 0, myImage0);
    LCD_show_image(750, 0, faceImg);
    //��Ϣ��ʾ
    sprintf(myInform, "������ѡ����������");
    LCD_ShowString(500, 0, myInform);
    PAUSE();
    /*--------------------------����͸�ӱ任--��̬����------------------------------------------------*/
    //����ͼƬ0����Ⱦ����
    CV_geom_param myParam = { .perspect.Imgin_w = myImage0->width,
                              .perspect.Imgin_h = myImage0->height,
                              .perspect.wsize_type = CV_WinSize_Const,//�̶�����
                              .perspect.scale = 1.0f };

    CVIMAGE myImage2 = NULL;
    YMCV_PADDING_VAL = 100;//�հ������ֵ
    //����ͼƬ0����Ⱦ����
    myParam.perspect.Imgin_w = myImage0->width;
    myParam.perspect.Imgin_h = myImage0->height;
    myParam.perspect.wsize_type = CV_WinSize_Const;//��̬����
    myParam.perspect.scale = 1.0f;
    for (int i = 0; i < 120; i++)
    {
        
        //��������
        float x_r = 0, y_r = 0, z_r = 0, x_t = 0, y_t = 0, z_s = 1.0f;
        if (i < 20) x_r += i * 2;//x����ת��
        else if (i < 40) y_r += (i - 20) * 2;//y����ת��
        else if (i < 60) z_r -= (i - 40) * 2;//z����ת��
        else if (i < 80) x_t -= (i - 60) * 5;//x��ƽ��
        else if (i < 100) y_t -= (i - 80) * 5;//y����ƽ��
        else if (i < 120) z_s -= (i - 100) * 0.02;//�߶�

        //�޸���Ⱦ����
        myParam.perspect.x_angle = x_r;
        myParam.perspect.y_angle = y_r;
        myParam.perspect.z_angle = z_r;
        myParam.perspect.x_move = x_t;
        myParam.perspect.y_move = y_t;
        myParam.perspect.scale = z_s;

        
        CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_Perspective);//�������α任��Ⱦ��
        YMCV_GeomKernel_Load(myRender1, &myParam);//ʹ�ò������м�����Ⱦ���á���ʼ��
        myImage2 = YMCV_Image_Geom_RenderTo(myImage0, myRender1);//��Ŀ��ͼ��Ľ�����Ⱦ
        
        //��ʾͼƬ2
        LCD_show_image(500, 0, myImage2);

        //--------------------------- meanshift���� ---------------------------------------
        if (i%20==0)//ͼ�����ã�roiҲ��������
        {
            ms_track->roi = myroi;
        }
        //meanshift ƥ��
        YMCV_Gray_MeanShift_Match(ms_track, myImage2, 20);

        //���ƾ���
        color24_t mycolor = { .R = 255 };
		LCD_Draw_Rect(500 + ms_track->roi.x, ms_track->roi.y, ms_track->roi.w, ms_track->roi.h, mycolor);

        //����������ȡ
        int resx = ms_track->roi.x + ms_track->roi.w / 2;
        int resy = ms_track->roi.y + ms_track->roi.h / 2;
        //��Ϣ��ʾ
        sprintf(myInform, " ͸�ӱ任  %g %g %g %g %g %g ,meanshift���ٽ�� x %d y %d    ", x_r, y_r, z_r, x_t, y_t, z_s,resx,resy);
        LCD_ShowString(0, 310, myInform);

        //�ͷ���Ⱦ��,�����ڴ�й¶
        YMCV_Free_GEOM_RenderFree(myRender1);
        //���ͷ�ͼ��
        YMCV_Free_ImgFree(myImage2);

        Delay(60);
        //PAUSE();
    }
    //�ͷ��ڴ�
    YMCV_Free_ImgFree(faceImg);
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_TargetTrack_Free(ms_track);

    LCD_Destory();
    return 0;
}


