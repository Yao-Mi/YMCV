#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_SpaceCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� ��ֵͼ��̬ѧ�˲�Ϊ����չʾ�˶�ֵͼ�� �����ͺ͸�ʴЧ��
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(500, 300);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(tuxin_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, tuxin_gray, sizeof(tuxin_gray));

    for (int i = 0; i < sizeof(tuxin_gray); i++)
    {
        myData[i] = (myData[i] > 127) ? 0 : 255;//���ٶ�ֵ��
    }
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVBinaryType);//��ֵͼ0
    //��ʾԭͼ
    LCD_show_image(0, 0, myImage0);


    //�����ռ���Ⱦ��
    uint8 kernel_d = 0;
    CVFMAT myKernel;

    //���ڻ�����
    CVIMAGE myImage1 = NULL;

    /*--------------------------��̬ѧ�����˲� �޸��˲��뾶--------------------------------------------------*/
    for (kernel_d = 1; kernel_d < 10; kernel_d +=2)
    {
        //������Ӧ�뾶����̬ѧ�˲���
        myKernel = YMCV_Creat_FMAT_Creat(kernel_d, kernel_d);

        //�ֶ����û��к˽ṹ
        CV_memset(myKernel->datap,0, myKernel->row * myKernel->col*sizeof(float));//ȫ����ʼΪ0
        //ѡ����Ҫ���еĲ��ּ�Ϊ1
        for (int i = 0; i < myKernel->row; i++)
        {
            for (int j = 0; j < myKernel->col; j++)
                myKernel->datap[i * myKernel->col + j] = 1.0f;
        }
        //������̬ѧ����Ч��
        myImage1 = YMCV_Binary_Morph_Hit_Cal(myImage0, myKernel,1);//���������>1% �Ľ��������Ч��
       
        //��ʾͼƬ1
        LCD_show_image(250, 0, myImage1);

        //��Ϣ��ʾ
        sprintf(myInform, "�����˲� �˲�ֱ��: %02d   . ", kernel_d);
        LCD_ShowString(100, 200, myInform);
        //��ͣ
        PAUSE();

        //���ͷ�ͼ�񣬱����´��õ�ʱ�����ڴ�й©
        YMCV_Free_ImgFree(myImage1);
        //�ͷſռ���Ⱦ��
        YMCV_Free_FMatFree(myKernel);
    }

    ///*--------------------------��̬ѧ��ʴ�˲� �޸��˲��뾶--------------------------------------------------*/
    for (kernel_d = 1; kernel_d < 10; kernel_d +=2)
    {
        //������Ӧ�뾶����̬ѧ�˲���
        myKernel = YMCV_Creat_FMAT_Creat(kernel_d, kernel_d);

        //�ֶ����û��к˽ṹ
        CV_memset(myKernel->datap, 0, myKernel->row * myKernel->col * sizeof(float));//ȫ����ʼΪ0
        //ѡ����Ҫ���еĲ��ּ�Ϊ1
        for (int i = 0; i < myKernel->row; i++)
        {
            for (int j = 0; j < myKernel->col; j++)
            {
                myKernel->datap[i * myKernel->col + j] = 1.0f;
            }
        }

        //������̬ѧ���н��
        myImage1 = YMCV_Binary_Morph_Hit_Cal(myImage0, myKernel,99);//���������>99%�Ĳ��� ����ʴЧ��

        //��ʾͼƬ1
        LCD_show_image(250, 0, myImage1);

        //��Ϣ��ʾ
        sprintf(myInform, "��ʴ�˲� �˲�ֱ��: %02d   . ", kernel_d);
        LCD_ShowString(100, 200, myInform);
        //��ͣ
        PAUSE();

        //���ͷ�ͼ�񣬱����´��õ�ʱ�����ڴ�й©
        YMCV_Free_ImgFree(myImage1);
        //�ͷſռ���Ⱦ��
        YMCV_Free_FMatFree(myKernel);
    }
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);

    PAUSE();
    LCD_Destory();
    return 0;
}





