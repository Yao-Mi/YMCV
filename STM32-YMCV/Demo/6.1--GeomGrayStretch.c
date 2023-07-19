#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� ͼ�������任Ϊ����չʾ���ڽ���ֵ��Ч��
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
	
    /*--------------------------�ڽ���ֵ--------------------------------------------------*/
    CVIMAGE myImage2 = NULL;
    CV_geom_param myParam = {.stretch.Imgin_w= myImage0_gray->width,
                             .stretch.Imgin_h = myImage0_gray->height, 
                             .stretch.out_w = myImage0_gray->width, 
                             .stretch.out_h = myImage0_gray->height, };
														 
    float k = 1.0f;
    for (int i = 1; i < 10; i++)
    {
        //��ʼ������
        myParam.stretch.out_w = (int)(k * myImage0_gray->width);
        myParam.stretch.out_h = (int)(k * myImage0_gray->height);


        //�������α任��Ⱦ��
        CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_Stretch);

        //ʹ�ò������м�����Ⱦ���á���ʼ��
        YMCV_GeomKernel_Load(myRender1, &myParam);

        //��Ŀ��ͼ��Ľ�����Ⱦ
        myImage2 = YMCV_Image_Geom_RenderTo(myImage0_gray,myRender1);

        LCD_Clear(0);
        //��ʾͼƬ2
        LCD_show_image(0, sty + 0, myImage2);
			
        //��Ϣ��ʾ
        sprintf(myInform, "nearest streth k: %02.1f    ", k);
        LCD_ShowString(0, 0, myInform);
        //��ͣ300ms
		    Delay(300);

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
   
    //ͨ�ò�����ʼ��
    myParam.stretch.Imgin_w = myImage1->width;
    myParam.stretch.Imgin_h = myImage1->height;
    k = 1.0f;
    for (int i = 1; i < 10; i++)
    {
        //��ʼ������
        myParam.stretch.out_w = (int)(k * myImage1->width);
        myParam.stretch.out_h = (int)(k * myImage1->height);

        //�������α任��Ⱦ��
        CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_Stretch);

        //ʹ�ò������м�����Ⱦ���á���ʼ��
        YMCV_GeomKernel_Load(myRender1, &myParam);

        //��Ŀ��ͼ��Ľ�����Ⱦ
        myImage2 = YMCV_Image_Geom_RenderTo(myImage1, myRender1);

        LCD_Clear(0);
        //��ʾͼƬ2
        LCD_show_image(0, sty + 0, myImage2);

        //��Ϣ��ʾ
        sprintf(myInform, "nearest streth k: %02.2f   ", k);
        LCD_ShowString(0, 0, myInform);
        //��ͣ300ms
		    Delay(300);

        if (i < 4)
        {
            k *= 0.9f;
        }
        else
        {
            k *= 1.4f;
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





