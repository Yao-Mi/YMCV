#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_FeatCal.h"
#include "../YMCV/CORE/YMCV_EdgeCal.h"
#include "../YMCV/CORE/YMCV_FormCge.h"
/*******************************************************************************************
    *
    * ˵��  �������� �����任Ϊ����չʾ�������任Ч��������任
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(640, 600);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(ctcut00_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, ctcut00_gray, sizeof(ctcut00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 250, CVGrayType);//�Ҷ�ͼ0

    /*-------------------------- RADON �任����--------------------------------------------------*/
    LCD_show_image(0, 0, myImage0);//ԭͼ��ʾ�����
    uint8 theta_shift_step = 0; //�Ƕȷֱ���Ϊ(2 ^ 0)��

    CVIMAGE radonImg = YMCV_Gray_Radon_Convert(myImage0, theta_shift_step, 0);//���������任

    //��ʾ����ռ�
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(radonImg->width, radonImg->height, CVGrayType);
    YMCV_Image_Pix_Change_Form(radonImg, myImage2, CV_IGrayToGray2);
    LCD_show_image(300, 0, myImage2);//�������ʾ���ұ�

        //��Ϣ��ʾ
    sprintf(myInform, "  �����任    ");
    LCD_ShowString(100, 300, myInform);
    PAUSE();
    /*-------------------------- RADON ��任����--------------------------------------------------*/
    CVIMAGE irimg = YMCV_Gray_Radon_IConvert(radonImg, theta_shift_step);

    CVIMAGE myImage3 = YMCV_Creat_Img_Creat(irimg->width, irimg->height, CVGrayType);
    YMCV_Image_Pix_Change_Form(irimg, myImage3, CV_IGrayToGray2);
    LCD_show_image(0, 0, myImage3);//�������ʾ���ұ�

        //��Ϣ��ʾ
    sprintf(myInform, "  �������任    ");
    LCD_ShowString(100, 300, myInform);

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);
    YMCV_Free_ImgFree(radonImg);

    PAUSE();
    LCD_Destory();
    return 0;
}





