/*----------------------------------------------------------------------------/
/ TJpgDec - Tiny JPEG Decompressor R0.01b                     (C)ChaN, 2012
/-----------------------------------------------------------------------------/
/ The TJpgDec is a generic JPEG decompressor module for tiny embedded systems.
/ This is a free software that opened for education, research and commercial
/  developments under license policy of following terms.
/
/  Copyright (C) 2012, ChaN, all right reserved.
/
/ * The TJpgDec module is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/-----------------------------------------------------------------------------/
/ Oct 04,'11 R0.01  First release.
/ Feb 19,'12 R0.01a Fixed decompression fails when scan starts with an escape seq.
/ Sep 03,'12 R0.01b Added JD_TBLCLIP option.
/----------------------------------------------------------------------------*/

#include "./tjpgd.h"

/*-----------------------------------------------*/
/* Zigzag-order to raster-order conversion table */
/*-----------------------------------------------*/
#define ZIG(n)	Zig[n]

static const uint8 Zig[64] = {	/* Zigzag-order to raster-order conversion table */
	 0,  1,  8, 16,  9,  2,  3, 10, 17, 24, 32, 25, 18, 11,  4,  5,
	12, 19, 26, 33, 40, 48, 41, 34, 27, 20, 13,  6,  7, 14, 21, 28,
	35, 42, 49, 56, 57, 50, 43, 36, 29, 22, 15, 23, 30, 37, 44, 51,
	58, 59, 52, 45, 38, 31, 39, 46, 53, 60, 61, 54, 47, 55, 62, 63
};



/*-------------------------------------------------*/
/* Input scale factor of Arai algorithm            */
/* (scaled up 16 bits for fixed point operations)  */
/*-------------------------------------------------*/
#define IPSF(n)	Ipsf[n]

static const uint16 Ipsf[64] = {	/* See also aa_idct.png */
	(uint16)(1.00000*8192), (uint16)(1.38704*8192), (uint16)(1.30656*8192), (uint16)(1.17588*8192), (uint16)(1.00000*8192), (uint16)(0.78570*8192), (uint16)(0.54120*8192), (uint16)(0.27590*8192),
	(uint16)(1.38704*8192), (uint16)(1.92388*8192), (uint16)(1.81226*8192), (uint16)(1.63099*8192), (uint16)(1.38704*8192), (uint16)(1.08979*8192), (uint16)(0.75066*8192), (uint16)(0.38268*8192),
	(uint16)(1.30656*8192), (uint16)(1.81226*8192), (uint16)(1.70711*8192), (uint16)(1.53636*8192), (uint16)(1.30656*8192), (uint16)(1.02656*8192), (uint16)(0.70711*8192), (uint16)(0.36048*8192),
	(uint16)(1.17588*8192), (uint16)(1.63099*8192), (uint16)(1.53636*8192), (uint16)(1.38268*8192), (uint16)(1.17588*8192), (uint16)(0.92388*8192), (uint16)(0.63638*8192), (uint16)(0.32442*8192),
	(uint16)(1.00000*8192), (uint16)(1.38704*8192), (uint16)(1.30656*8192), (uint16)(1.17588*8192), (uint16)(1.00000*8192), (uint16)(0.78570*8192), (uint16)(0.54120*8192), (uint16)(0.27590*8192),
	(uint16)(0.78570*8192), (uint16)(1.08979*8192), (uint16)(1.02656*8192), (uint16)(0.92388*8192), (uint16)(0.78570*8192), (uint16)(0.61732*8192), (uint16)(0.42522*8192), (uint16)(0.21677*8192),
	(uint16)(0.54120*8192), (uint16)(0.75066*8192), (uint16)(0.70711*8192), (uint16)(0.63638*8192), (uint16)(0.54120*8192), (uint16)(0.42522*8192), (uint16)(0.29290*8192), (uint16)(0.14932*8192),
	(uint16)(0.27590*8192), (uint16)(0.38268*8192), (uint16)(0.36048*8192), (uint16)(0.32442*8192), (uint16)(0.27590*8192), (uint16)(0.21678*8192), (uint16)(0.14932*8192), (uint16)(0.07612*8192)
};



/*---------------------------------------------*/
/* Conversion table for fast clipping process  */
/*---------------------------------------------*/
inline uint8 BYTECLIP (
	int val
)
{
	if (val < 0) val = 0;
	if (val > 255) val = 255;

	return (uint8)val;
}



/*-----------------------------------------------------------------------*/
/* Allocate a memory block from memory pool                              */
/*-----------------------------------------------------------------------*/
static void* alloc_pool (	/* Pointer to allocated memory block (NULL:no memory available) */
	JDEC* jd,		/* Pointer to the decompressor object */
	uint32 nd			/* Number of bytes to allocate */
)
{
	char *rp = NULL;

	nd = (nd + 3) & ~3;			/* Align block size to the word boundary */

	//�����ڴ渻��
	if (jd->sz_pool >= nd) {
		jd->sz_pool -= nd;
		rp = (char*)jd->pool;			/* Get start of available memory pool */
		jd->pool = (void*)(rp + nd);	/* Allocate requierd bytes */
	}

	cv_log_explain(rp == NULL, CV_LOG_ParamI, "���뻺��س��Ȳ���");

	return (void*)rp;	/* Return allocated memory block (NULL:no memory to allocate) */
}




/*-----------------------------------------------------------------------*/
/* Create de-quantization and prescaling tables with a DQT segment       */
/*-----------------------------------------------------------------------*/
static uint32 create_qt_tbl (	/* 0:OK, !0:Failed */
	JDEC* jd,			/* Pointer to the decompressor object */
	const uint8* data,	/* Pointer to the quantizer tables */
	uint32 ndata			/* Size of input data */
)
{
	uint32 i;
	uint8 d, z;
	int32*pb;


	while (ndata) {	/* Process all tables in the segment */
		if (ndata < 65) return JDR_FMT1;	/* Err: table size is unaligned */
		ndata -= 65;
		d = *data++;							/* Get table property */
		if (d & 0xF0) return JDR_FMT1;			/* Err: not 8-bit resolution */
		i = d & 3;								/* Get table ID */
		pb = alloc_pool(jd, 64 * sizeof (int32));/* Allocate a memory block for the table */
		if (!pb) return JDR_MEM1;				/* Err: not enough memory */
		jd->qttbl[i] = pb;						/* Register the table */
		for (i = 0; i < 64; i++) {				/* Load the table */
			z = ZIG(i);							/* Zigzag-order to raster-order conversion */
			pb[z] = (int32)((uint32)*data++ * IPSF(z));	/* Apply scale factor of Arai algorithm to the de-quantizers */
		}
	}

	return JDR_OK;
}




/*-----------------------------------------------------------------------*/
/* Create huffman code tables with a DHT segment                         */
/*-----------------------------------------------------------------------*/
static uint32 create_huffman_tbl (	/* 0:OK, !0:Failed */
	JDEC* jd,				/* Pointer to the decompressor object */
	const uint8* data,		/* Pointer to the packed huffman tables */
	uint32 ndata				/* Size of input data */
)
{
	uint32 i, j, b, np, cls, num;
	uint8 d, *pb, *pd;
	uint16 hc, *ph;

	while (ndata) {	/* Process all tables in the segment */
		if (ndata < 17) return JDR_FMT1;	/* Err: wrong data size */
		ndata -= 17;
		d = *data++;						/* Get table number and class */
		cls = (d >> 4); num = d & 0x0F;		/* class = dc(0)/ac(1), table number = 0/1 */
		if (d & 0xEE) return JDR_FMT1;		/* Err: invalid class/number */
		pb = alloc_pool(jd, 16);			/* Allocate a memory block for the bit distribution table */
		if (!pb) return JDR_MEM1;			/* Err: not enough memory */
		jd->huffbits[num][cls] = pb;
		for (np = i = 0; i < 16; i++) {		/* Load number of patterns for 1 to 16-bit code */
			pb[i] = b = *data++;
			np += b;	/* Get sum of code words for each code */
		}

		ph = alloc_pool(jd, np * sizeof (uint16));/* Allocate a memory block for the code word table */
		if (!ph) return JDR_MEM1;			/* Err: not enough memory */
		jd->huffcode[num][cls] = ph;
		hc = 0;
		for (j = i = 0; i < 16; i++) {		/* Re-build huffman code word table */
			b = pb[i];
			while (b--) ph[j++] = hc++;
			hc <<= 1;
		}

		if (ndata < np) return JDR_FMT1;	/* Err: wrong data size */
		ndata -= np;
		pd = alloc_pool(jd, np);			/* Allocate a memory block for the decoded data */
		if (!pd) return JDR_MEM1;			/* Err: not enough memory */
		jd->huffdata[num][cls] = pd;
		for (i = 0; i < np; i++) {			/* Load decoded data corresponds to each code ward */
			d = *data++;
			if (!cls && d > 11) return JDR_FMT1;
			*pd++ = d;
		}
	}

	return JDR_OK;
}


/*-----------------------------------------------------------------------*/
/* Extract N bits from input stream                                      */
/*-----------------------------------------------------------------------*/
static int32 bitext (	/* >=0: extracted data, <0: error code */
	JDEC* jd,	/* Pointer to the decompressor object */
	uint32 nbit	/* Number of bits to extract (1 to 11) */
)
{
	uint8 msk, s, *dp;
	uint32 dc, v, f;


	msk = jd->dmsk; dc = jd->dctr; dp = jd->dptr;	/* Bit mask, number of data available, read ptr */
	s = *dp; v = f = 0;
	do {
		if (!msk) {				/* Next byte? */
			if (!dc) {			/* No input data is available, re-fill input buffer */
				dp = jd->inbuf;	/* Top of input buffer */
				dc = jd->infunc(jd, dp, JD_SZBUF);
				if (!dc) return 0 - (int32)JDR_INP;	/* Err: read error or wrong stream termination */
			} else {
				dp++;			/* Next data ptr */
			}
			dc--;				/* Decrement number of available bytes */
			if (f) {			/* In flag sequence? */
				f = 0;			/* Exit flag sequence */
				if (*dp != 0) return 0 - (int32)JDR_FMT1;	/* Err: unexpected flag is detected (may be collapted data) */
				*dp = s = 0xFF;			/* The flag is a data 0xFF */
			} else {
				s = *dp;				/* Get next data byte */
				if (s == 0xFF) {		/* Is start of flag sequence? */
					f = 1; continue;	/* Enter flag sequence */
				}
			}
			msk = 0x80;		/* Read from MSB */
		}
		v <<= 1;	/* Get a bit */
		if (s & msk) v++;
		msk >>= 1;
		nbit--;
	} while (nbit);
	jd->dmsk = msk; jd->dctr = dc; jd->dptr = dp;

	return (int32)v;
}

/*-----------------------------------------------------------------------*/
/* Extract a huffman decoded data from input stream                      */
/*-----------------------------------------------------------------------*/
static int32 huffext (			/* >=0: decoded data, <0: error code */
	JDEC* jd,			/* Pointer to the decompressor object */
	const uint8* hbits,	/* Pointer to the bit distribution table */
	const uint16* hcode,	/* Pointer to the code word table */
	const uint8* hdata	/* Pointer to the data table */
)
{
	uint8 msk, s, *dp;
	uint32 dc, v, f, bl, nd;

	msk = jd->dmsk; 
	dc = jd->dctr; 
	dp = jd->dptr;	/* Bit mask, number of data available, read ptr */
	s = *dp; v = f = 0;
	bl = 16;	/* Max code length */
	do {
		if (!msk) {		/* Next byte? */
			if (!dc) {	/* No input data is available, re-fill input buffer */
				dp = jd->inbuf;	/* Top of input buffer */
				dc = jd->infunc(jd, dp, JD_SZBUF);
				if (!dc) return 0 - (int32)JDR_INP;	/* Err: read error or wrong stream termination */
			} else {
				dp++;	/* Next data ptr */
			}
			dc--;		/* Decrement number of available bytes */
			if (f) {		/* In flag sequence? */
				f = 0;		/* Exit flag sequence */
				if (*dp != 0)
					return 0 - (int32)JDR_FMT1;	/* Err: unexpected flag is detected (may be collapted data) */
				*dp = s = 0xFF;			/* The flag is a data 0xFF */
			} else {
				s = *dp;				/* Get next data byte */
				if (s == 0xFF) {		/* Is start of flag sequence? */
					f = 1; continue;	/* Enter flag sequence, get trailing byte */
				}
			}
			msk = 0x80;		/* Read from MSB */
		}
		v <<= 1;	/* Get a bit */
		if (s & msk) v++;
		msk >>= 1;

		for (nd = *hbits++; nd; nd--) {	/* Search the code word in this bit length */
			if (v == *hcode++) {		/* Matched? */
				jd->dmsk = msk; jd->dctr = dc; jd->dptr = dp;
				return *hdata;			/* Return the decoded data */
			}
			hdata++;
		}
		bl--;
	} while (bl);

	return 0 - (int32)JDR_FMT1;	/* Err: code not found (may be collapted data) */
}


/*-----------------------------------------------------------------------*/
/* Apply Inverse-DCT in Arai Algorithm (see also aa_idct.png)            */
/*-----------------------------------------------------------------------*/
static void block_idct (
	int32* src,	/* Input block data (de-quantized and pre-scaled for Arai Algorithm) */
	uint8* dst	/* Pointer to the destination to store the block as byte array */
)
{
	const int32 M13 = (int32)(1.41421*4096), M2 = (int32)(1.08239*4096), M4 = (int32)(2.61313*4096), M5 = (int32)(1.84776*4096);
	int32 v0, v1, v2, v3, v4, v5, v6, v7;
	int32 t10, t11, t12, t13;
	uint32 i;

	/* Process columns */
	for (i = 0; i < 8; i++) {
		v0 = src[8 * 0];	/* Get even elements */
		v1 = src[8 * 2];
		v2 = src[8 * 4];
		v3 = src[8 * 6];

		t10 = v0 + v2;		/* Process the even elements */
		t12 = v0 - v2;
		t11 = (v1 - v3) * M13 >> 12;
		v3 += v1;
		t11 -= v3;
		v0 = t10 + v3;
		v3 = t10 - v3;
		v1 = t11 + t12;
		v2 = t12 - t11;

		v4 = src[8 * 7];	/* Get odd elements */
		v5 = src[8 * 1];
		v6 = src[8 * 5];
		v7 = src[8 * 3];

		t10 = v5 - v4;		/* Process the odd elements */
		t11 = v5 + v4;
		t12 = v6 - v7;
		v7 += v6;
		v5 = (t11 - v7) * M13 >> 12;
		v7 += t11;
		t13 = (t10 + t12) * M5 >> 12;
		v4 = t13 - (t10 * M2 >> 12);
		v6 = t13 - (t12 * M4 >> 12) - v7;
		v5 -= v6;
		v4 -= v5;

		src[8 * 0] = v0 + v7;	/* Write-back transformed values */
		src[8 * 7] = v0 - v7;
		src[8 * 1] = v1 + v6;
		src[8 * 6] = v1 - v6;
		src[8 * 2] = v2 + v5;
		src[8 * 5] = v2 - v5;
		src[8 * 3] = v3 + v4;
		src[8 * 4] = v3 - v4;

		src++;	/* Next column */
	}

	/* Process rows */
	src -= 8;
	for (i = 0; i < 8; i++) {
		v0 = src[0] + (128L << 8);	/* Get even elements (remove DC offset (-128) here) */
		v1 = src[2];
		v2 = src[4];
		v3 = src[6];

		t10 = v0 + v2;				/* Process the even elements */
		t12 = v0 - v2;
		t11 = (v1 - v3) * M13 >> 12;
		v3 += v1;
		t11 -= v3;
		v0 = t10 + v3;
		v3 = t10 - v3;
		v1 = t11 + t12;
		v2 = t12 - t11;

		v4 = src[7];				/* Get odd elements */
		v5 = src[1];
		v6 = src[5];
		v7 = src[3];

		t10 = v5 - v4;				/* Process the odd elements */
		t11 = v5 + v4;
		t12 = v6 - v7;
		v7 += v6;
		v5 = (t11 - v7) * M13 >> 12;
		v7 += t11;
		t13 = (t10 + t12) * M5 >> 12;
		v4 = t13 - (t10 * M2 >> 12);
		v6 = t13 - (t12 * M4 >> 12) - v7;
		v5 -= v6;
		v4 -= v5;

		dst[0] = BYTECLIP((v0 + v7) >> 8);	/* Descale the transformed values 8 bits and output */
		dst[7] = BYTECLIP((v0 - v7) >> 8);
		dst[1] = BYTECLIP((v1 + v6) >> 8);
		dst[6] = BYTECLIP((v1 - v6) >> 8);
		dst[2] = BYTECLIP((v2 + v5) >> 8);
		dst[5] = BYTECLIP((v2 - v5) >> 8);
		dst[3] = BYTECLIP((v3 + v4) >> 8);
		dst[4] = BYTECLIP((v3 - v4) >> 8);
		dst += 8;

		src += 8;	/* Next row */
	}
}

/*-----------------------------------------------------------------------*/
/* Load all blocks in the MCU into working buffer                        */
/*-----------------------------------------------------------------------*/
static JRESULT mcu_load (
	JDEC* jd		/* Pointer to the decompressor object */
)
{
	int32 *tmp = (int32*)jd->workbuf;	/* Block working buffer for de-quantize and IDCT */
	uint32 blk, nby, nbc, i, z, id, cmp;
	int32 b, d, e;
	uint8 *bp;
	const uint8 *hb, *hd;
	const uint16 *hc;
	const int32 *dqf;


	nby = jd->msx * jd->msy;	/* Number of Y blocks (1, 2 or 4) */
	nbc = 2;					/* Number of C blocks (2) */
	bp = jd->mcubuf;			/* Pointer to the first block */

	for (blk = 0; blk < nby + nbc; blk++) {
		cmp = (blk < nby) ? 0 : blk - nby + 1;	/* Component number 0:Y, 1:Cb, 2:Cr */
		id = cmp ? 1 : 0;						/* Huffman table ID of the component */

		/* Extract a DC element from input stream */
		hb = jd->huffbits[id][0];				/* Huffman table for the DC element */
		hc = jd->huffcode[id][0];
		hd = jd->huffdata[id][0];
		b = huffext(jd, hb, hc, hd);			/* Extract a huffman coded data (bit length) */
		if (b < 0) return (JRESULT)(0 - b);		/* Err: invalid code or input */
		d = jd->dcv[cmp];						/* DC value of previous block */
		if (b) {								/* If there is any difference from previous block */
			e = bitext(jd, b);					/* Extract data bits */
			if (e < 0) return (JRESULT)(0 - e);	/* Err: input */
			b = 1 << (b - 1);					/* MSB position */
			if (!(e & b)) e -= (b << 1) - 1;	/* Restore sign if needed */
			d += e;								/* Get current value */
			jd->dcv[cmp] = (int16)d;			/* Save current DC value for next block */
		}
		dqf = jd->qttbl[jd->qtid[cmp]];			/* De-quantizer table ID for this component */
		tmp[0] = d * dqf[0] >> 8;				/* De-quantize, apply scale factor of Arai algorithm and descale 8 bits */

		/* Extract following 63 AC elements from input stream */
		for (i = 1; i < 64; i++) tmp[i] = 0;	/* Clear rest of elements */
		hb = jd->huffbits[id][1];				/* Huffman table for the AC elements */
		hc = jd->huffcode[id][1];
		hd = jd->huffdata[id][1];
		i = 1;					/* Top of the AC elements */
		do {
			b = huffext(jd, hb, hc, hd);		/* Extract a huffman coded value (zero runs and bit length) */
			if (b == 0) break;					/* EOB? */
			if (b < 0) return (JRESULT)(0 - b);	/* Err: invalid code or input error */
			z = (uint32)b >> 4;					/* Number of leading zero elements */
			if (z) {
				i += z;							/* Skip zero elements */
				if (i >= 64) return JDR_FMT1;	/* Too long zero run */
			}
			if (b &= 0x0F) {					/* Bit length */
				d = bitext(jd, b);				/* Extract data bits */
				if (d<0)return (JRESULT)(0-d);	/* Err: input device */
				b = 1 << (b - 1);				/* MSB position */
				if (!(d & b)) d -= (b << 1) - 1;/* Restore negative value if needed */
				z = ZIG(i);						/* Zigzag-order to raster-order converted index */
				tmp[z] = d * dqf[z] >> 8;		/* De-quantize, apply scale factor of Arai algorithm and descale 8 bits */
			}
		} while (++i < 64);		/* Next AC element */

		if (JD_USE_SCALE && jd->scale == 3)
			*bp = (*tmp / 256) + 128;	/* If scale ratio is 1/8, IDCT can be ommited and only DC element is used */
		else
			block_idct(tmp, bp);		/* Apply IDCT and store the block to the MCU buffer */

		bp += 64;				/* Next block */
	}

	return JDR_OK;	/* All blocks have been loaded successfully */
}




/*-----------------------------------------------------------------------*/
/* Output an MCU: Convert YCrCb to RGB and output it in RGB form         */
/*-----------------------------------------------------------------------*/
static JRESULT mcu_output (
	JDEC* jd,	/* Pointer to the decompressor object */
	JRESULT(*outfunc)(JDEC*, void*, JRECT*),	/* RGB output function */
	uint32 x,		/* MCU position in the image (left of the MCU) */
	uint32 y		/* MCU position in the image (top of the MCU) */
)
{
	const int32 CVACC = (sizeof (int32) > 2) ? 1024 : 128;
	uint32 ix, iy, mx, my, rx, ry;
	int32 yy, cb, cr;
	uint8 *py, *pc, *rgb24;
	JRECT rect;


	mx = jd->msx * 8; my = jd->msy * 8;					/* MCU size (pixel) */
	rx = (x + mx <= jd->width) ? mx : jd->width - x;	/* Output rectangular size (it may be clipped at right/bottom end) */
	ry = (y + my <= jd->height) ? my : jd->height - y;
	if (JD_USE_SCALE) {
		rx >>= jd->scale; ry >>= jd->scale;
		if (!rx || !ry) return JDR_OK;					/* Skip this MCU if all pixel is to be rounded off */
		x >>= jd->scale; y >>= jd->scale;
	}
	rect.left = x; rect.right = x + rx - 1;				/* Rectangular area in the frame buffer */
	rect.top = y; rect.bottom = y + ry - 1;


	if (!JD_USE_SCALE || jd->scale != 3) {	/* Not for 1/8 scaling */

		/* Build an RGB MCU from discrete comopnents */
		rgb24 = (uint8*)jd->workbuf;
		for (iy = 0; iy < my; iy++) {
			pc = jd->mcubuf;
			py = pc + iy * 8;
			if (my == 16) {		/* Double block height? */
				pc += 64 * 4 + (iy >> 1) * 8;
				if (iy >= 8) py += 64;
			} else {			/* Single block height */
				pc += mx * 8 + iy * 8;
			}
			for (ix = 0; ix < mx; ix++) {
				cb = pc[0] - 128; 	/* Get Cb/Cr component and restore right level */
				cr = pc[64] - 128;
				if (mx == 16) {					/* Double block width? */
					if (ix == 8) py += 64 - 8;	/* Jump to next block if double block heigt */
					pc += ix & 1;				/* Increase chroma pointer every two pixels */
				} else {						/* Single block width */
					pc++;						/* Increase chroma pointer every pixel */
				}
				yy = *py++;			/* Get Y component */

				/* Convert YCbCr to RGB */
				*rgb24++ = /* R */ BYTECLIP(yy + ((int32)(1.402 * CVACC) * cr) / CVACC);
				*rgb24++ = /* G */ BYTECLIP(yy - ((int32)(0.344 * CVACC) * cb + (int32)(0.714 * CVACC) * cr) / CVACC);
				*rgb24++ = /* B */ BYTECLIP(yy + ((int32)(1.772 * CVACC) * cb) / CVACC);
			}
		}

		/* Descale the MCU rectangular if needed */
		if (JD_USE_SCALE && jd->scale) {
			uint32 x, y, r, g, b, s, w, a;
			uint8 *op;

			/* Get averaged RGB value of each square correcponds to a pixel */
			s = jd->scale * 2;	/* Bumber of shifts for averaging */
			w = 1 << jd->scale;	/* Width of square */
			a = (mx - w) * 3;	/* Bytes to skip for next line in the square */
			op = (uint8*)jd->workbuf;
			for (iy = 0; iy < my; iy += w) {
				for (ix = 0; ix < mx; ix += w) {
					rgb24 = (uint8*)jd->workbuf + (iy * mx + ix) * 3;
					r = g = b = 0;
					for (y = 0; y < w; y++) {	/* Accumulate RGB value in the square */
						for (x = 0; x < w; x++) {
							r += *rgb24++;
							g += *rgb24++;
							b += *rgb24++;
						}
						rgb24 += a;
					}							/* Put the averaged RGB value as a pixel */
					*op++ = (uint8)(r >> s);
					*op++ = (uint8)(g >> s);
					*op++ = (uint8)(b >> s);
				}
			}
		}

	} else {	/* For only 1/8 scaling (left-top pixel in each block are the DC value of the block) */

		/* Build a 1/8 descaled RGB MCU from discrete comopnents */
		rgb24 = (uint8*)jd->workbuf;
		pc = jd->mcubuf + mx * my;
		cb = pc[0] - 128;		/* Get Cb/Cr component and restore right level */
		cr = pc[64] - 128;
		for (iy = 0; iy < my; iy += 8) {
			py = jd->mcubuf;
			if (iy == 8) py += 64 * 2;
			for (ix = 0; ix < mx; ix += 8) {
				yy = *py;	/* Get Y component */
				py += 64;

				/* Convert YCbCr to RGB */
				*rgb24++ = /* R */ BYTECLIP(yy + ((int32)(1.402 * CVACC) * cr / CVACC));
				*rgb24++ = /* G */ BYTECLIP(yy - ((int32)(0.344 * CVACC) * cb + (int32)(0.714 * CVACC) * cr) / CVACC);
				*rgb24++ = /* B */ BYTECLIP(yy + ((int32)(1.772 * CVACC) * cb / CVACC));
			}
		}
	}

	/* Squeeze up pixel table if a part of MCU is to be truncated */
	mx >>= jd->scale;
	if (rx < mx) {
		uint8 *s, *d;
		uint32 x, y;

		s = d = (uint8*)jd->workbuf;
		for (y = 0; y < ry; y++) {
			for (x = 0; x < rx; x++) {	/* Copy effective pixels */
				*d++ = *s++;
				*d++ = *s++;
				*d++ = *s++;
			}
			s += (mx - rx) * 3;	/* Skip truncated pixels */
		}
	}

	/* Convert RGB888 to RGB565 if needed */
	if (JD_FORMAT == 1) {
		uint8 *s = (uint8*)jd->workbuf;
		uint16 w, *d = (uint16*)s;
		uint32 n = rx * ry;

		do {
			w = (*s++ & 0xF8) << 8;		/* RRRRR----------- */
			w |= (*s++ & 0xFC) << 3;	/* -----GGGGGG----- */
			w |= *s++ >> 3;				/* -----------BBBBB */
			*d++ = w;
		} while (--n);
	}

	/* Output the RGB rectangular */
	return (JRESULT)outfunc(jd, jd->workbuf,&rect); //ȥ����Ŀ����,��ʡʱ��
}

/*-----------------------------------------------------------------------*/
/* Process restart interval                                              */
/*-----------------------------------------------------------------------*/
static JRESULT restart (
	JDEC* jd,	/* Pointer to the decompressor object */
	uint16 rstn	/* Expected restert sequense number */
)
{
	uint32 i, dc;
	uint16 d;
	uint8 *dp;


	/* Discard padding bits and get two bytes from the input stream */
	dp = jd->dptr; dc = jd->dctr;
	d = 0;
	for (i = 0; i < 2; i++) {
		if (!dc) {	/* No input data is available, re-fill input buffer */
			dp = jd->inbuf;
			dc = jd->infunc(jd, dp, JD_SZBUF);
			if (!dc) return JDR_INP;
		} else {
			dp++;
		}
		dc--;
		d = (d << 8) | *dp;	/* Get a byte */
	}
	jd->dptr = dp; jd->dctr = dc; jd->dmsk = 0;

	/* Check the marker */
	if ((d & 0xFFD8) != 0xFFD0 || (d & 7) != (rstn & 7))
		return JDR_FMT1;	/* Err: expected RSTn marker is not detected (may be collapted data) */

	/* Reset DC offset */
	jd->dcv[2] = jd->dcv[1] = jd->dcv[0] = 0;

	return JDR_OK;
}

/*-----------------------------------------------------------------------*/
/* Analyze the JPEG image and Initialize decompressor object             */
/*-----------------------------------------------------------------------*/

#define	LDB_WORD(ptr)		(uint16)(((uint16)*((uint8*)(ptr))<<8)|(uint16)*(uint8*)((ptr)+1))

//����׼������
JRESULT jd_prepare (
	JDEC* jd,			/* Blank decompressor object */
	uint32 (*infunc)(JDEC*, uint8*, uint32),	/* JPEG strem input function */
	void* pool,			/* Working buffer for the decompression session */
	uint32 sz_pool,		/* Size of working buffer */
	void* dev			/* I/O device identifier for the session */
)
{
	uint8 *seg, b;
	uint16 marker;
	uint32 ofs;//ԭ������unsiged long
	uint32 n, i, j, len;
	JRESULT rc;



	cv_log_explain(pool == NULL, CV_LOG_PtrI, "���뻺���pool������");
	if (!pool) return JDR_PAR;

	jd->pool = pool;		/* Work memroy */
	jd->sz_pool = sz_pool;	/* Size of given work memory */
	jd->infunc = infunc;	/* Stream input function */
	jd->device = dev;		/* I/O device identifier */
	jd->nrst = 0;			/* No restart interval (default) */

	for (i = 0; i < 2; i++) {	/* Nulls pointers */
		for (j = 0; j < 2; j++) {
			jd->huffbits[i][j] = 0;
			jd->huffcode[i][j] = 0;
			jd->huffdata[i][j] = 0;
		}
	}
	for (i = 0; i < 4; i++) jd->qttbl[i] = 0;

	jd->inbuf = seg = alloc_pool(jd, JD_SZBUF);		/* Allocate stream input buffer */
	if (!seg) return JDR_MEM1;

	if (jd->infunc(jd, seg, 2) != 2) return JDR_INP;/* Check SOI marker */
	if (LDB_WORD(seg) != 0xFFD8) return JDR_FMT1;	/* Err: SOI is not detected */
	ofs = 2;

	for (;;) {
		/* Get a JPEG marker */
		if (jd->infunc(jd, seg, 4) != 4) return JDR_INP;
		marker = LDB_WORD(seg);		/* Marker */
		len = LDB_WORD(seg + 2);	/* Length field */
		if (len <= 2 || (marker >> 8) != 0xFF) return JDR_FMT1;
		len -= 2;		/* Content size excluding length field */
		ofs += 4 + len;	/* Number of bytes loaded */

		switch (marker & 0xFF) {
		case 0xC0:	/* SOF0 (baseline JPEG) */
			/* Load segment data */
			if (len > JD_SZBUF) return JDR_MEM2;
			if (jd->infunc(jd, seg, len) != len) return JDR_INP;

			jd->width = LDB_WORD(seg+3);		/* Image width in unit of pixel */
			jd->height = LDB_WORD(seg+1);		/* Image height in unit of pixel */
			if (seg[5] != 3) return JDR_FMT3;	/* Err: Supports only Y/Cb/Cr format */

			/* Check three image components */
			for (i = 0; i < 3; i++) {	
				b = seg[7 + 3 * i];							/* Get sampling factor */
				if (!i) {	/* Y component */
					if (b != 0x11 && b != 0x22 && b != 0x21)/* Check sampling factor */
						return JDR_FMT3;					/* Err: Supports only 4:4:4, 4:2:0 or 4:2:2 */
					jd->msx = b >> 4; jd->msy = b & 15;		/* Size of MCU [blocks] */
				} else {	/* Cb/Cr component */
					if (b != 0x11) return JDR_FMT3;			/* Err: Sampling factor of Cr/Cb must be 1 */
				}
				b = seg[8 + 3 * i];							/* Get dequantizer table ID for this component */
				if (b > 3) return JDR_FMT3;					/* Err: Invalid ID */
				jd->qtid[i] = b;
			}
			break;

		case 0xDD:	/* DRI */
			/* Load segment data */
			if (len > JD_SZBUF) return JDR_MEM2;
			if (jd->infunc(jd, seg, len) != len) return JDR_INP;

			/* Get restart interval (MCUs) */
			jd->nrst = LDB_WORD(seg);
			break;

		case 0xC4:	/* DHT */
			/* Load segment data */
			if (len > JD_SZBUF) return JDR_MEM2;
			if (jd->infunc(jd, seg, len) != len) return JDR_INP;

			/* Create huffman tables */
			rc = (JRESULT)create_huffman_tbl(jd, seg, len);
			if (rc) return rc;
			break;

		case 0xDB:	/* DQT */
			/* Load segment data */
			if (len > JD_SZBUF) return JDR_MEM2;
			if (jd->infunc(jd, seg, len) != len) return JDR_INP;

			/* Create de-quantizer tables */
			rc = (JRESULT)create_qt_tbl(jd, seg, len);
			if (rc) return rc;
			break;

		case 0xDA:	/* SOS */
			/* Load segment data */
			if (len > JD_SZBUF) return JDR_MEM2;
			if (jd->infunc(jd, seg, len) != len) return JDR_INP;

			if (!jd->width || !jd->height) return JDR_FMT1;	/* Err: Invalid image size */

			if (seg[0] != 3) return JDR_FMT3;				/* Err: Supports only three color components format */

			/* Check if all tables corresponding to each components have been loaded */
			for (i = 0; i < 3; i++) {
				b = seg[2 + 2 * i];	/* Get huffman table ID */
				if (b != 0x00 && b != 0x11)	return JDR_FMT3;	/* Err: Different table number for DC/AC element */
				b = i ? 1 : 0;
				if (!jd->huffbits[b][0] || !jd->huffbits[b][1])	/* Check huffman table for this component */
					return JDR_FMT1;							/* Err: Huffman table not loaded */
				if (!jd->qttbl[jd->qtid[i]]) return JDR_FMT1;	/* Err: Dequantizer table not loaded */
			}

			/* Allocate working buffer for MCU and RGB */
			n = jd->msy * jd->msx;						/* Number of Y blocks in the MCU */
			if (!n) return JDR_FMT1;					/* Err: SOF0 has not been loaded */
			len = n * 64 * 2 + 64;						/* Allocate buffer for IDCT and RGB output */
			if (len < 256) len = 256;					/* but at least 256 byte is required for IDCT */
			jd->workbuf = alloc_pool(jd, len);			/* and it may occupy a part of following MCU working buffer for RGB output */
			if (!jd->workbuf) return JDR_MEM1;			/* Err: not enough memory */
			jd->mcubuf = alloc_pool(jd, (n + 2) * 64);	/* Allocate MCU working buffer */
			if (!jd->mcubuf) return JDR_MEM1;			/* Err: not enough memory */

			/* Pre-load the JPEG data to extract it from the bit stream */
			jd->dptr = seg; jd->dctr = 0; jd->dmsk = 0;	/* Prepare to read bit stream */
			if (ofs %= JD_SZBUF) {						/* Align read offset to JD_SZBUF */
				jd->dctr = jd->infunc(jd, seg + ofs, JD_SZBUF - (uint32)ofs);
				jd->dptr = seg + ofs - 1;
			}

			return JDR_OK;		/* Initialization succeeded. Ready to decompress the JPEG image. */

		case 0xC1:	/* SOF1 */
		case 0xC2:	/* SOF2 */
		case 0xC3:	/* SOF3 */
		case 0xC5:	/* SOF5 */
		case 0xC6:	/* SOF6 */
		case 0xC7:	/* SOF7 */
		case 0xC9:	/* SOF9 */
		case 0xCA:	/* SOF10 */
		case 0xCB:	/* SOF11 */
		case 0xCD:	/* SOF13 */
		case 0xCE:	/* SOF14 */
		case 0xCF:	/* SOF15 */
		case 0xD9:	/* EOI */
			return JDR_FMT3;	/* Unsuppoted JPEG standard (may be progressive JPEG) */

		default:	/* Unknown segment (comment, exif or etc..) */
			/* Skip segment data */
			if (jd->infunc(jd, 0, len) != len)	/* Null pointer specifies to skip bytes of stream */
				return JDR_INP;
		}
	}
}

/*-----------------------------------------------------------------------*/
/* Start to decompress the JPEG picture                                  */
/*-----------------------------------------------------------------------*/
JRESULT jd_decomp (
	JDEC* jd,								/* Initialized decompression object */
	JRESULT(*outfunc)(JDEC*, void*, JRECT*),	/* RGB output function */
	uint8 scale								/* Output de-scaling factor (0 to 3) */
)
{
	uint32 x, y, mx, my;
	uint16 rst, rsc;
	JRESULT rc;


	if (scale > (JD_USE_SCALE ? 3 : 0)) return JDR_PAR;
	jd->scale = scale;

	mx = jd->msx * 8; my = jd->msy * 8;			/* Size of the MCU (pixel) */

	jd->dcv[2] = jd->dcv[1] = jd->dcv[0] = 0;	/* Initialize DC values */
	rst = rsc = 0;

	rc = JDR_OK;
	for (y = 0; y < jd->height; y += my) {		/* Vertical loop of MCUs */
		for (x = 0; x < jd->width; x += mx) {	/* Horizontal loop of MCUs */
			if (jd->nrst && rst++ == jd->nrst) {	/* Process restart interval if enabled */
				rc = restart(jd, rsc++);
				if (rc != JDR_OK) return rc;
				rst = 1;
			}
			rc = mcu_load(jd);					/* Load an MCU (decompress huffman coded stream and apply IDCT) */
			if (rc != JDR_OK) return rc;
			rc = mcu_output(jd, outfunc, x, y);	/* Output the MCU (color space conversion, scaling and output) */
			if (rc != JDR_OK) return rc;
		}
	}

	return rc;
}

