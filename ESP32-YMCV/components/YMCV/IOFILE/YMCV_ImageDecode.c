#define _CV_STATIC static
#include "../CONFIG/YMCV_PubDefine.h"
#include "../CORE/YMCV_PixConv.h"
#include "../DEBUG/YMCV_Debug.h"
#include "../CONFIG/YMCV_Mem.h"
#include "../Code/tjpgd.h"
#include "../Code/bmp.h"
#include "./YMCV_IOFile.h"

/**
  ***************************************************************************************************************************
  *	@FileName:    YMCV_ImageDecode.c
  *	@Author:      yaomimaoren
  *	@Date:        2022-3-2
  *	@Description: 图像解码
  *	@Version:     1.0
  *
  ***************************************************************************************************************************
  *
  * 备注信息：
  * 1.支持格式：
  *   jpeg编码
  *   jpeg解码
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
	FILE* dev = (FILE*)jd->device;//待解码的文件的信息
	if (buff)     				//读取数据有效，开始读取数据
	{
		rb = YMCV_fread(buff,1, ndata, dev);//调用fread函数，用于把jpeg文件的数据读取出来
	}
	else
	{
		//重新定位文件指针，向后偏移num
		rb = (YMCV_fseek(dev, YMCV_ftell(dev) + ndata, SEEK_SET) == 0) ? ndata : 0;
	}
	return rb;//返回读取到的字节数目
}

static JRESULT tjpgdec_output_func(	/* 0:Ok, 1:Aborted */
	JDEC* jd,		/* Decompression object */
	void* bitmap,	/* Bitmap data to be output */
	JRECT* rect		/* Rectangular region to output */
)
{
	uint16 xstart = rect->left, ystart = rect->top;//输出图块的起始坐标
	uint16 width = rect->right - rect->left + 1;//输出图块的宽度
	uint16 height = rect->bottom - rect->top + 1;//输出图块的宽度
	uint16 xend = xstart + width;
	uint16 yend = ystart + height;
	uint16* src = bitmap;//rgb565

	if (tjpg_imgOutType == CVRgb16Type)
	{
		for (int yi = ystart, yimax = CVMin(tjpg_imgOutHeight, yend); yi < yimax; yi++)//y坐标
		{
			uint16* line0 = &tjpg_imgOutBuff[2 * (yi * tjpg_imgOutWidth + xstart)];//rgb565格式，一个数据两个byte
			//x坐标填充
			int xj = xstart, xjmax = CVMin(tjpg_imgOutWidth, xend);
			for (; xj < xjmax; xj++)//
			{
				*line0++ = *src++;//一个像素
			}
			//跨界区跳过
			for (; xj < xend; xj++)
				src++;
		}
	}
	else if (tjpg_imgOutType == CVGrayType)
	{
		for (int yi = ystart, yimax = CVMin(tjpg_imgOutHeight, yend); yi < yimax; yi++)//y坐标
		{
			CVgray* line0 = &tjpg_imgOutBuff[yi * tjpg_imgOutWidth + xstart];//rgb565格式，一个数据两个byte
			//x坐标填充
			int xj = xstart, xjmax = CVMin(tjpg_imgOutWidth, xend);
			for (; xj < xjmax; xj++, src++)
			{
				
				*line0++ = YMCV_Rgb565ToGray(*src);//一个像素
			}
			//跨界区跳过
			for (; xj < xend; xj++)
				src++;
		}
	}
	//未知格式
	else
	{
		return JDR_INTR;
	}
	return JDR_OK;	/* Continue to decompress */
}


/**
  * @brief 读取图片文件
  *		目前只支持jpg，bmp格式
  *		outType参数： rgb565或者gray
  */
CVIMAGE YMCV_Image_Read(const char* filepath, CVImgType outType)
{
	CVIMAGE myimg = NULL;

	cv_log_explain((outType != CVGrayType) && (outType != CVRgb16Type), CV_LOG_ParamI, "输出图像格式暂只支持rgb565或者灰度格式");

	char* chp = YMCV_Strrchr(filepath, '.'); 
	cv_log_explain(chp == NULL, CV_LOG_ParamI, "文件后缀获取失败");

	//大写转小写
	char file_type[6] = { 0 };
	for (int i = 0; (i < sizeof(file_type)) && ((*chp) != '\0'); i++, chp++)
	{
		file_type[i] = ((*chp) >= 'A' && (*chp) <= 'Z') ? (*chp) + 0x20 : (*chp);//大写字母转小写
	}
	//根据文件类型打开
	if (YMCV_Strcmp(file_type, ".jpg") == 0)//jpg文件
	{
		YMCV_FILE* f_jpeg = YMCV_fopen(filepath, "rb");//这里必须以二进制文件读取
		if (f_jpeg != NULL)
		{
			//TJpgDec至多需要3100字节用于JPEG图像。这完全取决于什么样的参数被用来创建JPEG图像。
			//3100字节是在默认输入缓存(JD_SZBUF == 512)下的最大内存需求，并随JD_SZBUF变化。
			//JD_SZBUF定义每次从输入流中读取多少字节。
			//TJpgDec对齐每个读请求缓冲区大小，512, 1024, 2048...字节是从存储设备读取的理想大小。
			int workBuffSize = 3100;//775 * 4 这里采用4字节对齐

			JDEC* jpeg_dev = CV_malloc0(sizeof(JDEC)); //待解码对象结构体指针  
			uint8* jpg_buffer = CV_malloc1(workBuffSize * sizeof(uint8)); //定义jpeg解码工作区大小(最少需要3092字节)，作为解压缓冲区，必须4字节对齐
			cv_log_explain(jpeg_dev == NULL, CV_LOG_Mem0, "解码对象创建失败");
			cv_log_explain(jpg_buffer == NULL, CV_LOG_Mem1, "解码缓存创建失败");
			cv_log_explain(JD_FORMAT != 1, CV_LOG_ParamI, "TjpgDec解码格式需设为RGB565");

			uint8 res = jd_prepare(jpeg_dev, tjpgdec_in_func, jpg_buffer, workBuffSize, f_jpeg);
			if (res == JDR_OK)//解码准备成功
			{
				uint8 scale = 0;
				uint16 outw = jpeg_dev->width >> scale;
				uint16 outh = jpeg_dev->height >> scale;
				myimg = YMCV_Creat_Img_Creat(outw, outh, outType);//创建输出图像
				tjpg_imgOutBuff = myimg->data;//数据输出
				tjpg_imgOutWidth = myimg->width;
				tjpg_imgOutHeight = myimg->height;
				tjpg_imgOutType = outType;
				/* Start JPEG decompression */
				uint8 rc = jd_decomp(jpeg_dev, tjpgdec_output_func, scale);
				cv_log_explain(rc != JDR_OK, CV_LOG_ParamI, "TjpgDec解码失败");
			}
			CV_free0(jpeg_dev);
			CV_free1(jpg_buffer);
		}
		else
		{
			cv_log_explain(f_jpeg == NULL, CV_LOG_ParamI, "文件打开失败");
		}
		
		YMCV_fclose(f_jpeg);
	}
	else if(YMCV_Strcmp(file_type, ".bmp") == 0)
	{
		// 实现参考 https://www.jb51.net/article/226189.htm
		YMCV_FILE* f_bmp = YMCV_fopen(filepath, "rb");//这里必须以二进制文件读取
		if (f_bmp != NULL)
		{
			//读取文件头
			YMCV_fread(&s_bmp_file_header.type, sizeof(uint16), 1, f_bmp);//由于4字节对齐，单独读取2字节
			YMCV_fread(&s_bmp_file_header.size, sizeof(s_bmp_file_header) - 4, 1, f_bmp);//由于4字节对齐，所以要少读取4字节
			cv_log_explain(s_bmp_file_header.type != 0x4d42, CV_LOG_ParamI, "文件不是BMP格式");

			//读取信息头
			YMCV_fread(&s_bmp_info_header, sizeof(s_bmp_info_header), 1, f_bmp);
			uint16 width = s_bmp_info_header.width;
			uint16 height = s_bmp_info_header.height;
			cv_log_explain(s_bmp_info_header.compression != 0, CV_LOG_ParamI, "暂不支持带压缩的BMP");

			myimg = YMCV_Creat_Img_Creat(width, height, outType);//创建输出图像
			
			//需要调色板处理
			if (s_bmp_info_header.bit_count < 16)
			{
				//申请失败
				cv_log_explain(1, CV_LOG_ParamI, "暂不支持带调色板图像（像素bit数<16）");
				//char temp[4 * 256] = { 0 };//调色板
				//fread(temp, 4 * 256, 1, file);
				//分别为 2*4 ，16*4 ，256*4个字节的调色板
			}
			//无需调色板
			else
			{
				if (s_bmp_info_header.bit_count == 24)
				{
					//字节数不是4的倍数，则以0补齐
					uint16 line_width = ((width + 3) / 4) * 4;
					pixel24_info_t* linebuff = CV_malloc0(line_width * sizeof(pixel24_info_t));
					cv_log_explain(linebuff == NULL, CV_LOG_Mem0, "解码缓存创建失败");
					//输出是rgb565
					if (outType == CVRgb16Type)
					{
						uint16* buf = myimg->data;
						for (int i = height - 1; i >= 0; i--)
						{
							YMCV_fread(linebuff, sizeof(pixel24_info_t), line_width, f_bmp);
							uint16* bufo = &buf[i * width];
							//有效部分输出
							for (int j = 0; j < width; j++)
							{
								//记录
								bufo[j] = YMCV_Rgb3ToRgb565(linebuff[j].r, linebuff[j].g, linebuff[j].b);
							}
						}
					}
					//灰度
					else if (outType == CVGrayType)
					{
						CVgray* buf = myimg->data;
						for (int i = height - 1; i >= 0; i--)
						{
							YMCV_fread(linebuff, sizeof(pixel24_info_t), line_width, f_bmp);
							CVgray* bufo = &buf[i * width];
							//有效部分输出
							for (int j = 0; j < width; j++)
							{
								//记录
								bufo[j] = YMCV_Rgb3ToGray(linebuff[j].r, linebuff[j].g, linebuff[j].b);
							}
						}
					}
					
					CV_free0(linebuff);
				}
				else
				{
					cv_log_explain(s_bmp_info_header.bit_count == 16, CV_LOG_ParamI, "该BMP像素为16位，待添加解码");
					cv_log_explain(s_bmp_info_header.bit_count == 32, CV_LOG_ParamI, "该BMP像素为32位，待添加解码");
				}
			}
		}
		else
		{
			cv_log_explain(f_bmp == NULL, CV_LOG_ParamI, "文件打开失败");
		}

		YMCV_fclose(f_bmp);
	}
	else
	{
		cv_log_explain(1, CV_LOG_ParamI, "暂不支持该格式解码");
	}

	return myimg;
}



//////////////////////////////////////////////////////////////////////// 以下为AVI解码添加的函数//////////////////////////////////////////////////////////


static char avi_frameBuffer[YMCV_AVI_FRAME_SIZE_MAX];//一张jpeg图片的缓存
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
	FILE* dev = (FILE*)jd->device;//待解码的文件的信息
	if (buff)     				//读取数据有效，开始读取数据
	{
		//把jpeg文件的数据读取出来
		for (int i = 0; i < ndata; i++)
		{
			buff[i] = avi_frameBuffer[avi_jpegdatai];
			avi_jpegdatai++;
		}
		rb = ndata;
	}
	else
	{
		//跳过数据
		avi_jpegdatai += ndata;
		rb = ndata;
	}
	return rb;//返回读取到的字节数目
}

static JRESULT avi_tjpgdec_output_func(	/* 0:Ok, 1:Aborted */
	JDEC* jd,		/* Decompression object */
	void* bitmap,	/* Bitmap data to be output */
	JRECT* rect		/* Rectangular region to output */
)
{
	uint16 xstart = rect->left, ystart = rect->top;//输出图块的起始坐标
	uint16 width = rect->right - rect->left + 1;//输出图块的宽度
	uint16 height = rect->bottom - rect->top + 1;//输出图块的宽度
	uint16 xend = xstart + width;
	uint16 yend = ystart + height;
	uint16* src = bitmap;//rgb565

	if (avi_imgOutType == CVRgb16Type)
	{
		for (int yi = ystart, yimax = CVMin(avi_imgOutHeight, yend); yi < yimax; yi++)//y坐标
		{
			uint16* line0 = &avi_imgOutBuff[2 * (yi * avi_imgOutWidth + xstart)];//rgb565格式，一个数据两个byte
			//x坐标填充
			int xj = xstart, xjmax = CVMin(avi_imgOutWidth, xend);
			for (; xj < xjmax; xj++)//
			{
				*line0++ = *src++;//一个像素
			}
			//跨界区跳过
			for (; xj < xend; xj++)
				src++;
		}
	}
	else if (avi_imgOutType == CVGrayType)
	{
		for (int yi = ystart, yimax = CVMin(avi_imgOutHeight, yend); yi < yimax; yi++)//y坐标
		{
			CVgray* line0 = &avi_imgOutBuff[yi * avi_imgOutWidth + xstart];//rgb565格式，一个数据两个byte
			//x坐标填充
			int xj = xstart, xjmax = CVMin(avi_imgOutWidth, xend);
			for (; xj < xjmax; xj++, src++)
			{

				*line0++ = YMCV_Rgb565ToGray(*src);//一个像素
			}
			//跨界区跳过
			for (; xj < xend; xj++)
				src++;
		}
	}
	//未知格式
	else
	{
		return JDR_INTR;
	}
	return JDR_OK;	/* Continue to decompress */
}


//加载外部jpeg数据数据
void YMCV_AVI_Decode_JpegBuff_LoadFromData(uint8* frameBuff, uint32 frameSize)
{
	cv_log_explain(frameSize > YMCV_AVI_FRAME_SIZE_MAX, CV_LOG_ParamI, "AVI解码的jpeg帧缓存越界，请增大帧缓存参数");
	//数据拷贝
	CV_memcpy(avi_frameBuffer,frameBuff,frameSize);
}

//从buff中解码数据
void YMCV_AVI_Decode_JpegBuff_ToImage(CVIMAGE outimg)
{
	//jpeg数据索引重置
	avi_jpegdatai = 0;

	int workBuffSize = 3100;//775 * 4 这里采用4字节对齐
	//jpeg解码
	JDEC* jpeg_dev = CV_malloc0(sizeof(JDEC)); //待解码对象结构体指针  
	uint8* jpg_buffer = CV_malloc1(workBuffSize * sizeof(uint8)); //定义jpeg解码工作区大小(最少需要3092字节)，作为解压缓冲区，必须4字节对齐
	cv_log_explain(jpeg_dev == NULL, CV_LOG_Mem0, "解码对象创建失败");
	cv_log_explain(jpg_buffer == NULL, CV_LOG_Mem1, "解码缓存创建失败");
	cv_log_explain(JD_FORMAT != 1, CV_LOG_ParamI, "TjpgDec解码格式需设为RGB565");

	uint8 res = jd_prepare(jpeg_dev, avi_tjpgdec_in_func, jpg_buffer, workBuffSize, NULL);
	uint32 backi = avi_jpegdatai;
	cv_log_explain(res != JDR_OK, CV_LOG_ParamI, "解码失败,视频帧不是MJPEG格式");
	avi_imgOutBuff = outimg->data;//数据输出
	avi_imgOutWidth = outimg->width;
	avi_imgOutHeight = outimg->height;
	avi_imgOutType = outimg->type;
	/* Start JPEG decompression */
	uint8 rc = jd_decomp(jpeg_dev, avi_tjpgdec_output_func, 0);
	cv_log_explain(rc != JDR_OK, CV_LOG_ParamI, "TjpgDec解码失败");

	CV_free0(jpeg_dev);
	CV_free1(jpg_buffer);
}
/*
** 返回参数 ：
**             1：关键帧
**             0：非关键帧(普通帧)
**            -1：非帧（读取结束，无法继续读取）
* 
* avilib使用参考：https://blog.csdn.net/hwd00001/article/details/127275498
**/
int YMCV_AVI_Decode_Next_Jpeg_Frame(avi_t* myavi, CVIMAGE outimg)
{
	cv_log_explain((myavi == NULL) || (outimg == NULL), CV_LOG_PtrI, "输出图像或AVI解码不存在");

	int frameSize = 0;      //单帧的字节数
	int keyFrame = 0;       //是否关键帧

	//读取数据到帧缓存中
	frameSize = AVI_read_frame(myavi, avi_frameBuffer, &keyFrame);
	//已经读取结束
	if (frameSize == -1)
		return -1;

	cv_log_explain(frameSize > YMCV_AVI_FRAME_SIZE_MAX, CV_LOG_ParamI, "AVI解码的jpeg帧缓存越界，请增大帧缓存参数");

	//解码帧缓存中的数据
	YMCV_AVI_Decode_JpegBuff_ToImage(outimg);

	//关键帧信息返回
	return keyFrame;
}

