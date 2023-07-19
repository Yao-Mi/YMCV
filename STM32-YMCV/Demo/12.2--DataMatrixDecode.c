#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  ������չʾ�� DM�����
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
	  LCD_Clear(GRAY);
	
    color24_t rgbchv = { .R = 255 }; LCD_Point_Color_Set(rgbchv);
    //��Ϣ��ʾ
    sprintf(myInform, "DataMatrix Decode ");
    LCD_ShowString(0, 200, myInform);
    /*----------------------------------- ͼ����ת ------------------------------------------*/
    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(DataMatrix_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, (void*)DataMatrix_gray, sizeof(DataMatrix_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVGrayType);//�Ҷ�ͼ0

    CV_geom_param myParam = { .rotate.Imgin_w = myImage0_gray->width,
                            .rotate.Imgin_h = myImage0_gray->height,
                            .rotate.angle = -15,//15��
                            .rotate.out_w = myImage0_gray->width ,//�̶�������Ҫ���������С
                            .rotate.out_h = myImage0_gray->height };
    YMCV_PADDING_VAL = 100;//�հ������ֵ

    CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_RotateWconst);//�������α任��Ⱦ��
    YMCV_GeomKernel_Load(myRender1, &myParam);//ʹ�ò������м�����Ⱦ���á���ʼ��
    CVIMAGE myImage1 = YMCV_Image_Geom_RenderTo(myImage0_gray, myRender1);//��Ŀ��ͼ��Ľ�����Ⱦ

    //��ʾͼƬ
    LCD_show_image(0, 0, myImage1);

    // DataMatrixʶ��
    CVVECTORS_U16 result = YMCV_Gray_Find_DataMatrices(myImage1, 200);

    //��ʾ���
    for (int i = 0; (result != NULL) && i < result->dataNum; i++)
    {
        CV_datamatrices_data* tda = (CV_datamatrices_data*)&result->datap[i * result->dim];

        color24_t rgbcv = { .G = 255 }; LCD_Point_Color_Set(rgbcv);
        //���ƾ��ο�
        LCD_Draw_Line(tda->corners[0].x, tda->corners[0].y, tda->corners[1].x, tda->corners[1].y);// 0 -> 1
        LCD_Draw_Line(tda->corners[1].x, tda->corners[1].y, tda->corners[2].x, tda->corners[2].y);// 1 -> 2
        LCD_Draw_Line(tda->corners[2].x, tda->corners[2].y, tda->corners[3].x, tda->corners[3].y);// 2 -> 3
        LCD_Draw_Line(tda->corners[3].x, tda->corners[3].y, tda->corners[0].x, tda->corners[0].y);// 3 -> 0

        // 0 -> 1 -> 2 -> 3 : �ڣ��ң��֣���ǳ�죬��
        rgbcv = (color24_t){ .R = 0 }; LCD_Fill_ConstRgbRect(tda->corners[0].x - 2, tda->corners[0].y - 2, 5, 5, rgbcv);
        rgbcv = (color24_t){ .R = 100 }; LCD_Fill_ConstRgbRect(tda->corners[1].x - 2, tda->corners[1].y - 2, 5, 5, rgbcv);
        rgbcv = (color24_t){ .R = 180 }; LCD_Fill_ConstRgbRect(tda->corners[2].x - 2, tda->corners[2].y - 2, 5, 5, rgbcv);
        rgbcv = (color24_t){ .R = 255 }; LCD_Fill_ConstRgbRect(tda->corners[3].x - 2, tda->corners[3].y - 2, 5, 5, rgbcv);

        //�������� 
        rgbcv = (color24_t){ .G = 150 };
        int centerx = tda->rect.x + tda->rect.w / 2; //��Ӿ��ο������
        int centery = tda->rect.y + tda->rect.h / 2;
        LCD_Fill_ConstRgbRect(centerx - 2, centery - 2, 5, 5, rgbcv);
        LCD_Draw_Rect(tda->rect.x,tda->rect.y, tda->rect.w, tda->rect.h, rgbcv);//��Ӿ��ο�

        // DM�������
        printf("DM������:%s\n����Ϊ%d���ַ�����������\\0�� ", tda->payload, tda->payload_len);

        //�ǶȲ���
        printf(" �Ƕ� %d ", tda->rotation);
    }

    
		//�ͷ��ڴ�
		YMCV_Free_Vector_U16_Free(result);
    YMCV_Free_GEOM_RenderFree(myRender1);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage1);

    Delay(300);
}

