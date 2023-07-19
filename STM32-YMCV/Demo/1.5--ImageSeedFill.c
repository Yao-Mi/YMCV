/*******************************************************************************************
    *
    * ˵��  �������� ɨ�������ӵ����Ϊ����չʾ����������Ч��
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];

    /*-------------------------- ���ӵ����--------------------------------------------------*/
    LCD_Clear(WHITE);
    LCD_show_image(stx, 0, myImage0);
    
    CVcolor fillval;
    fillval.rgb16 = 0xfe00;//���ֵ
    CVpoint seed = { .x = 60,.y = 45 };//���ӵ�����
    int8 floaterror = 50;//���ӵ㸽��ֵ���������Ϊ��50������䵱ǰ����ɫֵ��50����ͨ����
		
    color24_t thiscolor = { .R = 255 };
    LCD_Fill_ConstRgbRect(stx + seed.x - 1, seed.y - 1, 3, 3, thiscolor);//�������ӵ�
		
		
    //�Բ�ɫͼ�����������
    YMCV_Image_ScanLine_SeedFill(myImage0, seed, floaterror, fillval);

    LCD_show_image(stx, myImage0->height, myImage0);
    //��Ϣ��ʾ
    sprintf(myInform, "seed fill rgb ");
    LCD_ShowString(stx, myImage0->height, myInform);
}

