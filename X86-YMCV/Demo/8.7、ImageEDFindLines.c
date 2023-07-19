#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_EdgeCal.h"
#include"../YMCV/CORE/YMCV_FeatCal.h"
#include"../YMCV/CORE/YMCV_BasicObjCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� Canny�˲���Ϊ����չʾ�˱�Ե���Ч��
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 550);
    color24_t pencolor = { .R = 255 };
    LCD_Point_Color_Set(pencolor);//���û�����ɫ

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(tuxin_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, tuxin_gray, sizeof(tuxin_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 250, 140, CVGrayType);//�Ҷ�ͼ0

    /*-------------------------ED��Ե���--------------------------------------------------*/
    //���������ֵͼ��
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVBinaryType);//��ֵͼ

    CVCHAINS_U16 chains = YMCV_Fast_EdgeDrawing(myImage0, CV_Sobel_Graident, 5, myImage2);

    LCD_Clear(0);
    LCD_show_image(0, 0, myImage0);//��ʾͼƬ0
    LCD_show_image(300, 0, myImage2);  //��ʾͼƬ2

    //��Ϣ��ʾ
    sprintf(myInform, "ED ֱ�߼�� ");
    LCD_ShowString(100, 310, myInform);

    /*-------------------------- EDLinesֱ�߼�� --------------------------------------------------*/
    CVVECTORS_U16 lines = YMCV_Fast_EdgeDrawing_Lines(chains, 0.25f, 9);

    YMCV_Merge_LineSegments(lines, 3, 20);//���Խ��кϲ�
    for (size_t i = 0; i < lines->dataNum; i++)
    {
	    color24_t rgb = { .G = 255 };
	    LCD_Point_Color_Set(rgb);
	    CV_ed_line* point_p = lines->datap + i * lines->dim;
	    LCD_Draw_Line(point_p->start_end.x1, point_p->start_end.y1, point_p->start_end.x2, point_p->start_end.y2);
       // PAUSE(); //��ͣ   
    }

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_Chains_U16_Free(chains);//2D����洢����
    YMCV_Free_Vector_U16_Free(lines);//ֱ���ڴ�����

    PAUSE();
    LCD_Destory();
    return 0;
}





