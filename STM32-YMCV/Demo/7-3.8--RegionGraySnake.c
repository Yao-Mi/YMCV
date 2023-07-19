
/*******************************************************************************************
    *
    * ˵��  �������� Snake����ģ��Ϊ����չʾ����������ȡЧ��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{   
    int stx = 40;
    char myInform[100];
	
    //��ͼƬ����������Ϊ��(Ĭ�ϲ����г�ʼ��������Ϊ���ֵ)
    CVIMAGE myImage0_gray = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVGrayType);
		YMCV_Image_Pix_Change_Form(myImage0, myImage0_gray, CV_Rgb16ToGray);//rgbת�Ҷ�ͼ
	
	  for (int i = 0,imax = myImage0_gray->width*myImage0_gray->height; i < imax; i++)
    {
        myImage0_gray->data[i] = (myImage0_gray->data[i] >> 7) << 7;
    }
	  
		//��ʾͼƬ0
		LCD_show_image(stx + 0, 0, myImage0_gray);
	  
    color24_t thiscolor = { .R = 255,.G = 0 };
    /*-------------------------- �ֶ���������--------------------------------------------------*/
    
		//���ڵ�Ƭ��δ�Ӽ��̺���꣬�����ⲿ�ֱ�ɾ���ˣ�ֱ�Ӳ��ù̶��㣬����ϸ��ʹ�òο�PC�ϵ�Demo
		
    uint16 xpoint[] = { 143, 143, 142, 140, 135, 131, 128, 123, 120, 112, 105, 97, 93, 91, 91, 89, 87, 87, 88, 90, 91, 93, 95, 95, 97, 98, 101, 105, 109, 114, 119, 123, 128, 135, 139, 139, 143, 143, };
    uint16 ypoint[] = { 55, 49, 44, 32, 24, 18, 16, 11, 5, 2, 2, 3, 6, 10, 15, 21, 29, 35, 38, 42, 48, 51, 54, 58, 59, 63, 68, 70, 74, 74, 78, 80, 80, 76, 70, 68, 65, 59, }; 
    uint16 num = sizeof(xpoint)/2;

		int i;
		//�㼯̫�����ˣ������Ƶ��м�����
		for (i = 0; i < num; i++)
		{
			xpoint[i] -= 20;
			ypoint[i] += 20;
		}
		
    LCD_Point_Color_Set(thiscolor);;
    int spy = 0;
    for (i = 0; i < num - 1; i++)
    {
        LCD_Draw_Line(stx + xpoint[i], ypoint[i]+spy, stx + xpoint[i + 1], ypoint[i + 1]+spy);
    }
    LCD_Draw_Line(stx + xpoint[i], ypoint[i]+spy, stx + xpoint[0], ypoint[0]+spy);
		
    //��Ϣ��ʾ
    sprintf(myInform, "contour  init ");
    LCD_ShowString(0, 0, myInform);

    /*-------------------------- snake�����ָ� --------------------------------------------------*/
    int search_r = 3;
    //�����£���ȡֵ��Χ[-1,1]
    float alpha = 0.3;//�����仯�ʣ��仯�������ն�
    float beta = 0.35;//������������������ֵ����Բ���̶�
    float gamma = 1.0;//ͼ������

    //kapaȡֵ[-1,1]
    float imageKpa = 0.1;//�����������ɫ��£���������ɫ��£
    float gradientKpa = 1.0;//���ݶȸߵķ���£
    YMCV_Gray_Snake(myImage0_gray, xpoint, ypoint,num, search_r,500,alpha,beta,gamma, imageKpa, gradientKpa);//��Trimapͼ��ָ�����
     //��ʾͼƬ
    LCD_show_image(stx + 0, myImage0_gray->height, myImage0_gray);
    //��Ϣ��ʾ
    sprintf(myInform, "snake  contour");
    LCD_ShowString(0, myImage0_gray->height, myInform);
		
    spy = myImage0_gray->height;
    for (i = 0; i < num - 1; i++)
    {
        //printf("�㣺%d , %d\n", xpoint[i], ypoint[i]);
        LCD_Draw_Line(stx + xpoint[i], ypoint[i]+spy, stx + xpoint[i + 1], ypoint[i + 1]+spy);
    }
    LCD_Draw_Line(stx + xpoint[i], ypoint[i]+spy, stx + xpoint[0], ypoint[0]+spy);
    //��ͣ500ms
		Delay(500);
		//�ͷ��ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
}


