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
    * ˵��  �������� ͼ��fftΪ����չʾ�˽�һ��ͼ����а�����˹��ͨ�˲��Ľ����
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
    CVFFT2D_RENDER myKernel;
    YMCV_PADDING_VAL = 125;//�������ֵ

    /*--------------------------ͼ��fft�任��shift�任--------------------------------------------------*/

    //����������ͼ���ר�� fft�˲���
    myKernel = YMCV_Creat_FFT2D_Render_Creat(myImage0->width, myImage0->height);
    //ʹ���˲�������fft���ͼ�����ʹ���˲��������Ĳ���������,���ͱ���complex����
    //��Ȼ������ʹ���˲����Ŀ�Ⱥ͸߶Ƚ��д�����ͼ��Ҳ���Զ����¹滮��С���������˲�������
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVCGrayType);
    //����FFT�任
    YMCV_Gray_Image_FFT(myKernel, myImage0, myImage1);//�������myImage1

    //fftshift�任
    YMCV_CGray_Image_FFTshift(myImage1);

    //��ʾƵ��ͼƬ2
    YMCV_CGray_Image_LogMag_AndPhase(myImage1);//��ת���Ⱥ���λ
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage1->width, myImage1->height, myImage0->type);
    YMCV_Image_Pix_Change_Form(myImage1, myImage2, CV_CGrayToGray2);

    LCD_show_image(500, 0, myImage2);
    sprintf(myInform, "fft �任��shift���ͼ"); //��Ϣ��ʾ
    LCD_ShowString(100, 510, myInform);

    //��ͣ
    PAUSE();

    /*--------------------------Ƶ��ͼ���˲�--------------------------------------------------*/
    //����������˹��ͨ�˲���,������ʽΪ��������λ
    CVFREQ_RENDER freqKernl = YMCV_Creat_FreqKernel_Render_Creat(CV_ButterWorthHFilter, CV_RhoAndTheta);
    //�����˲�������
    CV_freq_1param myparam = { .butterworth.D0 = 15,.butterworth.n=2 };
    YMCV_Freq_Kernel_Load(freqKernl, myparam);
    //�Է��Ƚ����˲�
    YMCV_Gray_Image_Freq_Kernel_Render(myImage1, freqKernl);

    //��ʾ�˲���Ľ��
    YMCV_Image_Pix_Change_Form(myImage1, myImage2, CV_CGrayToGray2);

    LCD_show_image(500, 0, myImage2);
    sprintf(myInform, "������˹��ͨ�˲���Ľ��ͼ"); //��Ϣ��ʾ
    LCD_ShowString(100, 510, myInform);

    //��ͣ
    PAUSE();
    /*--------------------------Ƶ��ͼ��ifftshift�任��ifft�任--------------------------------------------------*/
    YMCV_CGray_Image_expMag_ToComplex(myImage1);//��Ƶͼת����ͼ
    //ifftshift�任 ����ż�����ͼ��shift��ishift������ͬ
    YMCV_CGray_Image_FFTshift(myImage1);


    //����ifft����Ľ��ͼ����ԭʼͼһ�¼���
    CVIMAGE myImage3 = YMCV_Creat_Img_Creat(myImage1->width, myImage1->height, myImage0->type);
    //����iFFT�任
    YMCV_Gray_Image_IFFT(myKernel, myImage3, myImage1);//������myImage3
    //��ʾͼƬ2
    LCD_show_image(500, 0, myImage3);
    sprintf(myInform, "ifft �任���ͼ              "); //��Ϣ��ʾ
    LCD_ShowString(100, 510, myInform);

    //�ͷ�FFT��Ⱦ��
    YMCV_Free_FFT2D_RenderFree(myKernel);
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








