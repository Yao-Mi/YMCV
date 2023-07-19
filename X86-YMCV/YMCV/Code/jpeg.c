//#include "../OPOBJ/YMCV_Creat.h"
//#include "../OPOBJ/YMCV_Free.h"
//#include "../DEBUG/YMCV_Debug.h"
//#include "../CORE/YMCV_PixConv.h"
//#include "../CORE/YMCV_Basic.h"
///**
//  ***************************************************************************************************************************
//  *	@FileName:    jpeg.c
//  *	@Author:      yaomimaoren
//  *	@Date:        2022-3-2
//  *	@Description: jpeg����ͽ���
//  *	@Version:     1.0
//  *
//  ***************************************************************************************************************************
//  *
//  * ��ע��Ϣ��
//  * 1.֧�ָ�ʽ��
//  *   jpeg����
//  *
//  * �������ⱸע��
//  *
//  ***************************************************************************************************************************
//  * <author> <time> <version > <desc>
//  * yaomi 22/03/02 1.0 build this moudle
//  *
//  *   __  __ ___    ____   __  ___ ____     ______ ______ ______ __  __
//  *   \ \/ //   |  / __ \ /  |/  //  _/    /_  __// ____// ____// / / /
//  *    \  // /| | / / / // /|_/ / / /       / /  / __/  / /    / /_/ /
//  *    / // ___ |/ /_/ // /  / /_/ /       / /  / /___ / /___ / __  /
//  *   /_//_/  |_|\____//_/  /_//___/      /_/  /_____/ \____//_/ /_/
//  *
//  * Copyright (C), 2021-2031, YAOMI Tech. Co., Ltd.
//  ***************************************************************************************************************************/
//
////MCU����С���뵥Ԫ��
//#define JPEG_MCU_SIZE 4
//
//int block[JPEG_MCU_SIZE][JPEG_MCU_SIZE], tblock[JPEG_MCU_SIZE][JPEG_MCU_SIZE];
////�㷨ʵ����http://www.javashuo.com/article/p-rtqnooot-kc.html
////ԭ��https://blog.csdn.net/newchenxf/article/details/51719597
////��Ƶ���룺https://blog.csdn.net/bigdream123/category_9433245.html
////�����㷨
//void forward4x4(int pos_y, int pos_x)
//{
//    int i, ii;
//    int tmp[16] = { 0 };
//    int* pTmp = tmp, * pblock;
//    int p0, p1, p2, p3;
//    int t0, t1, t2, t3;
//
//    // Horizontal
//    for (i = pos_y; i < pos_y + JPEG_MCU_SIZE; i++)
//    {
//        pblock = &block[i][pos_x];
//        p0 = *(pblock++);
//        p1 = *(pblock++);
//        p2 = *(pblock++);
//        p3 = *(pblock);
//
//        t0 = p0 + p3;
//        t1 = p1 + p2;
//        t2 = p1 - p2;
//        t3 = p0 - p3;
//
//        *(pTmp++) = t0 + t1;
//        *(pTmp++) = (t3 << 1) + t2;
//        *(pTmp++) = t0 - t1;
//        *(pTmp++) = t3 - (t2 << 1);
//    }
//
//    // Vertical 
//    for (i = 0; i < JPEG_MCU_SIZE; i++)
//    {
//        pTmp = tmp + i;
//        p0 = *pTmp;
//        p1 = *(pTmp += JPEG_MCU_SIZE);
//        p2 = *(pTmp += JPEG_MCU_SIZE);
//        p3 = *(pTmp += JPEG_MCU_SIZE);
//
//        t0 = p0 + p3;
//        t1 = p1 + p2;
//        t2 = p1 - p2;
//        t3 = p0 - p3;
//
//        ii = pos_x + i;
//        tblock[pos_y][ii] = t0 + t1;
//        tblock[pos_y + 1][ii] = t2 + (t3 << 1);
//        tblock[pos_y + 2][ii] = t0 - t1;
//        tblock[pos_y + 3][ii] = t3 - (t2 << 1);
//    }
//}
//
//static void jpeg_write_bytes(CV_queue* jpeg_buf, const uint8* data, int size)
//{
//    for (int i = 0; i < size; i++)
//    {
//        YMCV_Queue_PushU8(data[i], jpeg_buf);//д��buff������
//    }
//}
////jpeg�ļ���ʽ�ο���https://www.cnblogs.com/leaven/archive/2010/04/06/1705846.html
////JFIF��ʽ��JPEG�ļ�(*.jpg)��һ��˳��Ϊ��
////SOI(0xFFD8)
////APP0(0xFFE0)
////[APPn(0xFFEn)]��ѡ
////DQT(0xFFDB)
////SOF0(0xFFC0)
////DHT(0xFFC4)
////SOS(0xFFDA)
////
////ѹ������
////
////EOI(0xFFD9)
//void jpeg_write_head()
//{
//    CV_queue* jpeg_buf=NULL;
//    //�ֵĸߵ�λ����
//   //JPEG�ļ���ʽ�У�һ���֣�16λ���Ĵ洢ʹ�õ��� Motorola ��ʽ, ������ Intel ��ʽ��
//   //Ҳ����˵, һ���ֵĸ��ֽڣ���8λ������������ǰ��, ���ֽڣ���8λ�����������ĺ��棬��ƽʱϰ�ߵ�Intel��ʽ��һ����
//
//   // Number of components (1 or 3)
//    uint8_t nr_comp = (bpp == 1) ? 1 : 3;
//
//    // JPEG headers
//    uint8_t jpeg_soi[] = {
//        0xFF, 0xD8          // SOI�̶�ֵ0xFFD8
//    };
//    jpeg_write_bytes(jpeg_buf, jpeg_soi, sizeof(jpeg_soi));
//
//
//    //����Ƕο��԰���ͼ���һ��΢���汾����Ϊ24λ��RGB���ء�
//    //���û��΢��ͼ�������������������������ͼˮƽ����ֱ������Ŀ��ֵ��Ϊ0��
//    uint8_t jpeg_app0[] = {
//        0xFF, 0xE0,         // APP0�̶�ֵ0xFFE0
//        //����9�������ֶΣ�
//        0x00, 0x10, //9���ֶε��ܳ�
//        'J',  'F',  'I',  'F', 0x00,//�ַ�����JFIF0��
//        0x01,0x01,//�汾��һ����0x0101����ʾJFIF�İ汾��1.1
//        0x00,//X��Y���ܶȵ�λ  0���޵�λ��1������/Ӣ�磻2������/����
//        0x00, 0x01, //X���������ܶ�   2�ֽ� 
//        0x00, 0x01, //Y���������ܶ�   2�ֽ� 
//        0x00,//����ͼˮƽ������Ŀ 1�ֽ�
//        0x00 //����ͼ��ֱ������Ŀ  1�ֽ�
//        //����ͼRGBλͼ  ���ȿ�����3�ı���  ����ͼRGBλͼ����:
//        //��rgb[3*n] 
//    };
//    jpeg_write_bytes(jpeg_buf, jpeg_app0, sizeof(jpeg_app0));
//
//
//    //Ӧ�ó��������n������n=1��15(��ѡ)
//    uint8_t jpeg_appn[] =
//    {
//        0xFF, 0xF1,   //2�ֽ�     �̶�ֵ0xFFE1~0xFFF
//        //���������ֶ�
//        0x00, 0x0B,        //���ݳ���   2�ֽ�  2���ֶε��ܳ���
//        'Y',  'M',  'C',  'V', '-',  '2',  '.',  '0', 0x00, //��ϸ��Ϣ  ���ݳ��� - 2�ֽ�   ���ݲ���
//    };
//    jpeg_write_bytes(jpeg_buf, jpeg_appn, sizeof(jpeg_appn));
//
//
//    //����������
//    //�����У�����������ظ����֣���ʾ��������������ֻ�ܳ���4�Ρ�
//    uint8_t jpeg_dqt[] = {
//        0xFF, 0xDB,  // DQT  2�ֽ�  �̶�ֵ0xFFDB
//        //����9���ֶ�
//        0x00,0x00,// �ֶγ���  2�ֽ�
//        //�����������ٶ���:
//        //�پ��ȼ�������ID   1�ֽ�  
//        //��4λ�����ȣ�ֻ��������ѡֵ:0��8λ��1��16λ
//        //��4λ��������ID��ȡֵ��ΧΪ0��3
//        //�ڱ���(64��(���� + 1))�ֽ�
//        //����8λ���ȵ������� ������Ϊ64����0 + 1�� = 64�ֽ�
//    };
//    jpeg_dqt[2] = (bpp * 65 + 2) >> 8;// Header length MSB
//    jpeg_dqt[3] = (bpp * 65 + 2) & 0xFF;// Header length LSB
//
//    jpeg_write_bytes(jpeg_buf, jpeg_dqt, sizeof(jpeg_dqt));
//
//
//    //֡ͼ��ʼ
//    //�����У���ɫ������ϢӦ���ظ����֣��ж��ٸ���ɫ����(����3)���ͳ��ֶ��ٴΣ�3�Σ�
//    uint8_t jpeg_sof0[] = {
//        0xFF, 0xC0,         // SOF0 2�ֽ� �̶�ֵ0xFFC0
//        //����6���ֶ�
//        0x00,0x00,// �ֶγ���  2�ֽ�
//        0x00,//����  1�ֽ�  ÿ������������λ��
//        0x00,0x00,//ͼ��߶�  2�ֽ�  ͼ��߶ȣ���λ�����أ��������֧�� DNL �ͱ��� >0
//        0x00,0x00,//ͼ����  2�ֽ�  ͼ���ȣ���λ�����أ��������֧�� DNL �ͱ��� >0
//        0x03,//��ɫ������  1�ֽ�  ֻ��3����ֵ��ѡ 1���Ҷ�ͼ��3��YCrCb��YIQ��4��CMYK ����JFIF��ʹ��YCrCb����������ɫ��������Ϊ3
//        //��ɫ������Ϣ  ��ɫ��������3�ֽڣ�ͨ��Ϊ9�ֽڣ���
//        //�� ��ɫ����ID   1�ֽ�
//        //�� ˮƽ/��ֱ��������  1�ֽ�   ��4λ��ˮƽ�������� ��4λ����ֱ�������� ����������ĳ���ϰ������ߵ�ת�ˣ�
//        //�� ������ 1�ֽ�   ��ǰ����ʹ�õ��������ID
//    };
//    jpeg_sof0[2] = (nr_comp * 3 + 8) >> 8;   // Header length MSB
//    jpeg_sof0[3] = (nr_comp * 3 + 8) & 0xFF; // Header length LSB
//    jpeg_sof0[4] = 0x08; // Bits per sample
//    jpeg_sof0[5] = h >> 8; // Height
//    jpeg_sof0[6] = h & 0xFF;
//    jpeg_sof0[7] = w >> 8; // Width
//    jpeg_sof0[8] = w & 0xFF;
//    jpeg_sof0[9] = nr_comp;// Number of components
//
//    jpeg_write_bytes(jpeg_buf, jpeg_sof0, sizeof(jpeg_sof0));
//
//
//    //�����������
//    //�����У�������������ظ����֣�һ��4�Σ���Ҳ����ֻ����1�Ρ�
//    //���磬Adobe Photoshop ���ɵ�JPEGͼƬ�ļ���ֻ��1��DHT��ǶΣ���߰�����4����������
//    //��Macromedia Fireworks���ɵ�JPEGͼƬ�ļ�����4��DHT��ǶΣ�ÿ��DHT��Ƕ�ֻ��һ����������
//    uint8_t jpeg_dht[] = {
//        0xFF, 0xC4,         // DHT  2�ֽ� �̶�ֵ0xFFC4
//        //����2�������ֶΣ�
//        0x00, 0x00,//�ֶε��ܳ��� 2�ֽ�
//        //�������� ���ݳ��� - 2�ֽ�:
//        //�ٱ�ID�ͱ�����   1�ֽ�   ��4λ�����ͣ�ֻ������ֵ��ѡ 0��DCֱ����1��AC����
//        //��4λ����������ID�� ע�⣬DC���AC��ֿ�����
//        //�ڲ�ͬλ������������    16�ֽ�
//        //�۱������� 16����ͬλ������������֮�ͣ��ֽڣ�
//    };
//    jpeg_dht[2] = (bpp * 208 + 2) >> 8;     // Header length MSB
//    jpeg_dht[3] = (bpp * 208 + 2) & 0xFF;   // Header length LSB
//
//    jpeg_write_bytes(jpeg_buf, jpeg_dht, sizeof(jpeg_dht));
//
//
//
//    //�����ֱ����ۼƸ�λ�ļ��
//    //���û�б���ǶΣ�����ֵΪ0ʱ���ͱ�ʾ�������ؿ�ʼ����ͱ��RST
//    uint8_t jpeg_dri[] = {
//        0xFF, 0xDD,         // DRI 2�ֽ� �̶�ֵ0xFFDD
//        //����2�������ֶΣ�
//        0x00, 0x04,//�ֶε��ܳ���  2�ֽ�  �̶�ֵ0x0004
//        //��MCU��ĵ�Ԫ�е����¿�ʼ���
//        0x00, 0x00,//2�ֽ� ����ֵΪn�����ʾÿn��MCU�����һ��RSTn��ǡ�
//        //�磺��һ�������RST0���ڶ����� RST1�ȣ�RST7���ٴ�RST0�ظ���
//    };
//
//    jpeg_write_bytes(jpeg_buf, jpeg_dri, sizeof(jpeg_dri));
//
//    //ɨ�迪ʼ
//    //�����У���ɫ������ϢӦ���ظ����֣��ж��ٸ���ɫ��������3�������ͳ��ֶ��ٴΣ�һ��Ϊ3�Σ���
//    //���ν����󣬽����ž���������ͼ����Ϣ�ˡ�ͼ����Ϣֱ������һ����Ǵ�����Զ�������һ�������EOI��Ǳ�ʾ����
//    uint8_t jpeg_sos[] = {
//        0xFF, 0xDA, // SOS 2�ֽ�  �̶�ֵ0xFFDA
//        //����2�������ֶΣ�
//        0x00, 0x00,//�ֶε��ܳ���  2�ֽ�
//        0x03, //��ɫ������ 1�ֽ� Ӧ�ú�SOF�е��ֶ΢ݵ�ֵ��ͬ������ 1���Ҷ�ͼ�ǣ�3�� YCrCb��YIQ��4��CMYK�� ��JFIF��ʹ��YCrCb����������ɫ��������Ϊ3
//        //��ɫ������Ϣ��
//        //����ɫ����ID   1�ֽ� 
//        //��ֱ��/����ϵ����� 1�ֽ�     ��4λ��ֱ������ʹ�õĹ����������, ��4λ����������ʹ�õĹ����������
//        //ѹ��ͼ�����ݣ�
//        //����ѡ��ʼ 1�ֽ� �̶�ֵ0x00
//        //����ѡ����� 1�ֽ� �̶�ֵ0x3F
//        //����ѡ�� 1�ֽ� �ڻ���JPEG����Ϊ00
//    };
//    jpeg_sos[2] = (nr_comp * 2 + 6) >> 8; // Header length MSB
//    jpeg_sos[3] = (nr_comp * 2 + 6) & 0xFF; // Header length LSB
//    jpeg_sos[4] = nr_comp;            // Number of components
//
//    jpeg_write_bytes(jpeg_buf, jpeg_sos, sizeof(jpeg_sos));
//}
//void jpeg_write_data()
//{
//    //���ﲹ��˵��һ�£�������JPEG�ļ���0xFF���б�־�Ե���˼��
//    //������ѹ��������(������ͼ����Ϣ)�г���0xFF������Ҫ���ر������巽����:
//    //������0xFF�����һ��û�������0x00��
//    //���仰˵�������ͼ��������������0xFF��Ӧ�ü��������ŵ��ַ��������:
//    //1��0x00�����ʾ0xFF��ͼ��������ɲ��֣���Ҫ�������룻
//    //2��0xD9������0xFF��ɱ��EOI����ͼ����������ͬʱͼ���ļ�������
//    //3��0xD0~0xD7, �����RSTn��ǣ���Ҫ��������RSTn��ǣ������Ե�ǰ0xFF�ͽ��ӵ�0xDn�����ֽڽ������룬����RST��ǵĹ���������������
//    //3��0xFF������ӵ�ǰ0xFF���Ժ�һ��0xFF�����жϣ�
//    //4��������ֵ������ӵ�ǰ0xFF�����������ӵĴ���ֵ�������롣
//}
//void jped_write_end()
//{
//    //ͼ����� 2�ֽ�
//    uint8_t jpeg_eoi[] = {
//        0xFF, 0xD9,//ROI  2�ֽ�     �̶�ֵ0xFFD9
//    };
//    jpeg_write_bytes(jpeg_buf, jpeg_eoi, sizeof(jpeg_eoi));
//}
//static void jpeg_write_headers(jpeg_buf_t* jpeg_buf, int w, int h, int bpp, jpeg_subsample_t jpeg_subsample)
//{
//
//    // Write SOI marker
//    jpeg_put_bytes(jpeg_buf, m_soi, sizeof(m_soi));
//    // Write APP0 marker
//    jpeg_put_bytes(jpeg_buf, m_app0, sizeof(m_app0));
//
//    // Write DQT marker
//    jpeg_put_bytes(jpeg_buf, m_dqt, sizeof(m_dqt));
//    // Write Y quantization table (index, table)
//    jpeg_put_char(jpeg_buf, 0);
//    jpeg_put_bytes(jpeg_buf, YTable, sizeof(YTable));
//
//    if (bpp > 1) {
//        // Write UV quantization table (index, table)
//        jpeg_put_char(jpeg_buf, 1);
//        jpeg_put_bytes(jpeg_buf, UVTable, sizeof(UVTable));
//    }
//
//    // Write SOF0 marker
//    jpeg_put_bytes(jpeg_buf, m_sof0, sizeof(m_sof0));
//    for (int i = 0; i < nr_comp; i++) {
//        // Component ID, HV sampling, q table idx
//        jpeg_put_bytes(jpeg_buf, (uint8_t[3]) { i + 1, (i == 0 && bpp == 2) ? jpeg_subsample : 0x11, (i > 0) }, 3);
//
//    }
//
//    // Write DHT marker
//    jpeg_put_bytes(jpeg_buf, m_dht, sizeof(m_dht));
//
//    // Write DHT-YDC
//    jpeg_put_char(jpeg_buf, 0x00);
//    jpeg_put_bytes(jpeg_buf, std_dc_luminance_nrcodes + 1, sizeof(std_dc_luminance_nrcodes) - 1);
//    jpeg_put_bytes(jpeg_buf, std_dc_luminance_values, sizeof(std_dc_luminance_values));
//
//    // Write DHT-YAC
//    jpeg_put_char(jpeg_buf, 0x10);
//    jpeg_put_bytes(jpeg_buf, std_ac_luminance_nrcodes + 1, sizeof(std_ac_luminance_nrcodes) - 1);
//    jpeg_put_bytes(jpeg_buf, std_ac_luminance_values, sizeof(std_ac_luminance_values));
//
//    if (bpp > 1) {
//        // Write DHT-UDC
//        jpeg_put_char(jpeg_buf, 0x01);
//        jpeg_put_bytes(jpeg_buf, std_dc_chrominance_nrcodes + 1, sizeof(std_dc_chrominance_nrcodes) - 1);
//        jpeg_put_bytes(jpeg_buf, std_dc_chrominance_values, sizeof(std_dc_chrominance_values));
//
//        // Write DHT-UAC
//        jpeg_put_char(jpeg_buf, 0x11);
//        jpeg_put_bytes(jpeg_buf, std_ac_chrominance_nrcodes + 1, sizeof(std_ac_chrominance_nrcodes) - 1);
//        jpeg_put_bytes(jpeg_buf, std_ac_chrominance_values, sizeof(std_ac_chrominance_values));
//    }
//
//    // Write SOS marker
//    jpeg_put_bytes(jpeg_buf, m_sos, sizeof(m_sos));
//    for (int i = 0; i < nr_comp; i++) {
//        jpeg_put_bytes(jpeg_buf, (uint8_t[2]) { i + 1, (i == 0) ? 0x00 : 0x11 }, 2);
//    }
//
//    // Spectral selection
//    jpeg_put_bytes(jpeg_buf, (uint8_t[3]) { 0x00, 0x3F, 0x0 }, 3);
//}
//
//typedef enum
//{
//    Jpeg_Binary = 0,
//    Jpeg_Gray,
//    Jpeg_Rgb565,
//    Jpeg_Rgb888,
//}JpegPixType;
//static uint8_t* get_mcu()
//{
//    uint8_t* Y0 = mcubuf;
//    uint8_t* CB = mcubuf + 64;
//    uint8_t* CR = mcubuf + 128;
//
//    // Copy 8x8 MCUs
//    switch (jpeg_enc.img_bpp) {
//
//    case 0:
//        break;
//    case 1:
//        for (int y = jpeg_enc.y_offset; y < (jpeg_enc.y_offset + MCU_H); y++) {
//            for (int x = jpeg_enc.x_offset; x < (jpeg_enc.x_offset + MCU_W); x++) {
//                *Y0++ = jpeg_enc.pixels8[y * jpeg_enc.img_w + x];
//            }
//        }
//        break;
//    case 2: {
//        for (int y = jpeg_enc.y_offset, idx = 0; y < (jpeg_enc.y_offset + MCU_H); y++) {
//            for (int x = jpeg_enc.x_offset; x < (jpeg_enc.x_offset + MCU_W); x++, idx++) {
//                int ofs = y * jpeg_enc.img_w + x;
//                Y0[idx] = yuv_table[jpeg_enc.pixels16[ofs] * 3 + 0] - 128;
//                CB[idx] = yuv_table[jpeg_enc.pixels16[ofs] * 3 + 1] - 128;
//                CR[idx] = yuv_table[jpeg_enc.pixels16[ofs] * 3 + 2] - 128;
//            }
//        }
//        break;
//    }
//    case 3: {
//        uint16_t rgbbuf[64];
//        imlib_bayer_to_rgb565(jpeg_enc.img, 8, 8, jpeg_enc.x_offset, jpeg_enc.y_offset, rgbbuf);
//        for (int y = 0, idx = 0; y < 8; y++) {
//            for (int x = 0; x < 8; x++, idx++) {
//                Y0[idx] = yuv_table[rgbbuf[idx] * 3 + 0] - 128;
//                CB[idx] = yuv_table[rgbbuf[idx] * 3 + 1] - 128;
//                CR[idx] = yuv_table[rgbbuf[idx] * 3 + 2] - 128;
//            }
//        }
//        break;
//    }
//    }
//
//    jpeg_enc.x_offset += MCU_W;
//    if (jpeg_enc.x_offset == (jpeg_enc.mcu_row * MCU_W)) {
//        jpeg_enc.x_offset = 0;
//        jpeg_enc.y_offset += MCU_H;
//    }
//    return mcubuf;
//}
//


