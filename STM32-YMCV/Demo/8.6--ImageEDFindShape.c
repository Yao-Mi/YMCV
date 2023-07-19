#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� ED ShapeΪ����չʾ����״ʶ��Ч��
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
    sprintf(myInform, "ED Find Shape ");
    LCD_ShowString(stx + 0, sty + 0, myInform);
    Delay(300);//��ͣ
    /*-------------------------- EDCorner͹�Ǽ�� --------------------------------------------------*/
    CVVECTORS_U16 corner = YMCV_Fast_EdgeDrawing_ChainsCornerNum(chains, 30);
    for (int i = 0; i < corner->dataNum; i++)
    {
        CV_ed_corner* thisCorner = (CV_ed_corner*)&corner->datap[i * corner->dim];
        //��������
        color24_t rgb = { .R = 255 };
        LCD_Fill_ConstRgbRect(stx + thisCorner->center.x, thisCorner->center.y, 2, 2, rgb);
        //���ƽ���Բ��Զ��Բ
        rgb.G = 150;
        LCD_Point_Color_Set(rgb);
        LCD_Draw_Circle(stx + thisCorner->center.x, thisCorner->center.y, thisCorner->min_r);//����

        rgb.B = 180;
        LCD_Point_Color_Set(rgb);
        LCD_Draw_Circle(stx + thisCorner->center.x, thisCorner->center.y, thisCorner->max_r);//Զ��

        //��״�б�
        printf("͹������%d  ", thisCorner->CornerNum);
        switch (thisCorner->CornerNum)
        {
        case 0:
            printf("Բ��  ");
            printf("Բ�� %d,%d,�뾶%d\n", thisCorner->center.x, thisCorner->center.y, (thisCorner->max_r + thisCorner->min_r) / 2);
				    sprintf(myInform, "circle");
				    break;
        case 1:
            printf("��Բ��  ");  break;
        case 2:
            printf("��Բ�� ��С����������� ");
				    sprintf(myInform, "ballOa");
    				break;
        default:
					  sprintf(myInform, "Ang:%02d",thisCorner->CornerNum);
            printf("͹%d���� \n", thisCorner->CornerNum);  break;
        }
				
				//��Ϣ��ʾ
				LCD_ShowString(0, 0, myInform);
				
        CVVECTORS_U16 pixles= chains->chain[i];
        //ԭʼ�������
        for (int pi = 0; pi < pixles->dataNum; pi++)
        {
            CV_ed_point pxy;
            CV_memcpy(&pxy, &pixles->datap[pi * pixles->dim], sizeof(CVpoint));
            color24_t rgb = { .G = 255 };
            LCD_Fill_ConstRgbRect(stx + pxy.pos.x, pxy.pos.y, 1, 1, rgb);
        }
				//��ͣ800
        Delay(800);
    }

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage2);

    YMCV_Free_Chains_U16_Free(chains);//2D����洢����
    YMCV_Free_Vector_U16_Free(corner); //ת���ڴ��ͷ�

    //��ͣ500
    Delay(500);
}





