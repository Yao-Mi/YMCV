#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_SegnmCal.h"
#include"../YMCV/CORE/YMCV_DecisionTree_ID3.h"
#include <stdlib.h>

/*******************************************************************************************
    *
    * ˵��  �������� ID3 ����������Ϊ����չʾ������ʶ���Ч��
    *
    ***************************************************************************************/

int main()
{
    char myInform[100];
    LCD_Init(1000, 500);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(number0_9_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, number0_9_gray, sizeof(number0_9_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 400, 252, CVGrayType);//�Ҷ�ͼ0

    /*-------------------------- ��ֵ�� --------------------------------------------------*/
     //���������ֵͼ��
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);//��ֵͼ
    CV_binary_param myParam = { .gray_double_t.thresh_min = 0 ,.gray_double_t.thresh_max = 50 };

    CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_DoubleThreshold);//������ֵ�任��Ⱦ��
    YMCV_BinaryKernel_Load(myRender1, &myParam);//���ز���������Ⱦ���á���ʼ��
    YMCV_Image_ThreshTo_Binary(myImage0, myImage2, myRender1);//��Ŀ��ͼ��Ľ�����Ⱦ

    LCD_Clear(0);
    //��ʾԭͼ
    LCD_show_image(0, 0, myImage0);
    LCD_show_image(400, 0, myImage2);
    sprintf(myInform, " ��ֵ�� ");
    LCD_ShowString(0, 310, myInform);
    PAUSE();
    /*-------------------------- ������ģ�ͼ��� --------------------------------------------------*/
    uint8 decision_model[] = "[0,0,0,12]LR[205,0,0,14]LR[205,0,0,10]LR[205,0,0,11]LR[205,0,1,8]LR[205,0,0,15]LR[205,0,0,13]LR[8,1,0,-12851]--[205,0,1,20]LR[0,1,0,-12851]--[6,1,0,-12851]--[4,1,0,-12851]--[1,1,0,-12851]--[9,1,0,-12851]--[205,0,1,15]LR[2,1,0,-12851]--[7,1,0,-12851]--[5,1,0,-12851]--[3,1,0,-12851]--";
    CV_DECISION_TREE myID3 = YMCV_DecisionTree_Creat();//����ID3����ģ��
    YMCV_DecisionTree_Load(myID3, decision_model);//ģ�ͼ���

    /*-------------------------- y,xͶӰ�ַ��ָ����ȡ�������� --------------------------------------------------*/
    CVVECTORS_U16 yhist = YMCV_Binary_XY_Axis_Proj(myImage2, 1);//�Ȱ�y��ͶӰ
    YMCV_Binary_Proj_Hist_Seg(yhist, 20, 5);//ͶӰֵͳ��С��20������С��5������Ϊ0
    LCDProjHistShow(yhist, 1, 0, 250);//��ʾͶӰͼ
    //��y�������
    for (int y = 0; y < yhist->dataNum; y++)
    {
        //ȡ��0�����λ��
        if (yhist->datap[y] > 0)
        {
            int ry = y;
            while ((ry < yhist->dataNum) && (yhist->datap[ry]))
            {
                ry++;
            }
            //��ȡ����ͼƬ
            CVIMAGE ySegImg = YMCV_Creat_ImgCopyTo(myImage2, 0, y, myImage2->width, ry - y);
            LCD_show_image(0, 0, ySegImg);
            PAUSE();
            //x��ͶӰ����
            CVVECTORS_U16 xhist = YMCV_Binary_XY_Axis_Proj(ySegImg, 0);//ͶӰ��x��
            YMCV_Binary_Proj_Hist_Seg(xhist, 6, 5);//��ͶӰֱ��ͼ���зָ�
            LCDProjHistShow(xhist, 0, 0, 80);//��ʾͶӰͼ
            PAUSE();
            //��x�������
            for (int x = 0; x < xhist->dataNum; x++)
            {
                //ȡ��0�����λ��
                if (xhist->datap[x] > 0)
                {
                    int rx = x;
                    while ((rx < xhist->dataNum) && (xhist->datap[rx]))
                    {
                        rx++;
                    }
                    //��ȡ����ͼƬ
                    CVIMAGE xSegImg = YMCV_Creat_ImgCopyTo(ySegImg, x, 0, rx - x, ySegImg->height);
                    LCD_show_image(400, 250, xSegImg);
                    //PAUSE();
                    /*-------------------------- ����������ȡ--------------------------------------------------*/
                    //��ͼ���Ϊ16��16��С
                    CVIMAGE resultImg = YMCV_Binary_MaxPooling_ScaleSizeTo_MxM(xSegImg, 16);
                    LCD_show_image(500, 250, resultImg);
                    CVVECTORS_U16 numberhist = YMCV_Binary_Number16x16_ProjHist16(resultImg);//1x16
                    LCDProjHistShow(numberhist, 0, 530, 250);//��ʾͶӰ����ͼ
                    sprintf(myInform, "16x1����������");
                    LCD_ShowString(510, 300, myInform);
                    //ǿ��תΪ16x1
                    numberhist->dim = numberhist->dataNum;
                    numberhist->dataNum = 1;
                    uint8 lable;

                    YMCV_DecisionTree_Classify(numberhist, &lable,myID3);//�������б�
                    sprintf(myInform, "ʶ����Ϊ %d ", lable);
                    LCD_ShowString(550, 250, myInform);

                    YMCV_Free_ImgFree(resultImg);
                    YMCV_Free_Vector_U16_Free(numberhist);
                    PAUSE();
                    LCD_Clear(0);
                    /*-----------------------------------------------------------------------------------------------*/
                    //�ͷ��ڴ�
                    YMCV_Free_ImgFree(xSegImg);
                    //�����Ѿ�ȡ��������
                    x = rx;
                }
            }
            //�ͷŽ�ͼ�ڴ��ͶӰͼ
            YMCV_Free_ImgFree(ySegImg);
            YMCV_Free_Vector_U16_Free(xhist);
            //�����Ѿ�ȡ��������
            y = ry;
        }
    }
    //�ͷ��ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_Vector_U16_Free(yhist);

    PAUSE();
    LCD_Destory();
    return 0;
}
