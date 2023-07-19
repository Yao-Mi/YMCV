#include"../YMCV/CORE/YMCV_DecisionTree_ID3.h"

void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 20,sty = 40;
    /*-------------------------- ID3����������ǩ����ѵ���ͷ������ --------------------------------------------------*/
    CVIMAGE myImagek = YMCV_Creat_Img_Creat(200, 150, CVGrayType);
	  LCD_Clear(WHITE);
	  YMCV_Image_FullFill(myImagek,(CVcolor){.gray=128});//����ɫΪ��ɫ
    LCD_show_image(stx + 0, sty + 0, myImagek);//��ͼ����ˢ��

    YMCV_Srand(99);//��ʼ�����������

#define dataN 500 //������500��

    //���ݱ�ǩ����Ҫ�ʹ��������ݸ�����ͬ
    uint8 labels[dataN] = {2,1,3,0,0,3,3,0,0,0,0,0,1,1,0,4,2,0,3,2,3,4,0,3,4,4,3,0,3,3,2,4,4,0,0,4,1,1,0,1,1,3,4,3,0,4,4,2,3,1,1,2,0,0,2,2,3,1,4,1,1,3,3,2,4,4,2,1,2,2,2,2,3,2,3,1,0,0,0,1,3,0,0,3,2,3,2,0,2,3,2,1,3,4,1,2,1,1,1,3,1,3,4,3,1,0,0,2,0,2,2,1,3,0,1,4,0,0,2,0,3,2,0,2,1,1,3,4,1,0,1,2,0,1,1,1,0,3,4,2,1,0,1,3,0,0,4,1,1,4,3,0,4,1,4,0,3,2,2,0,2,1,0,0,2,4,0,0,0,2,0,4,3,0,3,1,0,2,0,2,2,2,1,1,1,3,2,0,2,0,1,1,2,3,4,1,0,1,4,1,4,4,4,4,3,1,2,0,0,0,2,4,2,2,0,3,0,2,0,1,2,2,0,1,0,2,3,2,2,1,0,1,1,1,2,1,3,3,0,0,2,2,4,2,0,4,1,1,0,1,0,4,0,0,0,2,2,4,2,2,2,4,4,2,1,0,3,4,3,1,2,0,1,4,4,2,4,3,1,1,3,2,0,4,3,3,3,1,0,2,0,1,2,0,1,4,1,2,1,1,0,2,0,0,3,1,4,0,0,1,2,1,0,2,0,0,1,2,3,0,3,2,3,2,2,0,2,0,1,2,3,1,3,1,1,2,2,1,3,4,2,0,4,0,3,3,4,1,1,2,0,2,1,3,2,0,3,1,4,2,0,0,1,1,4,1,0,1,2,1,4,3,1,4,1,0,0,0,4,3,3,0,3,1,1,2,1,2,4,1,3,2,2,1,0,0,2,4,1,1,2,0,0,4,1,0,2,0,1,0,4,1,0,1,3,1,4,2,4,0,0,2,4,3,0,2,1,4,2,1,4,4,2,0,1,4,4,2,4,2,1,1,0,4,3,1,2,2,4,3,1,2,2,2,1,3,1,2,0,0,3,2,2,1,2,0,1,1,1,3,2,2,3,2,1,0,3,4,2,1,1,4,2,4,0,1,0,4,1,4,0,3,4,4,2,0,3,1,2,0,};
		
    CVVECTORS_U16 myvec = YMCV_Creat_Vector_U16(2, dataN);//��������

    for (int i = 0; i < myvec->dataNum; i++)//��ʼ������ֵ
    {
        uint16* thisvec = myvec->datap + i * myvec->dim;
        thisvec[0] = YMCV_Randint() % myImagek->width;//��
        thisvec[1] = YMCV_Randint() % myImagek->height;//��
    }

    uint8 Knum = 5;

    // ����kmeansԤ�ȷ��� �õ�������ѵ������
    //CVVECTORS_F32 center = YMCV_Creat_Vector_F32(2, Knum);//����k������,����ά����������ͬ
    ////Kmeans����������ȡ
    //YMCV_FVector_Kmeans_GetCenter(myvec, NULL, center, 100);//labels��ǩ���·���
    //YMCV_FVector_Kmeans_Classify(myvec, center, labels);


    //ID3����������
    CV_DECISION_TREE myID3 = YMCV_DecisionTree_Creat();//��������ģ��

    YMCV_DecisionTree_ID3_Trains(myID3, myvec, labels);//ID3����������ѵ��(��Ҫ����ȷ�ı�ǩ)

    //���������ݷ���
    YMCV_DecisionTree_Classify(myvec, labels,myID3);

    //��������
    for (int i = 0; i < myvec->dataNum; i++)
    {
        int16* thisvec = (int16*)myvec->datap + i * myvec->dim;
        int cl = 255 * labels[i] / (Knum - 1);//���ת��ɫ����
        //printf("%d,", labels[i]);
        //��������С���Σ���ɫ������������
        LCD_Fill_ConstRgbRect(stx + thisvec[0] - 1, sty + thisvec[1] - 1, 2, 2, YMCV_HeatPalette[cl]);
    }

    Delay(300);
    YMCV_Free_Vector_U16_Free(myvec);
    YMCV_DecisionTree_Free(myID3);//�ͷ�ģ��
    YMCV_Free_ImgFree(myImagek);
}