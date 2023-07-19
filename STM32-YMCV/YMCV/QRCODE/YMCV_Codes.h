#ifndef _YMCVCODES_H
#define _YMCVCODES_H


#include "../OPOBJ/YMCV_Creat.h"

#define	QR_NUM_MODE  1   //����ģʽ
#define	QR_AN_MODE   2   //��ĸ����ģʽ
#define	QR_HZ_MODE   3   //����ģʽ
#define	QR_8B_MODE   4   //8λ�ֽ�ģʽ
#define	QR_ECI_MODE  5   //ECIģʽ


#define QRWB  125//��������


///////// apriltag //////////////
typedef enum
{
    CV_AprilTag16H5 = 0x01,
    CV_AprilTag25H7 = 0x02,
    CV_AprilTag25H9 = 0x04,
    CV_AprilTag36H10 = 0x08,
    CV_AprilTag36H11 = 0x10,
    CV_ArToolKit = 0x20,
}CVAprilTagType;

typedef struct CV_apriltags_data_ {
    CVpoint corners[4];
    CVrect rect;
    uint16_t id;
    uint8_t family, hamming;
    CVpoint centroid;
    float goodness, decision_margin;
    float Txyz[3];
    float Rxyz[3][3];
    float x_rotation, y_rotation, z_rotation;
} CV_apriltags_data;


typedef struct CV_apriltag_detect_info_ {
    CVAprilTagType types;// detection type
    float tagsize; // In meters.
    float fx; // In pixels.
    float fy; // In pixels.
    float cx; // In pixels.
    float cy; // In pixels.
} CV_apriltag_detect_info;

CVVECTORS_U16 YMCV_Gray_Find_AprilTags(CVIMAGE gray, CV_apriltag_detect_info detect_info);

//////// datamatrix ///////

typedef struct CV_datamatrices_data_ {
    CVpoint corners[4];
    CVrect rect;
    int16_t rotation;
    uint8_t rows, columns;
    uint16_t capacity, padding;
    size_t payload_len;
    char* payload;//�䳤����
} CV_datamatrices_data;

CVVECTORS_U16 YMCV_Gray_Find_DataMatrices(CVIMAGE gray, int max_iterations);

//////// qrcode ///////

typedef struct CV_qrcodes_data_ {
    CVpoint corners[4];
    CVrect rect;
    uint8_t version, ecc_level, mask, data_type;
    uint32_t eci;
    size_t payload_len;
    char* payload;//�䳤����
} CV_qrcodes_data;

CVVECTORS_U16 YMCV_Gray_Find_QRCode(CVIMAGE gray);

//////// barcode //////

typedef enum 
{
    CV_BarCode_UNKNOW,//δ֪����
    CV_BarCode_EAN2,
    CV_BarCode_EAN5,
    CV_BarCode_EAN8,
    CV_BarCode_UPCE,
    CV_BarCode_ISBN10,
    CV_BarCode_UPCA,
    CV_BarCode_EAN13,
    CV_BarCode_ISBN13,
    CV_BarCode_I25,
    CV_BarCode_DATABAR,
    CV_BarCode_DATABAR_EXP,
    CV_BarCode_CODABAR,
    CV_BarCode_CODE39,
    CV_BarCode_PDF417,
    CV_BarCode_CODE93,
    CV_BarCode_CODE128
} CVBarCodeType;

typedef struct CV_barcodes_data_ {
    CVpoint corners[4];
    CVrect rect;
    uint16_t bartype, rotation;
    int quality;
    size_t payload_len;
    char* payload;
} CV_barcodes_data;

CVVECTORS_U16 YMCV_Gray_Find_BarCode(CVIMAGE gray);

///////// qrencode ////////

CVIMAGE YMCV_QRCode_EncodeToGrayImg(const char* str, uint8 padw);//����qrencode
CVIMAGE YMCV_QRCode_EncodeToGrayImg_Fast(const char* str, uint8 padw);//����libqr

#endif //!YMCVCODES_H

