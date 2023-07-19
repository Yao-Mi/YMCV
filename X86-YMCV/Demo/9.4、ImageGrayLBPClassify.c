#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_MatchingCal.h"
/*******************************************************************************************
    *
    * ˵��  �������� LBP ��������Ϊ����չʾ �������� ���
    *
    ***************************************************************************************/
int main()
{
    char myInform[100];
    LCD_Init(740, 480);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(luojiuchuan_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, luojiuchuan_gray, sizeof(luojiuchuan_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 740, 400, CVGrayType);//�Ҷ�ͼ

        //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    myData = CV_malloc1(sizeof(luojiuchuan_112x112_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, luojiuchuan_112x112_gray, sizeof(luojiuchuan_112x112_gray));
    CVIMAGE myImage1 = YMCV_Creat_DataToImg((uint8*)myData, 112, 112, CVGrayType);//�Ҷ�ͼ0

    /*-------------------------- CasCade ������� --------------------------------------------------*/
    CV_cascade facedect;
    YMCV_Load_HaarCascade(&facedect, "frontface");//���� ����(��ǰ) ���������

    CVVECTORS_U16 result = YMCV_HaarCascade_Detect_Objects(myImage0, &facedect, 1.0f);

    //��ʾͼƬ
    LCD_show_image(0, 0, myImage0);
    //����������
    for (int i = 0; (result != NULL) && (i < result->dataNum); i++)
    {
        CVrect* recp = &result->datap[i * result->dim];
        //���ƾ��ο�
        color24_t rgbval = { .R = 255 };
        LCD_Draw_Rect(recp->x, recp->y, recp->w, recp->h, rgbval);
    }
    //��Ϣ��ʾ
    sprintf(myInform, " Haar Cascade ��� �������� ");
    LCD_ShowString(200, 420, myInform);
    PAUSE();


    /*-------------------------����LBPֱ��ͼ�������--------------------------------------------------*/

    //��������ͼƬ�޶����Ϊ112��112��֧��orl�������ݼ���92��112�Ĵ�С
    CV_FACE_LBP_DESC lbpDescribe = YMCV_Face_UniformLBP_Describe(myImage1);

    LCD_Clear(0);
    LCD_show_image(112, 0, myImage1);//��ʾͼƬ0
    LCDLbpHistShow(lbpDescribe, 0, 0);//��ʾLBP������
    //��Ϣ��ʾ
    sprintf(myInform, " ģ������������ȡ ");
    LCD_ShowString(200, 420, myInform);//
    PAUSE();

    float32 mindis = 1.5f;//����1.5���費���
    int  mini = -1;
    //��ȡ��Ƭ�е�����
    for (int i = 0; (result != NULL) && (i < result->dataNum); i++)
    {
        CVrect* recp = &result->datap[i * result->dim];
        //��ȡͼƬ
        CVIMAGE faceimg = YMCV_Creat_ImgROICopyTo(myImage0, recp);

        //������������112��С
        float scale_k = 112.0f / ((recp->w> recp->h)? recp->w: recp->h);//����������Ϊ112
        if (scale_k < 1.0f)
        {
            CV_geom_param myParam = { .stretch.Imgin_w = faceimg->width,
                                      .stretch.Imgin_h = faceimg->height,
                                      .stretch.out_w = scale_k * faceimg->width,
                                      .stretch.out_h = scale_k * faceimg->height, };
            //�������α任��Ⱦ��
            CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_StretchL2);//˫���Բ�ֵ
            YMCV_GeomKernel_Load(myRender1, &myParam);//ʹ�ò������м�����Ⱦ���á���ʼ��
            CVIMAGE myImage2 = YMCV_Image_Geom_RenderTo(faceimg, myRender1);//��Ⱦ

            //�ͷ�ԭ����ͼƬ�ڴ�
            YMCV_Free_ImgFree(faceimg);
            //����������С��ͼƬ���¸���
            faceimg = myImage2;
        }

        //LBP������ȡ
        CV_FACE_LBP_DESC facelbp = YMCV_Face_UniformLBP_Describe(faceimg);

        LCD_show_image(230, 0, faceimg);//��ʾͼƬ0
        LCDLbpHistShow(facelbp, 230+112, 0);//��ʾLBP������

        //����ֱ��ͼ��������ƥ��
        float32 dis = YMCV_Face_LBP_Hist_X2_Distance(lbpDescribe, facelbp);//�����ȶ�
        sprintf(myInform, " ����������� %5.2f ",dis);
        LCD_ShowString(200, 420, myInform);//

        //ȡ��С������Ϊƥ����
        if (mindis > dis)
        {
            mindis = dis;
            mini = i;
        }

        //�ͷ��ڴ�
        YMCV_Free_ImgFree(faceimg);
        YMCV_Free_FaceLbp_Hist_free(facelbp);
        PAUSE();
    }

    //��ʾƥ����
    LCD_show_image(0, 0, myImage0);
    LCD_show_image(0, 0, myImage1);//ģ��
    if (mini != -1)
    {
        color24_t rgbval = { .R = 255 };
        LCD_Draw_Rect(0, 0, myImage1->width, myImage1->height, rgbval);
        
        //���ƽ�����ο�
        CVrect* recp = &result->datap[mini * result->dim];
        color24_t rgbval2 = { .G = 255 };
        LCD_Draw_Rect(recp->x, recp->y, recp->w, recp->h, rgbval2);

        sprintf(myInform, "��Ŵ�");
        LCD_ShowString(recp->x - 16 + recp->w/2, recp->y - 16 + recp->h/2, myInform);
    } 
    sprintf(myInform, "    ƥ����չʾ    ");
    LCD_ShowString(200, 420, myInform);//

    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_FaceLbp_Hist_free(lbpDescribe);//�����ڴ��ͷ�

    PAUSE();
    LCD_Destory();
    return 0;
}


