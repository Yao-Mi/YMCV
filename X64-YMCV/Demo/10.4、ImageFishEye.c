#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_Special_Effect.h"
#include"../YMCV/CORE/YMCV_AreaFillCal.h"
#include <stdlib.h>

/*******************************************************************************************
    *
    * ˵��  �������� FishEyeΪ����չʾ������ģ�⼰����Ч��
    *
    ***************************************************************************************/

int main()
{
    int i;
    char myInform[100];
    LCD_Init(1100, 450);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(house_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, house_gray, sizeof(house_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 500, 270, CVGrayType);//�Ҷ�ͼ0

    LCD_Clear(0);
    //��ʾԭͼ
    LCD_show_image(0, 0, myImage0);
    /*-------------------------- ����Ч�� --------------------------------------------------*/
   
    CVIMAGE feImg = YMCV_Gray_FishEye_Effect(myImage0, myImage0->width, myImage0->height);
    LCD_show_image(500, 0, feImg);
    //��Ϣ��ʾ
    sprintf(myInform, "����Ч��ͼ  ");
    LCD_ShowString(400, 310, myInform);
    PAUSE();
    /*-------------------------- ͸��ʧ����� --------------------------------------------------*/
    YMCV_PADDING_VAL = 100;//�հ״����ֵ
    CVIMAGE invfeImg = YMCV_Gray_invFishEye_Effect(feImg,4, 0.43, 0.85);
    LCD_show_image(0, 0, invfeImg);
    //��Ϣ��ʾ
    sprintf(myInform, "����ʧ�����ͼ  ");
    LCD_ShowString(400, 310, myInform);

    //�ͷ��ڴ�
    YMCV_Free_ImgFree(feImg);
    YMCV_Free_ImgFree(myImage0);

    PAUSE();
    LCD_Destory();
    return 0;
}

