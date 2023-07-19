#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/IOFILE/YMCV_IOFile.h"
#include"../YMCV/QRCODE/YMCV_Codes.h"
/*******************************************************************************************
    *
    * ˵��  ������չʾ�� QR�����
    *
    ***************************************************************************************/
int main()
{
    LCD_Init(1000, 480);
    char myInform[100];
    //��Ϣ��ʾ
    sprintf(myInform, "QRCode ��ά�����  ");
    LCD_ShowString(400, 410, myInform);
    /*----------------------------------- ͼ����ת ------------------------------------------*/

    CVIMAGE myImage0 = YMCV_Image_Read("D:/Documents/1012_YMCV/ͼƬ�ز�/QRCode/QR-test.jpg", CVGrayType);

    CV_geom_param myParam = { .rotate.Imgin_w = myImage0->width,
                            .rotate.Imgin_h = myImage0->height,
                            .rotate.angle = -20,//20��
                            .rotate.out_w = myImage0->width ,//�̶�������Ҫ���������С
                            .rotate.out_h = myImage0->height };
    YMCV_PADDING_VAL = 100;//�հ������ֵ

    CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_RotateWconst);//�������α任��Ⱦ��
    YMCV_GeomKernel_Load(myRender1, &myParam);//ʹ�ò������м�����Ⱦ���á���ʼ��
    CVIMAGE myImage1 = YMCV_Image_Geom_RenderTo(myImage0, myRender1);//��Ŀ��ͼ��Ľ�����Ⱦ

    //��ʾͼƬ
    LCD_show_image(0, 0, myImage1);

    // QR��ʶ��
    CVVECTORS_U16 result = YMCV_Gray_Find_QRCode(myImage1);

    //��ʾ���
    for (int i = 0; (result != NULL) && i < result->dataNum; i++)
    {
        CV_qrcodes_data* tda = &result->datap[i * result->dim];

        color24_t rgbcv = { .G = 255 }; LCD_Point_Color_Set(rgbcv);
        //���ƾ��ο�
        LCD_Draw_Line(tda->corners[0].x, tda->corners[0].y, tda->corners[1].x, tda->corners[1].y);// 0 -> 1
        LCD_Draw_Line(tda->corners[1].x, tda->corners[1].y, tda->corners[2].x, tda->corners[2].y);// 1 -> 2
        LCD_Draw_Line(tda->corners[2].x, tda->corners[2].y, tda->corners[3].x, tda->corners[3].y);// 2 -> 3
        LCD_Draw_Line(tda->corners[3].x, tda->corners[3].y, tda->corners[0].x, tda->corners[0].y);// 3 -> 0

        // 0 -> 1 -> 2 -> 3 : �ڣ��ң��֣���ǳ�죬��
        rgbcv = (color24_t){ .R = 0 }; LCD_Fill_ConstRgbRect(tda->corners[0].x - 2, tda->corners[0].y - 2, 5, 5, rgbcv);
        rgbcv = (color24_t){ .R = 100 }; LCD_Fill_ConstRgbRect(tda->corners[1].x - 2, tda->corners[1].y - 2, 5, 5, rgbcv);
        rgbcv = (color24_t){ .R = 180 }; LCD_Fill_ConstRgbRect(tda->corners[2].x - 2, tda->corners[2].y - 2, 5, 5, rgbcv);
        rgbcv = (color24_t){ .R = 255 }; LCD_Fill_ConstRgbRect(tda->corners[3].x - 2, tda->corners[3].y - 2, 5, 5, rgbcv);

        //�������� 
        rgbcv = (color24_t){ .G = 150 };
        int centerx = tda->rect.x + tda->rect.w / 2; //��Ӿ��ο������
        int centery = tda->rect.y + tda->rect.h / 2;
        LCD_Fill_ConstRgbRect(centerx - 2, centery - 2, 5, 5, rgbcv);
        LCD_Draw_Rect(tda->rect.x, tda->rect.y, tda->rect.w, tda->rect.h, rgbcv);//��Ӿ��ο�

        // DM�������
        printf("QR������:%s\n����Ϊ%d���ַ�����������\\0�� ", tda->payload, tda->payload_len);

        //�汾����
        printf(" �汾 %d ", tda->version);
    }

    YMCV_Free_Vector_U16_Free(result);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);

    PAUSE();
    LCD_Destory();
    return 0;
}


