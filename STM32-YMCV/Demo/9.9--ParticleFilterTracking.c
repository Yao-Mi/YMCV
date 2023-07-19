#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� �����˲����� Ϊ����չʾ��ͶӰͼ�������˲�����Ч��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 0;
    char myInform[100];
	
    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = (uint8*)CV_malloc1(sizeof(mm02_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, (void*)mm02_gray, sizeof(mm02_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg(myData, 160, 120, CVGrayType);//�Ҷ�ͼ
    //------------------ ȷ������������-------------------
    CVrect myroi = { .x = 30,.y = 30,.w = 40,.h = 40 }, findroi;
    CVIMAGE trackImg = YMCV_Creat_ImgROICopyTo(myImage0_gray, &myroi); //��������
    //ֱ��ͼ����
    int32 myImage1Hist[256];
    //��ȡͼ��ֱ��ͼ
    YMCV_Image_Hist_Get(trackImg, myImage1Hist, 256, PIXChannelNum1);
    CVIMAGE myImage0bk = YMCV_Creat_Img_BackUp(myImage0_gray);
    //image1ֱ��ͼ��ͶӰ��image0 ����ʾ
    YMCV_GrayHist_BackProject(myImage1Hist, 256, myImage0bk);

    /*---------------------------- ���������� --------------------------------------------*/
    CV_PARTICLE_TRACK pt_track = YMCV_Creat_ParticleTrack_Creat(myImage0bk, &myroi,100,10,0.1f);//��������������������̫��Ч������
    //��ʾͼ��
    LCD_show_image(stx + 0, 0, myImage0_gray);
    LCD_show_image(stx + myImage0_gray->width, 0, trackImg);
		//��Ϣ��ʾ
    sprintf(myInform, "chose track area");
    LCD_ShowString(0, 0, myInform);
		Delay(300);
    LCD_show_image(stx + 0, 0, myImage0bk);
		int sty = myImage0_gray->height;

    color24_t mycolorc = { .R = 255 };
    LCD_Draw_Rect(stx + myroi.x, myroi.y, myroi.w, myroi.h, mycolorc);
    //��Ϣ��ʾ
    sprintf(myInform, "back Project image");
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
        if (i < 10) x_r += i * 2;//x����ת��
        else if (i < 20) x_r += (20 - i) * 2;
        else if (i < 30) y_r += (i - 20) * 2;//y����ת��
        else if (i < 40) y_r += (40 - i) * 2;
        else if (i < 50) z_r -= (i - 40) * 2;//z����ת��
        else if (i < 60) z_r -= (60 - i) * 2;
        else if (i < 70) x_t -= (i - 60) * 5;//x��ƽ��
        else if (i < 80) x_t -= (80 - i) * 5;
        else if (i < 90) y_t -= (i - 80) * 5;//y����ƽ��
        else if (i < 100) y_t -= (100 - i) * 5;
        else if (i < 120) z_s -= (i - 100) * 0.02;//�߶�
        else if (i < 120) z_s -= (120 - i) * 0.02;

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

        //ֱ��ͼ��ͶӰ
        YMCV_GrayHist_BackProject(myImage1Hist, 256, myImage2);
        //��ʾͼƬ2
        LCD_show_image(stx+0, sty+0, myImage2);

        //--------------------------- �����˲����� ---------------------------------------
  
        //���ٳɹ�
        if (YMCV_GrayParticleTracking(pt_track, myImage2))
        {
            //���ƾ���
            color24_t mycolor = { .R = 255 };
            LCD_Draw_Rect(stx + pt_track->result.x, sty + pt_track->result.y, pt_track->result.w, pt_track->result.h, mycolor);

            //����������ȡ
            int resx = pt_track->result.x + pt_track->result.w / 2;
            int resy = pt_track->result.y + pt_track->result.h / 2;

            //��Ϣ��ʾ
            sprintf(myInform, " Perspect  %g %g %g %g %g %g   \ntrack result x %d y %d  ", x_r, y_r, z_r, x_t, y_t, z_s, resx, resy);
				}
        else
        {
            //��Ϣ��ʾ
					  sprintf(myInform, " Perspect  %g %g %g %g %g %g   \ntrack result: failed    ", x_r, y_r, z_r, x_t, y_t, z_s);
        }
				LCD_ShowString(0, sty - 16*2, myInform);

//        //����ÿ������
//        for (int ik = 0; ik < pt_track->particleNum; ik++)
//        { 
//            //���ƾ���
//            color24_t mycolor = { .G = 255 };
//            LCD_Draw_Rect(stx + pt_track->states[ik].x - pt_track->states[ik].Hx, sty + pt_track->states[ik].y - pt_track->states[ik].Hy,
//                2* pt_track->states[ik].Hx+1, 2 * pt_track->states[ik].Hy + 1, mycolor);
//        }

        //�ͷ���Ⱦ��,�����ڴ�й¶
        YMCV_Free_GEOM_RenderFree(myRender1);
        //���ͷ�ͼ��
        YMCV_Free_ImgFree(myImage2);

        Delay(60);
        //PAUSE();
    }
    //�ͷ��ڴ�
    YMCV_Free_ImgFree(trackImg);
    YMCV_Free_ImgFree(myImage0bk);
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ParticleTrack_Free(pt_track);

}


