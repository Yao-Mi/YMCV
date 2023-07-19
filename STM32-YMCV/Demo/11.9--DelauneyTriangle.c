
/*******************************************************************************************
    *
    * ˵��  �������� Delauney��������Ϊ����չʾ���ڶ��㼯�д�����������
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 20,sty = 40,i;

    char myInform[100];
    /*-------------------------- ����Ч�� --------------------------------------------------*/
    LCD_Clear(GRAY);
    //��Ϣ��ʾ
    sprintf(myInform, "Mesh Point  ");
    LCD_ShowString(0, 0, myInform);

    // ȷ�������
    color24_t thiscolor = { .R = 255,.G = 0 };

    uint16 xpoint[] = { 89, 100, 110, 127, 140, 126, 103, 99, 121, 112, 117 };
    uint16 ypoint[] = { 34, 31, 28, 20, 51, 74, 65, 41, 32, 53, 62 };
    uint16 num = sizeof(xpoint) / 2;
    for (i = 0; i < num; i++)
    {
        LCD_Fill_ConstRgbRect(xpoint[i] -1, ypoint[i] - 1, 2, 2, (color24_t){.G=255});
    }
		Delay(300);
    //�����Ƶ���ο�PC�汾��Demo

    CVVECTORS_F32 fpointList = YMCV_Creat_Vector_F32(sizeof(CVfpoint) / sizeof(float32), num);
    for (i = 0; i < num; i++)
    {
        CVfpoint* fpthis = (CVfpoint*)(fpointList->datap + i * fpointList->dim);
        fpthis->x = xpoint[i];
        fpthis->y = ypoint[i];
    }

    //Delauney����������
    CVVECTORS_F32 myTriangles = YMCV_DelauneyTriangle_Construct(fpointList);

    LCD_Point_Color_Set(thiscolor);
    for (i = 0; (myTriangles != NULL) && (i < myTriangles->dataNum); i++)
    {
        CVftriangle* fpthis = (CVftriangle*)(myTriangles->datap + i * myTriangles->dim);

        LCD_Draw_Line(fpthis->A.x, fpthis->A.y, fpthis->B.x, fpthis->B.y);//AB
        LCD_Draw_Line(fpthis->B.x, fpthis->B.y, fpthis->C.x, fpthis->C.y);//BC
        LCD_Draw_Line(fpthis->C.x, fpthis->C.y, fpthis->A.x, fpthis->A.y);//CA
    }
    //��Ϣ��ʾ
    sprintf(myInform, "Mesh  Result");
    LCD_ShowString(0, 0, myInform);
    Delay(500);
		
		
    YMCV_Free_Vector_F32_Free(fpointList);
    YMCV_Free_Vector_F32_Free(myTriangles);
}