#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_Special_Effect.h"
#include"../YMCV/CORE/YMCV_BasicObjCal.h"
#include <stdlib.h>

/*******************************************************************************************
    *
    * ˵��  �������� ͼ����Ա���Ϊ����չʾ���ڿ��Ƶ������µı���Ч��
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1100, 450);
    int i = 0, j = 0;

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(huochairen_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, huochairen_gray, sizeof(huochairen_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 181, 240, CVGrayType);//�Ҷ�ͼ0

    //��ʾԭͼ
    LCD_show_image(0, 0, myImage0);
        // ȷ�����Ƶ�
    color24_t thiscolor = { .R = 255,.G = 0 };

    uint16 xpoint[] = {36, 61, 150, 128, 47, 70, 140, 114, /*91,*/ 91, 92};
    uint16 ypoint[] = {126, 82, 128, 86, 232, 176, 231, 174, /*7,*/ 57, 126};
    uint16 xpoint2[] = { 11, 66, 141, 136, 69, 65, 172, 136, /*72,*/ 104, 93 };
    uint16 ypoint2[] = { 98, 94, 131, 82, 228, 170, 200, 156, /*10,*/ 56, 126 };
    uint16 num = sizeof(xpoint) / 2;

    //uint16 xpoint[100], ypoint[100], num = 0;
    // 
    //char pflag = 0;
    //printf("׼�����ƣ�\n'c':ѡ��\n�ո񣺽�������");
    //int px = 0, py = 0, w, h;
    //while (1)
    //{
    //    char mchr;
    //   
    //    if (pflag)
    //    {
    //        //��ԭͼ�ϻ���
    //        LCD_Fill_ConstRgbRect(px - 1, py - 1, 3, 3, thiscolor);
    //    }
    // 
    //    if (LCD_GetChar(&mchr))
    //    {
    //        
    //        if (mchr == 'c')
    //        {
    //            pflag = 1;
    //            if (LCD_GetXY(0, 0, myImage0->width, myImage0->height, &px, &py))
    //            {
    //                xpoint[num] = px;
    //                ypoint[num] = py;
    //                num++;
    //                printf("ѡ��㣺%d , %d\n", px, py);
    //            }
    //        }
    //        if (mchr == ' ') { printf("��������\n"); break; }
    //    }
    //    if (num >= 100) break;
    //}
    //
    //for (int i = 0; i < num; i++)
    //{
    //    printf("%d,",xpoint[i]);
    //}
    //printf("\n");
    //for (int i = 0; i < num; i++)
    //{
    //    printf("%d,",ypoint[i]);
    //}
    //printf("\n");
    

    CVVECTORS_F32 pList = YMCV_Creat_Vector_F32(sizeof(CVfpoint) / sizeof(float32), num);
    CVVECTORS_F32 qList = YMCV_Creat_Vector_F32(sizeof(CVfpoint) / sizeof(float32), num);
    for (i = 0; i < num; i++)
    {
        //P
        CVfpoint* fpthis = pList->datap + i * pList->dim;
        fpthis->x = xpoint[i];
        fpthis->y = ypoint[i]; 
        
        //Q
        fpthis = qList->datap + i * qList->dim;
        fpthis->x = xpoint2[i];
        fpthis->y = ypoint2[i];
    }
    YMCV_PADDING_VAL = 0;//�հ������0

    //���Ա任��P��ԭʼ���Ƶ㣬Q�Ǳ任��Ŀ��Ƶ�
    CVIMAGE myImage1 = YMCV_Gray_RigidDeformation_Effect(myImage0, pList, qList);
    LCD_show_image(500, 0, myImage1);

    //��ԭͼ�ϻ���
    for (i = 0; i < num; i++)
    {
        //P
        CVfpoint* fpthis = pList->datap + i * pList->dim;
        fpthis->x = xpoint[i];
        fpthis->y = ypoint[i];
        LCD_Fill_ConstRgbRect(fpthis->x - 1, fpthis->y - 1, 3, 3, thiscolor);

        //Q
        fpthis = qList->datap + i * qList->dim;
        fpthis->x = xpoint2[i];
        fpthis->y = ypoint2[i];
        LCD_Fill_ConstRgbRect(500+fpthis->x - 1, fpthis->y - 1, 3, 3, thiscolor);
    }
    
    PAUSE();
}
