#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_MatchingCal.h"
#include"../YMCV/CORE/YMCV_AreaFillCal.h"

/*******************************************************************************************
    *
    * ˵��  �������� ��λ���ƥ��Ϊ����չʾͼ��ƽ�ƺ��ƥ��Ч��
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(1000, 480);
    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = (uint8*)CV_malloc1(sizeof(linglong00_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, linglong00_gray, sizeof(linglong00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg(myData, 500, 255, CVGrayType);//�Ҷ�ͼ

        //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData2 = (uint8*)CV_malloc1(sizeof(linglong00b_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData2, linglong00b_gray, sizeof(linglong00b_gray));
    CVIMAGE myImage_0 = YMCV_Creat_DataToImg(myData2, 100, 87, CVGrayType);//�Ҷ�ͼ

    /*---------------------------- ͼ��ƽ�� --------------------------------------------*/
    //����ͼƬ0����Ⱦ����
    CV_geom_param myParam = { .perspect.Imgin_w = myImage0->width,
                              .perspect.Imgin_h = myImage0->height,
                              .perspect.wsize_type = CV_WinSize_Const,//�̶�����
                              .perspect.scale = 1.0f };
    //�޸���Ⱦ����
    myParam.perspect.x_angle = 0;
    myParam.perspect.y_angle = 0;
    myParam.perspect.z_angle = 0;
    //����ƫ�����豣����[���� / 2������ / 2]��Χ�ڣ�
    myParam.perspect.x_move = 100; //ƽ�Ʊ任
    myParam.perspect.y_move = 50;
    myParam.perspect.scale = 1;
    //�������α任��Ⱦ��
    CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_Perspective);

    //ʹ�ò������м�����Ⱦ���á���ʼ��
    YMCV_GeomKernel_Load(myRender1, &myParam);

    //��Ŀ��ͼ��Ľ�����Ⱦ
    CVIMAGE myImage1 = YMCV_Image_Geom_RenderTo(myImage0, myRender1);

    LCD_show_image(0, 0, myImage0);
    LCD_show_image(500, 0, myImage1);
    //��Ϣ��ʾ
    sprintf(myInform, " ͼ��ƽ�� ");
    LCD_ShowString(100, 300, myInform);
    PAUSE();

    /*-------------------------- ��λ���ƥ�� --------------------------------------------------*/
    float32 moveX, moveY;
    YMCV_Image_Gray_PhaseCorrelate(myImage0, myImage1, &moveX, &moveY);
    //��ʾͼƬ
    LCD_show_image(0, 0, myImage0);

    //��Ϣ��ʾ
    sprintf(myInform, "��λ���ƥ�� ���ƫ�ƣ�x%.2f y%.2f ", moveX, moveY);
    LCD_ShowString(100, 300, myInform);
    PAUSE();

    //�ͷ��ڴ�
    YMCV_Free_GEOM_RenderFree(myRender1);
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage_0);
    YMCV_Free_ImgFree(myImage1);

    LCD_Destory();
    return 0;
}


