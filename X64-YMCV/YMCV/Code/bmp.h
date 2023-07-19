#ifndef YMCV_BMP_H
#define YMCV_BMP_H

#include "../CONFIG/YMCV_PubType.h"

// �ļ���Ϣͷ�ṹ��
typedef struct {
    uint16 type; //��Ϊ'BM'
    uint32 size; //�ļ��ֽ���(2-5)
    uint32 reserved;//λͼ�ļ������֣���Ϊ0(6-9)
    uint32 off_bits; //��������ƫ�� (10-13)
}bmp_file_header_t;

//ͼ����Ϣͷ�ṹ��
typedef struct {
    uint32 size; // �ṹ��ߴ� (14-17)
    int32 width; // ͼ���� ?(18-21)
    int32 height; // ͼ��߶� ?(22-25)
    uint16 planes; // ɫ��ƽ����,Ŀ���豸�ļ���Ϊ1(26-27)
    uint16 bit_count; // ����λ����Ϊ1��4��8��16,24,32(28-29),Ϊ16,24,32ʱû�е�ɫ��(��ɫ��)
    uint32 compression; // ѹ����ʽ��0Ϊ��ѹ����1ΪBI_RLE8��2ΪBI_RLE4(30-33)
    uint32 size_image; // ���������ݴ�С,����bfSize-bfOffBits (34-37)
    int32 x_pels_permeter; // x����ֱ��ʣ�һ��Ϊ0 (38-41)
    int32 y_pels_permeter; // y����ֱ��ʣ�һ��Ϊ0 (42-45)
    uint32 clr_used; // ��ɫ����ɫ����0��ʾʹ�����е�ɫ����(46-49)
    uint32 clr_important; // ��Ҫ��ɫ��������Ŀ��0��ʾ����Ҫ(50-53)
} bmp_info_header_t;

//24λͼ������Ϣ�ṹ��,����ɫ��
typedef struct {
    uint8 b; //��ɫ���� ?(0-255)
    uint8 g; //��ɫ���� ?(0-255)
    uint8 r; //��ɫ���� ?(0-255)
    uint8 alpha;// ����������Ϊ0
} pixel32_info_t;

typedef struct {
    uint8 b; //��ɫ���� ?(0-255)
    uint8 g; //��ɫ���� ?(0-255)
    uint8 r; //��ɫ���� ?(0-255)
} pixel24_info_t;

typedef struct {
    bmp_info_header_t bmiHeader;// λͼ��Ϣͷ
    //pixel_info_t bmiColors[1];//��ɫ��
} bmp_info_t;

static bmp_file_header_t s_bmp_file_header = { 0x4d42, 0, 0, 0};
static bmp_info_header_t s_bmp_info_header = { 0, 0, 0, 1, 8, 0, 0, 0, 0, 0, 0 };


#endif // !YMCV_BMP_H


