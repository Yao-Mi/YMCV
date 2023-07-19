#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� meanshift���� Ϊ����չʾ��meanshift����Ч��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = (uint8*)CV_malloc1(sizeof(mm02_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, (void*)mm02_gray, sizeof(mm02_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg(myData, 160, 120, CVGrayType);//�Ҷ�ͼ
    //------------------ ȷ������������-------------------
    CVrect myroi = { .x = 30,.y = 30,.w = 40,.h = 40 };
//    CVIMAGE faceImg = YMCV_Creat_ImgROICopyTo(myImage0_gray, &myroi); //��������
		
    /*---------------------------- ���������� --------------------------------------------*/
    CV_TARGET_TRACK ms_track = YMCV_Creat_TargetTrack_Creat(myImage0_gray, &myroi);//�������������
    //��ʾͼ��
    LCD_show_image(stx + 0, 0, myImage0_gray);
//    LCD_show_image(750, 0, faceImg);
		LCD_Draw_Rect(stx + myroi.x, myroi.y, myroi.w, myroi.h,(color24_t){ .R = 255 });
		int sty = myImage0_gray->height;

    //��Ϣ��ʾ
    sprintf(myInform, "chose meanshift area");
    LCD_ShowString(0, 0, myInform);
    Delay(300);
    /*--------------------------����͸�ӱ任--��̬����------------------------------------------------*/
    //����ͼƬ0����Ⱦ����
    CV_geom_param myParam = { .perspect.Imgin_w = myImage0_gray->width,
                              .perspect.Imgin_h = myImage0_gray->height,
                              .perspect.wsize_type = CV_WinSize_Const,//�̶�����
                              .perspect.scale = 1.0f };

    CVIMAGE myImage2 = NULL;
    YMCV_PADDING_VAL = 100;//�հ������ֵ
    //����ͼƬ0����Ⱦ����
    myParam.perspect.Imgin_w = myImage0_gray->width;
    myParam.perspect.Imgin_h = myImage0_gray->height;
    myParam.perspect.wsize_type = CV_WinSize_Const;//��̬����
    myParam.perspect.scale = 1.0f;
		
    for (int i = 0; i < 120; i++)
    {
        
        //��������
        float x_r = 0, y_r = 0, z_r = 0, x_t = 0, y_t = 0, z_s = 1.0f;
        if (i < 20) x_r += i * 2;//x����ת��
        else if (i < 40) y_r += (i - 20) * 2;//y����ת��
        else if (i < 60) z_r -= (i - 40) * 1;//z����ת��
        else if (i < 80) x_t -= (i - 60) * 2;//x��ƽ��
        else if (i < 100) y_t -= (i - 80) * 2;//y����ƽ��
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
        myImage2 = YMCV_Image_Geom_RenderTo(myImage0_gray, myRender1);//��Ŀ��ͼ��Ľ�����Ⱦ
        
        //��ʾͼƬ2
        LCD_show_image(stx + 0, sty, myImage2);

        //--------------------------- meanshift���� ---------------------------------------
        if ((i%20) == 0)//ͼ�����ã�roiҲ��������
        {
            ms_track->roi = myroi;
        }
        //meanshift ƥ��
        YMCV_Gray_MeanShift_Match(ms_track, myImage2, 20);

        //���ƾ���
        color24_t mycolor = { .R = 255 };
		    LCD_Draw_Rect(stx + ms_track->roi.x, sty + ms_track->roi.y, ms_track->roi.w, ms_track->roi.h, mycolor);

        //����������ȡ
        int resx = ms_track->roi.x + ms_track->roi.w / 2;
        int resy = ms_track->roi.y + ms_track->roi.h / 2;
        //��Ϣ��ʾ
        sprintf(myInform, " Perspect %g %g %g %g %g %g   \nmeanshift result: x %d y %d ", x_r, y_r, z_r, x_t, y_t, z_s,resx,resy);
        LCD_ShowString(0, sty - 16*2, myInform);

        //�ͷ���Ⱦ��,�����ڴ�й¶
        YMCV_Free_GEOM_RenderFree(myRender1);
        //���ͷ�ͼ��
        YMCV_Free_ImgFree(myImage2);

        Delay(60);
        //PAUSE();
    }
    //�ͷ��ڴ�
//    YMCV_Free_ImgFree(faceImg);
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_TargetTrack_Free(ms_track);
}


