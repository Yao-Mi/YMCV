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
//  *	@Description: jpeg编码和解码
//  *	@Version:     1.0
//  *
//  ***************************************************************************************************************************
//  *
//  * 备注信息：
//  * 1.支持格式：
//  *   jpeg编码
//  *
//  * 存在问题备注：
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
////MCU（最小编码单元）
//#define JPEG_MCU_SIZE 4
//
//int block[JPEG_MCU_SIZE][JPEG_MCU_SIZE], tblock[JPEG_MCU_SIZE][JPEG_MCU_SIZE];
////算法实践：http://www.javashuo.com/article/p-rtqnooot-kc.html
////原理：https://blog.csdn.net/newchenxf/article/details/51719597
////视频编码：https://blog.csdn.net/bigdream123/category_9433245.html
////蝶形算法
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
//        YMCV_Queue_PushU8(data[i], jpeg_buf);//写入buff队列中
//    }
//}
////jpeg文件格式参考：https://www.cnblogs.com/leaven/archive/2010/04/06/1705846.html
////JFIF格式的JPEG文件(*.jpg)的一般顺序为：
////SOI(0xFFD8)
////APP0(0xFFE0)
////[APPn(0xFFEn)]可选
////DQT(0xFFDB)
////SOF0(0xFFC0)
////DHT(0xFFC4)
////SOS(0xFFDA)
////
////压缩数据
////
////EOI(0xFFD9)
//void jpeg_write_head()
//{
//    CV_queue* jpeg_buf=NULL;
//    //字的高低位问题
//   //JPEG文件格式中，一个字（16位）的存储使用的是 Motorola 格式, 而不是 Intel 格式。
//   //也就是说, 一个字的高字节（高8位）在数据流的前面, 低字节（低8位）在数据流的后面，与平时习惯的Intel格式不一样。
//
//   // Number of components (1 or 3)
//    uint8_t nr_comp = (bpp == 1) ? 1 : 3;
//
//    // JPEG headers
//    uint8_t jpeg_soi[] = {
//        0xFF, 0xD8          // SOI固定值0xFFD8
//    };
//    jpeg_write_bytes(jpeg_buf, jpeg_soi, sizeof(jpeg_soi));
//
//
//    //本标记段可以包含图像的一个微缩版本，存为24位的RGB像素。
//    //如果没有微缩图像（这种情况更常见），则缩略图水平、垂直像素数目的值均为0。
//    uint8_t jpeg_app0[] = {
//        0xFF, 0xE0,         // APP0固定值0xFFE0
//        //包含9个具体字段：
//        0x00, 0x10, //9个字段的总长
//        'J',  'F',  'I',  'F', 0x00,//字符串“JFIF0”
//        0x01,0x01,//版本号一般是0x0101，表示JFIF的版本号1.1
//        0x00,//X和Y的密度单位  0：无单位；1：点数/英寸；2：点数/厘米
//        0x00, 0x01, //X方向像素密度   2字节 
//        0x00, 0x01, //Y方向像素密度   2字节 
//        0x00,//缩略图水平像素数目 1字节
//        0x00 //缩略图垂直像素数目  1字节
//        //缩略图RGB位图  长度可能是3的倍数  缩略图RGB位图数据:
//        //①rgb[3*n] 
//    };
//    jpeg_write_bytes(jpeg_buf, jpeg_app0, sizeof(jpeg_app0));
//
//
//    //应用程序保留标记n，其中n=1～15(任选)
//    uint8_t jpeg_appn[] =
//    {
//        0xFF, 0xF1,   //2字节     固定值0xFFE1~0xFFF
//        //包含两个字段
//        0x00, 0x0B,        //数据长度   2字节  2个字段的总长度
//        'Y',  'M',  'C',  'V', '-',  '2',  '.',  '0', 0x00, //详细信息  数据长度 - 2字节   内容不定
//    };
//    jpeg_write_bytes(jpeg_buf, jpeg_appn, sizeof(jpeg_appn));
//
//
//    //定义量化表
//    //本段中，量化表可以重复出现，表示多个量化表，但最多只能出现4次。
//    uint8_t jpeg_dqt[] = {
//        0xFF, 0xDB,  // DQT  2字节  固定值0xFFDB
//        //包含9个字段
//        0x00,0x00,// 字段长度  2字节
//        //量化表，后面再定义:
//        //①精度及量化表ID   1字节  
//        //高4位：精度，只有两个可选值:0：8位；1：16位
//        //低4位：量化表ID，取值范围为0～3
//        //②表项(64×(精度 + 1))字节
//        //例如8位精度的量化表 其表项长度为64×（0 + 1） = 64字节
//    };
//    jpeg_dqt[2] = (bpp * 65 + 2) >> 8;// Header length MSB
//    jpeg_dqt[3] = (bpp * 65 + 2) & 0xFF;// Header length LSB
//
//    jpeg_write_bytes(jpeg_buf, jpeg_dqt, sizeof(jpeg_dqt));
//
//
//    //帧图像开始
//    //本段中，颜色分量信息应该重复出现，有多少个颜色分量(比如3)，就出现多少次（3次）
//    uint8_t jpeg_sof0[] = {
//        0xFF, 0xC0,         // SOF0 2字节 固定值0xFFC0
//        //包含6个字段
//        0x00,0x00,// 字段长度  2字节
//        0x00,//精度  1字节  每个数据样本的位数
//        0x00,0x00,//图像高度  2字节  图像高度（单位：像素），如果不支持 DNL 就必须 >0
//        0x00,0x00,//图像宽度  2字节  图像宽度（单位：像素），如果不支持 DNL 就必须 >0
//        0x03,//颜色分量数  1字节  只有3个数值可选 1：灰度图；3：YCrCb或YIQ；4：CMYK ，而JFIF中使用YCrCb，故这里颜色分量数恒为3
//        //颜色分量信息  颜色分量数×3字节（通常为9字节）：
//        //① 颜色分量ID   1字节
//        //② 水平/垂直采样因子  1字节   高4位：水平采样因子 低4位：垂直采样因子 （曾经看到某资料把这两者调转了）
//        //③ 量化表 1字节   当前分量使用的量化表的ID
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
//    //定义哈夫曼表
//    //本段中，哈夫曼表可以重复出现（一般4次），也可以只出现1次。
//    //例如，Adobe Photoshop 生成的JPEG图片文件中只有1个DHT标记段，里边包含了4个哈夫曼表；
//    //而Macromedia Fireworks生成的JPEG图片文件则有4个DHT标记段，每个DHT标记段只有一个哈夫曼表。
//    uint8_t jpeg_dht[] = {
//        0xFF, 0xC4,         // DHT  2字节 固定值0xFFC4
//        //包含2个具体字段：
//        0x00, 0x00,//字段的总长度 2字节
//        //哈夫曼表 数据长度 - 2字节:
//        //①表ID和表类型   1字节   高4位：类型，只有两个值可选 0：DC直流；1：AC交流
//        //低4位：哈夫曼表ID， 注意，DC表和AC表分开编码
//        //②不同位数的码字数量    16字节
//        //③编码内容 16个不同位数的码字数量之和（字节）
//    };
//    jpeg_dht[2] = (bpp * 208 + 2) >> 8;     // Header length MSB
//    jpeg_dht[3] = (bpp * 208 + 2) & 0xFF;   // Header length LSB
//
//    jpeg_write_bytes(jpeg_buf, jpeg_dht, sizeof(jpeg_dht));
//
//
//
//    //定义差分编码累计复位的间隔
//    //如果没有本标记段，或间隔值为0时，就表示不存在重开始间隔和标记RST
//    uint8_t jpeg_dri[] = {
//        0xFF, 0xDD,         // DRI 2字节 固定值0xFFDD
//        //包含2个具体字段：
//        0x00, 0x04,//字段的总长度  2字节  固定值0x0004
//        //②MCU块的单元中的重新开始间隔
//        0x00, 0x00,//2字节 设其值为n，则表示每n个MCU块就有一个RSTn标记。
//        //如：第一个标记是RST0，第二个是 RST1等，RST7后再从RST0重复。
//    };
//
//    jpeg_write_bytes(jpeg_buf, jpeg_dri, sizeof(jpeg_dri));
//
//    //扫描开始
//    //本段中，颜色分量信息应该重复出现，有多少个颜色分量（如3个），就出现多少次（一般为3次）。
//    //本段结束后，紧接着就是真正的图像信息了。图像信息直至遇到一个标记代码就自动结束，一般就是以EOI标记表示结束
//    uint8_t jpeg_sos[] = {
//        0xFF, 0xDA, // SOS 2字节  固定值0xFFDA
//        //包含2个具体字段：
//        0x00, 0x00,//字段的总长度  2字节
//        0x03, //颜色分量数 1字节 应该和SOF中的字段⑤的值相同，即： 1：灰度图是；3： YCrCb或YIQ；4：CMYK。 而JFIF中使用YCrCb，故这里颜色分量数恒为3
//        //颜色分量信息：
//        //①颜色分量ID   1字节 
//        //②直流/交流系数表号 1字节     高4位：直流分量使用的哈夫曼树编号, 低4位：交流分量使用的哈夫曼树编号
//        //压缩图像数据：
//        //③谱选择开始 1字节 固定值0x00
//        //④谱选择结束 1字节 固定值0x3F
//        //⑤谱选择 1字节 在基本JPEG中总为00
//    };
//    jpeg_sos[2] = (nr_comp * 2 + 6) >> 8; // Header length MSB
//    jpeg_sos[3] = (nr_comp * 2 + 6) & 0xFF; // Header length LSB
//    jpeg_sos[4] = nr_comp;            // Number of components
//
//    jpeg_write_bytes(jpeg_buf, jpeg_sos, sizeof(jpeg_sos));
//}
//void jpeg_write_data()
//{
//    //这里补充说明一下，由于在JPEG文件中0xFF具有标志性的意思，
//    //所以在压缩数据流(真正的图像信息)中出现0xFF，就需要作特别处理。具体方法是:
//    //在数据0xFF后添加一个没有意义的0x00。
//    //换句话说，如果在图像数据流中遇到0xFF，应该检测其紧接着的字符，如果是:
//    //1）0x00，则表示0xFF是图像流的组成部分，需要进行译码；
//    //2）0xD9，则与0xFF组成标记EOI，则图像流结束，同时图像文件结束；
//    //3）0xD0~0xD7, 则组成RSTn标记，则要忽视整个RSTn标记，即不对当前0xFF和紧接的0xDn两个字节进行译码，并按RST标记的规则调整译码变量；
//    //3）0xFF，则忽视当前0xFF，对后一个0xFF再作判断；
//    //4）其他数值，则忽视当前0xFF，并保留紧接的此数值用于译码。
//}
//void jped_write_end()
//{
//    //图像结束 2字节
//    uint8_t jpeg_eoi[] = {
//        0xFF, 0xD9,//ROI  2字节     固定值0xFFD9
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


