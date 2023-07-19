#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_SpaceCal.h"
#include"../YMCV/CORE/YMCV_FeatCal.h"
#include"../YMCV/CORE/YMCV_BinaryCal.h"
#include <stdlib.h>

/*******************************************************************************************
    *
    * ˵��  �������� FishEyeΪ����չʾ������ģ�⼰����Ч��
    *
    ***************************************************************************************/

int main()
{
    int i;
    char myInform[100];
    LCD_Init(1100, 700);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(answerTmp_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, answerTmp_gray, sizeof(answerTmp_gray));
    CVIMAGE tempImg = YMCV_Creat_DataToImg((uint8*)myData, 500, 647, CVGrayType);//�Ҷ�ͼ0

    uint8* myData2 = CV_malloc1(sizeof(answer1_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData2, answer1_gray, sizeof(answer1_gray));
    CVIMAGE as1Img = YMCV_Creat_DataToImg((uint8*)myData2, 500, 647, CVGrayType);//�Ҷ�ͼ0

    //��ʾԭͼ
    LCD_show_image(0, 0, tempImg);
    LCD_show_image(510, 0, as1Img);
    //��Ϣ��ʾ
    sprintf(myInform, "ԭͼ  ");
    LCD_ShowString(0, 0, myInform);
    PAUSE();
    /*-------------------------- ��ֵ�� --------------------------------------------------*/
     //���������ֵͼ��
    CVIMAGE Tmpbin = YMCV_Creat_Img_Creat(tempImg->width, tempImg->height, CVBinaryType);//��ֵͼ
    CVIMAGE as1bin = YMCV_Creat_Img_Creat(as1Img->width, as1Img->height, CVBinaryType);//��ֵͼ
    //���ж�ֵ��
    CV_binary_param myParam = { .gray_double_t.thresh_min = 0 ,.gray_double_t.thresh_max = 170 };//ȡ0-80����
    CVBINARY_RENDER myRender1 = YMCV_Creat_BinaryRender_Creat(CV_DoubleThreshold); //������ֵ�任��Ⱦ��
    YMCV_BinaryKernel_Load(myRender1, &myParam); //���ز���������Ⱦ���á���ʼ��

    //��Ŀ��ͼ��Ľ�����Ⱦ
    YMCV_Image_ThreshTo_Binary(tempImg, Tmpbin, myRender1);
    YMCV_Image_ThreshTo_Binary(as1Img, as1bin, myRender1);

    //��ʾ��ֵͼ
    LCD_show_image(0, 0, Tmpbin);
    LCD_show_image(510, 0, as1bin);
    //��Ϣ��ʾ
    sprintf(myInform, "��ֵͼ  ");
    LCD_ShowString(0, 0, myInform);
    PAUSE();

    /*-------------------------- ��ģ��ͼ����� --------------------------------------------------*/
    YMCV_Image_Binary_Logic_Cal(Tmpbin, as1bin, CV_And);// myImage3 = myImage3 & myImage2

    LCD_show_image(510, 0, as1bin);
    //��Ϣ��ʾ
    sprintf(myInform, " �߼���  ");
    LCD_ShowString(510, 0, myInform);
    PAUSE();

    /*-------------------------- ��̬ѧ������ --------------------------------------------------*/

    CVKERN_RENDER minKernel, maxKernel;
    YMCV_PADDING_VAL = 0;//�������ֵ

    //������Ⱦ�����������ڽ��г�ʼ��
    CV_kernel_1param morpParam;//���󡢼�С�˲��������
    minKernel = YMCV_Creat_SpaceRender_Creat(CV_MinFilter, PIXChannelNum1, 1, OutPaddingConst);//�����뾶Ϊ3���˲���
    YMCV_Space_SingleKernel_Load(minKernel, &morpParam);//ʹ�ò���������Ⱦ��ͨ�����á���ʼ������Сֵ�˲�������أ��ò���Ҳ��ʡ��
    //ʹ�ÿռ���Ⱦ��������Ⱦ�Ҷ�ͼƬ
    CVIMAGE as1bin_min = YMCV_Image_Space_SigleKernel_Render(as1bin, minKernel);

    LCD_show_image(510, 0, as1bin_min);
    //��Ϣ��ʾ
    sprintf(myInform, " ��̬ѧ��ʴ  ");
    LCD_ShowString(510, 0, myInform);

    PAUSE();

    maxKernel = YMCV_Creat_SpaceRender_Creat(CV_MaxFilter, PIXChannelNum1, 1, OutPaddingConst);//�����뾶Ϊ3���˲���
    YMCV_Space_SingleKernel_Load(maxKernel, &morpParam);//ʹ�ò���������Ⱦ��ͨ�����á���ʼ������Сֵ�˲�������أ��ò���Ҳ��ʡ��
    //ʹ�ÿռ���Ⱦ��������Ⱦ�Ҷ�ͼƬ
    CVIMAGE as1bin_max = YMCV_Image_Space_SigleKernel_Render(as1bin_min, maxKernel);

    LCD_show_image(510, 0, as1bin_max);
    //��Ϣ��ʾ
    sprintf(myInform, " ��̬ѧ����  ");
    LCD_ShowString(510, 0, myInform);

    PAUSE();
    /*-------------------------- ���������ȡ --------------------------------------------------*/
    CVcolor minVal = { .gray = 150 }, maxVal = {.gray =255};
    CVVECTORS_U16 blobs = YMCV_Image_ColorBox_Find(as1bin_max, minVal, maxVal, 10, 1);

    //LCD_show_image(510, 0, as1bin_max);
    LCD_show_image(510, 0, as1Img);
    LCD_show_image(0, 0, tempImg);

    //��ʾblob��box
    color24_t thiscolor = { .R = 255 };
    LCD_Point_Color_Set(thiscolor);
    int score = 0;
    for (int i = 0; (blobs != NULL) && (i < blobs->dataNum); i++)
    {
        CVblob* blobp = blobs->datap + i * blobs->dim;//blobp= &data[i]
        //LCD_Draw_Rect(510 + blobp->box.x, blobp->box.y, blobp->box.w, blobp->box.h, thiscolor);
        //��?
        int x1 = 510 + blobp->box.x - 10;
        int x2 = 510 + blobp->box.x + 5;
        int y1 = blobp->box.y;
        int y2 = blobp->box.y + 8;
        LCD_Draw_Line(x1, y1, x2, y2);
        x1 += 30;
        y1 -= 15;
        LCD_Draw_Line(x2, y2, x1, y1);
        score++;//�÷�ͳ��
    }
    //��Ϣ��ʾ
    sprintf(myInform, "����� %d ����  ", score);
    LCD_ShowString(680, 50, myInform);

    PAUSE();
    //�ͷ��ڴ�
    YMCV_Free_ImgFree(as1Img);
    YMCV_Free_ImgFree(tempImg);
    YMCV_Free_ImgFree(as1bin);
    YMCV_Free_ImgFree(Tmpbin);
    YMCV_Free_ImgFree(as1bin_min);
    YMCV_Free_ImgFree(as1bin_max);
    YMCV_Free_KERN_RenderFree(minKernel);
    YMCV_Free_KERN_RenderFree(maxKernel);
    PAUSE();
    LCD_Destory();
    return 0;
}

