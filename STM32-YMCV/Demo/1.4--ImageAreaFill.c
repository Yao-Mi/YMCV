/*******************************************************************************************
    *
    * ˵��  �������� ����ɨ�������Ϊ����չʾ�˻�����������Ч��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40,i;
    char myInform[100];
	
    /*-------------------------- �ֶ�����TriMap--------------------------------------------------*/
    LCD_Clear(WHITE);
    //��ʾԭͼ
    LCD_show_image(40, myImage0->height, myImage0);

    uint16 xpoint[] = { 121, 141, 159, 151, 131, 88, 66, 65, 67, 84, 105, };
    uint16 ypoint[] = { 97, 86, 45, 22, 13, 18, 22, 38, 59, 78, 91, };
    uint16 num = sizeof(xpoint) / 2;

    //��������
		
    for (i = 0; i < num - 1; i++)
    {
        LCD_Draw_Line(stx + xpoint[i], ypoint[i], stx + xpoint[i + 1], ypoint[i + 1]);
    }
    LCD_Draw_Line(stx + xpoint[i], ypoint[i], stx + xpoint[0], ypoint[0]);

    //����Ϊ�㼯
    CVpoint* myPoint = (CVpoint*)CV_malloc1(num * sizeof(CVpoint));//�㼯
    for (int i = 0; i < num; i++)
    {
        CVpoint* thiscolor = myPoint + i;
        thiscolor->x = xpoint[i];
        thiscolor->y = ypoint[i];
    }

    CVIMAGE TriMapMarkers = YMCV_Creat_Img_Creat(myImage0->width, myImage0->height, CVTrimapType);
    YMCV_TriMap_Init(TriMapMarkers); //trimap��ʼ��
    LCD_show_image(stx, 0, TriMapMarkers);
    //��Ϣ��ʾ
    sprintf(myInform, "old - TriMap");
    LCD_ShowString(stx, 0, myInform);

		Delay(300);
		/*----------------------------�����������----------------------------*/
		
    CVcolor fillval;
    fillval.gray = CVTriMapMaxVl - 10;//����Ϊǰ����ֵ

    //��Trimap�����������
    YMCV_Img_Scanline_AreaFill(TriMapMarkers, myPoint, num, fillval);

    LCD_show_image(stx, 0, TriMapMarkers);
    //��Ϣ��ʾ
    sprintf(myInform, "ring fill  ");
    LCD_ShowString(stx, 0, myInform);
    Delay(300);

		/*----------------------------��ɫͼ���----------------------------*/
		
    fillval.rgb16 = 0xfe00;
    //�Բ�ɫͼ�����������
    YMCV_Img_Scanline_AreaFill(myImage0, myPoint, num, fillval);
    LCD_show_image(stx, 0, myImage0);
    //��Ϣ��ʾ
    sprintf(myInform, "rgb fill  ");
    LCD_ShowString(stx, 0, myInform);

    //�ͷ��ڴ�
    YMCV_Free_ImgFree(TriMapMarkers);

    Delay(300);
}

