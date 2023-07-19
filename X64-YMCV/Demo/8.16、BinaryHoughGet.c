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
    * ˵��  �������� ��ֵͼ����任Ϊ����չʾ�˻���ռ��Ч��
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(640, 600);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(tuxin_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, tuxin_gray, sizeof(tuxin_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//�Ҷ�ͼ0


        /*--------------------------�Ҷ�Sobel��Ե���--------------------------------------------------*/
    //���������ֵͼ��
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);//��ֵͼ
    CV_edge_param myParam = { .EdgeType = CV_Sobel ,.Thresh.sobel.minT = 100 };
    YMCV_Gray_EdgeDetect(myImage0, myImage1, &myParam);

    //��ʾͼƬ
    LCD_show_image(0, 0, myImage0);
    LCD_show_image(300, 0, myImage1);

    //��Ϣ��ʾ
    sprintf(myInform, " ��Ե���");
    LCD_ShowString(100, 300, myInform);
    //��ͣ
    PAUSE();

    /*-------------------------- hough �ռ����--------------------------------------------------*/
    LCD_show_image(0, 0, myImage1);//ԭͼ��ʾ�����

    CVIMAGE houghImg = YMCV_Binary_To_Hough(myImage1, 0);//תΪhough�ռ� �Ƕȷֱ���Ϊ (2^0)��

    //��ʾ����ռ�
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(houghImg->width, houghImg->height, CVGrayType);
    YMCV_Image_Pix_Change_Form(houghImg, myImage2, CV_IGrayToGray);
    LCD_show_image(300, 0, myImage2);//�������ʾ���ұ�

        //��Ϣ��ʾ
    sprintf(myInform, "  hough�ռ�    ");
    LCD_ShowString(100, 300, myInform);


    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(houghImg);

    PAUSE();
    LCD_Destory();
    return 0;
}





