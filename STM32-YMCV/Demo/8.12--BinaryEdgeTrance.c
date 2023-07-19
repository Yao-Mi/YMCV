#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� �߽���� Ϊ����չʾ�˱߽�������ȡ�Լ���״�б�
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
    /*------------------------ �Ҷ�ɫ����ȡ ------------------------------*/

    CVcolor minVal, maxVal;
    minVal.gray = 200;
    maxVal.gray = 255;

    CVVECTORS_U16 blobs = YMCV_Image_ColorBox_Find(myImage0_gray, minVal, maxVal, 100, 1);//����ȡ��������myImage0
    myImage0_gray->type = CVBinaryType;//ǿ��תΪ��ֵͼ
    LCD_show_image(stx + 0, 0, myImage0_gray);
    //��ʾblob��box
    for (int i = 0; (blobs != NULL) && (i < blobs->dataNum); i++)
    {
        CVblob* blobp = (CVblob*)(blobs->datap + i * blobs->dim);//blobp= &data[i]
        color24_t thiscolor = { .G = 255 };
        //LCD_Draw_Rect(300 + blobp->box.x, blobp->box.y, blobp->box.w, blobp->box.h, thiscolor);
    }

    //��Ϣ��ʾ
    sprintf(myInform, "gray Box ");
    LCD_ShowString(stx + 0, 0, myInform);
    Delay(500);

    /*-------------------------- �߽���� --------------------------------------------------*/
    CVCHAINS_U16 edgeChains = YMCV_Image_Binary_Edge_Trace(myImage0_gray, blobs);//�õ�����blobs�ı߽�����

		//����
		LCD_Fill_ConstRgbRect(stx + 0, myImage0_gray->height,myImage0_gray->width,myImage0_gray->height,(color24_t){0,0,0});
		
    //��ʾ
    for (int i = 0; i < edgeChains->cNum; i++)//�����߽�����
    {
        //����������������
        CVVECTORS_U16 pixles = edgeChains->chain[i];
        for (int pi = 0; pi < pixles->dataNum; pi++)//ԭʼ�������
        {
            CVpoint* pxy = (CVpoint*)&pixles->datap[pi * pixles->dim];
            color24_t rgb = { .G = 255 };
            LCD_Fill_ConstRgbRect(stx + pxy->x, sty + pxy->y, 1, 1, rgb);
        }
    }
    //��Ϣ��ʾ
    sprintf(myInform, "Binary Edge Trance");
    LCD_ShowString(stx + 0, sty, myInform);
    //��ͣ
    Delay(500);

    /*-------------------------- EDCorner͹�Ǽ�� --------------------------------------------------*/
    CVVECTORS_U16 corner = YMCV_Fast_EdgeDrawing_ChainsCornerNum(edgeChains, 20);//���ȶ���20������

    //��ʾ
    for (int i = 0; i < corner->dataNum; i++)//�����߽�����
    {
        //ȡ�ö�Ӧ͹����Ϣ
        CV_ed_corner* thisCorner = (CV_ed_corner*)&corner->datap[i * corner->dim];
        
        //��״�б�
        switch (thisCorner->CornerNum)//�ж�͹�Ǹ���
        {
        case 0:
            sprintf(myInform, "circle");//Բ
            break;
        case 1:
            sprintf(myInform, "halfcr");//��Բ��
            break;
        case 2:
            sprintf(myInform, "ballcr");//��Բ�λ��С�����������
            break;
        default:
					  sprintf(myInform, "Ang:%2d", thisCorner->CornerNum);
            break;
        }
        //�����Ļ����ı�
        LCD_ShowString(stx + thisCorner->center.x - 16, sty + thisCorner->center.y-5, myInform);
				//��ͣ200ms
				Delay(200);
    }
    //��Ϣ��ʾ
    sprintf(myInform, "Edge Shape");
    LCD_ShowString(stx+0, sty, myInform);


    //�ͷ�blob�ڴ�
    YMCV_Free_Vector_U16_Free(blobs);
    YMCV_Free_Vector_U16_Free(corner);
    YMCV_Free_Chains_U16_Free(edgeChains);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
    //YMCV_Free_ImgFree(myImage1);

    Delay(500);
}





