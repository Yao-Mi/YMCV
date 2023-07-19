#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_EdgeCal.h"
#include"../YMCV/CORE/YMCV_MergeCal.h"
#include"../YMCV/CORE/YMCV_SegnmCal.h"
#include <stdlib.h>

/*******************************************************************************************
    *
    * ˵��  �������� Snake����ģ��Ϊ����չʾ����������ȡЧ��
    *
    ***************************************************************************************/
CVIMAGE YMCV_GetTriMap_Demo(CVIMAGE image0);

int main()
{
    int i;
    char myInform[100];
    LCD_Init(1000, 450);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(mm00_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, mm00_gray, sizeof(mm00_gray));
    for (int i = 0; i < sizeof(mm00_gray); i++)
    {
        myData[i] = (myData[i] >> 7) << 7;
    }
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//�Ҷ�ͼ0


    /*-------------------------- �ֶ���������--------------------------------------------------*/
    LCD_Clear(0);
    //��ʾԭͼ
    LCD_show_image(0, 0, myImage0);
    sprintf(myInform, "�ֶ����������� ");
    LCD_ShowString(0, 310, myInform);

    color24_t thiscolor = { .R = 255,.G = 0 };
    char pflag = 0;
    printf("׼�����ƣ�\n'c':ѡ��\n�ո񣺽�������");
    int px=0, py=0, w, h;
    uint16 xpoint[] = { 143, 143, 142, 140, 135, 131, 128, 123, 120, 112, 105, 97, 93, 91, 91, 89, 87, 87, 88, 90, 91, 93, 95, 95, 97, 98, 101, 105, 109, 114, 119, 123, 128, 135, 139, 139, 143, 143, };
    uint16 ypoint[] = { 55, 49, 44, 32, 24, 18, 16, 11, 5, 2, 2, 3, 6, 10, 15, 21, 29, 35, 38, 42, 48, 51, 54, 58, 59, 63, 68, 70, 74, 74, 78, 80, 80, 76, 70, 68, 65, 59, }; 
    uint16 num = sizeof(xpoint)/2;

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


    LCD_Point_Color_Set(thiscolor);
    LCD_show_image(300, 0, myImage0);
    for (i = 0; i < num - 1; i++)
    {
        LCD_Draw_Line(300 + xpoint[i], ypoint[i], 300 + xpoint[i + 1], ypoint[i + 1]);
    }
    LCD_Draw_Line(300 + xpoint[i], ypoint[i], 300 + xpoint[0], ypoint[0]);
    //��Ϣ��ʾ
    sprintf(myInform, "ԭʼ����ͼ  ");
    LCD_ShowString(400, 310, myInform);
    PAUSE();

    /*-------------------------- snake�����ָ� --------------------------------------------------*/
    int search_r = 3;
    //�����£���ȡֵ��Χ[-1,1]
    float alpha = 0.2;//�����仯�ʣ��仯�������ն�
    float beta = 0.25;//������������������ֵ����Բ���̶�
    float gamma = 1.0;//ͼ������

    //kapaȡֵ[-1,1]
    float imageKpa = 0.1;//�����������ɫ��£���������ɫ��£
    float gradientKpa = 1.0;//���ݶȸߵķ���£
    YMCV_Gray_Snake(myImage0, xpoint, ypoint,num, search_r,3000,alpha,beta,gamma, imageKpa, gradientKpa);//��Trimapͼ��ָ�����
     //��ʾͼƬ
    LCD_show_image(600, 0, myImage0);
    //��Ϣ��ʾ
    sprintf(myInform, "��������ͼ  ");
    LCD_ShowString(700, 310, myInform);
    
    for ( i = 0; i < num-1; i++)
    {
        printf("�㣺%d , %d\n", xpoint[i], ypoint[i]);
        LCD_Draw_Line(600+xpoint[i], ypoint[i], 600+xpoint[i+1],ypoint[i+1]);
    }
    LCD_Draw_Line(600+xpoint[i], ypoint[i], 600+xpoint[0], ypoint[0]);
    //CVIMAGE SegImage = YMCV_Image_Mask(TriMapMarkers, myImage0);//�ָ�ͼ��������ʹ��

    //��ʾͼƬ�ָ���
    //LCD_show_image(300, 0, SegImage);

    //�ͷ��ڴ�
    //YMCV_Free_ImgFree(TriMapMarkers);
    //YMCV_Free_ImgFree(SegImage);
    YMCV_Free_ImgFree(myImage0);

    PAUSE();
    LCD_Destory();
    return 0;
}


