#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_FeatCal.h"
#include "../YMCV/CORE/YMCV_EdgeCal.h"
#include "../YMCV/CORE/YMCV_FormCge.h"
#include "../YMCV/CORE/YMCV_PubData.h"
#include<time.h>
/*******************************************************************************************
    *
    * ˵��  �������� ����ƥ��LM�㷨Ϊ����չʾ���Ӳ�ͼ����Ч��
    *
    ***************************************************************************************/
CVIMAGE YMCV_computerSAD(CVIMAGE left, CVIMAGE right);
int main()
{
    char myInform[100];
    LCD_Init(1300, 600);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(sleft_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, sleft_gray, sizeof(sleft_gray));
    CVIMAGE myImageL = YMCV_Creat_DataToImg((uint8*)myData, 425, 425, CVGrayType);//�Ҷ�ͼ0

    myData = CV_malloc1(sizeof(sright_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, sright_gray, sizeof(sright_gray));
    CVIMAGE myImageR = YMCV_Creat_DataToImg((uint8*)myData, 425, 425, CVGrayType);//�Ҷ�ͼ0
     
    /*-------------------------- �Ӳ�ͼ����--------------------------------------------------*/

    LCD_show_image(0, 0, myImageL);//ԭͼ��ʾ�����
    LCD_show_image(myImageL->width, 0, myImageR);//ԭͼ��ʾ���ұ�

    CVIMAGE dImg = YMCV_Gray_Stereo_LineMatching(myImageL, myImageR, 20, 5);//�����Ӳ����

    //CVPIX_RENDER myRender2 = YMCV_Creat_PixRender_Creat(CV_Channel_SingleToThree, PIXChannelNum3);//��ͨ��ת��ͨ��
    //CVIMAGE myImage3 = YMCV_Creat_Img_Creat(dImg->width, dImg->height, CVRgb16Type);
    ////ʹ������ͼ ��ɫ�������Ⱦ��ͨ�����á���ʼ��
    //YMCV_ThreeChannel_PaletteMapTo(myRender2, &YMCV_HeatPalette);
    ////ʹ��ͨ����Ⱦ��������ȾͼƬ
    //YMCV_Image_SingleToThree_ChannelRenderTo(dImg, myImage3, myRender2);
   
    LCD_show_image(2 * myImageL->width, 0, dImg);//�������ʾ���ұ�

        //��Ϣ��ʾ
    sprintf(myInform, "  �Ӳ����    ");
    LCD_ShowString(100, 500, myInform);
    PAUSE();

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImageL);
    YMCV_Free_ImgFree(myImageR);
    //YMCV_Free_ImgFree(myImage3);
    YMCV_Free_ImgFree(dImg);

    PAUSE();
    LCD_Destory();
    return 0;
}





