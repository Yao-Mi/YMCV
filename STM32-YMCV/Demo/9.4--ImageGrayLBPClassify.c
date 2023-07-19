#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� LBP ��������Ϊ����չʾ �������� ���
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(luojiuchuan2_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, (void*)luojiuchuan2_gray, sizeof(luojiuchuan2_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVGrayType);//�Ҷ�ͼ

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    myData = CV_malloc1(sizeof(luojiuchuan_112x112_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, (void*)luojiuchuan_112x112_gray, sizeof(luojiuchuan_112x112_gray));
    CVIMAGE myImage1 = YMCV_Creat_DataToImg((uint8*)myData, 112, 112, CVGrayType);//�Ҷ�ͼ0

    /*-------------------------- CasCade ������� --------------------------------------------------*/
    CV_cascade facedect;
    YMCV_Load_HaarCascade(&facedect, "frontface");//���� ����(��ǰ) ���������

    CVVECTORS_U16 result = YMCV_HaarCascade_Detect_Objects(myImage0_gray, &facedect, 1.0f);

    //��ʾͼƬ
	  LCD_Clear(GRAY);
    LCD_show_image(stx + 0, 16, myImage0_gray);
    //����������
    for (int i = 0; (result != NULL) && (i < result->dataNum); i++)
    {
        CVrect* recp = (CVrect*)(&result->datap[i * result->dim]);
        //���ƾ��ο�
        color24_t rgbval = { .R = 255 };
        LCD_Draw_Rect(stx + recp->x, 16+recp->y, recp->w, recp->h, rgbval);
    }
    //��Ϣ��ʾ
    sprintf(myInform, " Haar Cascade front face ");
    LCD_ShowString(0, 0, myInform);
    //��ͣ
    Delay(500);


    /*-------------------------����LBPֱ��ͼ�������--------------------------------------------------*/

    //��������ͼƬ�޶����Ϊ112��112��֧��orl�������ݼ���92��112�Ĵ�С
    CV_FACE_LBP_DESC lbpDescribe = YMCV_Face_UniformLBP_Describe(myImage1);

    LCD_Clear(GRAY);
    LCD_show_image(0, 0, myImage1);//��ʾͼƬ0
    LCDLbpHistShow(lbpDescribe, 112, 0);//��ʾLBP������
		int sty = myImage1->height;
    //��Ϣ��ʾ
    sprintf(myInform, " face LBP feature");
    LCD_ShowString(0, 0, myInform);//
    Delay(300);

    //float32 mindis = 1.5f;//����1.5���費���
		float32 mindis = 30.5f;//����Ϊ��Ч�����ô�һЩ
    int  mini = -1;
    //��ȡ��Ƭ�е�����
    for (int i = 0; (result != NULL) && (i < result->dataNum); i++)
    {
        CVrect* recp = (CVrect*)(&result->datap[i * result->dim]);
        //��ȡͼƬ
        CVIMAGE faceimg = YMCV_Creat_ImgROICopyTo(myImage0_gray, recp);

        //������������112��С
        float scale_k = 112.0f / ((recp->w> recp->h)? recp->w: recp->h);//����������Ϊ112
        if (scale_k < 1.0f)
        {
            CV_geom_param myParam = { .stretch.Imgin_w = faceimg->width,
                                      .stretch.Imgin_h = faceimg->height,
                                      .stretch.out_w = scale_k * faceimg->width,
                                      .stretch.out_h = scale_k * faceimg->height, };
            //�������α任��Ⱦ��
            CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_StretchL2);//˫���Բ�ֵ
            YMCV_GeomKernel_Load(myRender1, &myParam);//ʹ�ò������м�����Ⱦ���á���ʼ��
            CVIMAGE myImage2 = YMCV_Image_Geom_RenderTo(faceimg, myRender1);//��Ⱦ

            //�ͷ�ԭ����ͼƬ�ڴ�
            YMCV_Free_ImgFree(faceimg);
            //����������С��ͼƬ���¸���
            faceimg = myImage2;
        }

        //LBP������ȡ
        CV_FACE_LBP_DESC facelbp = YMCV_Face_UniformLBP_Describe(faceimg);

        LCD_show_image(0, sty, faceimg);//��ʾͼƬ0
				Delay(300);
        LCDLbpHistShow(facelbp, 120,sty);//��ʾLBP������

        //����ֱ��ͼ��������ƥ��
        float32 dis = YMCV_Face_LBP_Hist_X2_Distance(lbpDescribe, facelbp);//�����ȶ�
        sprintf(myInform, " chi-squared dis %5.2f ",dis);//�����������
        LCD_ShowString(0, sty, myInform);//

        //ȡ��С������Ϊƥ����
        if (mindis > dis)
        {
            mindis = dis;
            mini = i;
        }

        //�ͷ��ڴ�
        YMCV_Free_ImgFree(faceimg);
        YMCV_Free_FaceLbp_Hist_free(facelbp);
        Delay(500);
    }

    //��ʾƥ����
		LCD_Clear(GRAY);
    LCD_show_image(0, 0, myImage1);//ģ��
    LCD_show_image(stx+0, sty, myImage0_gray);
    if (mini != -1)
    {
        color24_t rgbval = { .R = 255 };
        LCD_Draw_Rect(0, 0, myImage1->width, myImage1->height, rgbval);
        
        //���ƽ�����ο�
        CVrect* recp = (CVrect*)(&result->datap[mini * result->dim]);
        color24_t rgbval2 = { .G = 255 };
        LCD_Draw_Rect(stx+recp->x, sty+recp->y, recp->w, recp->h, rgbval2);


        LCD_Point_Color_Set(rgbval);
        sprintf(myInform, "LuoJiuC");
        LCD_ShowString(stx+recp->x - 16 + recp->w/2, sty+recp->y - 16 + recp->h/2, myInform);
    } 
//    sprintf(myInform, "match result");
//    LCD_ShowString(200, 420, myInform);//

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_Vector_U16_Free(result);
    YMCV_Free_FaceLbp_Hist_free(lbpDescribe);//�����ڴ��ͷ�

    Delay(500);
}


