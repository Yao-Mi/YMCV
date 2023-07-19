#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� �Ҷ�ͼΪ����չʾ��HOG��������ȡЧ��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];
	
    color24_t pencolor = { .R = 255 };
    LCD_Point_Color_Set(pencolor);//���û�����ɫ

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(mm02_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, (void*)mm02_gray, sizeof(mm02_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVGrayType);//�Ҷ�ͼ0

    /*-------------------------HOG�������--------------------------------------------------*/
    CVVECTORS_F32 hogDescribe= YMCV_Gray_Find_Hog_Describe(myImage0_gray);

    LCD_show_image(stx + 0, 0, myImage0_gray);//��ʾͼƬ0
		
		//����
		LCD_Fill_ConstRgbRect(stx + 0, myImage0_gray->height,myImage0_gray->width,myImage0_gray->height,(color24_t){0,0,0});
		
		static uint8 showMode = 1;
		//showMode = (showMode)?0:1;//�л���ʾģʽ
    LCDHogHistShow(hogDescribe, stx + 0, myImage0_gray->height,showMode);//��ʾHOG������

    //��Ϣ��ʾ
    sprintf(myInform, " HOG Hist ");
    LCD_ShowString(stx + 0, myImage0_gray->height-16, myInform);

    //��ͣ800ms
    Delay(800);
						
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_Vector_F32_Free(hogDescribe);//�����ڴ��ͷ�

}





