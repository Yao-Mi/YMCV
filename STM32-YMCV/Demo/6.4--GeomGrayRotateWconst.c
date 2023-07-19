#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� ͼ����ת�任Ϊ����չʾ��ͼƬ��ת�Ĺ̶��������Ч��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int sty = 16;
    char myInform[100];
	
    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage0_gray = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_gray, CV_Rgb16ToGray);//rgbת�Ҷ�ͼ

    uint8* myData1 = CV_malloc1(sizeof(xgmm_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData1, (void*)xgmm_gray, sizeof(xgmm_gray));
    CVIMAGE myImage1 = YMCV_Creat_DataToImg((uint8*)myData1, 50, 75, CVGrayType);//�Ҷ�ͼ1

		//��ʾͼƬ0
		LCD_show_image(0, sty + 0, myImage0_gray);
		//��ͣ500ms
		Delay(500);

    /*--------------------------��ת�Զ����Ų�ֵ--------------------------------------------------*/
    CVIMAGE myImage2 = NULL;

    CV_geom_param myParam = { .rotate.Imgin_w = myImage0_gray->width,
                              .rotate.Imgin_h = myImage0_gray->height,
                              .rotate.angle = 30,//30��
                              .rotate.out_w= myImage0_gray->width ,//�̶�������Ҫ���������С
                              .rotate.out_h= myImage0_gray->height };
    float k = 1.0f;
    YMCV_PADDING_VAL = 50;//�հ������ֵ

    for (int i = 1; i < 10; i++)
    {
        //����������ڴ�С
        myParam.rotate.out_w = (int)(k * myImage0_gray->width);
        myParam.rotate.out_h = (int)(k * myImage0_gray->height);

        //�������α任��Ⱦ��
        CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_RotateWconst);

        //ʹ�ò������м�����Ⱦ���á���ʼ��
        YMCV_GeomKernel_Load(myRender1, &myParam);

        //��Ŀ��ͼ��Ľ�����Ⱦ
        myImage2 = YMCV_Image_Geom_RenderTo(myImage0_gray, myRender1);


        LCD_Clear(0);
        //��ʾͼƬ2
        LCD_show_image(0, sty + 0, myImage2);

        //��Ϣ��ʾ
			  sprintf(myInform, "outImage size:(%03d %03d) ", myParam.rotate.out_w, myParam.rotate.out_h);
        LCD_ShowString(0, 0, myInform);
        //��ͣ500ms
		    Delay(500);

        if (i < 4)
        {
            k -= 0.2f;
        }
        else
        {
            k += 0.2f;
        }
        //�ͷ���Ⱦ��
        YMCV_Free_GEOM_RenderFree(myRender1);
        //���ͷ�ͼ�񣬱����´��õ�ʱ�����ڴ�й©
        YMCV_Free_ImgFree(myImage2);
    }

    k = 0.0f;
    //���ض�ͼƬ1����Ⱦ����
    myParam.rotate.Imgin_w = myImage1->width;
    myParam.rotate.Imgin_h = myImage1->height;
    myParam.rotate.angle = 0;//0��
    //�̶�������Ҫ���������С
    myParam.rotate.out_w = 200;
    myParam.rotate.out_h = 200;

    for (int i = 1; i < 10; i++)
    {
        //������ת�Ƕ�
        myParam.rotate.angle = k;
        //�������α任��Ⱦ��
        CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_RotateWconst);

        //ʹ�ò������м�����Ⱦ���á���ʼ��
        YMCV_GeomKernel_Load(myRender1, &myParam);

        //��Ŀ��ͼ��Ľ�����Ⱦ
        myImage2 = YMCV_Image_Geom_RenderTo(myImage1, myRender1);

        LCD_Clear(0);
        //��ʾͼƬ2
        LCD_show_image(0, sty + 0, myImage2);

        //��Ϣ��ʾ
        sprintf(myInform, "constWin rotate:%02.1fdegree", k);
        LCD_ShowString(0, 0, myInform);
        //��ͣ500ms
		    Delay(500);

        if (i < 4)
        {
            k += 20;
        }
        else
        {
            k -= 20;
        }
        //�ͷ���Ⱦ��
        YMCV_Free_GEOM_RenderFree(myRender1);
        //���ͷ�ͼ�񣬱����´��õ�ʱ�����ڴ�й©
        YMCV_Free_ImgFree(myImage2);
    }
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage1);

}





