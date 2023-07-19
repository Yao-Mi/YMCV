

#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include"../YMCV/CORE/YMCV_Special_Effect.h"
#include"../YMCV/CORE/YMCV_AreaFillCal.h"
#include"../YMCV/IOFILE/YMCV_IOFile.h"
#include <stdlib.h>

/*******************************************************************************************
    *
    * ˵��  �������� ����������Ϊ����չʾ��������ѧϰЧ��
    *
    ***************************************************************************************/

#include "../YMCV/CORE/YMCV_FastMath.h"
#include "../YMCV/CORE/YMCV_MergeCal.h"

#include"../YMCV/CORE/YMCV_FormCge.h"
#include"../YMCV/CORE/YMCV_MatchingCal.h"

int main()
{
    int i;
    char myInform[100];
    LCD_Init(1000, 600);
    uint16 movex = 50, movey = 50;//ƽ����
    CVIMAGE Backimg = YMCV_Creat_Img_Creat(350, 350,CVGrayType);
    CVIMAGE calibrat = YMCV_Image_Read("D:/Documents/1012_YMCV/ͼƬ�ز�/GAN-Face/template.jpg", CVRgb16Type);
    //��ʾԭͼ
    LCD_show_image(0, 0, Backimg);
    LCD_show_image(400, 0, calibrat);

    sprintf(myInform, "��ȡ������� ");
    LCD_ShowString(0, 310, myInform);

    /*-------------------------- ��ȡ����ʾ������� --------------------------------------------------*/

    color24_t thiscolor = { .R = 0,.G = 255 };//��ɫ
    LCD_Point_Color_Set(thiscolor);

    thiscolor = (color24_t){ .R = 255 };//��ɫ
    
    FILE* recordfile = fopen("D:/Documents/1012_YMCV/ͼƬ�ز�/GAN-Face/CalibratingData_ok.txt", "r");//��ֻ����ʽ��
    uint8 databuff[400];
    uint16 outdi = 0, fupNum = 30;
    uint16 outdata[200] = {0};
    //40�������㣬�������
    CVVECTORS_F32 Xpts = YMCV_Creat_Vector_F32(40 * sizeof(CVfpoint) / sizeof(float32), fupNum);

    
    //��ȡ30��������
    for (int i = 0; i < fupNum; i++)
    {
        sprintf(myInform, "D:/Documents/1012_YMCV/ͼƬ�ز�/GAN-Face/%d.jpg", i);
        //��ȡһ��ͼƬ
        CVIMAGE PrImg = YMCV_Image_Read(myInform, CVGrayType);

        YMCV_Image_Cover(PrImg, Backimg, movex, movey);//�ϲ�����ɫͼƬ��

        LCD_show_image(0, 0, Backimg);
        //�ӱ�������ı��л�ȡ����
        fgets(databuff, sizeof(databuff), recordfile);//��ȡһ��
        //�ı�ת����
        char* sptr = databuff;
        outdi = 0;
        while (sptr != NULL)
        {
            if ((*sptr) == '\n')//��β����
                break;
            outdata[outdi++] = strtol(sptr, &sptr, 10) + movex;
            outdata[outdi++] = strtol(sptr, &sptr, 10) + movey;
        }

        //��ʾ
        LCD_FaceFeaturePoint_Show(outdata, 0, 0);
        //��¼
        CVfpoint* fup = &Xpts->datap[i * Xpts->dim];
        for (int pi = 0; pi < 40; pi++)//40��������
        {
            fup[pi].x = outdata[2 * pi + 0];
            fup[pi].y = outdata[2 * pi + 1];
        }
        //PAUSE();
        //Delay(20);
        //�ͷ�֮ǰ�ڴ�
        YMCV_Free_ImgFree(PrImg); 
    }
    fclose(recordfile);//�ر��ļ�

    /*-------------------------- ����ѵ�� --------------------------------------------------*/

    CV_facefpoint_train mybase = YMCV_FacePoint_Train_Bases(Xpts);//���������������������ͶӰ���

    //�����������
    for (int i = 0; i < fupNum; i++)//��ȡ30��ͼ��
    {
        sprintf(myInform, "D:/Documents/1012_YMCV/ͼƬ�ز�/GAN-Face/%d.jpg", i);
        //��ȡһ��ͼƬ
        CVIMAGE PrImg = YMCV_Image_Read(myInform, CVGrayType);
        YMCV_Image_Cover(PrImg, Backimg, movex, movey);//�ϲ�����ɫͼƬ�У����Ҫ��֮ǰ��ȡ��ʱ��һ��

        //��ͼ����б任���ͱ�׼ģ����
        float32 matdata[9] = { 0.0f };
        //����任����
       // a -b  0   1  0  -cx    a -b  -(a*cx - b*cy)
       // b  a  0 * 0  1  -cy =  b  a  -(b*cx + a*cy)
       // 0  0  1   0  0   1     0  0   1
       float32* alm = &mybase.alignMat->datap[i * mybase.alignMat->col];
       matdata[0] = alm[0]; matdata[1] = alm[1];
       matdata[3] = alm[3]; matdata[4] = alm[4];
       matdata[8] = 1.0f;
       float cx = alm[2], cy = alm[5];
       matdata[2] = -(alm[0] * cx + alm[1] * cy);
       matdata[5] = -(alm[3] * cx + alm[4] * cy);

       CVfmat tmat = { .row = 3,.col = 3,.datap = matdata };
       //����
       //s  0  0   a -b  tx    s*a -b   tx
       //0  s  0 * b  a  ty =   b  s*a  ty
       //0  0  1   0  0  1      0   0   1
       tmat.datap[0] *= mybase.scale;//x������
       tmat.datap[4] *= mybase.scale;//y
       //ƫ�ƻ�����
       //1  0  cx   a -b  tx    a -b  (tx + cx)
       //0  1  cy * b  a  ty =  b  a  (tx + cy)
       //0  0   1   0  0  1      0  0   1
       //tmat.datap[2] += cx;
       //tmat.datap[5] += cy;

       //����תΪ���α任��Ⱦ��
       CVGEOM_RENDER geoHrender = YMCV_HomographyMat_ToPerspective_GeomRender(&tmat, Backimg->width, Backimg->height, CV_WinSize_Const);//�̶�����
       CVIMAGE myImage2 = YMCV_Image_Geom_RenderTo(Backimg, geoHrender);//������Ⱦ

       LCD_show_image(0, 0, myImage2);//��ʾ
        //��������б任
       uint16 xypoints[40 * 2];
       CVfpoint* fup = &Xpts->datap[i * Xpts->dim];
       for (int pi = 0; pi < 40; pi++)//40��������
       {
           xypoints[2 * pi + 0] = matdata[0] * fup[pi].x + matdata[1] * fup[pi].y + matdata[2];
           xypoints[2 * pi + 1] = matdata[3] * fup[pi].x + matdata[4] * fup[pi].y + matdata[6];
       }
       //patchѵ��
       YMCV_FacePoint_Train_Patch(&mybase, myImage2, xypoints, 40, fupNum);
       for (int pi = 0; pi < 40; pi++)//40��������
       {
           //��ʾpatchͼ
           CVimage fimg = { .width = mybase.patch_d ,.height = mybase.patch_d,.type = CVFGrayType,.data = &mybase.patchModel->datap[pi * mybase.patchModel->col] };
           CVIMAGE myImage1 = YMCV_Creat_Img_Creat(fimg.width, fimg.height, CVGrayType);
           YMCV_Image_Pix_Change_Form(&fimg, myImage1, CV_FGrayToGray2);
           LCD_show_image(xypoints[2 * pi + 0] - mybase.patch_d / 2, xypoints[2 * pi + 1] - mybase.patch_d / 2, myImage1);
           YMCV_Free_ImgFree(myImage1);
       }

       //Cascade���������
       CV_cascade facedect;
       YMCV_Load_HaarCascade(&facedect, "frontface");//���� ����(��ǰ) ���������
       CVVECTORS_U16 result = YMCV_HaarCascade_Detect_Objects(myImage2, &facedect, 1.0f);//�������

       if (result== NULL) printf("û��⵽����");
       else if (result->dataNum != 1) printf("��⵽�������");
       CVrect* recp = result->datap;

       //�������������ƫ����ѵ��
       YMCV_FacePoint_Train_bindingFaceDector(&mybase, recp, xypoints, 40, fupNum);

       //����������//���ƾ��ο�
       color24_t rgbval = { .R = 255 };
       LCD_Draw_Rect(recp->x, recp->y, recp->w, recp->h, rgbval);

       YMCV_Free_Vector_U16_Free(result);
       //LCD_FaceFeaturePoint_Show(xypoints, 0, 0);//��ʾ������
       Delay(30);
       //PAUSE();
        //�ͷ�֮ǰ�ڴ�
       YMCV_Free_ImgFree(PrImg);
       YMCV_Free_ImgFree(myImage2);
       YMCV_Free_GEOM_RenderFree(geoHrender);
    }

    //����
    CV_facefpoint_dector facepoint_dect;
    YMCV_Load_FacePointDector(&facepoint_dect, "frontfacepoint");
    for (int i = 0; i < fupNum; i++)//��ȡ30��ͼ��
    {
        sprintf(myInform, "D:/Documents/1012_YMCV/ͼƬ�ز�/GAN-Face/%d.jpg", i);
        //��ȡһ��ͼƬ
        CVIMAGE PrImg = YMCV_Image_Read(myInform, CVGrayType);
        YMCV_Image_Cover(PrImg, Backimg, movex, movey);//�ϲ�����ɫͼƬ�У����Ҫ��֮ǰ��ȡ��ʱ��һ��

        LCD_show_image(0, 0, Backimg);

       //Cascade���������
        CV_cascade facedect;
        YMCV_Load_HaarCascade(&facedect, "frontface");//���� ����(��ǰ) ���������
        CVVECTORS_U16 result = YMCV_HaarCascade_Detect_Objects(Backimg, &facedect, 1.0f);//�������
        CVrect* recp = result->datap;

        CVVECTORS_U16 facepts = YMCV_FacePoint_Detect_Objects(Backimg, &facepoint_dect, recp,15, 20);

        LCD_FaceFeaturePoint_Show(facepts->datap, 0, 0);//��ʾ

        //����������//���ƾ��ο�
        color24_t rgbval = { .R = 255 };
        LCD_Draw_Rect(recp->x, recp->y, recp->w, recp->h, rgbval);
        PAUSE();
        YMCV_Free_Vector_U16_Free(result);
    }
    // ------------------- ��ʾ��׼ģ�ͺ���������Ĺ�ϵ --------------------------

    CVrect demo = { .x = 200,.y = 100,.w = 150,.h = 150 };
    LCD_Draw_Rect(demo.x, demo.y, demo.w, demo.h, (color24_t){ .R = 255 });
    float32 dcenterx = demo.x + demo.w / 2.0f;
    float32 dcentery = demo.y + demo.h / 2.0f;
    float32 recw = demo.w;
    for (int i = 0; i < 40; i++)
    {
        CVimage fimg = { .width = mybase.patch_d ,.height = mybase.patch_d,.type = CVFGrayType,.data = &mybase.patchModel->datap[i * mybase.patchModel->col] };
        CVIMAGE myImage1 = YMCV_Creat_Img_Creat(fimg.width, fimg.height, CVGrayType);
        YMCV_Image_Pix_Change_Form(&fimg, myImage1, CV_FGrayToGray2);
       
        float px = mybase.standModel->datap[2 * i + 0];
        float py = mybase.standModel->datap[2 * i + 1];

        outdata[2 * i + 0] = px * recw * mybase.facedec_offsets + dcenterx + mybase.facedec_offsetx;
        outdata[2 * i + 1] = py * recw * mybase.facedec_offsets + dcentery + mybase.facedec_offsety;
        //��ʾͼƬ1
        LCD_show_image(outdata[2 * i + 0]- mybase.patch_d / 2, outdata[2 * i + 1]- mybase.patch_d / 2, myImage1); 
        YMCV_Free_ImgFree(myImage1);
    }
    LCD_FaceFeaturePoint_Show(outdata, 0, 0);//��ʾ

    // ---------------------------- ģ����� -----------------------
    printf("��׼ģ�ͣ�\n");
    for (int i = 0; i < mybase.standModel->dim; i++)//
    {
        printf("%f, ", mybase.standModel->datap[i]);
    }
    printf("\n���Ͼ���\n");
    for (int i = 0; i < mybase.CombBase->row; i++)//
    {
        for (int j = 0; j < mybase.CombBase->col; j++)
        {
            printf("%f, ", mybase.CombBase->datap[i * mybase.CombBase->col +j]);
        }
        printf("\n");
    }
    printf("\n����������\n");
    for (int i = 0; i < mybase.patchModel->row; i++)//
    {
        for (int j = 0; j < mybase.patchModel->col; j++)
        {
            printf("%f, ", mybase.patchModel->datap[i * mybase.patchModel->col + j]);
        }
        printf("\n");
    }
    printf("\n d:%d\n", mybase.patch_d);
    printf("\n offset: %f,%f,%f\n", mybase.facedec_offsetx, mybase.facedec_offsety, mybase.facedec_offsets);
    //�ͷ��ڴ�
    YMCV_Free_ImgFree(calibrat);
    YMCV_Free_ImgFree(Backimg);

    PAUSE();
    LCD_Destory();
    return 0;
}


////                          0                          9   10                19  20    22 23    25 26 27 28 29 30 31 32 33 34 35,36,37 38 39
//int8 ReflectiveIndex[40] = { 10,11,12,13,14,15,16,17,18,19, 0,1,2,3,4,5,6,7,8,9, 25,24,23,22,21,20,29,28,27,26,31,30,33,32,35,34,37,36,38,39 };
////���ҶԳƵ������㣬��������������
//for (int dai = 0; dai < dataNum; dai++)
//{
//    CVfpoint* xout0 = &Xredo->datap[dai * Xredo->dim];
//    CVfpoint* xout1 = &Xredo->datap[(dataNum + dai) * Xredo->dim];//�ڶ���
//    //����x�ᷴ�Գ�
//    for (int i = 0; i < 40; i++)
//    {
//        int rdi = ReflectiveIndex[i];
//        xout1[i].x = -xout0[rdi].x;
//        xout1[i].y = xout0[rdi].y;
//
//        color24_t pencolor = YMCV_HeatPalette[((i + 1) * 389) % 255];
//        //����������
//        LCD_Fill_ConstRgbRect(200 + xout0[i].x - 1, 200 + xout0[i].y - 1, 3, 3, pencolor);
//        LCD_Fill_ConstRgbRect(200 + xout1[i].x - 1, 200 + xout1[i].y - 1, 3, 3, pencolor);
//    }
//}

