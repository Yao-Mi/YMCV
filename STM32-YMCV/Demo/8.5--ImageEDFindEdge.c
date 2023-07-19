#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� ED Ϊ����չʾ�˱�Ե��ȡЧ��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
    color24_t pencolor = { .R = 255 };
    LCD_Point_Color_Set(pencolor);//���û�����ɫ

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(tuxin02_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, (void*)tuxin02_gray, sizeof(tuxin02_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVGrayType);//�Ҷ�ͼ0
   
    LCD_show_image(stx + 0, 0, myImage0_gray);//��ʾͼƬ0
		int sty = myImage0_gray->height;
    /*-------------------------ED��Ե���--------------------------------------------------*/
    //���������ֵͼ��
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0_gray->width, myImage0_gray->height, CVBinaryType);//��ֵͼ

    CVCHAINS_U16 chains = YMCV_Fast_EdgeDrawing(myImage0_gray, CV_Sobel_Graident, 20, myImage2);

    LCD_show_image(stx + 0, sty + 0, myImage2);  //��ʾͼƬ2
    //��Ϣ��ʾ
    sprintf(myInform, "ED Find Edge ");
    LCD_ShowString(stx + 0, sty + 0, myInform);
    Delay(300);//��ͣ

    //��ԭͼ�ϻ��Ʊ�Ե����
    for (int i = 0; i < chains->cNum; i++)
    {
        CVVECTORS_U16 pixles = chains->chain[i];

        //��㵥������
        CV_ed_point pxy;
        CV_memcpy(&pxy, &pixles->datap[0], sizeof(CV_ed_point));
        color24_t rgb = { .R = 255 };
        LCD_Fill_ConstRgbRect(stx + pxy.pos.x, pxy.pos.y, 2, 2, rgb);
        //ʣ���
        for (int pi = 1; pi < pixles->dataNum; pi++)
        {
            CV_ed_point pxy;
            CV_memcpy(&pxy, &pixles->datap[pi * pixles->dim], sizeof(CVpoint));
            color24_t rgb = { .G = 255 };
            LCD_Fill_ConstRgbRect(stx + pxy.pos.x, pxy.pos.y, 1, 1, rgb);
        }
				//��ͣ
        Delay(200);
    }


    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_Chains_U16_Free(chains);//2D����洢����

    Delay(500);
}





