#include "../EGE_LCD/EGE_LCD.h"
#include "../YMCV/CONFIG/YMCV_PubType.h"
#include  "../ImageSource/imageSource.h"
#include"../YMCV/OPOBJ/YMCV_Creat.h"
#include"../YMCV/OPOBJ/YMCV_Free.h"
#include"../YMCV/CONFIG/YMCV_Mem.h"
#include "../YMCV/CORE/YMCV_ChannelCal.h"
#include "../YMCV/CORE/YMCV_PubData.h"
/*******************************************************************************************
    *
    * ˵��   ���������Զ���Ⱦɫ������ǿ����ɫ����ȾΪ����չʾ��ʹ���Զ��庯��������Ⱦ����Ρ��Ҷ�ͼת����ͼ��ʾ���Ҷ�ͼ�Զ�����ɫ��Ⱦ
    *
    ***************************************************************************************/
int32 myFunction(int32 color);
float32 myFun_k = 1.0f;
int main()
{
    char myInform[100];
    LCD_Init(1000, 480);

    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(linglong00_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, linglong00_gray, sizeof(linglong00_gray));
    CVIMAGE myImage0 = YMCV_Creat_DataToImg((uint8*)myData, 500, 255, CVGrayType);//�Ҷ�ͼ

    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, myImage0->type);
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVRgb16Type);


    /*-------------------------- ����ͼ��ɫ����ʾ --------------------------------------------------*/
    color24_t* mycstomTable = YMCV_HeatPalette;
    color24_t rectclv = { .R = 100,.G = 100,.B = 100 };
 
    uint16 rsx = 500, rsy = 400, rsw = 256, rsh = 20;
    LCD_Draw_Rect(rsx - 1, rsy - 1, rsw + 1, rsh + 2, rectclv);
    LCD_Draw_Rect(rsx - 1, rsy - 3 - rsh, rsw + 1, rsh + 2, rectclv);
    for (int i = 0 ,imax = CVMin(256,rsw); i < rsw; i++)
    {
        //�Ҷȱ����
        rectclv.R = rectclv.G = rectclv.B = i;// 0 -- 255
        LCD_Point_Color_Set(rectclv);
        LCD_Draw_Line(rsx + i, rsy, rsx + i, rsy + rsh);
        //����ͼ�����
        LCD_Point_Color_Set(mycstomTable[i]);
        LCD_Draw_Line(rsx + i, rsy - 2 - rsh, rsx + i, rsy - 2);
    }

    /*--------------------------�Զ�����ǿ--------------------------------------------------*/

    //����ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Channel_CustomTransform, PIXChannelNum1);//�Զ�����Ⱦ��
    CVPIX_RENDER myRender2 = YMCV_Creat_PixRender_Creat(CV_Channel_SingleToThree, PIXChannelNum3);//��ͨ��ת��ͨ��

    for (int i = 0; i < 10; i++)
    {
        //ʹ���Լ��ĺ�����ʼ����Ⱦͨ��
        YMCV_SingleChannel_CustomMapTo(myRender, myFunction);
        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_ChannelRenderTo(myImage0, myImage1, myRender);
        //��ʾͼƬ1
        LCD_show_image(0, 0, myImage1);
        //��ʾͨ��ת��ͼ
        LCDChannelTransformShow(myRender, 30, 320, 40);
        //��Ϣ��ʾ
        sprintf(myInform, "�Զ���������ǿ: k =%03.2f .", myFun_k);
        LCD_ShowString(100, 300, myInform);

        //תΪ����ͼ
        //ʹ������ͼ ��ɫ�������Ⱦ��ͨ�����á���ʼ��
        YMCV_ThreeChannel_PaletteMapTo(myRender2, mycstomTable);
        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_SingleToThree_ChannelRenderTo(myImage1, myImage2, myRender2);
        //��ʾͼƬ1
        LCD_show_image(500, 0, myImage2);

        //��ͣ
        PAUSE();

        //��������
        myFun_k += 1.0f;
    }
    //�ͷ���Ⱦ��1
    YMCV_Free_PIX_RenderFree(myRender);


    /*--------------------------�Զ����ɫ��Ⱦɫ--------------------------------------------------*/

    //������ɫ��
    color24_t myPalette[256] ={0};

    //ʹ��RGB��220��150��100���ԻҶ�ͼ����Ⱦɫ
    color24_t myColor = { .R = 220,.G = 150,.B = 100 };
    for (int i = 0; i < 256; i++)
    {
        myPalette[i].R = myColor.R * i / 255;
        myPalette[i].G = myColor.G * i / 255;
        myPalette[i].B = myColor.B * i / 255;
    }

    //ʹ�õ�ɫ�������Ⱦ��ͨ�����á���ʼ��
    YMCV_ThreeChannel_PaletteMapTo(myRender2, myPalette);
    //ʹ��ͨ����Ⱦ��������ȾͼƬ
    YMCV_Image_SingleToThree_ChannelRenderTo(myImage0, myImage2, myRender2);
    //��ʾͼƬ1
    LCD_Clear(0);
    LCD_show_image(0, 0, myImage0);
    LCD_show_image(500, 0, myImage2);
    sprintf(myInform, "�Զ�����ɫ��Ⱦ:RGB=��%03d,%03d,%03d��", myColor.R, myColor.G, myColor.B);
    LCD_ShowString(300, 300, myInform);
    //��ͣ
    PAUSE();

    //�ͷ���Ⱦ��2
    YMCV_Free_PIX_RenderFree(myRender2);


    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0);
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);

    PAUSE();
    LCD_Destory();
    return 0;
}

//�Զ������Ժ���
int32 myFunction(int32 color)
{
    return myFun_k * color;
}



