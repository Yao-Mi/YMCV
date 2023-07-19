#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_FeatCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� �߽���� Ϊ����չʾ�˱߽�������ȡ�Լ���״�б�
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(640, 480);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(tuxin_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, tuxin_gray, sizeof(tuxin_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//�Ҷ�ͼ0

    /*------------------------ �Ҷ�ɫ����ȡ ------------------------------*/

    CVcolor minVal, maxVal;
    minVal.gray = 200;
    maxVal.gray = 255;

    CVVECTORS_U16 blobs = YMCV_Image_ColorBox_Find(myImage0, minVal, maxVal, 100, 1);//����ȡ��������myImage0
    myImage0->type = CVBinaryType;//ǿ��תΪ��ֵͼ
    LCD_show_image(300, 0, myImage0);
    //��ʾblob��box
    for (int i = 0; (blobs != NULL) && (i < blobs->dataNum); i++)
    {
        CVblob* blobp = blobs->datap + i * blobs->dim;//blobp= &data[i]
        color24_t thiscolor = { .G = 255 };
        //LCD_Draw_Rect(300 + blobp->box.x, blobp->box.y, blobp->box.w, blobp->box.h, thiscolor);
    }

    //��Ϣ��ʾ
    sprintf(myInform, "�Ҷ�����ɫ��boxͼ  ");
    LCD_ShowString(400, 310, myInform);
    PAUSE();

    /*-------------------------- �߽���� --------------------------------------------------*/
    CVCHAINS_U16 edgeChains = YMCV_Image_Binary_Edge_Trace(myImage0, blobs);//�õ�����blobs�ı߽�����

    //��ʾ
    for (int i = 0; i < edgeChains->cNum; i++)//�����߽�����
    {
        //����������������
        CVVECTORS_U16 pixles = edgeChains->chain[i];
        for (int pi = 0; pi < pixles->dataNum; pi++)//ԭʼ�������
        {
            CVpoint* pxy = &pixles->datap[pi * pixles->dim];
            color24_t rgb = { .G = 255 };
            LCD_Fill_ConstRgbRect(pxy->x, pxy->y, 1, 1, rgb);
        }
    }
    //��Ϣ��ʾ
    sprintf(myInform, "      ��ֵͼ��߽����     ");
    LCD_ShowString(100, 300, myInform);
    //��ͣ
    PAUSE();

    /*-------------------------- EDCorner͹�Ǽ�� --------------------------------------------------*/
    CVVECTORS_U16 corner = YMCV_Fast_EdgeDrawing_ChainsCornerNum(edgeChains, 20);//���ȶ���20������

    //��ʾ
    for (int i = 0; i < corner->dataNum; i++)//�����߽�����
    {
        //ȡ�ö�Ӧ͹����Ϣ
        CV_ed_corner* thisCorner = &corner->datap[i * corner->dim];
        
        //��״�б�
        switch (thisCorner->CornerNum)//�ж�͹�Ǹ���
        {
        case 0:
            sprintf(myInform, "Բ��");
            break;
        case 1:
            sprintf(myInform, "��Բ��");
            break;
        case 2:
            sprintf(myInform, "��Բ��");
            break;
        default:
            sprintf(myInform, "%d���� \n", thisCorner->CornerNum);
            break;
        }
        //�����Ļ����ı�
        LCD_ShowString(300 + thisCorner->center.x - 16, thisCorner->center.y-5, myInform);
    }
    //��Ϣ��ʾ
    sprintf(myInform, "�߽�������״�б�  ");
    LCD_ShowString(400, 310, myInform);


    //�ͷ�blob�ڴ�
    YMCV_Free_Vector_U16_Free(blobs);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    //YMCV_Free_ImgFree(myImage1);
    YMCV_Free_Chains_U16_Free(edgeChains);//2D����洢����

    PAUSE();
    LCD_Destory();
    return 0;
}





