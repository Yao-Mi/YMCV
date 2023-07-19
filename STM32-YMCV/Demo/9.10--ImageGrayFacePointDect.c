#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� Harr CasCadeΪ����չʾ40����������������
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

    /*-------------------------- CasCade ������� --------------------------------------------------*/
    CV_cascade facedect;
    YMCV_Load_HaarCascade(&facedect, "frontface");//���� ����(��ǰ) ���������

    CVVECTORS_U16 result = YMCV_HaarCascade_Detect_Objects(myImage0_gray, &facedect, 0.2f);

    /*-------------------------- ������������ --------------------------------------------------*/
    CV_facefpoint_dector facepoint_dect;
    YMCV_Load_FacePointDector(&facepoint_dect, "frontfacepoint");


    //��ʾͼƬ
    LCD_show_image(stx + 0, 0, myImage0_gray);
    //���������򲢼��������
    for (int i = 0; (result != NULL) && (i < result->dataNum); i++)
    {
        CVrect* recp = (CVrect*)&result->datap[i * result->dim];
        CVVECTORS_U16 facepts = YMCV_FacePoint_Detect_Objects(myImage0_gray, &facepoint_dect, recp, 5,100);

        LCD_FaceFeaturePoint_Show((CVpoint*)facepts->datap, stx + 0, 0);//��ʾ
        //���ƾ��ο�
        color24_t rgbval = { .R = 255 };
        LCD_Draw_Rect(stx + recp->x, recp->y, recp->w, recp->h, rgbval);
        YMCV_Free_Vector_U16_Free(facepts);
				
        Delay(300);
    }
    //��Ϣ��ʾ
    sprintf(myInform, " face 40 points ");
    LCD_ShowString(stx+0, 0, myInform);

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_Vector_U16_Free(result);
    Delay(300);
}





