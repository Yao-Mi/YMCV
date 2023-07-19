#include "../OPOBJ/YMCV_Creat.h"
#include "../OPOBJ/YMCV_Free.h"
#include "../DEBUG/YMCV_Debug.h"
#include "../CORE/YMCV_PixConv.h"
/**
  ***************************************************************************************************************************
  *	@FileName:    YMCV_ImageEncode.c
  *	@Author:      yaomimaoren
  *	@Date:        2022-3-2
  *	@Description: 图像编码
  *	@Version:     1.0
  *
  ***************************************************************************************************************************
  *
  * 备注信息：
  * 1.支持格式：
  *   jpeg编码
  *
  * 存在问题备注：
  *
  ***************************************************************************************************************************
  * <author> <time> <version > <desc>
  * yaomi 22/03/02 1.0 build this moudle
  *
  *   __  __ ___    ____   __  ___ ____     ______ ______ ______ __  __
  *   \ \/ //   |  / __ \ /  |/  //  _/    /_  __// ____// ____// / / /
  *    \  // /| | / / / // /|_/ / / /       / /  / __/  / /    / /_/ /
  *    / // ___ |/ /_/ // /  / /_/ /       / /  / /___ / /___ / __  /
  *   /_//_/  |_|\____//_/  /_//___/      /_/  /_____/ \____//_/ /_/
  *
  * Copyright (C), 2021-2031, YAOMI Tech. Co., Ltd.
  ***************************************************************************************************************************/




//  //image 转 文件
//void YMGRE_Image_LoadTo_Bmp_File(const char* file_path, GRErgb24* image, uint32 width, uint32 height)
//{
//    YMGRE_FILE* file = NULL;
//    gre_log_explain((NULL == image), GRE_LOG_PtrI, "输入图像不存在");
//
//    uint32 line_width = ((width + 3) / 4) * 4;
//    uint8 line_align[4] = { 0 };
//    s_bmp_file_header.type = 0x4d42;//'BM'
//    //带调色板
//    uint16 clr_nums = 0;//4 * 256
//    s_bmp_file_header.off_bits = (sizeof(bmp_file_header_t) - 2) + sizeof(bmp_info_header_t) + clr_nums;//由于4字节对齐，所以要少2个字节
//    //文件大小
//    s_bmp_file_header.size = s_bmp_file_header.off_bits + line_width * height;
//
//    //信息头
//    s_bmp_info_header.size = sizeof(bmp_info_header_t);
//    s_bmp_info_header.width = width;
//    s_bmp_info_header.height = height;
//    s_bmp_info_header.size_image = line_width * height;
//    s_bmp_info_header.clr_used = 0;//不使用调色板
//    s_bmp_info_header.clr_important = 0;
//    s_bmp_info_header.compression = 0;//不带压缩
//    s_bmp_info_header.planes = 1;
//    s_bmp_info_header.bit_count = 24;//24位
//    s_bmp_info_header.x_pels_permeter = 1;
//    s_bmp_info_header.y_pels_permeter = 1;
//
//    file = YMGRE_fopen(file_path, "wb");
//    gre_log_explain((NULL == file), GRE_LOG_FILE, "文件打开失败");
//
//    //必须分开写入，由于未能4字节对齐
//    YMGRE_fwrite(&s_bmp_file_header.type, sizeof(s_bmp_file_header.type), 1, file);//先写入2字节
//    YMGRE_fwrite(&s_bmp_file_header.size, sizeof(s_bmp_file_header) - 4, 1, file);//剩余字节写入，因为4字节对齐所以分开写入
//    //直接写入信息块
//    YMGRE_fwrite(&s_bmp_info_header, sizeof(bmp_info_header_t), 1, file);
//
//    ////写入调色板
//    //uint8_t alpha = 0;
//    //for (i = 0; i < 256; i++)
//    //{
//    //    fwrite(&i, 1, sizeof(uint8_t), file);
//    //    fwrite(&i, 1, sizeof(uint8_t), file);
//    //    fwrite(&i, 1, sizeof(uint8_t), file);
//    //    fwrite(&alpha, 1, sizeof(uint8_t), file);
//    //}
//    pixel24_info_t* linebuff = GRE_malloc0(line_width * sizeof(pixel24_info_t));
//    GRE_memset(linebuff, 0, line_width * sizeof(pixel24_info_t));//初始化
//
//    for (int32 i = height - 1; i >= 0; i--)
//    {
//        GRErgb24* bufo = &image[i * width];
//        //有效部分写入
//        for (int j = 0; j < width; j++)
//        {
//            //记录
//            linebuff[j].b = bufo[j].B;
//            linebuff[j].g = bufo[j].G;
//            linebuff[j].r = bufo[j].R;
//        }
//        YMGRE_fwrite(linebuff, sizeof(pixel24_info_t), line_width, file);//写入24bit
//    }
//    //fflush(file);//刷入内存
//    //关闭文件自动刷入内存中
//    YMGRE_fclose(file);
//}



//
//void gif_open(FIL* fp, int width, int height, bool color, bool loop)
//{
//    file_buffer_on(fp);
//
//    write_data(fp, "GIF89a", 6);
//    write_word(fp, width);
//    write_word(fp, height);
//    write_data(fp, (uint8_t[]) { 0xF6, 0x00, 0x00 }, 3);
//
//    if (color) {
//        for (int i = 0; i < 128; i++) {
//            int red = ((((i & 0x60) >> 5) * 255) + 1.5) / 3;
//            int green = ((((i & 0x1C) >> 2) * 255) + 3.5) / 7;
//            int blue = (((i & 0x3) * 255) + 1.5) / 3;
//            write_data(fp, (uint8_t[]) { red, green, blue }, 3);
//        }
//    }
//    else {
//        for (int i = 0; i < 128; i++) {
//            int gray = ((i * 255) + 63.5) / 127;
//            write_data(fp, (uint8_t[]) { gray, gray, gray }, 3);
//        }
//    }
//
//    if (loop) {
//        write_data(fp, (uint8_t[]) { '!', 0xFF, 0x0B }, 3);
//        write_data(fp, "NETSCAPE2.0", 11);
//        write_data(fp, (uint8_t[]) { 0x03, 0x01, 0x00, 0x00, 0x00 }, 5);
//    }
//
//    file_buffer_off(fp);
//}
//
//void gif_add_frame(FIL* fp, image_t* img, uint16_t delay)
//{
//    file_buffer_on(fp);
//
//    if (delay) {
//        write_data(fp, (uint8_t[]) { '!', 0xF9, 0x04, 0x04 }, 4);
//        write_word(fp, delay);
//        write_word(fp, 0); // end
//    }
//
//    write_byte(fp, 0x2C);
//    write_long(fp, 0);
//    write_word(fp, img->w);
//    write_word(fp, img->h);
//    write_data(fp, (uint8_t[]) { 0x00, 0x07 }, 2); // 7-bits
//
//    int bytes = img->h * img->w;
//    int blocks = (bytes + BLOCK_SIZE - 1) / BLOCK_SIZE;
//
//    if (IM_IS_GS(img)) {
//        for (int y = 0; y < blocks; y++) {
//            int block_size = IM_MIN(BLOCK_SIZE, bytes - (y * BLOCK_SIZE));
//            write_byte(fp, 1 + block_size);
//            write_byte(fp, 0x80); // clear code
//            for (int x = 0; x < block_size; x++) {
//                write_byte(fp, img->pixels[(y * BLOCK_SIZE) + x] >> 1);
//            }
//        }
//    }
//    else if (IM_IS_RGB565(img)) {
//        for (int y = 0; y < blocks; y++) {
//            int block_size = IM_MIN(BLOCK_SIZE, bytes - (y * BLOCK_SIZE));
//            write_byte(fp, 1 + block_size);
//            write_byte(fp, 0x80); // clear code
//            for (int x = 0; x < block_size; x++) {
//                int pixel = ((uint16_t*)img->pixels)[(y * BLOCK_SIZE) + x];
//                int red = IM_R565(pixel) >> 3;
//                int green = IM_G565(pixel) >> 3;
//                int blue = IM_B565(pixel) >> 3;
//                write_byte(fp, (red << 5) | (green << 2) | blue);
//            }
//        }
//    }
//    else if (IM_IS_BAYER(img)) {
//        for (int y = 0; y < blocks; y++) {
//            int block_size = IM_MIN(BLOCK_SIZE, bytes - (y * BLOCK_SIZE));
//            write_byte(fp, 1 + block_size);
//            write_byte(fp, 0x80); // clear code
//            for (int x = 0; x < block_size; x++) {
//                int r = 0, g = 0, b = 0;
//                int x_offs = ((y * BLOCK_SIZE) + x) % img->w;
//                int y_offs = (y * BLOCK_SIZE) / img->w;
//                if (x_offs > 0 && y_offs > 0 && x_offs < img->w - 1 && y_offs < img->h - 1) {
//                    COLOR_BAYER_TO_RGB565(img, x_offs, y_offs, r, g, b);
//                }
//                r >>= 3; g >>= 3; b >>= 3;
//                write_byte(fp, (r << 5) | (g << 2) | b);
//            }
//        }
//    }
//
//
//    write_data(fp, (uint8_t[]) { 0x01, 0x81, 0x00 }, 3); // end code
//
//    file_buffer_off(fp);
//}
//
//void gif_close(FIL* fp)
//{
//    write_byte(fp, ';');
//    file_close(fp);
//}
