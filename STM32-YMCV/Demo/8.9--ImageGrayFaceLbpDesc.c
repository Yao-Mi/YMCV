#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� �����Ҷ�ͼΪ����չʾ��LBPֱ��ͼ��������ȡЧ��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];
	
    color24_t pencolor = { .R = 255 };
    LCD_Point_Color_Set(pencolor);//���û�����ɫ

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(luojiuchuan_112x112_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, (void*)luojiuchuan_112x112_gray, sizeof(luojiuchuan_112x112_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 112, 112, CVGrayType);//�Ҷ�ͼ0

    /*-------------------------����LBPֱ��ͼ�������--------------------------------------------------*/

    //��������ͼƬ�޶����Ϊ112��112��֧��orl�������ݼ���92��112�Ĵ�С
    CV_FACE_LBP_DESC lbpDescribe = YMCV_Face_UniformLBP_Describe(myImage0_gray);

    
    LCD_show_image(stx + 0, 0, myImage0_gray);//��ʾͼƬ0
		
    LCDLbpHistShow(lbpDescribe, stx + 0, myImage0_gray->height);//��ʾLBP������

    //��Ϣ��ʾ
    sprintf(myInform, " LBP Hist ");
    LCD_ShowString(stx + 0, myImage0_gray->height, myInform);
    
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_FaceLbp_Hist_free(lbpDescribe);//�����ڴ��ͷ�

    Delay(800);
}





