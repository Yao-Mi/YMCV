#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_HistCal.h"
#include"../YMCV/CORE/YMCV_Special_Effect.h"
/*******************************************************************************************
    *
    * ˵��  ������ ��ͨ��ȥ��ʵ�� ����Ч��
    ***************************************************************************************/

int main()
{
    char myInform[100];
    LCD_Init(1000, 700);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = (uint8*)CV_malloc1(sizeof(wutu_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, wutu_gray, sizeof(wutu_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg(myData, 500, 322, CVGrayType);//�Ҷ�ͼ
    //��ʾԭͼ
    LCD_show_image(0, 0, myImage0);


    //�����ռ���Ⱦ��
    uint8 kernel_r = 1;
    YMCV_PADDING_VAL = 125;//�������ֵ


    //���ڻ�����
    CVIMAGE myImage1 = NULL, myImage2 = NULL;

    //��ð�ͨ��ͼ
    myImage1 = YMCV_Gray_Image_GetDarkChannel(myImage0, kernel_r);
    //�����������A
    uint8 AtmosValue = YMCV_Gray_Image_GetAtmosphericlighting(myImage0, myImage1);

    /*--------------------------��ͨ��ȥ�� �޸��˲��뾶--------------------------------------------------*/
    for (float32 k = 0.1; k < 1.0; k+=0.1)
    {
        //���ó����� k���õ�ȥ��ͼ��
        myImage2 = YMCV_Gray_Image_RemovalFog(myImage0, myImage1, AtmosValue,k);
        //������ԵĻ�����ʹ��ͨ����Ⱦ������һ��ͼ����������ȣ�Ч����ȽϺ�
        
        //������ʾͼƬ2
        LCD_show_image(500, 0, myImage2);

        //��Ϣ��ʾ
        sprintf(myInform, "������: %.2f  ", k);
        LCD_ShowString(100, 510, myInform);
        //��ͣ
        PAUSE();

        //���ͷ�ͼ�񣬱����´��õ�ʱ�����ڴ�й©
        YMCV_Free_ImgFree(myImage2);
    }
    

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);

    PAUSE();
    LCD_Destory();
    return 0;
}








