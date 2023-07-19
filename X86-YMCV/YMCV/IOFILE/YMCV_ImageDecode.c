#include "./YMCV_IOFile.h"
#include "../CONFIG/YMCV_PubDefine.h"
#include "../DEBUG/YMCV_Debug.h"
#include "../CORE/YMCV_PixConv.h"
#include "../CONFIG/YMCV_Mem.h"
#include "../Code/tjpgd.h"
#include "../Code/bmp.h"

/**
  ***************************************************************************************************************************
  *	@FileName:    YMCV_ImageDecode.c
  *	@Author:      yaomimaoren
  *	@Date:        2022-3-2
  *	@Description: ͼ�����
  *	@Version:     1.0
  *
  ***************************************************************************************************************************
  *
  * ��ע��Ϣ��
  * 1.֧�ָ�ʽ��
  *   jpeg����
  *   jpeg����
  *
  * �������ⱸע��
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


static uint8 tjpg_imgOutType = CVRgb16Type;
static uint8* tjpg_imgOutBuff = NULL;
static uint16 tjpg_imgOutWidth = 0, tjpg_imgOutHeight = 0;


uint32 tjpgdec_in_func(/* Returns number of bytes read (zero on error) */
JDEC* jd,		/* Decompression object */
uint8_t* buff,	/* Pointer to the read buffer (null to remove data) */
size_t ndata	/* Number of bytes to read/skip */
)
{
	uint32  rb;
	FILE* dev = (FILE*)jd->device;//��������ļ�����Ϣ
	if (buff)     				//��ȡ������Ч����ʼ��ȡ����
	{
		rb = YMCV_fread(buff,1, ndata, dev);//����fread���������ڰ�jpeg�ļ������ݶ�ȡ����
	}
	else
	{
		//���¶�λ�ļ�ָ�룬���ƫ��num
		rb = (YMCV_fseek(dev, YMCV_ftell(dev) + ndata, SEEK_SET) == 0) ? ndata : 0;
	}
	return rb;//���ض�ȡ�����ֽ���Ŀ
}

static JRESULT tjpgdec_output_func(	/* 0:Ok, 1:Aborted */
	JDEC* jd,		/* Decompression object */
	void* bitmap,	/* Bitmap data to be output */
	JRECT* rect		/* Rectangular region to output */
)
{
	uint16 xstart = rect->left, ystart = rect->top;//���ͼ�����ʼ����
	uint16 width = rect->right - rect->left + 1;//���ͼ��Ŀ��
	uint16 height = rect->bottom - rect->top + 1;//���ͼ��Ŀ��
	uint16 xend = xstart + width;
	uint16 yend = ystart + height;
	uint16* src = bitmap;//rgb565

	if (tjpg_imgOutType == CVRgb16Type)
	{
		for (int yi = ystart, yimax = CVMin(tjpg_imgOutHeight, yend); yi < yimax; yi++)//y����
		{
			uint16* line0 = &tjpg_imgOutBuff[2 * (yi * tjpg_imgOutWidth + xstart)];//rgb565��ʽ��һ����������byte
			//x�������
			int xj = xstart, xjmax = CVMin(tjpg_imgOutWidth, xend);
			for (; xj < xjmax; xj++)//
			{
				*line0++ = *src++;//һ������
			}
			//���������
			for (; xj < xend; xj++)
				*src++;
		}
	}
	else if (tjpg_imgOutType == CVGrayType)
	{
		for (int yi = ystart, yimax = CVMin(tjpg_imgOutHeight, yend); yi < yimax; yi++)//y����
		{
			CVgray* line0 = &tjpg_imgOutBuff[yi * tjpg_imgOutWidth + xstart];//rgb565��ʽ��һ����������byte
			//x�������
			int xj = xstart, xjmax = CVMin(tjpg_imgOutWidth, xend);
			for (; xj < xjmax; xj++, src++)
			{
				
				*line0++ = YMCV_Rgb565ToGray(*src);//һ������
			}
			//���������
			for (; xj < xend; xj++)
				*src++;
		}
	}
	//δ֪��ʽ
	else
	{
		return JDR_INTR;
	}
	return JDR_OK;	/* Continue to decompress */
}


/**
  * @brief ��ȡͼƬ�ļ�
  *		Ŀǰֻ֧��jpg��bmp��ʽ
  *		outType������ rgb565����gray
  */
CVIMAGE YMCV_Image_Read(const char* filepath, CVImgType outType)
{
	CVIMAGE myimg = NULL;

	cv_log_explain((outType != CVGrayType) && (outType != CVRgb16Type), CV_LOG_ParamI, "���ͼ���ʽ��ֻ֧��rgb565���߻Ҷȸ�ʽ");

	char* chp = YMCV_Strrchr(filepath, '.'); 
	cv_log_explain(chp == NULL, CV_LOG_ParamI, "�ļ���׺��ȡʧ��");

	//��дתСд
	char file_type[6] = { 0 };
	for (int i = 0; (i < sizeof(file_type)) && ((*chp) != '\0'); i++, chp++)
	{
		file_type[i] = ((*chp) >= 'A' && (*chp) <= 'Z') ? (*chp) + 0x20 : (*chp);//��д��ĸתСд
	}
	//�����ļ����ʹ�
	if (YMCV_Strcmp(file_type, ".jpg") == 0)//jpg�ļ�
	{
		YMCV_FILE* f_jpeg = YMCV_fopen(filepath, "rb");//��������Զ������ļ���ȡ
		if (f_jpeg != NULL)
		{
			//TJpgDec������Ҫ3100�ֽ�����JPEGͼ������ȫȡ����ʲô���Ĳ�������������JPEGͼ��
			//3100�ֽ�����Ĭ�����뻺��(JD_SZBUF == 512)�µ�����ڴ����󣬲���JD_SZBUF�仯��
			//JD_SZBUF����ÿ�δ��������ж�ȡ�����ֽڡ�
			//TJpgDec����ÿ�������󻺳�����С��512, 1024, 2048...�ֽ��ǴӴ洢�豸��ȡ�������С��
			int workBuffSize = 3100;//775 * 4 �������4�ֽڶ���

			JDEC* jpeg_dev = CV_malloc0(sizeof(JDEC)); //���������ṹ��ָ��  
			uint8* jpg_buffer = CV_malloc1(workBuffSize * sizeof(uint8)); //����jpeg���빤������С(������Ҫ3092�ֽ�)����Ϊ��ѹ������������4�ֽڶ���
			cv_log_explain(jpeg_dev == NULL, CV_LOG_Mem0, "������󴴽�ʧ��");
			cv_log_explain(jpg_buffer == NULL, CV_LOG_Mem1, "���뻺�洴��ʧ��");
			cv_log_explain(JD_FORMAT != 1, CV_LOG_ParamI, "TjpgDec�����ʽ����ΪRGB565");

			uint8 res = jd_prepare(jpeg_dev, tjpgdec_in_func, jpg_buffer, workBuffSize, f_jpeg);
			if (res == JDR_OK)//����׼���ɹ�
			{
				uint8 scale = 0;
				uint16 outw = jpeg_dev->width >> scale;
				uint16 outh = jpeg_dev->height >> scale;
				myimg = YMCV_Creat_Img_Creat(outw, outh, outType);//�������ͼ��
				tjpg_imgOutBuff = myimg->data;//�������
				tjpg_imgOutWidth = myimg->width;
				tjpg_imgOutHeight = myimg->height;
				tjpg_imgOutType = outType;
				/* Start JPEG decompression */
				uint8 rc = jd_decomp(jpeg_dev, tjpgdec_output_func, scale);
				cv_log_explain(rc != JDR_OK, CV_LOG_ParamI, "TjpgDec����ʧ��");
			}
			CV_free0(jpeg_dev);
			CV_free1(jpg_buffer);
		}
		else
		{
			cv_log_explain(f_jpeg == NULL, CV_LOG_ParamI, "�ļ���ʧ��");
		}
		
		YMCV_fclose(f_jpeg);
	}
	else if(YMCV_Strcmp(file_type, ".bmp") == 0)
	{
		// ʵ�ֲο� https://www.jb51.net/article/226189.htm
		YMCV_FILE* f_bmp = YMCV_fopen(filepath, "rb");//��������Զ������ļ���ȡ
		if (f_bmp != NULL)
		{
			//��ȡ�ļ�ͷ
			YMCV_fread(&s_bmp_file_header.type, sizeof(uint16), 1, f_bmp);//����4�ֽڶ��룬������ȡ2�ֽ�
			YMCV_fread(&s_bmp_file_header.size, sizeof(s_bmp_file_header) - 4, 1, f_bmp);//����4�ֽڶ��룬����Ҫ�ٶ�ȡ4�ֽ�
			cv_log_explain(s_bmp_file_header.type != 0x4d42, CV_LOG_ParamI, "�ļ�����BMP��ʽ");

			//��ȡ��Ϣͷ
			YMCV_fread(&s_bmp_info_header, sizeof(s_bmp_info_header), 1, f_bmp);
			uint16 width = s_bmp_info_header.width;
			uint16 height = s_bmp_info_header.height;
			cv_log_explain(s_bmp_info_header.compression != 0, CV_LOG_ParamI, "�ݲ�֧�ִ�ѹ����BMP");

			myimg = YMCV_Creat_Img_Creat(width, height, outType);//�������ͼ��
			
			//��Ҫ��ɫ�崦��
			if (s_bmp_info_header.bit_count < 16)
			{
				//����ʧ��
				cv_log_explain(1, CV_LOG_ParamI, "�ݲ�֧�ִ���ɫ��ͼ������bit��<16��");
				//char temp[4 * 256] = { 0 };//��ɫ��
				//fread(temp, 4 * 256, 1, file);
				//�ֱ�Ϊ 2*4 ��16*4 ��256*4���ֽڵĵ�ɫ��
			}
			//�����ɫ��
			else
			{
				if (s_bmp_info_header.bit_count == 24)
				{
					//�ֽ�������4�ı���������0����
					uint16 line_width = ((width + 3) / 4) * 4;
					pixel24_info_t* linebuff = CV_malloc0(line_width * sizeof(pixel24_info_t));
					cv_log_explain(linebuff == NULL, CV_LOG_Mem0, "���뻺�洴��ʧ��");
					//�����rgb565
					if (outType == CVRgb16Type)
					{
						uint16* buf = myimg->data;
						for (int i = height - 1; i >= 0; i--)
						{
							YMCV_fread(linebuff, sizeof(pixel24_info_t), line_width, f_bmp);
							uint16* bufo = &buf[i * width];
							//��Ч�������
							for (int j = 0; j < width; j++)
							{
								//��¼
								bufo[j] = YMCV_Rgb3ToRgb565(linebuff[j].r, linebuff[j].g, linebuff[j].b);
							}
						}
					}
					//�Ҷ�
					else if (outType == CVGrayType)
					{
						CVgray* buf = myimg->data;
						for (int i = height - 1; i >= 0; i--)
						{
							YMCV_fread(linebuff, sizeof(pixel24_info_t), line_width, f_bmp);
							CVgray* bufo = &buf[i * width];
							//��Ч�������
							for (int j = 0; j < width; j++)
							{
								//��¼
								bufo[j] = YMCV_Rgb3ToGray(linebuff[j].r, linebuff[j].g, linebuff[j].b);
							}
						}
					}
					
					CV_free0(linebuff);
				}
				else
				{
					cv_log_explain(s_bmp_info_header.bit_count == 16, CV_LOG_ParamI, "��BMP����Ϊ16λ������ӽ���");
					cv_log_explain(s_bmp_info_header.bit_count == 32, CV_LOG_ParamI, "��BMP����Ϊ32λ������ӽ���");
				}
			}
		}
		else
		{
			cv_log_explain(f_bmp == NULL, CV_LOG_ParamI, "�ļ���ʧ��");
		}

		YMCV_fclose(f_bmp);
	}
	else
	{
		cv_log_explain(1, CV_LOG_ParamI, "�ݲ�֧�ָø�ʽ����");
	}

	return myimg;
}



//////////////////////////////////////////////////////////////////////// ����ΪAVI������ӵĺ���//////////////////////////////////////////////////////////


static char avi_frameBuffer[YMCV_AVI_FRAME_SIZE_MAX];//һ��jpegͼƬ�Ļ���
uint32 avi_jpegdatai = 0;

static uint8 avi_imgOutType = CVRgb16Type;
static uint8* avi_imgOutBuff = NULL;
static uint16 avi_imgOutWidth = 0, avi_imgOutHeight = 0;


uint32 avi_tjpgdec_in_func(/* Returns number of bytes read (zero on error) */
	JDEC* jd,		/* Decompression object */
	uint8_t* buff,	/* Pointer to the read buffer (null to remove data) */
	size_t ndata	/* Number of bytes to read/skip */
)
{
	uint32  rb;
	FILE* dev = (FILE*)jd->device;//��������ļ�����Ϣ
	if (buff)     				//��ȡ������Ч����ʼ��ȡ����
	{
		//��jpeg�ļ������ݶ�ȡ����
		for (int i = 0; i < ndata; i++)
		{
			buff[i] = avi_frameBuffer[avi_jpegdatai];
			avi_jpegdatai++;
		}
		rb = ndata;
	}
	else
	{
		//��������
		avi_jpegdatai += ndata;
		rb = ndata;
	}
	return rb;//���ض�ȡ�����ֽ���Ŀ
}

static JRESULT avi_tjpgdec_output_func(	/* 0:Ok, 1:Aborted */
	JDEC* jd,		/* Decompression object */
	void* bitmap,	/* Bitmap data to be output */
	JRECT* rect		/* Rectangular region to output */
)
{
	uint16 xstart = rect->left, ystart = rect->top;//���ͼ�����ʼ����
	uint16 width = rect->right - rect->left + 1;//���ͼ��Ŀ��
	uint16 height = rect->bottom - rect->top + 1;//���ͼ��Ŀ��
	uint16 xend = xstart + width;
	uint16 yend = ystart + height;
	uint16* src = bitmap;//rgb565

	if (avi_imgOutType == CVRgb16Type)
	{
		for (int yi = ystart, yimax = CVMin(avi_imgOutHeight, yend); yi < yimax; yi++)//y����
		{
			uint16* line0 = &avi_imgOutBuff[2 * (yi * avi_imgOutWidth + xstart)];//rgb565��ʽ��һ����������byte
			//x�������
			int xj = xstart, xjmax = CVMin(avi_imgOutWidth, xend);
			for (; xj < xjmax; xj++)//
			{
				*line0++ = *src++;//һ������
			}
			//���������
			for (; xj < xend; xj++)
				*src++;
		}
	}
	else if (avi_imgOutType == CVGrayType)
	{
		for (int yi = ystart, yimax = CVMin(avi_imgOutHeight, yend); yi < yimax; yi++)//y����
		{
			CVgray* line0 = &avi_imgOutBuff[yi * avi_imgOutWidth + xstart];//rgb565��ʽ��һ����������byte
			//x�������
			int xj = xstart, xjmax = CVMin(avi_imgOutWidth, xend);
			for (; xj < xjmax; xj++, src++)
			{

				*line0++ = YMCV_Rgb565ToGray(*src);//һ������
			}
			//���������
			for (; xj < xend; xj++)
				*src++;
		}
	}
	//δ֪��ʽ
	else
	{
		return JDR_INTR;
	}
	return JDR_OK;	/* Continue to decompress */
}

/*
** ���ز��� ��
**             1���ؼ�֡
**             0���ǹؼ�֡(��ͨ֡)
**            -1����֡����ȡ�������޷�������ȡ��
* 
* avilibʹ�òο���https://blog.csdn.net/hwd00001/article/details/127275498
**/
int YMCV_AVI_Decode_Next_Jpeg_Frame(avi_t* myavi, CVIMAGE outimg)
{
	cv_log_explain((myavi == NULL) || (outimg == NULL), CV_LOG_PtrI, "���ͼ���AVI���벻����");

	int frameSize = 0;      //��֡���ֽ���
	int keyFrame = 0;       //�Ƿ�ؼ�֡

	frameSize = AVI_read_frame(myavi, avi_frameBuffer, &keyFrame);
	//�Ѿ���ȡ����
	if (frameSize == -1)
		return -1;

	cv_log_explain(frameSize > YMCV_AVI_FRAME_SIZE_MAX, CV_LOG_ParamI, "AVI����ĵ�֡����Խ�磬������֡�������");

	//jpeg������������
	avi_jpegdatai = 0;

	int workBuffSize = 3100;//775 * 4 �������4�ֽڶ���
	//jpeg����
	JDEC* jpeg_dev = CV_malloc0(sizeof(JDEC)); //���������ṹ��ָ��  
	uint8* jpg_buffer = CV_malloc1(workBuffSize * sizeof(uint8)); //����jpeg���빤������С(������Ҫ3092�ֽ�)����Ϊ��ѹ������������4�ֽڶ���
	cv_log_explain(jpeg_dev == NULL, CV_LOG_Mem0, "������󴴽�ʧ��");
	cv_log_explain(jpg_buffer == NULL, CV_LOG_Mem1, "���뻺�洴��ʧ��");
	cv_log_explain(JD_FORMAT != 1, CV_LOG_ParamI, "TjpgDec�����ʽ����ΪRGB565");

	uint8 res = jd_prepare(jpeg_dev, avi_tjpgdec_in_func, jpg_buffer, workBuffSize, NULL);
	uint32 backi = avi_jpegdatai;
	cv_log_explain(res != JDR_OK, CV_LOG_ParamI, "����ʧ��,��Ƶ֡����MJPEG��ʽ");
	avi_imgOutBuff = outimg->data;//�������
	avi_imgOutWidth = outimg->width;
	avi_imgOutHeight = outimg->height;
	avi_imgOutType = outimg->type;
	/* Start JPEG decompression */
	uint8 rc = jd_decomp(jpeg_dev, avi_tjpgdec_output_func, 0);
	cv_log_explain(rc != JDR_OK, CV_LOG_ParamI, "TjpgDec����ʧ��");

	CV_free0(jpeg_dev);
	CV_free1(jpg_buffer);
	//�ؼ�֡��Ϣ����
	return keyFrame;
}

