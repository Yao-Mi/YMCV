#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_AreaFillCal.h"
#include <stdlib.h>

/*******************************************************************************************
    *
    * ˵��  �������� ɨ�������ӵ����Ϊ����չʾ����������Ч��
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
    color24_t thiscolor = { .R = 255 };

    CVcolor fillval;
    fillval.gray =255;//���ֵ
    CVpoint seed = { .x = 130,.y = 45 };//���ӵ�����
    int8 floaterror = 60;//���ӵ㸽��ֵ���������Ϊ��50������䵱ǰ����ɫֵ��50����ͨ����
    LCD_Fill_ConstRgbRect(seed.x - 1, seed.y - 1, 3, 3, thiscolor);

    //�ԻҶ�ͼ�����������
    YMCV_Image_ScanLine_SeedFill(myImage0, seed, floaterror, fillval);

    LCD_show_image(300, 0, myImage0);
    //��Ϣ��ʾ
    sprintf(myInform, "�����������ͼ  ");
    LCD_ShowString(400, 310, myInform);
    PAUSE();

    fillval.rgb16 = 0xfe00;
    //�Բ�ɫͼ�����������
    YMCV_Image_ScanLine_SeedFill(myImage2, seed, floaterror, fillval);

    LCD_show_image(600, 0, myImage2);
    //��Ϣ��ʾ
    sprintf(myInform, "��ɫͼ���  ");
    LCD_ShowString(700, 310, myInform);

    //�ͷ��ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage2);

    PAUSE();
    LCD_Destory();
    return 0;
}

