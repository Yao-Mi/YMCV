


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
    * ˵��  �������� �궨����Ϊ����չʾ���������������ݱ궨����ȡ
    *
    ***************************************************************************************/

int main()
{
    int i;
    char myInform[100];
    LCD_Init(1000, 600);

    CVIMAGE calibrat = YMCV_Image_Read("D:/Documents/1012_YMCV/ͼƬ�ز�/GAN-Face/template.jpg", CVRgb16Type);
    //��ʾԭͼ
    LCD_show_image(400, 0, calibrat);

    sprintf(myInform, "�ֶ��궨������ ");
    LCD_ShowString(0, 310, myInform);
    /*-------------------------- �ֶ��궨 --------------------------------------------------*/

    color24_t thiscolor = { .R = 255,.G = 0 };
    LCD_Point_Color_Set(thiscolor);
    char pflag = 1;
    printf("׼�����ƣ�\n'c':ѡ��\n'z'����\n'n'��һ��\n�ո񣺽�������\n");
    int px = 0, py = 0, w, h;

    uint16 xpoint[100], ypoint[100];
    int8 num = -1,readimgi = 0;
    //��ȡһ��ͼƬ
    CVIMAGE PrImg = YMCV_Image_Read("D:/Documents/1012_YMCV/ͼƬ�ز�/GAN-Face/0.jpg", CVRgb16Type);

    while (1)
    {
        char mchr;
       
        //��ȡ����ֵ
        if (LCD_GetChar(&mchr))
        {
            
            if (mchr == 'c')
            {
                num++;
                if (LCD_GetXY(0, 0, PrImg->width, PrImg->height, &px, &py))
                {
                    xpoint[num] = px;
                    ypoint[num] = py;
                    printf("%02dcѡ��㣺%d , %d\n",num + 1, px, py);
                }
                pflag = 1;
            }
            else if (mchr == 'z')
            {
                //����һ��
                num--;
                if (num < 0)num = -1;
                pflag = 1;
            }
            else if ((mchr == 'n') && ((num + 1) == 40))//��¼��40��������
            {
                FILE* recordfile = fopen("D:/Documents/1012_YMCV/ͼƬ�ز�/GAN-Face/CalibratingData.txt", "a");//��׷��д�뷽ʽ��
                uint8 databuff[20];
                //�����㱣��
                for (int i = 0; i <= num; i++)
                {
                    sprintf(databuff, " %3d %3d", xpoint[i], ypoint[i]);//ת�ı�
                    fwrite(databuff, strlen(databuff), 1, recordfile);//д��
                }
                fwrite("\n", 1, 1, recordfile);//д�뻻��
                fclose(recordfile);//�ر��ļ�

                //��һ��ͼƬ
                readimgi++;
                if (readimgi >= 30)//30������ȫ����ע����
                {
                    break;
                }
                sprintf(myInform, "D:/Documents/1012_YMCV/ͼƬ�ز�/GAN-Face/%d.jpg", readimgi);
                LCD_Fill_ConstRgbRect(0, 0, PrImg->width, PrImg->height, (color24_t){ 0 });//�������
                //�ͷ�֮ǰ�ڴ�
                YMCV_Free_ImgFree(PrImg);
                //��ȡһ��ͼƬ
                PrImg = YMCV_Image_Read(myInform, CVRgb16Type);
                num = -1;
                pflag = 1;
                system("cls");
                printf("׼�����ƣ�\n'c':ѡ��\n'z'����\n'n'��һ��\n�ո񣺽�������\n");
            }
        }
        if (num >= 100) break;

        //�ػ�ͼ��
        if (pflag)
        {
            LCD_show_image(0, 0, PrImg);
            //��
            for (int i = 0; i <= num; i++)
            {
                //��ԭͼ�ϻ���
                LCD_Fill_ConstRgbRect(xpoint[i] - 1, ypoint[i] - 1, 3, 3, thiscolor);
            }
            pflag = 0;
        }
    }

    //�ͷ��ڴ�
    YMCV_Free_ImgFree(PrImg);
    YMCV_Free_ImgFree(calibrat);

    PAUSE();
    LCD_Destory();
    return 0;
}

