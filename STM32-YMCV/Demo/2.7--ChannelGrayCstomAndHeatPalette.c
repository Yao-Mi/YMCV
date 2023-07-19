
/*******************************************************************************************
    *
    * ˵��   ���������Զ���Ⱦɫ������ǿ����ɫ����ȾΪ����չʾ��ʹ���Զ��庯��������Ⱦ����Ρ��Ҷ�ͼת����ͼ��ʾ���Ҷ�ͼ�Զ�����ɫ��Ⱦ
    *
    ***************************************************************************************/
int32 myFunction(int32 color);
float32 myFun_k = 1.0f;

void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage1 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
    CVIMAGE myImage2 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
    CVIMAGE myImage3 = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVRgb16Type);

		YMCV_Image_Pix_Change_Form(myImage0, myImage1, CV_Rgb16ToGray);//rgbת�Ҷ�ͼ
	  //��ʾͼƬ1
    LCD_Clear(0);
    LCD_show_image(stx + 0, 0, myImage1);
	
    /*-------------------------- ����ͼ��ɫ����ʾ --------------------------------------------------*/
    color24_t* mycstomTable = (color24_t*)YMCV_HeatPalette;
    color24_t rectclv = { .R = 100,.G = 100,.B = 100 };
 
    uint16 rsx = stx+10, rsy = 50, rsw = 128, rsh = 20; //��Ϊrsw
    LCD_Draw_Rect(rsx - 1, rsy - 1, rsw + 1, rsh + 2, rectclv);
    LCD_Draw_Rect(rsx - 1, rsy - 3 - rsh, rsw + 1, rsh + 2, rectclv);
    for (int i = 0 ,imax = CVMin(256,rsw); i < rsw; i++)
    {
			  int curgrayv = i* 256 / imax;
        //�Ҷȱ����
        rectclv.R = rectclv.G = rectclv.B = curgrayv;// 0 -- 255
        LCD_Point_Color_Set(rectclv);
        LCD_Draw_Line(rsx + i, rsy, rsx + i, rsy + rsh);
        //����ͼ�����
        LCD_Point_Color_Set(mycstomTable[curgrayv]);
        LCD_Draw_Line(rsx + i, rsy - 2 - rsh, rsx + i, rsy - 2);
    }

    /*--------------------------�Զ�����ǿ--------------------------------------------------*/

    //����ͨ����Ⱦ��
    CVPIX_RENDER myRender = YMCV_Creat_PixRender_Creat(CV_Channel_CustomTransform, PIXChannelNum1);//�Զ�����Ⱦ��
    CVPIX_RENDER myRender2 = YMCV_Creat_PixRender_Creat(CV_Channel_SingleToThree, PIXChannelNum3);//��ͨ��ת��ͨ��

		myFun_k = 1.0f; //��ʼ��k����
    for (int i = 0; i < 10; i++)
    {
        //ʹ���Լ��ĺ�����ʼ����Ⱦͨ��
        YMCV_SingleChannel_CustomMapTo(myRender, myFunction);
        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_ChannelRenderTo(myImage1, myImage2, myRender);
        //��ʾͼƬ1
        LCD_show_image(stx + 0, myImage1->height, myImage2);
			  //��ʾͨ��ת��ͼ
        LCDChannelTransformShow(myRender, 170, 220, 40);
        //��Ϣ��ʾ
        sprintf(myInform, "cstom liner: k =%03.2f", myFun_k);
        LCD_ShowString(stx + 0, myImage1->height, myInform);
        //��ͣ300ms
        Delay(300);
			
        //תΪ����ͼ
        //ʹ������ͼ ��ɫ�������Ⱦ��ͨ�����á���ʼ��
        YMCV_ThreeChannel_PaletteMapTo(myRender2, mycstomTable);
        //ʹ��ͨ����Ⱦ��������ȾͼƬ
        YMCV_Image_SingleToThree_ChannelRenderTo(myImage2, myImage3, myRender2);
        
        //��ʾͼƬ3
        LCD_show_image(stx + 0, myImage1->height, myImage3);

        //��ͣ300ms
        Delay(300);

        //��������
        myFun_k += 1.0f;
    }
    //�ͷ���Ⱦ��1
    YMCV_Free_PIX_RenderFree(myRender);


    /*--------------------------�Զ����ɫ��Ⱦɫ--------------------------------------------------*/

    //������ɫ��
    color24_t myPalette[256] ={0};

    //ʹ��RGB��220��150��100���ԻҶ�ͼ����Ⱦɫ
    color24_t myColor = { .R = 250,.G = 100,.B = 50 };
    for (int i = 0; i < 256; i++)
    {
        myPalette[i].R = myColor.R * i / 255;
        myPalette[i].G = myColor.G * i / 255;
        myPalette[i].B = myColor.B * i / 255;
    }

    //ʹ�õ�ɫ�������Ⱦ��ͨ�����á���ʼ��
    YMCV_ThreeChannel_PaletteMapTo(myRender2, myPalette);
    //ʹ��ͨ����Ⱦ��������ȾͼƬ
    YMCV_Image_SingleToThree_ChannelRenderTo(myImage1, myImage3, myRender2);
    //��ʾͼƬ1��3
    LCD_show_image(stx + 0, 0, myImage1);
    LCD_show_image(stx + 0, myImage1->height, myImage3);
    sprintf(myInform, "cstom:RGB=(%03d,%03d,%03d)  ", myColor.R, myColor.G, myColor.B);
    LCD_ShowString(0, myImage1->height, myInform);
    //��ͣ1000ms
    Delay(1000);

    //�ͷ���Ⱦ��2
    YMCV_Free_PIX_RenderFree(myRender2);


    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage1);
    YMCV_Free_ImgFree(myImage2);
    YMCV_Free_ImgFree(myImage3);
}

//�Զ������Ժ���
int32 myFunction(int32 color)
{
    return myFun_k * color;
}



