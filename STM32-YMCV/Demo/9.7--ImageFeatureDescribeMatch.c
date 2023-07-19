#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� ����������ƥ�� Ϊ����չʾ��brief����������ƥ�� ����Ӧ������㲢����ͼ�����
    *
    ***************************************************************************************/

void show_brief_discribe(int rx, int ry, CVVECTORS_U16 featpoints2, CVVECTORS_U16 featdesc2);

void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(mm02_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, (void*)mm02_gray, sizeof(mm02_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVGrayType);//�Ҷ�ͼ0

	  LCD_Clear(GRAY);
    LCD_show_image(stx + 0, 0, myImage0_gray);
	  int sty =  myImage0_gray->height;
    /*------------------------ SUSAN ��������ȡ ------------------------------*/
    CV_featpoint_param myparam;
    myparam.type = CV_FeatPoint_SUSAN;
    myparam.param.susan.thresh = 20;//
    CVVECTORS_U16 featpoints = YMKV_FeaturePoint_Find(myImage0_gray, &myparam);//��������
	
    /*------------------------ rotate brief������������ȡ ------------------------------*/
    CVVECTORS_U16 featdesc = YMCV_FeatureDescribe_RotateBrief_Cal(myImage0_gray, featpoints);

    //��ʾ����������
    show_brief_discribe(stx + 0, 0, featpoints, featdesc);

    //��Ϣ��ʾ
    sprintf(myInform, "SUSAN And RotateBRIEF");
    LCD_ShowString(0, 0, myInform);
    Delay(300);

    //----------------------------- ͼ�񼸺α任 -----------------------------------
     //����ͼƬ0����Ⱦ����
    CV_geom_param geoParam = { .perspect.Imgin_w = myImage0->width,
                              .perspect.Imgin_h = myImage0->height,
                              .perspect.wsize_type = CV_WinSize_Const,//�̶�����
                              .perspect.scale = 1.0f };
    //�޸���Ⱦ����
    geoParam.perspect.x_angle = 0;
    geoParam.perspect.y_angle = 0;
    geoParam.perspect.z_angle = -30;
    geoParam.perspect.x_move = 30; //ƽ�Ʊ任
    geoParam.perspect.y_move = 20;

    //�������α任��Ⱦ��
    CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_Perspective);

    //ʹ�ò������м�����Ⱦ���á���ʼ��
    YMCV_GeomKernel_Load(myRender1, &geoParam);

    //��Ŀ��ͼ��Ľ�����Ⱦ
    CVIMAGE myImage1 = YMCV_Image_Geom_RenderTo(myImage0_gray, myRender1);

    LCD_show_image(stx + 0, sty, myImage1);//��ʾͼƬ

    /*------------------------ SUSAN ������ �� rotate brief������������ȡ��ȡ ------------------------------*/
    CVVECTORS_U16 featpoints2 = YMKV_FeaturePoint_Find(myImage1, &myparam);//��������
    CVVECTORS_U16 featdesc2 = YMCV_FeatureDescribe_RotateBrief_Cal(myImage1, featpoints2);
    show_brief_discribe(stx + 0, sty, featpoints2, featdesc2);

    Delay(300);
    //---------------------------- ����ƥ�� --------------------------------------
    uint16* resultID = CV_malloc1(featdesc2->dataNum * sizeof(uint16));
    uint8* matched = CV_malloc1(featdesc2->dataNum * sizeof(uint8));
    uint8 thresh = 80;//80%
    YMCV_FeatureDescribe_Match(featdesc, featdesc2, resultID, matched, thresh);

    //��ʾƥ����
    int matchnum = 0;
    for (int i = 0; i < featdesc2->dataNum; i++)
    {
        if (matched[i])
        {
            matchnum++;
            int id1 = resultID[i];
            //��ȡ����������
            CVpoint* pp1 = (CVpoint*)(featpoints->datap + id1 * featpoints->dim);//������1
            CVpoint* pp2 = (CVpoint*)(featpoints2->datap + i * featpoints2->dim);//������2

            //����
            color24_t thiscolor = { .R = 255, };
            LCD_Point_Color_Set(thiscolor);
            LCD_Draw_Line(stx + pp1->x, pp1->y, stx + pp2->x, sty + pp2->y);
        }
    }        //��Ϣ��ʾ
    sprintf(myInform, " try to matching ...    ");
    LCD_ShowString(0, 0, myInform);
    Delay(300);
    //---------------------------------- ��ȡ�任���� --------------------------------

    CVVECTORS_U16 X1 = YMCV_Creat_Vector_U16(sizeof(CVpoint) / sizeof(uint16), matchnum);
    CVVECTORS_U16 X2 = YMCV_Creat_Vector_U16(sizeof(CVpoint) / sizeof(uint16), matchnum);
    //��ȡ��Ч��ƥ����
    for (int i = 0, matchi = 0; i < featdesc2->dataNum; i++)
    {
        if (matched[i])
        {
            int id1 = resultID[i];
            //��ȡ����������
            CVpoint* pp1 = (CVpoint*)(featpoints->datap + id1 * featpoints->dim);//������1
            CVpoint* pp2 = (CVpoint*)(featpoints2->datap + i * featpoints2->dim);//������2

            //��¼
            CVpoint* pax1 = (CVpoint*)(X1->datap + matchi * X1->dim);//�����㻺��
            CVpoint* pax2 = (CVpoint*)(X2->datap + matchi * X2->dim);
            matchi++;

            pax1->x = pp1->x; pax1->y = pp1->y;
            pax2->x = pp2->x; pax2->y = pp2->y;

        }
    }

    //ʹ��RANSAC ��������任���� ��X1 <-- X2
    CVFMAT myHmat = YMCV_FindHomography_Ransac(X1, X2,0.99,25);

    //����תΪ���α任��Ⱦ��
    CVGEOM_RENDER geoHrender = YMCV_HomographyMat_ToPerspective_GeomRender(myHmat, myImage1->width, myImage1->height, CV_WinSize_Const);//�̶�����

    //��Ŀ��ͼ��Ľ�����Ⱦ
    CVIMAGE myImage2 = YMCV_Image_Geom_RenderTo(myImage1, geoHrender);

    LCD_show_image(stx + 0, 0, myImage0_gray);//��ʾͼƬ
    LCD_show_image(stx + 0, sty + 0, myImage2);//��ʾ����ͼƬ
        //��Ϣ��ʾ  
    sprintf(myInform, "try to correction");
    LCD_ShowString(0, sty, myInform);
    Delay(800);

    //�ͷ�featpoints�ڴ�
    YMCV_Free_Vector_U16_Free(featdesc);
    YMCV_Free_Vector_U16_Free(featpoints);
    YMCV_Free_Vector_U16_Free(featdesc2);
    YMCV_Free_Vector_U16_Free(featpoints2);
    YMCV_Free_Vector_U16_Free(X1);
    YMCV_Free_Vector_U16_Free(X2);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    //�����ڴ�
    YMCV_Free_GEOM_RenderFree(myRender1);
    YMCV_Free_GEOM_RenderFree(geoHrender);
    YMCV_Free_FMatFree(myHmat);
    CV_free1(resultID);
    CV_free1(matched);

}


void show_brief_discribe(int rx,int ry,CVVECTORS_U16 featpoints2, CVVECTORS_U16 featdesc2)
{
    color24_t oldcolor = LCD_Pen_Color_Get();
    //��ʾ����������
    for (int i = 0; (featpoints2 != NULL) && (i < featpoints2->dataNum); i++)
    {
        CVpoint* pp = (CVpoint*)(featpoints2->datap + i * featpoints2->dim);//������
        CV_featdesc_type* desc = (CV_featdesc_type*)(featdesc2->datap + i * featdesc2->dim);//��������

        //�������ķ���ֱ��
        int r = 15;//����ģ��ѡ��r=15������������������ֱ���û���r = 15��Բ��
        int lendx = r * YMCV_Cos(desc->angle * YMCV_Deg2Rad);
        int lendy = r * YMCV_Sin(desc->angle * YMCV_Deg2Rad);
        color24_t thiscolor = { .B = 255 };
        LCD_Point_Color_Set(thiscolor);
        LCD_Draw_Line(rx + pp->x, ry + pp->y, rx + pp->x + lendx, ry + pp->y + lendy);
        //���Ƶ�λ��
        color24_t thiscolor2 = { .G = 255 };
        LCD_Point_Color_Set(thiscolor2);
        LCD_Draw_Circle(rx + pp->x, ry + pp->y, r);
    }

    LCD_Point_Color_Set(oldcolor);
}


