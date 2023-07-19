#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_FormCge.h"
#include"../YMCV/CORE/YMCV_FreqCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� ͼ��dwtΪ����չʾ�˽�һ��ͼ�����dwt�任������任�Ľ����
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 700);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(mm00_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, mm00_gray, sizeof(mm00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//�Ҷ�ͼ
    //��ʾԭͼ
    LCD_show_image(0, 0, myImage0);

    //�����ռ���Ⱦ��
    uint8 kernel_r = 0;
    CVDWT2D_RENDER myKernel;
    YMCV_PADDING_VAL = 125;//�������ֵ

    /*--------------------------�Ҷ�ͼ��dwt�任--------------------------------------------------*/

    //����������ͼ���ר�� dwt�˲���
    myKernel = YMCV_Creat_DWT2D_Render_Creat(myImage0->width, myImage0->height);
    //ʹ���˲�������dwt���ͼ�񣬱���ʹ���˲��������Ĳ���������,�������˲�������,���ͱ���float����
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myKernel->width, myKernel->height, CVFGrayType);
    //����DWT�任
    YMCV_Gray_Image_DWT(myKernel, myImage0, myImage1);//�������myImage1

    /*--------------------------�任ͼ����չʾ--------------------------------------------------*/
    //��ʾƵ�׷���ͼƬ2
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage1->width, myImage1->height, myImage0->type);
    YMCV_Image_Pix_Change_Form(myImage1, myImage2, CV_FGrayToGray);

    LCD_show_image(500, 0, myImage2);
    sprintf(myInform, "dwt �任��Ľ��ͼ"); //��Ϣ��ʾ
    LCD_ShowString(100, 510, myInform);

    //��ͣ
    PAUSE();
    /*--------------------------Ƶ��ͼ���˲�--------------------------------------------------*/
    //��������С��Ӳ��ֵ����
    CVFREQ_RENDER freqKernl = YMCV_Creat_FreqKernel_Render_Creat(CV_HarrThreshFilter, 0);
    //�����˲�������
    CV_freq_1param myparam = { .harr.threshold= 45 };
    YMCV_Freq_Kernel_Load(freqKernl, myparam);
    //�Է��Ƚ����˲�
    YMCV_Gray_Image_Freq_Kernel_Render(myImage1, freqKernl);

    /*--------------------------ͼ��idwt�任--------------------------------------------------*/
     //����idwt����Ľ��ͼ����ԭʼͼһ�¼���
    CVIMAGE myImage3 = YMCV_Creat_Img_Creat(myImage1->width, myImage1->height, myImage0->type);
    //����iDWT�任
    YMCV_Gray_Image_IDWT(myKernel, myImage3, myImage1);//������myImage3
    //��ʾͼƬ2
    LCD_show_image(500, 0, myImage3);
    sprintf(myInform, "idwt �任��Ľ��ͼ"); //��Ϣ��ʾ
    LCD_ShowString(100, 510, myInform);


    //�ͷ�DWT��Ⱦ��
    YMCV_Free_DWT2D_RenderFree(myKernel);
    YMCV_Free_FreqKernel_RenderFree(freqKernl);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);

    PAUSE();
    LCD_Destory();
    return 0;
}








