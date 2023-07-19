#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_EdgeCal.h"
#include"../YMCV/CORE/YMCV_FeatCal.h"
#include"../YMCV/CORE/YMCV_BasicObjCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� ED ShapeΪ����չʾ����״ʶ��Ч��
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 550);
    color24_t pencolor = { .R = 255 };
    LCD_Point_Color_Set(pencolor);//���û�����ɫ

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(tuxin_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, tuxin_gray, sizeof(tuxin_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//�Ҷ�ͼ0

    /*-------------------------ED��Ե���--------------------------------------------------*/
    //���������ֵͼ��
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);//��ֵͼ

    CVCHAINS_U16 chains = YMCV_Fast_EdgeDrawing(myImage0, CV_Sobel_Graident, 5, myImage2);

    LCD_Clear(0);
    LCD_show_image(0, 0, myImage0);//��ʾͼƬ0
    LCD_show_image(300, 0, myImage2);  //��ʾͼƬ2

    //��Ϣ��ʾ
    sprintf(myInform, "ED�պ����� ͹�����ʶ�� ");
    LCD_ShowString(100, 310, myInform);

    /*-------------------------- EDCorner͹�Ǽ�� --------------------------------------------------*/
    CVVECTORS_U16 corner = YMCV_Fast_EdgeDrawing_ChainsCornerNum(chains, 30);
    for (int i = 0; i < corner->dataNum; i++)
    {
        CV_ed_corner* thisCorner = &corner->datap[i * corner->dim];
        //��������
        color24_t rgb = { .R = 255 };
        LCD_Fill_ConstRgbRect(thisCorner->center.x, thisCorner->center.y, 2, 2, rgb);
        //���ƽ���Բ��Զ��Բ
        rgb.G = 150;
        LCD_Point_Color_Set(rgb);
        LCD_Draw_Circle(thisCorner->center.x, thisCorner->center.y, thisCorner->min_r);//����

        rgb.B = 180;
        LCD_Point_Color_Set(rgb);
        LCD_Draw_Circle(thisCorner->center.x, thisCorner->center.y, thisCorner->max_r);//Զ��

        //��״�б�
        printf("͹������%d  ", thisCorner->CornerNum);
        switch (thisCorner->CornerNum)
        {
        case 0:
            printf("Բ��  ");
            printf("Բ�� %d,%d,�뾶%d\n", thisCorner->center.x, thisCorner->center.y, (thisCorner->max_r + thisCorner->min_r) / 2);
            break;
        case 1:
            printf("��Բ��  ");  break;
        case 2:
            printf("��Բ��  ");  break;
        default:
            printf("͹%d���� \n", thisCorner->CornerNum);  break;
        }

        CVVECTORS_U16 pixles= chains->chain[i];
        //ԭʼ�������
        for (int pi = 0; pi < pixles->dataNum; pi++)
        {
            CV_ed_point pxy;
            CV_memcpy(&pxy, &pixles->datap[pi * pixles->dim], sizeof(CVpoint));
            color24_t rgb = { .G = 255 };
            LCD_Fill_ConstRgbRect(pxy.pos.x, pxy.pos.y, 1, 1, rgb);
        }
        PAUSE();    //��ͣ
    }

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage2);

    YMCV_Free_Chains_U16_Free(chains);//2D����洢����
   
    YMCV_Free_Vector_U16_Free(corner); //ת���ڴ��ͷ�

    PAUSE();
    LCD_Destory();
    return 0;
}





