#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� ֱ��ͼ���⻯��ֱ��ͼ�涨��Ϊ����չʾ��ֱ��ͼ��ʹ�ü���ʾ
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(linglong00b));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, (void*)linglong00b, sizeof(linglong00b));
    CVIMAGE myImage1 = YMCV_Creat_DataToImg((uint8*)myData, 100, 87, CVRgb16Type);//��ɫͼ1

		LCD_Clear(0);
    //ֱ��ͼ����
    int32 myImage0Hist[3*256], myImage1Hist[3*256];

    /*--------------------------��ɫͼֱ��ͼ���⻯--------------------------------------------------*/

    //��ȡͼ��ֱ��ͼ
    YMCV_Image_Hist_Get(myImage0, myImage0Hist, 3 * 256, PIXChannelNum3);
    //��ʾͼƬ��ֱ��ͼ
    LCD_show_image(stx + 0, 0, myImage0);
    LCDHistShow(myImage0Hist, PIXChannelNum3, 150, 80, 60);

    //��ͼƬ0����һ��
    CVIMAGE myImage2 = YMCV_Creat_Img_BackUp(myImage0);
    //��myImage2�Ľ���ֱ��ͼ���⻯����
    YMCV_Image_Hist_Equalize(myImage2, myImage2);//�����������

    //��ȡͼ��ֱ��ͼ����ʾ
    YMCV_Image_Hist_Get(myImage2, myImage1Hist, 3 * 256, PIXChannelNum3);
    LCD_show_image(stx + 0, myImage1->height, myImage2);
    LCDHistShow(myImage1Hist, PIXChannelNum3, 150, 220, 60);//���ڴ�С������60������ת����״���ܲ���ʵ��������256�۲���ʵֱ��ͼ

    sprintf(myInform, "3 channel equalize indepen"); //3ͨ�����������⻯������ʽ���⻯��
    LCD_ShowString(0,  myImage1->height, myInform);
    //��ͣ300ms
    Delay(300);

    //��myImage0�Ľ���ֱ��ͼ���⻯����
    YMCV_Image_Hist_Equalize_UGray(myImage0, myImage2);

    //��ȡͼ��ֱ��ͼ����ʾ
    YMCV_Image_Hist_Get(myImage2, myImage1Hist, 3 * 256, PIXChannelNum3);
		LCD_show_image(stx + 0, myImage1->height, myImage2);
    LCDHistShow(myImage1Hist, PIXChannelNum3, 150, 220, 60);//���ڴ�С������60������ת����״���ܲ���ʵ��������256�۲���ʵֱ��ͼ

    sprintf(myInform, "3 channel equalize by gray"); //3ͨ��ͬʱ�����⻯���ϲ�ʽ���⻯��
    LCD_ShowString(0,  myImage1->height, myInform);
    //��ͣ1000ms
    Delay(1000);
		

    /*--------------------------��ɫͼֱ��ͼ�涨��--------------------------------------------------*/

    ////�ֶ���ֱ��ͼ�涨Ϊĳ����״
    //for (int i = 0; i < 256; i++)
    //{
    //    myImage1Hist[i] = i + 50;//���Ժ�������ֱ��ͼΪ���ν���
    //}
    //��ͼ��1��ȡֱ��ͼ����ʾ
    YMCV_Image_Hist_Get(myImage1, myImage1Hist, 3*256, PIXChannelNum3);
		LCD_show_image(stx + 0, 0, myImage1);
    LCDHistShow(myImage1Hist, PIXChannelNum3, 150, 80, 60);

    sprintf(myInform, "target Hist");
    LCD_ShowString(stx + 0, 0, myInform);
    //��ͣ500ms
    Delay(500);

    //��ͼ��0����ֱ��ͼ�涨��
    YMCV_Image_Hist_To(myImage0, myImage2, myImage1Hist, 3*256, PIXChannelNum3);
    //��ȡͼ��ֱ��ͼ����ʾ
    YMCV_Image_Hist_Get(myImage2, myImage1Hist, 3*256, PIXChannelNum3);
    LCD_show_image(stx + 0, myImage1->height, myImage2);
    LCDHistShow(myImage1Hist, PIXChannelNum3, 150, 220, 60);//���ڴ�С������60������ת����״���ܲ���ʵ��������256�۲���ʵֱ��ͼ

    sprintf(myInform, "hist specification    ");
    LCD_ShowString(stx + 0, myImage1->height, myInform);
		
		//��ͣ1000ms
    Delay(1000);
		
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
}





