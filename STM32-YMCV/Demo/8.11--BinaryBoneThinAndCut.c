#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� ��ֵͼ�Ǽ�ϸ�� �ͼ�֦ Ϊ����չʾ��ϸ���ͼ�֦Ч��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(gujia1_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, (void*)gujia1_gray, sizeof(gujia1_gray));
    //��ֵͼ
    for (int i = 0; i < sizeof(gujia1_gray); i++)
    {
        myData[i] = (myData[i] < 127) ? 255 : 0;//���ٶ�ֵ��
    }
    CVIMAGE myImage0b = YMCV_Creat_DataToImg((uint8*)myData, 120, 120, CVBinaryType);//��ֵͼ

    LCD_show_image(stx + 0, 0, myImage0b);
		int sty = myImage0b->height;
    /*-------------------------- ��̬ѧ�����޸�С�� --------------------------------------------------*/
    CV_kernel_1param myParam;//���ֵ�˲������������
    YMCV_PADDING_VAL = 0;//���ñ߽����ֵΪ0
    //����ֵ�˲�����ֵͼ����Ч��
    CVKERN_RENDER myKernel = YMCV_Creat_SpaceRender_Creat(CV_MaxFilter, PIXChannelNum1, 1, OutPaddingConst);//������Ӧ�뾶���˲���
    YMCV_Space_SingleKernel_Load(myKernel, &myParam); //ʹ�ò���������Ⱦ��ͨ�����á���ʼ��
    CVIMAGE myImage1 = YMCV_Image_Space_SigleKernel_Render(myImage0b, myKernel);//ʹ�ÿռ���Ⱦ��������Ⱦ�Ҷ�ͼƬ


    //��ʾͼƬ
    LCD_show_image(stx + 0, sty, myImage1);

    //��Ϣ��ʾ
    sprintf(myInform, "Morp Dilation");
    LCD_ShowString(stx + 0, sty, myInform);
    //��ͣ
    Delay(500);

    /*--------------------------�Ǽ�ϸ������--------------------------------------------------*/
		
    YMCV_Image_Binary_Bone_Thin(myImage1);

    LCD_show_image(stx + 0, sty, myImage1);
    //��Ϣ��ʾ
    sprintf(myInform, "Binary Thin ");
    LCD_ShowString(stx + 0, sty, myInform);
    //��ͣ
    Delay(500);

    /*--------------------------�Ǽܼ�֦����--------------------------------------------------*/

    YMCV_Image_Binary_Bone_Cut(myImage1, 5);//����С��5��ë�̶��޳�

    LCD_show_image(stx + 0, sty, myImage1);//�������ʾ���ұ�

        //��Ϣ��ʾ
    sprintf(myInform, " Bone Cut  ");
    LCD_ShowString(stx + 0, sty, myInform);


		//�ͷſռ���Ⱦ��
		YMCV_Free_KERN_RenderFree(myKernel);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0b);
    YMCV_Free_ImgFree(myImage1);

    Delay(500);
}





