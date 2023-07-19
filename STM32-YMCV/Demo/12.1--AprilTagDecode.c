#include  "../ImageSource/imageSource.h"

/*******************************************************************************************
    *
    * ˵��  ������չʾ�� apriltag��ǩ����Ͷ�λ
    *
    ***************************************************************************************/
void runYMCV_Demo(CVIMAGE myImage0)
{
    int stx = 40;
    char myInform[100];
	
	  LCD_Clear(GRAY);
    //��Ϣ��ʾ
    sprintf(myInform, "AprilTag Decode  ");
    LCD_ShowString(0, 0, myInform);
    /*----------------------------------- ����apriltag ------------------------------------------*/
    //Ϊ�˲��ƻ�ԭʼ���ݣ��������������ڴ�,�������ݷ�װΪͼƬ��ʽ 
    uint8* myData = CV_malloc1(sizeof(aprilTag2_gray));//�������CV_malloc1ΪͼƬ���������ڴ�
    CV_memcpy(myData, (void*)aprilTag2_gray, sizeof(aprilTag2_gray));
    CVIMAGE myImage0_gray = YMCV_Creat_DataToImg((uint8*)myData, 160, 120, CVGrayType);//�Ҷ�ͼ0

    YMCV_PADDING_VAL = 200;//�հ������ֵ
    //����ͼƬ0����Ⱦ����
    CV_geom_param myParam = { .perspect.Imgin_w = myImage0_gray->width,
                              .perspect.Imgin_h = myImage0_gray->height,
                              .perspect.wsize_type = CV_WinSize_Const,//�̶�����
                              .perspect.scale = 1.0f };
    //�޸���Ⱦ����
    myParam.perspect.x_angle = -10;
    myParam.perspect.y_angle = 30;
    myParam.perspect.z_angle = 20;
    myParam.perspect.x_move = -25;
    myParam.perspect.y_move = 10;
    myParam.perspect.scale = 0.5f;
    
    CVGEOM_RENDER myRender1 = YMCV_Creat_GeomRender_Creat(CV_Perspective);//�������α任��Ⱦ��
    YMCV_GeomKernel_Load(myRender1, &myParam);//ʹ�ò������м�����Ⱦ���á���ʼ��
    CVIMAGE myImage1 = YMCV_Image_Geom_RenderTo(myImage0_gray, myRender1);//��Ŀ��ͼ��Ľ�����Ⱦ

    //��ʾͼƬ
    LCD_show_image(0, 0, myImage1);

    //input camera intrinsic matrix
    CV_apriltag_detect_info apriltag_info;
    apriltag_info.types = CV_AprilTag16H5 + CV_AprilTag36H11;//�ɼ��ض��ּ���������

    //info.tagsize = 0.146 - 0.012 * 4;   //The size of Tag ��λ��m
    apriltag_info.tagsize = myParam.perspect.scale * myImage0->width / tanf(30 * YMCV_Deg2Rad);//����������͸������Ĳ���
    //
    //        /|
    //       / |Hr
    //   Lv /  |
    //  ��������
    //Hv| /  Lr
    //  |/
    //����Lv �� ���Lr ���� CCDоƬ�ߴ��ͼƬ�ߴ�(H or V) / ����ߴ�FOV(H or V)��
    // 2.8mm Focal Length w/ OV7725 sensor for reference.
    //apriltag_info.fx = (2.8 / 3.984) * myImage1->width;//focal length of x
    //apriltag_info.fy = (2.8 / 2.952) * myImage1->height;//focal length of y
    apriltag_info.fx = -myImage1->width;
    apriltag_info.fy = -myImage1->height;
    // Use the image versus the roi here since the image should be projected from the camera center.
    apriltag_info.cx = myImage1->width * 0.5;//cap.get(CV_CAP_PROP_FRAME_WIDTH) / 2;
    apriltag_info.cy = myImage1->height * 0.5;//cap.get(CV_CAP_PROP_FRAME_HEIGHT) / 2;

    // apriltag��ǩʶ��
    CVVECTORS_U16 result = YMCV_Gray_Find_AprilTags(myImage1, apriltag_info);

    //��ʾ���
    for (int i = 0; (result != NULL) && i < result->dataNum; i++)
    {
        CV_apriltags_data* tda = (CV_apriltags_data*)&result->datap[i * result->dim];

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
        LCD_Fill_ConstRgbRect(tda->centroid.x - 2, tda->centroid.y - 2, 5, 5, rgbcv);

        //��ǩ����
        if (tda->family & CV_AprilTag16H5)
        {
            printf("16h5 - ");
        }
        if (tda->family & CV_AprilTag25H7)
        {
            printf("25h7 - ");
        }
        if (tda->family & CV_AprilTag25H9)
        {
            printf("25h9 - ");
        }
        if (tda->family & CV_AprilTag36H10)
        {
            printf("36h10 - ");
        }
        if (tda->family & CV_AprilTag36H11)
        {
            printf("36h11 - ");
        }
        if (tda->family & CV_ArToolKit)
        {
            printf("artoolkit - ");
        }

        // ��ǩID
        printf(" ID = %d ", tda->id);

        //λ�ò���
        printf(" position [x,y,z]= %f %f %f", tda->Txyz[0], tda->Txyz[1], tda->Txyz[2]);
        //��ת�������
        //��������ϵ
        int len = 20;
        rgbcv = (color24_t){ .R = 255 }; LCD_Point_Color_Set(rgbcv);
        LCD_Draw_Line(tda->centroid.x, tda->centroid.y, tda->centroid.x + len * tda->Rxyz[0][0], tda->centroid.y + len * tda->Rxyz[1][0]);// x��0 -> 1
        rgbcv = (color24_t){ .G = 255 }; LCD_Point_Color_Set(rgbcv);
        LCD_Draw_Line(tda->centroid.x, tda->centroid.y, tda->centroid.x + len * tda->Rxyz[0][1], tda->centroid.y + len * tda->Rxyz[1][1]);// y��0 -> 2
        rgbcv = (color24_t){ .B = 255 }; LCD_Point_Color_Set(rgbcv);
        LCD_Draw_Line(tda->centroid.x, tda->centroid.y, tda->centroid.x + len * tda->Rxyz[0][2], tda->centroid.y + len * tda->Rxyz[1][2]);// z��0 -> 3


        //�ǶȲ���
        printf(" Rotate [x,y,z]= %f %f %f", tda->x_rotation, tda->y_rotation, tda->z_rotation);
    }

    
		//�ͷ��ڴ�
		YMCV_Free_Vector_U16_Free(result);
    YMCV_Free_GEOM_RenderFree(myRender1);
    //ʹ����ͼƬ��Ҫ�ͷ�ͼƬ�ڴ�
    YMCV_Free_ImgFree(myImage0_gray);
    YMCV_Free_ImgFree(myImage1);

    Delay(800);
}


