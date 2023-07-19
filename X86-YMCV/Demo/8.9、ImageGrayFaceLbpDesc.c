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
    * ˵��  �������� �����Ҷ�ͼΪ����չʾ��LBPֱ��ͼ��������ȡЧ��
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 550);
    color24_t pencolor = { .R = 255 };
    LCD_Point_Color_Set(pencolor);//���û�����ɫ

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(luojiuchuan_112x112_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, luojiuchuan_112x112_gray, sizeof(luojiuchuan_112x112_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 112, 112, CVGrayType);//�Ҷ�ͼ0

    /*-------------------------����LBPֱ��ͼ�������--------------------------------------------------*/

    //��������ͼƬ�޶����Ϊ112��112��֧��orl�������ݼ���92��112�Ĵ�С
    CV_FACE_LBP_DESC lbpDescribe = YMCV_Face_UniformLBP_Describe(myImage0);

    
    LCD_Clear(0);
    LCD_show_image(0, 0, myImage0);//��ʾͼƬ0
    LCDLbpHistShow(lbpDescribe, 300, 0);//��ʾLBP������

    //��Ϣ��ʾ
    sprintf(myInform, " LBPֱ��ͼ  ");
    LCD_ShowString(100, 310, myInform);
    
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_FaceLbp_Hist_free(lbpDescribe);//�����ڴ��ͷ�

    PAUSE();
    LCD_Destory();
    return 0;
}





