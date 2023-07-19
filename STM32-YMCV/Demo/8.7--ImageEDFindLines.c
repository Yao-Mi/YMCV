#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  �������� Canny�˲���Ϊ����չʾ�˱�Ե���Ч��
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
    sprintf(myInform, "ED Find Lines ");
    LCD_ShowString(stx + 0, sty + 0, myInform);
    Delay(300);//��ͣ

    /*-------------------------- EDLinesֱ�߼�� --------------------------------------------------*/
    CVVECTORS_U16 lines = YMCV_Fast_EdgeDrawing_Lines(chains, 0.25f, 6);

    YMCV_Merge_LineSegments(lines, 3, 20);//���Խ��кϲ�
    for (size_t i = 0; i < lines->dataNum; i++)
    {
	    color24_t rgb = { .R = 255 };
	    LCD_Point_Color_Set(rgb);
	    CV_ed_line* point_p = (CV_ed_line*)(lines->datap + i * lines->dim);
	    LCD_Draw_Line(stx + point_p->start_end.x1, point_p->start_end.y1, stx + point_p->start_end.x2, point_p->start_end.y2);  
    }

		Delay(300); //��ͣ 
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_Chains_U16_Free(chains);//2D����洢����
    YMCV_Free_Vector_U16_Free(lines);//ֱ���ڴ�����
}





