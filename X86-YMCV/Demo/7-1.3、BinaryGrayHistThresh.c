#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_BinaryCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� Otsu��ֵ�ָ�Ϊ����չʾ�˻Ҷ�ͼ�ָ��Ч��
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 450);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(mm01_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, mm01_gray, sizeof(mm01_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 375, 500, CVGrayType);//�Ҷ�ͼ

       //ֱ��ͼ����
    int32 myImage0Hist[256];
    /*--------------------------�Ҷ�ͼֱ��ͼ--------------------------------------------------*/
    //��ȡͼ��ֱ��ͼ
    YMCV_Image_Hist_Get(myImage0, myImage0Hist, 256, PIXChannelNum1);
    //��ʾimage0ͼƬ��ֱ��ͼ
    LCD_show_image(0, 0, myImage0);
    LCDHistShow(myImage0Hist, PIXChannelNum1, 350, 140, 128);

    /*--------------------------�Ҷ�Otsu��ֵ��ֵ��--------------------------------------------------*/
    //���������ֵͼ��
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);//��ֵͼ
    CV_binary_param myParam = {.hist_auto_t.thresh_type= CV_HistOtsu };//��ʼ��Ϊotsu��ֵ

    //������ֵ�任��Ⱦ��
    CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_HistThreshold);

    //����Ϊotsu��ֵ��
    myParam.hist_auto_t.thresh_type = CV_HistOtsu;//��ֵ�����ж��ֿ�ѡ����ʹ��p-tile������������ٷֱȲ���
    //myParam.hist_auto_t.p_tile_val = 40;//��������ٷֱȲ���
    
    //���ز�����������
    YMCV_BinaryKernel_Load(myRender1, &myParam);

    //��Ŀ��ͼ��Ľ�����Ⱦ
    YMCV_Image_ThreshTo_Binary(myImage0, myImage2, myRender1);

    //��ʾͼƬ2
    LCD_show_image(500, 0, myImage2);

    //��Ϣ��ʾ
    sprintf(myInform, "��ֵͼ�� Otsu��ֵ  ");
    LCD_ShowString(100, 310, myInform);
    //��ͣ
    PAUSE();

    //�ͷ���Ⱦ���������´��õ�ʱ�����ڴ�й©
    YMCV_Free_Binary_RenderFree(myRender1);

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage2);

    PAUSE();
    LCD_Destory();
    return 0;
}





