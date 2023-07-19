#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� �����任Ϊ����չʾ�������任Ч��������任
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(ctcut01_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, (void*)ctcut01_gray, sizeof(ctcut01_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 112, 112, CVGrayType);//�Ҷ�ͼ0
	
    LCD_show_image(stx + 0, 0, myImage0_gray);//ԭͼ��ʾ
	  int sty = myImage0_gray->height;
    /*-------------------------- RADON �任����--------------------------------------------------*/
    uint8 theta_shift_step = 0; //�Ƕȷֱ���Ϊ(2 ^ 0)��

    CVIMAGE radonImg = YMCV_Gray_Radon_Convert(myImage0_gray, theta_shift_step, 0);//���������任

    //��ʾ����ռ�
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(radonImg->width, radonImg->height, CVGrayType);
    YMCV_Image_Pix_Change_Form(radonImg, myImage2, CV_IGrayToGray2);
    LCD_show_image(stx + 0, sty + 0, myImage2);//�������ʾ���ұ�

        //��Ϣ��ʾ
    sprintf(myInform, "radon Transform");
    LCD_ShowString(stx + 0, sty + 0, myInform);
    Delay(500);
    /*-------------------------- RADON ��任����--------------------------------------------------*/
    CVIMAGE irimg = YMCV_Gray_Radon_IConvert(radonImg, theta_shift_step);

    CVIMAGE myImage3 = YMCV_Creat_Img_Creat(irimg->width, irimg->height, CVGrayType);
    YMCV_Image_Pix_Change_Form(irimg, myImage3, CV_IGrayToGray2);
    LCD_show_image(stx + 0, sty + 0, myImage3);//�������ʾ���ұ�

        //��Ϣ��ʾ
    sprintf(myInform, "radon invers   ");
    LCD_ShowString(stx + 0, sty + 0, myInform);

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);
    YMCV_Free_ImgFree(irimg);
    YMCV_Free_ImgFree(radonImg);

    Delay(500);
}





