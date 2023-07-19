#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_EdgeCal.h"
#include"../YMCV/CORE/YMCV_MergeCal.h"
#include"../YMCV/CORE/YMCV_SegnmCal.h"
#include <stdlib.h>

/*******************************************************************************************
    *
    * ˵��  �������� �Զ�TriMap���ֶ�TriMapΪ����չʾ�˷�ˮ��ָ��Ч��
    *
    ***************************************************************************************/
CVIMAGE YMCV_GetTriMap_Demo(CVIMAGE image0);

int main()
{
    char myInform[100];
    LCD_Init(1000, 450);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(mm00_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, mm00_gray, sizeof(mm00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//�Ҷ�ͼ0


    /*-------------------------- �Զ�����TriMap --------------------------------------------------*/
    //ʹ��7-3.6���̵ķ�������TriMap
    CVIMAGE TriMapMarkers = YMCV_GetTriMap_Demo(myImage0);

    //��ʾԭͼ
    LCD_show_image(0, 0, myImage0);
    //��ʾͼƬtrimap
    LCD_show_image(600, 0, TriMapMarkers);
    //��Ϣ��ʾ
    sprintf(myInform, "TriMapͼ  ");
    LCD_ShowString(700, 310, myInform);

    PAUSE();

    /*-------------------------- ��ˮ��ָ� --------------------------------------------------*/
    YMCV_Gray_Watershed(myImage0, TriMapMarkers);//��Trimapͼ��ָ�����
     //��ʾͼƬ
    LCD_show_image(600, 0, TriMapMarkers);
    //��Ϣ��ʾ
    sprintf(myInform, "�ָ�����ͼ  ");
    LCD_ShowString(700, 310, myInform);

    CVIMAGE SegImage = YMCV_Image_Mask(TriMapMarkers, myImage0);//�ָ�ͼ��������ʹ��

    //��ʾͼƬ�ָ���
    LCD_show_image(300, 0, SegImage);
    
    //��Ϣ��ʾ
    sprintf(myInform, "�ָ���ԭͼ  ");
    LCD_ShowString(400, 310, myInform);

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(TriMapMarkers);
    YMCV_Free_ImgFree(SegImage);

    PAUSE();

    /*-------------------------- ʹ�ü��̺���� �ֶ�����TriMap --------------------------------------------------*/
    // ��cѡ���Ƿ����
    // q,bѡ��ǰ������߱����㣬�ո��˳�
    TriMapMarkers = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVTrimapType);
    YMCV_TriMap_Init(TriMapMarkers);//��ʼ���е�Ϊ�����״̬

    LCD_Clear(0);
    //��ʾԭͼ
    LCD_show_image(0, 0, myImage0);
    sprintf(myInform, "�ֶ�����ǰ����ͱ��� ��� ");
    LCD_ShowString(0, 310, myInform);

    color24_t thiscolor = { .R = 255,.G = 0 };
    char pflg = 0,qhflg=1;//Ĭ���Ȼ���ǰ����
    int qval= CVTriMapMaxVl,bval= CVTriMapMinVl;
    printf("׼�����ƣ�\n'c':��ʼ/��ͣ\n'q':ѡ��ǰ��\n'b':ѡ�񱳾�\n�ո񣺽�������");
    while (1)
    {
        int px, py,w,h;
        char mchr;
        w = TriMapMarkers->width;
        h = TriMapMarkers->height;
        //��ԭͼ�ϻ���
        if (LCD_GetXY(0, 0, myImage0->width, myImage0->height,&px,&py)&& pflg)
        {
            LCD_Draw_Point(px,  py, thiscolor);
            //���ǰ����
            if (qhflg)
            {
                TriMapMarkers->data[py * w + px] = qval;
            }
            //������
            else
            {
                TriMapMarkers->data[py * w + px] = bval;
            }
        }
        if (LCD_GetChar(&mchr))
        {
            if (mchr == ' ') { printf("��������\n"); break; }
            if (mchr == 'c')
            {
                pflg = !pflg;
                if (pflg)printf("��ʼ����\n"); else printf("��ͣ\n");
                
            }
            //���Ʊ���
            if (mchr == 'b')
            {
                qhflg = 0;
                //�޸ı��ֵ�����Ϊ��ͬ����,����+1�Ϳ��ԣ�����Ϊ����ʾЧ������+20
                bval += 20;
                if (bval >= CVTriMapMidle)bval = CVTriMapMinVl;
                printf("����=%d\n", bval);
                thiscolor.R = 0;
                thiscolor.G += rand() % 255;
                thiscolor.B = 125+ rand() % 128;
            }
            //����ǰ��
            if (mchr == 'q')
            {
                qhflg = 1;
                //�޸ı��ֵ�����Ϊ��ͬ����,����-1�Ϳ��ԣ�����Ϊ����ʾЧ������-20
                qval -= 20;
                if (qval <= CVTriMapMidle)qval = CVTriMapMaxVl;
                printf("ǰ��=%d\n", qval);
                thiscolor.R = 125 + rand() % 128;
                thiscolor.G += rand()%255;
                thiscolor.B = 0;
            }
        }
    } 
    //��ʾͼƬtrimap
    LCD_show_image(600, 0, TriMapMarkers);
    //��Ϣ��ʾ
    sprintf(myInform, "TriMapͼ  ");
    LCD_ShowString(700, 310, myInform);

    PAUSE();
    /*-------------------------- ��ˮ��ָ� --------------------------------------------------*/
    YMCV_Gray_Watershed(myImage0, TriMapMarkers);//��Trimapͼ��ָ�����
     //��ʾͼƬ
    LCD_show_image(600, 0, TriMapMarkers);
    //��Ϣ��ʾ
    sprintf(myInform, "�ָ�����ͼ  ");
    LCD_ShowString(700, 310, myInform);


    SegImage = YMCV_Image_Mask(TriMapMarkers, myImage0);//�ָ�ͼ��������ʹ��

    //��ʾͼƬ�ָ���
    LCD_show_image(300, 0, SegImage);

    //��Ϣ��ʾ
    sprintf(myInform, "�ָ���ǰ��ͼ  ");
    LCD_ShowString(400, 310, myInform);

    //�ͷ��ڴ�
    YMCV_Free_ImgFree(TriMapMarkers);
    YMCV_Free_ImgFree(SegImage);
    YMCV_Free_ImgFree(myImage0);

    PAUSE();
    LCD_Destory();
    return 0;
}




//Trimap����
CVIMAGE YMCV_GetTriMap_Demo(CVIMAGE image0)
{
    /*--------------------------����任����ǰ��ͼ--------------------------------------------------*/
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(image0->width, image0->height, CVBinaryType);

    //������ֵ��ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Thresh2ToBin, PIXChannelNum1);//��ͨ����ֵ����һ��ͨ��

    CV_pixchannel_1param myParam = { .gray_2tobin.leftThresh = 0,.gray_2tobin.rightThresh = 180 };    //������Ⱦ������
    YMCV_SingleChannel_MapTo(myRender, &myParam);//ʹ�ò���������Ⱦ��ͨ�����á���ʼ��  

    //ͼ���ֵ��
    YMCV_Image_ChannelRenderTo(image0, myImage1, myRender);//ʹ��ͨ����Ⱦ��������ȾͼƬ


    //�Զ�ֵͼ���о���任
    CVIMAGE myImage2 = YMCV_Binary_Distance_Transform(myImage1);

    //�Ծ���ͼ��ֵ��
    myParam.gray_2tobin.leftThresh = 45;
    myParam.gray_2tobin.rightThresh = 255;
    YMCV_SingleChannel_MapTo(myRender, &myParam);

    YMCV_Image_ChannelRenderTo(myImage2, myImage2, myRender);//��ֵ����Ⱦ

    /*--------------------------����任��������ͼ--------------------------------------------------*/

    //��������ȡ��ǰ���ֱ���ȡ��
    YMCV_Image_Binary_Logic_Cal(myImage1, myImage1, CV_Not);//ͼ1 �߼���  ������������ͼ1

    //�Զ�ֵͼ���о���任
    CVIMAGE myImage3 = YMCV_Binary_Distance_Transform(myImage1);

    //�Ծ���ͼ��ֵ��
    myParam.gray_2tobin.leftThresh = 40;
    myParam.gray_2tobin.rightThresh = 255;
    YMCV_SingleChannel_MapTo(myRender, &myParam);

    YMCV_Image_ChannelRenderTo(myImage3, myImage3, myRender);//��ֵ����Ⱦ

    /*-------------------------- TriMapͼ����--------------------------------------------------*/
    //����ʹ��Chanel��Ⱦ������ı�ͼ�����ͣ����ｫ�Ҷȸ�ʽǿ��תΪ��ֵͼ
    myImage2->type = CVBinaryType;
    myImage3->type = CVBinaryType;
    //ʹ��ǰ��ͼ�ͱ���ͼ������trimapͼ
    CVIMAGE TriMapImage = YMCV_BinaryTo_TriMap(myImage2, myImage3);

    //�ͷ���Ⱦ��
    YMCV_Free_PIX_RenderFree(myRender);
    //�ͷ��ڴ�
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);
    return TriMapImage;
}




