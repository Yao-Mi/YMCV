#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_SegnmCal.h"
#include"../YMCV/CORE/YMCV_AreaFillCal.h"
#include <stdlib.h>

/*******************************************************************************************
    *
    * ˵��  �������� ����ɨ�������Ϊ����չʾ�˻�����������Ч��
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

    /*-------------------------- �ֶ�����TriMap--------------------------------------------------*/
    LCD_Clear(0);
    //��ʾԭͼ
    LCD_show_image(0, 0, myImage0);
    sprintf(myInform, "�ֶ����������� ");
    LCD_ShowString(0, 310, myInform);

    char pflag = 0;
    printf("׼�����ƣ�\n'c':ѡ��\n�ո񣺽�������");
    int px = 0, py = 0, w, h;

    uint16 xpoint[] = { 121, 171, 187, 174, 131, 88, 66, 65, 67, 84, 105, };
    uint16 ypoint[] = { 97, 86, 45, 22, 13, 18, 22, 38, 59, 78, 91, };
    uint16 num = sizeof(xpoint) / 2;

    //uint16 xpoint[100], ypoint[100], num = 0;

    //while (1)
    //{
    //    char mchr;
    //   
    //    if (pflag)
    //    {
    //        //��ԭͼ�ϻ���
    //        LCD_Fill_ConstRgbRect(px - 1, py - 1, 3, 3, thiscolor);
    //    }

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
    //printf("\n");
    //for (int i = 0; i < num; i++)
    //    printf("%d,", xpoint[i]);
    //printf("\n");
    //for (int i = 0; i < num; i++)
    //    printf("%d,", ypoint[i]);

    //��������
    for (i = 0; i < num - 1; i++)
    {
        LCD_Draw_Line(xpoint[i], ypoint[i], xpoint[i + 1], ypoint[i + 1]);
    }
    LCD_Draw_Line(xpoint[i], ypoint[i], xpoint[0], ypoint[0]);

    //����Ϊ�㼯
    CVpoint* myPoint = (CVpoint*)CV_malloc1(num * sizeof(CVpoint));//�㼯
    for (int i = 0; i < num; i++)
    {
        CVpoint* thiscolor = myPoint + i;
        thiscolor->x = xpoint[i];
        thiscolor->y = ypoint[i];
    }

    CVIMAGE TriMapMarkers = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVTrimapType);
    YMCV_TriMap_Init(TriMapMarkers); //trimap��ʼ��
    LCD_show_image(300, 0, TriMapMarkers);
    //��Ϣ��ʾ
    sprintf(myInform, "ԭʼTriMapͼ  ");
    LCD_ShowString(400, 310, myInform);
    PAUSE();

    CVcolor fillval;
    fillval.gray = CVTriMapMaxVl - 10;//����Ϊǰ����ֵ

    //��Trimap�����������
    YMCV_Img_Scanline_AreaFill(TriMapMarkers, myPoint, num, fillval);

    LCD_show_image(300, 0, TriMapMarkers);
    //��Ϣ��ʾ
    sprintf(myInform, "�����������ͼ  ");
    LCD_ShowString(400, 310, myInform);
    PAUSE();

    fillval.rgb16 = 0xfe00;
    //�Բ�ɫͼ�����������
    YMCV_Img_Scanline_AreaFill(myImage2, myPoint, num, fillval);
    LCD_show_image(600, 0, myImage2);
    //��Ϣ��ʾ
    sprintf(myInform, "��ɫͼ���  ");
    LCD_ShowString(700, 310, myInform);

    //�ͷ��ڴ�
    YMCV_Free_ImgFree(TriMapMarkers);
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage2);

    PAUSE();
    LCD_Destory();
    return 0;
}

