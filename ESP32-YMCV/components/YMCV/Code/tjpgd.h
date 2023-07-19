/*----------------------------------------------------------------------------/
/ TJpgDec - Tiny JPEG Decompressor include file               (C)ChaN, 2012
/----------------------------------------------------------------------------*/
#ifndef _TJPGDEC_H
#define _TJPGDEC_H
/*---------------------------------------------------------------------------*/
/* System Configurations */
#define	JD_SZBUF			512	/* Size of stream input buffer */ //必须是512的倍数
#define JD_FORMAT			1		/* Output pixel format 0:RGB888 (3 uint8/pix), 1:RGB565 (1 uint16/pix) */
#define	JD_USE_SCALE		1		/* Use descaling feature for output */


/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_PubType.h"
#include "../DEBUG/YMCV_Debug.h"
	
	
/* Error code */
typedef enum {
	JDR_OK = 0,	/* 0: Succeeded */
	JDR_INTR,	/* 1: Interrupted by output function */	
	JDR_INP,	/* 2: Device error or wrong termination of input stream */
	JDR_MEM1,	/* 3: Insufficient memory pool for the image */
	JDR_MEM2,	/* 4: Insufficient stream input buffer */
	JDR_PAR,	/* 5: Parameter error */
	JDR_FMT1,	/* 6: Data format error (may be damaged data) */
	JDR_FMT2,	/* 7: Right format but not supported */
	JDR_FMT3	/* 8: Not supported JPEG standard */
} JRESULT;


/* Rectangular structure */
typedef struct {
	uint16 left, right, top, bottom;
} JRECT;


/* Decompressor object structure */
typedef struct JDEC JDEC;
struct JDEC {
	uint32 dctr;				/* Number of bytes available in the input buffer */
	uint8* dptr;				/* Current data read ptr */
	uint8* inbuf;			/* Bit stream input buffer */
	uint8 dmsk;				/* Current bit in the current read byte */
	uint8 scale;				/* Output scaling ratio */
	uint8 msx, msy;			/* MCU size in unit of block (width, height) */
	uint8 qtid[3];			/* Quantization table ID of each component */
	int16 dcv[3];			/* Previous DC element of each component */
	uint16 nrst;				/* Restart inverval */
	uint32 width, height;		/* Size of the input image (pixel) */
	uint8* huffbits[2][2];	/* Huffman bit distribution tables [id][dcac] */
	uint16* huffcode[2][2];	/* Huffman code word tables [id][dcac] */
	uint8* huffdata[2][2];	/* Huffman decoded data tables [id][dcac] */
	int32* qttbl[4];			/* Dequaitizer tables [id] */
	void* workbuf;			/* Working buffer for IDCT and RGB output */
	uint8* mcubuf;			/* Working buffer for the MCU */
	void* pool;				/* Pointer to available memory pool */
	uint32 sz_pool;			/* Size of momory pool (bytes available) */
	uint32 (*infunc)(JDEC*, uint8*, uint32);/* Pointer to jpeg stream input function */
	void* device;			/* Pointer to I/O device identifiler for the session */
};

/* TJpgDec API functions */
JRESULT jd_prepare(JDEC* jd,uint32(*infunc)(JDEC*, uint8*, uint32),	void* pool,	uint32 sz_pool,	void* dev);
JRESULT jd_decomp(JDEC* jd, JRESULT(*outfunc)(JDEC*, void*, JRECT*),uint8 scale);

#ifdef __cplusplus
}
#endif

#endif /* _TJPGDEC */


