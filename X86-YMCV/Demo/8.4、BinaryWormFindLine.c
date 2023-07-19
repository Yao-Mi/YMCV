#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_EdgeCal.h"
#include"../YMCV/CORE/YMCV_FeatCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� ����������㷨Ϊ����չʾ��ֱ�߼��Ч�������㷨Ϊ�����㷨����ҵ���֪
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 450);
    color24_t pencolor = { .R = 255 };
    LCD_Point_Color_Set(pencolor);//���û�����ɫ

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(mm00_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, mm00_gray, sizeof(mm00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//�Ҷ�ͼ0


    /*--------------------------Canny��Ե���--------------------------------------------------*/
    //���������ֵͼ��
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);//��ֵͼ
    CV_edge_param myParam = { .EdgeType = CV_Canny,.Thresh.canny.StrongT = 100,.Thresh.canny.WeakT = 40 };

    YMCV_Gray_EdgeDetect(myImage0, myImage2, &myParam);//��Ե���

    LCD_Clear(0);
    LCD_show_image(0, 0, myImage0);//��ʾͼƬ0
    LCD_show_image(500, 0, myImage2);  //��ʾͼƬ2

    //��Ϣ��ʾ
    sprintf(myInform, "Canny ��Ե���ͼ�� ");
    LCD_ShowString(100, 310, myInform);
    PAUSE();    //��ͣ

    /*-------------------------- ���ֱ�߼���� --------------------------------------------------*/
    CVVECTORS_U16 lines = YMCV_WormFindLines(myImage2, 0.001, 10,1);
    if (lines != NULL)
    {
        for (int i = 0; i < lines->dataNum; i++)
        {
            CVline* thisline = lines->datap + i * lines->dim;
            LCD_Draw_Line(thisline->x1, thisline->y1, thisline->x2, thisline->y2);//�����߶�
        }
    }

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_Vector_U16_Free(lines);

    PAUSE();
    LCD_Destory();
    return 0;
}





