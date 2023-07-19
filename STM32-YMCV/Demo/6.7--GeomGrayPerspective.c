#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� ͼ��Ͷ�ӱ任Ϊ����չʾ��ͼƬ�任Ч��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int sty = 16*4;
    char myInform[100];
	  
    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage0_gray = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_gray, CV_Rgb16ToGray);//rgbת�Ҷ�ͼ

		//��ʾͼƬ0
    LCD_Clear(0);
		LCD_show_image(0, sty + 0, myImage0_gray);
		//��ͣ500ms
		Delay(500);
	
    /*--------------------------����͸�ӱ任--------------------------------------------------*/
    CVIMAGE myImage2 = NULL;
    YMCV_PADDING_VAL = 100;//�հ������ֵ
    //����ͼƬ0����Ⱦ����
    CV_geom_param myParam = { .perspect.Imgin_w = myImage0_gray->width,
                              .perspect.Imgin_h = myImage0_gray->height,
                              .perspect.wsize_type = CV_WinSize_Const,//�̶�����
                              .perspect.scale=1.0f };

    for (int i = 0; i < 120; i++)
    {   
        //��������
        float x_r = 0, y_r = 0, z_r = 0, x_t = 0, y_t = 0, z_s = 0.5;
        if (i < 20) x_r += i * 5;//x����ת��
        else if (i < 40) y_r += (i - 20) * 5;//y����ת��
        else if (i < 60) z_r += (i - 40) * 5;//z����ת��
        else if (i < 80) x_t += (i - 60) * 2;//x��ƽ��
        else if (i < 100) y_t += (i - 80) * 2;//y����ƽ��
        else if (i < 120) z_s += (i - 100) * 0.1;//�߶�

        //�޸���Ⱦ����
        myParam.perspect.x_angle = x_r;
        myParam.perspect.y_angle = y_r;
        myParam.perspect.z_angle = z_r;
        myParam.perspect.x_move = x_t;
        myParam.perspect.y_move = y_t;
        myParam.perspect.scale = z_s;

        //�������α任��Ⱦ��
        CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_Perspective);

        //ʹ�ò������м�����Ⱦ���á���ʼ��
        YMCV_GeomKernel_Load(myRender1, &myParam);

        //��Ŀ��ͼ��Ľ�����Ⱦ
        myImage2 = YMCV_Image_Geom_RenderTo(myImage0_gray, myRender1);
        
        //��ʾͼƬ2
        LCD_show_image(0, sty + 0, myImage2);

        //��Ϣ��ʾ
        sprintf(myInform, "constWin perspective:\n   rotate:%g %g %g   \n   move:%g %g\n   size:%g  ",
				                  x_r, y_r, z_r,x_t,y_t,z_s);
        LCD_ShowString(0, 0, myInform);


        Delay(50);
				
        //�ͷ���Ⱦ��,�����ڴ�й¶
        YMCV_Free_GEOM_RenderFree(myRender1);
        //���ͷ�ͼ��
        YMCV_Free_ImgFree(myImage2);
    }
 

    /*--------------------------����͸�ӱ任--��̬����------------------------------------------------*/

		YMCV_PADDING_VAL = 100;//�հ������ֵ
    //����ͼƬ0����Ⱦ����
    myParam.perspect.Imgin_w = myImage0_gray->width;
    myParam.perspect.Imgin_h = myImage0_gray->height;
    myParam.perspect.wsize_type = CV_WinSize_Auto;//��̬����
    myParam.perspect.scale = 1.0f;

    LCD_Clear(0);//����
    for (int i = 0; i < 120; i++)
    {
        //��������
        float x_r = 0, y_r = 0, z_r = 0, x_t = 0, y_t = 0, z_s = 0.5;
        if (i < 20) x_r += i * 5;//x����ת��
        else if (i < 40) y_r += (i - 20) * 5;//y����ת��
        else if (i < 60) z_r += (i - 40) * 5;//z����ת��
        else if (i < 80) x_t += (i - 60) * 2;//x��ƽ��
        else if (i < 100) y_t += (i - 80) * 2;//y����ƽ��
        else if (i < 120) z_s += (i - 100) * 0.05;//�߶�

        //�޸���Ⱦ����
        myParam.perspect.x_angle = x_r;
        myParam.perspect.y_angle = y_r;
        myParam.perspect.z_angle = z_r;
        myParam.perspect.x_move = x_t;
        myParam.perspect.y_move = y_t;
        myParam.perspect.scale = z_s;

        //�������α任��Ⱦ��
        CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_Perspective);

        //ʹ�ò������м�����Ⱦ���á���ʼ��
        YMCV_GeomKernel_Load(myRender1, &myParam);

        //��Ŀ��ͼ��Ľ�����Ⱦ
        myImage2 = YMCV_Image_Geom_RenderTo(myImage0_gray, myRender1);

        //��ʾͼƬ2
        LCD_show_image(0, sty + 0, myImage2);

        //��Ϣ��ʾ
        sprintf(myInform, "autoWin perspective:\n   rotate:%g %g %g   \n   move:%g %g\n   size:%g  ",
				                  x_r, y_r, z_r,x_t,y_t,z_s);
        LCD_ShowString(0, 0, myInform);
				
        Delay(50);
				//����
				LCD_Fill_ConstRgbRect(0,sty + 0,myImage2->width,myImage2->height,(color24_t){0});
        //�ͷ���Ⱦ��,�����ڴ�й¶
        YMCV_Free_GEOM_RenderFree(myRender1);
        //���ͷ�ͼ��
        YMCV_Free_ImgFree(myImage2);

    }

    YMCV_Free_ImgFree(myImage0_gray);
}





