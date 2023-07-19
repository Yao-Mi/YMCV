#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_FeatCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� ��ֵͼ�Ǽ�ϸ�� �ͼ�֦ Ϊ����չʾ��ϸ���ͼ�֦Ч��
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(640, 480);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(guajia_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, guajia_gray, sizeof(guajia_gray));
    //��ֵͼ
    for (int i = 0; i < sizeof(guajia_gray); i++)
    {
        myData[i] = (myData[i] < 127) ? 255 : 0;//���ٶ�ֵ��
    }
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 300, 300, CVBinaryType);//�Ҷ�ͼ

    /*-------------------------- ��̬ѧ���� --------------------------------------------------*/
    CV_kernel_1param myParam;//���ֵ�˲������������
    YMCV_PADDING_VAL = 0;//���ñ߽����ֵΪ0
    //����ֵ�˲�����ֵͼ����Ч��
    CVKERN_RENDER myKernel = YMCV_Creat_SpaceRender_Creat(CV_MaxFilter, PIXChannelNum1, 1, OutPaddingConst);//������Ӧ�뾶���˲���
    YMCV_Space_SingleKernel_Load(myKernel, &myParam); //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
    CVIMAGE myImage1 = YMCV_Image_Space_SigleKernel_Render(myImage0, myKernel);//ʹ�ÿռ���Ⱦ��������Ⱦ�Ҷ�ͼƬ


    //��ʾͼƬ
    LCD_show_image(0, 0, myImage0);
    LCD_show_image(300, 0, myImage1);

    //��Ϣ��ʾ
    sprintf(myInform, " ��̬ѧ�����޸�С��.");
    LCD_ShowString(100, 300, myInform);
    //��ͣ
    PAUSE();

    /*--------------------------�Ǽ�ϸ������--------------------------------------------------*/
    LCD_show_image(0, 0, myImage1);//ԭͼ��ʾ�����

    YMCV_Image_Binary_Bone_Thin(myImage1);

    LCD_show_image(300, 0, myImage1);//�������ʾ���ұ�
    //��Ϣ��ʾ
    sprintf(myInform, "      ��ֵͼ�Ǽ�ϸ��     ");
    LCD_ShowString(100, 300, myInform);
    //��ͣ
    PAUSE();

    /*--------------------------�Ǽܼ�֦����--------------------------------------------------*/
    LCD_show_image(0, 0, myImage1);//ԭͼ��ʾ�����

    YMCV_Image_Binary_Bone_Cut(myImage1, 5);//����С��5��ë�̶��޳�

    LCD_show_image(300, 0, myImage1);//�������ʾ���ұ�

        //��Ϣ��ʾ
    sprintf(myInform, "      ��ֵͼ�Ǽܼ�֦     ");
    LCD_ShowString(100, 300, myInform);


    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);

    PAUSE();
    LCD_Destory();
    return 0;
}





