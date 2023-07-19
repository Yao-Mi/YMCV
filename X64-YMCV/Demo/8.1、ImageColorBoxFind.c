#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_FeatCal.h"
#include"../YMCV/CORE/YMCV_PixConv.h"
#include <stdlib.h>

/*******************************************************************************************
    *
    * ˵��  �������� ɫ�����Ϊ����չʾ��bolb��ȡ��Ч��
    *
    ***************************************************************************************/
int main()
{
    int i;
    char myInform[100];
    LCD_Init(1000, 450);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(mm00_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, mm00_gray, sizeof(mm00_gray));
    uint8* myData2 = CV_malloc1(sizeof(mm00));
    CV_memcpy(myData2, mm00, sizeof(mm00));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//�Ҷ�ͼ0
    CVIMAGE myImage2 = YMCV_Creat_DataToImg((uint8*)myData2, 250, 140, CVRgb16Type);


    /*-------------------------- ���ӵ����--------------------------------------------------*/
    LCD_Clear(0);

    //��ʾԭͼ
    LCD_show_image(0, 0, myImage0);
    color24_t thiscolor = { .G = 255 };

    /*------------------------ �Ҷ�ɫ����ȡ ------------------------------*/

    CVcolor minVal,maxVal;
    minVal.gray = 100;
    maxVal.gray = 150;

    CVVECTORS_U16 blobs= YMCV_Image_ColorBox_Find(myImage0, minVal, maxVal, 100,1);
    LCD_show_image(300, 0, myImage0);
    //��ʾblob��box
    for (int i = 0; (blobs!=NULL)&&(i < blobs->dataNum); i++)
    {
        CVblob* blobp = blobs->datap + i * blobs->dim;//blobp= &data[i]
        LCD_Draw_Rect(300+blobp->box.x, blobp->box.y, blobp->box.w, blobp->box.h, thiscolor);
    }

    //�ͷ�blob�ڴ�
    YMCV_Free_Vector_U16_Free(blobs);
    
    //��Ϣ��ʾ
    sprintf(myInform, "�Ҷ�����ɫ��boxͼ  ");
    LCD_ShowString(400, 310, myInform);
    PAUSE();

    /*------------------------ ��ɫɫ����ȡ ------------------------------*/
    minVal.rgb16 = YMCV_Rgb3ToRgb565(100, 60, 40);
    maxVal.rgb16 = YMCV_Rgb3ToRgb565(200, 150, 100);
    blobs = YMCV_Image_ColorBox_Find(myImage2, minVal, maxVal, 100,1);
    LCD_show_image(600, 0, myImage2);
    //��ʾblob��box
    for (int i = 0; (blobs != NULL) && (i < blobs->dataNum); i++)
    {
        CVblob* blobp = blobs->datap + i * blobs->dim;//blobp= &data[i]
        LCD_Draw_Rect(600 + blobp->box.x, blobp->box.y, blobp->box.w, blobp->box.h, thiscolor);
    }
    //��Ϣ��ʾ
    sprintf(myInform, "��ɫͼɫ��boxͼ  ");
    LCD_ShowString(700, 310, myInform);
    //�ͷ�blob�ڴ�
    YMCV_Free_Vector_U16_Free(blobs);

    //�ͷ��ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage2);

    PAUSE();
    LCD_Destory();
    return 0;
}

