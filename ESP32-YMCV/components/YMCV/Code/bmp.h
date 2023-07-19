#ifndef YMCV_BMP_H
#define YMCV_BMP_H

#include "../CONFIG/YMCV_PubType.h"

// 文件信息头结构体
typedef struct {
    uint16 type; //必为'BM'
    uint32 size; //文件字节数(2-5)
    uint32 reserved;//位图文件保留字，必为0(6-9)
    uint32 off_bits; //像素数据偏移 (10-13)
}bmp_file_header_t;

//图像信息头结构体
typedef struct {
    uint32 size; // 结构体尺寸 (14-17)
    int32 width; // 图像宽度 ?(18-21)
    int32 height; // 图像高度 ?(22-25)
    uint16 planes; // 色彩平面数,目标设备的级别，为1(26-27)
    uint16 bit_count; // 像素位数，为1、4、8或16,24,32(28-29),为16,24,32时没有调色板(颜色表)
    uint32 compression; // 压缩方式，0为不压缩、1为BI_RLE8、2为BI_RLE4(30-33)
    uint32 size_image; // 单像素数据大小,等于bfSize-bfOffBits (34-37)
    int32 x_pels_permeter; // x方向分辨率，一般为0 (38-41)
    int32 y_pels_permeter; // y方向分辨率，一般为0 (42-45)
    uint32 clr_used; // 调色板颜色数，0表示使用所有调色板项(46-49)
    uint32 clr_important; // 重要颜色索引的数目，0表示都重要(50-53)
} bmp_info_header_t;

//24位图像素信息结构体,即调色板
typedef struct {
    uint8 b; //蓝色分量 ?(0-255)
    uint8 g; //绿色分量 ?(0-255)
    uint8 r; //红色分量 ?(0-255)
    uint8 alpha;// 保留，必须为0
} pixel32_info_t;

typedef struct {
    uint8 b; //蓝色分量 ?(0-255)
    uint8 g; //绿色分量 ?(0-255)
    uint8 r; //红色分量 ?(0-255)
} pixel24_info_t;

typedef struct {
    bmp_info_header_t bmiHeader;// 位图信息头
    //pixel_info_t bmiColors[1];//颜色表
} bmp_info_t;

static bmp_file_header_t s_bmp_file_header = { 0x4d42, 0, 0, 0};
static bmp_info_header_t s_bmp_info_header = { 0, 0, 0, 1, 8, 0, 0, 0, 0, 0, 0 };


#endif // !YMCV_BMP_H


