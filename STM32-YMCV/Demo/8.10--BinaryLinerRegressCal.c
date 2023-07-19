#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� ��ֵͼ���ػع�Ϊ����չʾ�����Իع�Ч����
    *  �÷���ͨ����ͼ���ز���Ϊ������25x25��С��ͼ�񣬽��лع�������ٽ���任��ԭͼ�����ܻ������ԭͼʵ��ֱ�߲���ȫ�غϵ����
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];
	
    color24_t pencolor = { .R = 255 };
    LCD_Point_Color_Set(pencolor);//���û�����ɫ

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(xunxian03_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, (void*)xunxian03_gray, sizeof(xunxian03_gray));
    
    for (int i = 0; i < sizeof(xunxian03_gray); i++)
    {
        myData[i] = (myData[i] < 127) ? 255 : 0; //���ٶ�ֵ��
    }
    CVIMAGE myImage0b = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVBinaryType);//�Ҷ�ͼ0

    /*-------------------------��С���� ֱ�߻ع����--------------------------------------------------*/
    CVline imgline = YMCV_BinaryMax25x25_Regression_Line(myImage0b, CVRegress_LSE);

    LCD_show_image(stx + 0, 0, myImage0b);//��ʾͼƬ0
    LCD_Draw_Line(stx + imgline.x1, imgline.y1, stx + imgline.x2, imgline.y2);//����ֱ��

    //��Ϣ��ʾ
    sprintf(myInform, "LES regress ");
    LCD_ShowString(stx + 0, 0, myInform);
    Delay(500);


    /*-------------------------̩��ɭ ֱ�߻ع����--------------------------------------------------*/
    imgline = YMCV_BinaryMax25x25_Regression_Line(myImage0b, CVRegress_TSE);

    LCD_show_image(stx + 0, 0, myImage0b);//��ʾͼƬ0
    LCD_Draw_Line(stx + imgline.x1, imgline.y1, stx + imgline.x2, imgline.y2);//����ֱ��
    //��Ϣ��ʾ
    sprintf(myInform, "TSE regress ");
    LCD_ShowString(stx + 0, 0, myInform);
    Delay(500);


    /*-------------------------RANSAC ֱ�߻ع����--------------------------------------------------*/
    imgline = YMCV_BinaryMax25x25_Regression_Line(myImage0b, CVRegress_RANSAC);

    LCD_show_image(stx + 0, 0, myImage0b);//��ʾͼƬ0
    LCD_Draw_Line(stx + imgline.x1, imgline.y1, stx + imgline.x2, imgline.y2);//����ֱ��
    //��Ϣ��ʾ
    sprintf(myInform, "RANSAC regress");
    LCD_ShowString(stx + 0, 0, myInform);
    Delay(500);


    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0b);

}





