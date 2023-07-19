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
    * ˵��  �������� ED Ϊ����չʾ�˱�Ե��ȡЧ��
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
    sprintf(myInform, "ED ��Ե���ͼ�� ");
    LCD_ShowString(100, 310, myInform);
    PAUSE();    //��ͣ

    //��ԭͼ�ϻ��Ʊ�Ե����
    for (int i = 0; i < chains->cNum; i++)
    {
        CVVECTORS_U16 pixles = chains->chain[i];

        //��㵥������
        CV_ed_point pxy;
        CV_memcpy(&pxy, &pixles->datap[0], sizeof(CV_ed_point));
        color24_t rgb = { .R = 255 };
        LCD_Fill_ConstRgbRect(pxy.pos.x, pxy.pos.y, 2, 2, rgb);
        //ʣ���
        for (int pi = 1; pi < pixles->dataNum; pi++)
        {
            CV_ed_point pxy;
            CV_memcpy(&pxy, &pixles->datap[pi * pixles->dim], sizeof(CVpoint));
            color24_t rgb = { .G = 255 };
            LCD_Fill_ConstRgbRect(pxy.pos.x, pxy.pos.y, 1, 1, rgb);
        }
        PAUSE();
    }


    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_Chains_U16_Free(chains);//2D����洢����

    PAUSE();
    LCD_Destory();
    return 0;
}





