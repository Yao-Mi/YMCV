#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_MatchingCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� Harr CasCadeΪ����չʾ���������
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(740, 480);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(luojiuchuan_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, luojiuchuan_gray, sizeof(luojiuchuan_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 740, 400, CVGrayType);//�Ҷ�ͼ

    /*-------------------------- CasCade ������� --------------------------------------------------*/
    CV_cascade facedect;
    YMCV_Load_HaarCascade(&facedect, "frontface");//���� ����(��ǰ) ���������

    CVVECTORS_U16 result = YMCV_HaarCascade_Detect_Objects(myImage0, &facedect, 1.0f);

    //��ʾͼƬ
    LCD_show_image(0, 0, myImage0);
    //����������
    for (int i = 0; (result != NULL) && (i < result->dataNum); i++)
    {
        CVrect* recp = &result->datap[i * result->dim];
        //���ƾ��ο�
        color24_t rgbval = { .R = 255 };
        LCD_Draw_Rect(recp->x, recp->y, recp->w, recp->h, rgbval);
    }
    //��Ϣ��ʾ
    sprintf(myInform, " Haar Cascade ��� �������� ");
    LCD_ShowString(200, 420, myInform);
    
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_Vector_U16_Free(result);
    PAUSE();
    LCD_Destory();
    return 0;
}





