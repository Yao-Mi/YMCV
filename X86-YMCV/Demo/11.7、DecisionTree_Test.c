#include "../EGE_LCD/EGE_LCD.h"
#include"../YMCV/CORE/YMCV_PubData.h"
#include"../YMCV/CORE/YMCV_DecisionTree_ID3.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CORE/YMCV_InnerMath.h"

int main()
{
    LCD_Init(1000, 450);

    /*-------------------------- ID3����������ǩ����ѵ���ͷ������ --------------------------------------------------*/
    CVIMAGE myImagek = YMCV_Creat_Img_Creat(250, 150, CVGrayType);
    LCD_show_image(0, 0, myImagek);//��ͼ����ˢ��

    YMCV_Srand(99);//��ʼ�����������

#define dataN 500 //������2000��

    //���ݱ�ǩ����Ҫ�ʹ��������ݸ�����ͬ
    uint8 labels[dataN] = { 1,0,1,2,2,1,1,3,0,3,3,0,3,3,2,1,4,0,2,1,3,3,0,3,1,4,3,3,3,3,1,1,4,0,0,4,0,3,2,2,3,1,3,4,3,3,3,1,4,2,2,0,0,0,1,3,4,0,1,2,0,4,4,1,4,3,4,0,1,1,1,3,3,4,1,0,0,0,0,0,1,0,2,1,4,1,1,2,2,1,1,0,2,2,0,3,0,3,0,1,0,3,1,2,2,0,0,1,0,1,1,0,1,2,2,4,2,2,3,2,2,1,2,4,2,2,2,1,3,0,3,4,3,2,2,2,3,4,3,3,2,3,0,1,3,0,3,0,3,3,4,3,1,0,3,2,3,1,4,0,4,0,0,2,4,1,3,2,0,4,2,1,1,0,1,3,3,4,0,1,1,4,2,0,2,4,4,3,1,3,0,2,3,2,3,3,3,3,4,2,4,4,1,4,1,0,1,2,0,0,1,4,1,4,2,1,2,4,0,3,4,1,0,3,3,3,1,1,3,0,0,2,0,0,1,0,3,4,0,0,1,1,3,0,3,1,2,2,0,2,2,3,1,3,2,1,1,1,4,4,0,4,4,1,0,2,1,1,1,3,4,0,0,4,3,3,4,1,3,2,1,1,2,1,3,1,1,0,3,3,0,0,2,0,3,4,0,1,0,2,0,1,0,2,4,3,1,2,3,3,1,3,3,1,2,2,3,1,4,2,4,4,1,3,1,2,1,2,3,1,0,2,3,2,3,4,4,3,3,4,1,2,4,2,1,1,4,2,2,3,3,3,3,4,4,2,3,2,3,3,2,0,2,0,1,3,2,3,1,0,4,3,2,3,3,0,2,3,3,4,1,2,4,3,3,4,3,3,1,2,1,1,4,0,3,0,1,3,3,3,1,2,2,1,0,3,1,3,0,2,3,3,0,2,4,3,4,3,1,0,0,1,4,4,2,2,0,3,4,2,1,3,1,3,3,4,4,4,4,1,2,0,0,4,1,0,4,4,4,0,0,1,3,4,3,1,3,1,0,3,3,4,4,0,4,2,0,3,0,4,1,4,4,1,3,3,1,4,3,0,0,3,3,1,2,3,0,1,2,1,0,4,4,4,4,2,3,3,4,0, };

    CVVECTORS_U16 myvec = YMCV_Creat_Vector_U16(2, dataN);//��������

    for (int i = 0; i < myvec->dataNum; i++)//��ʼ������ֵ
    {
        int16* thisvec = (int16*)myvec->datap + i * myvec->dim;
        thisvec[0] = YMCV_Randint() % 250;//��
        thisvec[1] = YMCV_Randint() % 150;//��
    }

    uint8 Knum = 5;

    // ����kmeansԤ�ȷ��� �õ�������ѵ������
    //CVVECTORS_F32 center = YMCV_Creat_Vector_F32(2, Knum);//����k������,����ά����������ͬ
    ////Kmeans����������ȡ
    //YMCV_FVector_Kmeans_GetCenter(myvec, NULL, center, 100);//labels��ǩ���·���
    //YMCV_FVector_Kmeans_Classify(myvec, center, labels);


    //ID3����������
    CV_DECISION_TREE myID3 = YMCV_DecisionTree_Creat();//��������ģ��

    YMCV_DecisionTree_ID3_Trains(myID3, myvec, labels);//ID3����������ѵ��

    //���������ݷ���
    YMCV_DecisionTree_Classify(myvec, labels,myID3);

    //��������
    for (int i = 0; i < myvec->dataNum; i++)
    {
        int16* thisvec = (int16*)myvec->datap + i * myvec->dim;
        int cl = 255 * labels[i] / (Knum - 1);//���ת��ɫ����
        //printf("%d,", labels[i]);
        //��������С���Σ���ɫ������������
        LCD_Fill_ConstRgbRect(thisvec[0] - 1, thisvec[1] - 1, 2, 2, YMCV_HeatPalette[cl]);
    }

    PAUSE();
    YMCV_DecisionTree_Free(myID3);//�ͷ�ģ��
    YMCV_Free_ImgFree(myImagek);
}