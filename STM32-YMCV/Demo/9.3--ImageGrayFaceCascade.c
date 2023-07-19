#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� Harr CasCadeΪ����չʾ���������
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

    CVVECTORS_U16 result = YMCV_HaarCascade_Detect_Objects(myImage0_gray, &facedect, 0.2f);//������Խ���ٶ�Խ��

    //��ʾͼƬ
    LCD_show_image(stx + 0, 16, myImage0_gray);
    //����������
    for (int i = 0; (result != NULL) && (i < result->dataNum); i++)
    {
        CVrect* recp = (CVrect*)&result->datap[i * result->dim];
        //���ƾ��ο�
        color24_t rgbval = { .R = 255 };
        LCD_Draw_Rect(stx + recp->x, 16+recp->y, recp->w, recp->h, rgbval);
    }
    //��Ϣ��ʾ
    sprintf(myInform, " Haar Cascade front face ");
    LCD_ShowString(0, 0, myInform);
    //��ͣ
    Delay(500);
		
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_Vector_U16_Free(result);

}





