#include "gifdec.h"
/**
  ***************************************************************************************************************************
  *	@FileName:    gifdec.c
  *	@Author:      yaomimaoren
  *	@Date:        2022-10-2
  *	@Description: 带全局颜色表的GIF89a解码器
  *	@Version:     1.0
  *
  ***************************************************************************************************************************
  *
  * 备注信息：Based on https ://github.com/lecram/gifdec
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

#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define gif_false 0
#define gif_true 1

typedef struct Entry {
    uint16 length;
    uint16 prefix;
    uint8  suffix;
} Entry;

typedef struct Table {
    int32 bulk;
    int32 nentries;
    Entry *entries;
} Table;

static gd_GIF *  gif_open(gd_GIF * gif);
static uint8 f_gif_open(gd_GIF * gif, const void * path, uint8 is_file);
static void f_gif_read(gd_GIF * gif, void * buf, size_t len);
static int32 f_gif_seek(gd_GIF * gif, size_t pos, int32 k);
static void f_gif_close(gd_GIF * gif);

static uint16 read_num(gd_GIF * gif)
{
    uint8 bytes[2];

    f_gif_read(gif, bytes, 2);
    return bytes[0] + (((uint16) bytes[1]) << 8);
}


static gd_GIF * gif_open(gd_GIF * gif_base)
{
    uint8 sigver[3];
    uint16 width, height, depth;
    uint8 fdsz, bgidx, aspect;
    int32 i;
    uint8 *bgcolor;
    int32 gct_sz;
    gd_GIF* gif = NULL;
    
    /* Header */
    f_gif_read(gif_base, sigver, 3);
    if (YMCV_Memcmp(sigver, "GIF", 3) != 0) {
        //invalid signature
        cv_log_explain(1, CV_LOG_ParamI, "不是GIF文件");
        goto fail;
    }
    /* Version */
    f_gif_read(gif_base, sigver, 3);
    if (YMCV_Memcmp(sigver, "89a", 3) != 0) {
        //invalid version
        cv_log_explain(1, CV_LOG_ParamI, "暂只支持GIF89a版本");
        goto fail;
    }
    /* Width x Height */
    width  = read_num(gif_base);
    height = read_num(gif_base);
    /* FDSZ */
    f_gif_read(gif_base, &fdsz, 1);
    /* Presence of GCT */
    if (!(fdsz & 0x80)) {
        //no global color table
        cv_log_explain(1, CV_LOG_ParamI, "暂只支持带全局颜色表的GIF");
        goto fail;
    }
    /* Color Space's Depth */
    depth = ((fdsz >> 4) & 7) + 1;
    /* Ignore Sort Flag. */
    /* GCT Size */
    gct_sz = 1 << ((fdsz & 0x07) + 1);
    /* Background Color Index */
    f_gif_read(gif_base, &bgidx, 1);
    /* Aspect Ratio */
    f_gif_read(gif_base, &aspect, 1);
    
    /* Create gd_GIF Structure. */
#if GIF_COLOR_OUT_TYPE == GIF_COLOR_RGB888
    gif = CV_malloc1(sizeof(gd_GIF) + 4 * width * height);
#elif GIF_COLOR_OUT_TYPE == GIF_COLOR_RGB565
    gif = CV_malloc1(sizeof(gd_GIF) + 3 * width * height);
#elif GIF_COLOR_OUT_TYPE == GIF_COLOR_GRAY
    gif = CV_malloc1(sizeof(gd_GIF) + 2 * width * height);
#endif

    if (!gif) goto fail;
    CV_memcpy(gif, gif_base, sizeof(gd_GIF));
    gif->width  = width;
    gif->height = height;
    gif->depth  = depth;
    /* Read GCT */
    gif->gct.size = gct_sz;
    f_gif_read(gif, gif->gct.colors, 3 * gif->gct.size);
    gif->palette = &gif->gct;
    gif->bgindex = bgidx;
    gif->canvas = (uint8 *) &gif[1];
#if GIF_COLOR_OUT_TYPE == GIF_COLOR_RGB888
    gif->frame = &gif->canvas[3 * width * height];
#elif GIF_COLOR_OUT_TYPE == GIF_COLOR_RGB565
    uint16* rgb565p = gif->canvas;
    gif->frame = &gif->canvas[2 * width * height];
#elif GIF_COLOR_OUT_TYPE == GIF_COLOR_GRAY
    gif->frame = &gif->canvas[1 * width * height];
#endif
    if (gif->bgindex)
        CV_memset(gif->frame, gif->bgindex, gif->width * gif->height);
    bgcolor = &gif->palette->colors[gif->bgindex * 3];

    if (bgcolor[0] || bgcolor[1] || bgcolor [2])
        for (i = 0; i < gif->width * gif->height; i++) {
#if GIF_COLOR_OUT_TYPE == GIF_COLOR_RGB888
            gif->canvas[i * 3 + 0] = bgcolor[0];//r
            gif->canvas[i * 3 + 1] = bgcolor[1];//g
            gif->canvas[i * 3 + 2] = bgcolor[2];//b
#elif GIF_COLOR_OUT_TYPE == GIF_COLOR_RGB565
            //lv_color_t c = lv_color_make(*(bgcolor + 0), *(bgcolor + 1), *(bgcolor + 2));
            // 注释部分可能存在大小端问题
            //gif->canvas[i * 2 + 0] = ((bgcolor[1] << 3) & 0xE0) + ((bgcolor[2] >> 3) & 0x1F);
            //gif->canvas[i * 2 + 1] = (bgcolor[0] & 0xF8) + ((bgcolor[1] >> 5) & 0x07);
            rgb565p[i] = ((bgcolor[0] << 8) & 0xF800) + ((bgcolor[1] << 3) & 0x7E0) + ((bgcolor[2] >> 3) & 0x001F);
#elif GIF_COLOR_OUT_TYPE == GIF_COLOR_GRAY
            //lv_color_t c = lv_color_make(*(bgcolor + 0), *(bgcolor + 1), *(bgcolor + 2));
            gif->canvas[i] = ((bgcolor[0] * 76 + bgcolor[1] * 150 + bgcolor[2] * 30) >> 8);
#endif
        }
    gif->anim_start = f_gif_seek(gif, 0, SEEK_CUR);
    goto ok;
fail:
    f_gif_close(gif_base);
ok:
    return gif;
}

static void discard_sub_blocks(gd_GIF *gif)
{
    uint8 size;

    do {
        f_gif_read(gif, &size, 1);
        f_gif_seek(gif, size, SEEK_CUR);
    } while (size);
}

static void read_plain_text_ext(gd_GIF *gif)
{
    if (gif->plain_text) {
        uint16 tx, ty, tw, th;
        uint8 cw, ch, fg, bg;
        size_t sub_block;
        f_gif_seek(gif, 1, SEEK_CUR); /* block size = 12 */
        tx = read_num(gif);
        ty = read_num(gif);
        tw = read_num(gif);
        th = read_num(gif);
        f_gif_read(gif, &cw, 1);
        f_gif_read(gif, &ch, 1);
        f_gif_read(gif, &fg, 1);
        f_gif_read(gif, &bg, 1);
        sub_block = f_gif_seek(gif, 0, SEEK_CUR);
        gif->plain_text(gif, tx, ty, tw, th, cw, ch, fg, bg);
        f_gif_seek(gif, sub_block, SEEK_SET);
    } else {
        /* Discard plain text metadata. */
        f_gif_seek(gif, 13, SEEK_CUR);
    }
    /* Discard plain text sub-blocks. */
    discard_sub_blocks(gif);
}

static void read_graphic_control_ext(gd_GIF *gif)
{
    uint8 rdit;

    /* Discard block size (always 0x04). */
    f_gif_seek(gif, 1, SEEK_CUR);
    f_gif_read(gif, &rdit, 1);
    gif->gce.disposal = (rdit >> 2) & 3;
    gif->gce.input = rdit & 2;
    gif->gce.transparency = rdit & 1;
    gif->gce.delay = read_num(gif);
    f_gif_read(gif, &gif->gce.tindex, 1);
    /* Skip block terminator. */
    f_gif_seek(gif, 1, SEEK_CUR);
}

static void read_comment_ext(gd_GIF *gif)
{
    if (gif->comment) {
        size_t sub_block = f_gif_seek(gif, 0, SEEK_CUR);
        gif->comment(gif);
        f_gif_seek(gif, sub_block, SEEK_SET);
    }
    /* Discard comment sub-blocks. */
    discard_sub_blocks(gif);
}

static void read_application_ext(gd_GIF *gif)
{
    char app_id[8];
    char app_auth_code[3];

    /* Discard block size (always 0x0B). */
    f_gif_seek(gif, 1, SEEK_CUR);
    /* Application Identifier. */
    f_gif_read(gif, app_id, 8);
    /* Application Authentication Code. */
    f_gif_read(gif, app_auth_code, 3);
    if (!strncmp(app_id, "NETSCAPE", sizeof(app_id))) {
        /* Discard block size (0x03) and constant byte (0x01). */
        f_gif_seek(gif, 2, SEEK_CUR);
        gif->loop_count = read_num(gif);
        /* Skip block terminator. */
        f_gif_seek(gif, 1, SEEK_CUR);
    } else if (gif->application) {
        size_t sub_block = f_gif_seek(gif, 0, SEEK_CUR);
        gif->application(gif, app_id, app_auth_code);
        f_gif_seek(gif, sub_block, SEEK_SET);
        discard_sub_blocks(gif);
    } else {
        discard_sub_blocks(gif);
    }
}

static void read_ext(gd_GIF *gif)
{
    uint8 label;

    f_gif_read(gif, &label, 1);
    switch (label) {
    case 0x01:
        read_plain_text_ext(gif);
        break;
    case 0xF9:
        read_graphic_control_ext(gif);
        break;
    case 0xFE:
        read_comment_ext(gif);
        break;
    case 0xFF:
        read_application_ext(gif);
        break;
    default:
        //unknown extension: label
        cv_log_explain(1, CV_LOG_ParamI, "读取到未知的扩展");
    }
}

static Table *new_table(int32 key_size)
{
    int32 key;
    int32 init_bulk = MAX(1 << (key_size + 1), 0x100);
    Table *table = CV_malloc1(sizeof(*table) + sizeof(Entry) * init_bulk);
    if (table) {
        table->bulk = init_bulk;
        table->nentries = (1 << key_size) + 2;
        table->entries = (Entry *) &table[1];
        for (key = 0; key < (1 << key_size); key++)
            table->entries[key] = (Entry) {1, 0xFFF, key};
    }
    return table;
}

/* Add table entry. Return value:
 *  0 on success
 *  +1 if key size must be incremented after this addition
 *  -1 if could not realloc table */
static int32 add_entry(Table **tablep, uint16 length, uint16 prefix, uint8 suffix)
{
    Table *table = *tablep;
    if (table->nentries == table->bulk) {
        table->bulk *= 2;
        table = realloc(table, sizeof(*table) + sizeof(Entry) * table->bulk);
        if (!table) return -1;
        table->entries = (Entry *) &table[1];
        *tablep = table;
    }
    table->entries[table->nentries] = (Entry) {length, prefix, suffix};
    table->nentries++;
    if ((table->nentries & (table->nentries - 1)) == 0)
        return 1;
    return 0;
}

static uint16 get_key(gd_GIF *gif, int32 key_size, uint8 *sub_len, uint8 *shift, uint8 *byte)
{
    int32 bits_read;
    int32 rpad;
    int32 frag_size;
    uint16 key;

    key = 0;
    for (bits_read = 0; bits_read < key_size; bits_read += frag_size) {
        rpad = (*shift + bits_read) % 8;
        if (rpad == 0) {
            /* Update byte. */
            if (*sub_len == 0) {
                f_gif_read(gif, sub_len, 1); /* Must be nonzero! */
                if (*sub_len == 0) return 0x1000;
            }
            f_gif_read(gif, byte, 1);
            (*sub_len)--;
        }
        frag_size = MIN(key_size - bits_read, 8 - rpad);
        key |= ((uint16) ((*byte) >> rpad)) << bits_read;
    }
    /* Clear extra bits to the left. */
    key &= (1 << key_size) - 1;
    *shift = (*shift + key_size) % 8;
    return key;
}

/* Compute output index of y-th input line, in frame of height h. */
static int32 interlaced_line_index(int32 h, int32 y)
{
    int32 p; /* number of lines in current pass */

    p = (h - 1) / 8 + 1;
    if (y < p) /* pass 1 */
        return y * 8;
    y -= p;
    p = (h - 5) / 8 + 1;
    if (y < p) /* pass 2 */
        return y * 8 + 4;
    y -= p;
    p = (h - 3) / 4 + 1;
    if (y < p) /* pass 3 */
        return y * 4 + 2;
    y -= p;
    /* pass 4 */
    return y * 2 + 1;
}

/* Decompress image pixels.
 * Return 0 on success or -1 on out-of-memory (w.r.t. LZW code table). */
static int32 read_image_data(gd_GIF *gif, int32 interlace)
{
    uint8 sub_len, shift, byte;
    int32 init_key_size, key_size, table_is_full;
    int32 frm_off, frm_size, str_len, i, p, x, y;
    uint16 key, clear, stop;
    int32 ret;
    Table *table;
    Entry entry;
    size_t start, end;

    f_gif_read(gif, &byte, 1);
    key_size = (int32) byte;
    start = f_gif_seek(gif, 0, SEEK_CUR);
    discard_sub_blocks(gif);
    end = f_gif_seek(gif, 0, SEEK_CUR);
    f_gif_seek(gif, start, SEEK_SET);
    clear = 1 << key_size;
    stop = clear + 1;
    table = new_table(key_size);
    key_size++;
    init_key_size = key_size;
    sub_len = shift = 0;
    key = get_key(gif, key_size, &sub_len, &shift, &byte); /* clear code */
    frm_off = 0;
    ret = 0;
    frm_size = gif->fw*gif->fh;
    while (frm_off < frm_size) {
        if (key == clear) {
            key_size = init_key_size;
            table->nentries = (1 << (key_size - 1)) + 2;
            table_is_full = 0;
        } else if (!table_is_full) {
            ret = add_entry(&table, str_len + 1, key, entry.suffix);
            if (ret == -1) {
                CV_free1(table);
                return -1;
            }
            if (table->nentries == 0x1000) {
                ret = 0;
                table_is_full = 1;
            }
        }
        key = get_key(gif, key_size, &sub_len, &shift, &byte);
        if (key == clear) continue;
        if (key == stop || key == 0x1000) break;
        if (ret == 1) key_size++;
        entry = table->entries[key];
        str_len = entry.length;
        for (i = 0; i < str_len; i++) {
            p = frm_off + entry.length - 1;
            x = p % gif->fw;
            y = p / gif->fw;
            if (interlace)
                y = interlaced_line_index((int32) gif->fh, y);
            gif->frame[(gif->fy + y) * gif->width + gif->fx + x] = entry.suffix;
            if (entry.prefix == 0xFFF)
                break;
            else
                entry = table->entries[entry.prefix];
        }
        frm_off += str_len;
        if (key < table->nentries - 1 && !table_is_full)
            table->entries[table->nentries - 1].suffix = entry.suffix;
    }
    CV_free1(table);
    if (key == stop) f_gif_read(gif, &sub_len, 1); /* Must be zero! */
    f_gif_seek(gif, end, SEEK_SET);
    return 0;
}

/* Read image.
 * Return 0 on success or -1 on out-of-memory (w.r.t. LZW code table). */
static int32 read_image(gd_GIF *gif)
{
    uint8 fisrz;
    int32 interlace;

    /* Image Descriptor. */
    gif->fx = read_num(gif);
    gif->fy = read_num(gif);
    gif->fw = read_num(gif);
    gif->fh = read_num(gif);
    f_gif_read(gif, &fisrz, 1);
    interlace = fisrz & 0x40;
    /* Ignore Sort Flag. */
    /* Local Color Table? */
    if (fisrz & 0x80) {
        /* Read LCT */
        gif->lct.size = 1 << ((fisrz & 0x07) + 1);
        f_gif_read(gif, gif->lct.colors, 3 * gif->lct.size);
        gif->palette = &gif->lct;
    } else
        gif->palette = &gif->gct;
    /* Image Data. */
    return read_image_data(gif, interlace);
}

static void render_frame_rect(gd_GIF *gif, uint8 *buffer)
{
    int32 i, j, k;
    uint8 index, *color;
    uint16* rgb565p = buffer;//避免大小端问题导致的解算误差
    i = gif->fy * gif->width + gif->fx;

    for (j = 0; j < gif->fh; j++) {
        for (k = 0; k < gif->fw; k++) {
            index = gif->frame[(gif->fy + j) * gif->width + gif->fx + k];
            color = &gif->palette->colors[index * 3];
            if (!gif->gce.transparency || index != gif->gce.tindex) {
#if GIF_COLOR_OUT_TYPE == GIF_COLOR_RGB888
                buffer[(i + k) * 3 + 0] = color[0];
                buffer[(i + k) * 3 + 1] = color[1];
                buffer[(i + k) * 3 + 2] = color[2];
#elif GIF_COLOR_OUT_TYPE == GIF_COLOR_RGB565
                //lv_color_t c = lv_color_make(*(color + 0), *(color + 1), *(color + 2));
                // 注释部分可能存在大小端问题
                //buffer[(i + k) * 2 + 0] = ((color[1] << 3) & 0xE0) + ((color[2] >> 3) & 0x1F);
                //buffer[(i + k) * 2 + 1] = (color[0] & 0xF8) + ((color[1] >> 5) & 0x07);
                rgb565p[(i + k)] = ((color[0] << 8) & 0xF800) + ((color[1] << 3) & 0x7E0) + ((color[2] >> 3) & 0x001F);
#elif GIF_COLOR_OUT_TYPE == GIF_COLOR_GRAY
                //lv_color_t c = lv_color_make(*(color + 0), *(color + 1), *(color + 2));
                buffer[(i + k)] = ((color[0] * 76 + color[1] * 150 + color[2] * 30) >> 8);
#endif
            }
        }
        i += gif->width;
    }
}

static void dispose(gd_GIF *gif)
{
    int32 i, j, k;
    uint8 *bgcolor;
    switch (gif->gce.disposal) {
    case 2: /* Restore to background color. */
        bgcolor = &gif->palette->colors[gif->bgindex*3];

        uint8 opa = 0xff;
        if(gif->gce.transparency) opa = 0x00;
        uint16* rgb565p = gif->canvas;
        i = gif->fy * gif->width + gif->fx;
        for (j = 0; j < gif->fh; j++) {
            for (k = 0; k < gif->fw; k++) {
#if GIF_COLOR_OUT_TYPE == GIF_COLOR_RGB888
                gif->canvas[(i + k) * 3 + 0] = bgcolor[0];
                gif->canvas[(i + k) * 3 + 1] = bgcolor[1];
                gif->canvas[(i + k) * 3 + 2] = bgcolor[2];
                //gif->canvas[(i+k)*4 + 3] = opa;
#elif GIF_COLOR_OUT_TYPE == GIF_COLOR_RGB565
                //lv_color_t c = lv_color_make(*(bgcolor + 0), *(bgcolor + 1), *(bgcolor + 2));
                // 注释部分可能存在大小端问题
                //gif->canvas[(i + k) * 2 + 0] = ((bgcolor[1] << 3) & 0xE0) + ((bgcolor[2] >> 3) & 0x1F);
                //gif->canvas[(i + k) * 2 + 1] = (bgcolor[0] & 0xF8) + ((bgcolor[1] >> 5) & 0x07);
                rgb565p[(i + k)] = ((bgcolor[0] << 8) & 0xF800) + ((bgcolor[1] << 3) & 0x7E0) + ((bgcolor[2] >> 3) & 0x001F);
                //gif->canvas[(i + k) * 3 + 2] = opa;
#elif GIF_COLOR_OUT_TYPE == GIF_COLOR_GRAY
                //lv_color_t c = lv_color_make(*(bgcolor + 0), *(bgcolor + 1), *(bgcolor + 2));
                gif->canvas[(i + k)] = ((bgcolor[0] * 76 + bgcolor[1] * 150 + bgcolor[2] * 30) >> 8);
                //gif->canvas[(i + k) * 2 + 1] = opa;
#endif
            }
            i += gif->width;
        }
        break;
    case 3: /* Restore to previous, i.e., don't update canvas.*/
        break;
    default:
        /* Add frame non-transparent pixels to canvas. */
        render_frame_rect(gif, gif->canvas);
    }
}

/* Return 1 if got a frame; 0 if got GIF trailer; -1 if error. */
int32 gd_get_frame(gd_GIF *gif)
{
    char sep;

    dispose(gif);
    f_gif_read(gif, &sep, 1);
    while (sep != ',') {
        if (sep == ';')
            return 0;
        if (sep == '!')
            read_ext(gif);
        else return -1;
        f_gif_read(gif, &sep, 1);
    }
    if (read_image(gif) == -1)
        return -1;
    return 1;
}

void gd_render_frame(gd_GIF *gif, uint8 *buffer)
{
    //uint32 i;
    //uint32 j;
    //for(i = 0, j = 0; i < gif->width * gif->height * 3; i+= 3, j+=3) {
    //    buffer[j + 0] = gif->canvas[i + 0];
    //    buffer[j + 1] = gif->canvas[i + 1];
    //    buffer[j + 2] = gif->canvas[i + 2];
    //    //buffer[j + 3] = 0xFF;
    //}
    CV_memcpy(buffer, gif->canvas, gif->width * gif->height * 3);
    //render_frame_rect(gif, buffer);
}

int32 gd_is_bgcolor(gd_GIF *gif, uint8 color[3])
{
    return !YMCV_Memcmp(&gif->palette->colors[gif->bgindex*3], color, 3);
}

void gd_rewind(gd_GIF *gif)
{
    f_gif_seek(gif, gif->anim_start, SEEK_SET);
}

static uint8 f_gif_open(gd_GIF * gif, const void * path, uint8 is_file)
{
    gif->f_rw_p = 0;
    gif->data = NULL;
    gif->is_file = is_file;

    if(is_file) {
        //必须二进制打开
        gif->device = YMCV_fopen(path, "rb");
        if(gif->device == NULL)
            return gif_false;
        else return gif_true;
    } else {
        gif->data = path;
        return gif_true;
    }
}

static void f_gif_read(gd_GIF * gif, void * buf, size_t len)
{
    if(gif->is_file) {
        YMCV_fread(buf, 1, len, gif->device);
    } else
    {
        CV_memcpy(buf, &gif->data[gif->f_rw_p], len);
        gif->f_rw_p += len;
    }
}

static int32 f_gif_seek(gd_GIF * gif, size_t pos, int32 k)
{
    if(gif->is_file) {
        YMCV_fseek(gif->device, pos, k);
        uint32 x;
        x = YMCV_ftell(gif->device);
        return x;
    } else {
        if(k == SEEK_CUR) gif->f_rw_p += pos;
        else if(k == SEEK_SET) gif->f_rw_p = pos;
        return gif->f_rw_p;
    }
}


static void f_gif_close(gd_GIF * gif)
{
    if(gif->is_file) {
        YMCV_fclose(gif->device);
    }
}

gd_GIF* gd_prepare_from_data(const void* data)
{
    gd_GIF gif_base;
    CV_memset(&gif_base, 0, sizeof(gif_base));

    uint8 res = f_gif_open(&gif_base, data, gif_false);
    if (!res) return NULL;

    return gif_open(&gif_base);
}


gd_GIF* gd_prepare_from_file(const char* fname)
{
    gd_GIF gif_base;
    CV_memset(&gif_base, 0, sizeof(gif_base));

    uint8 res = f_gif_open(&gif_base, fname, gif_true);
    if (!res) return NULL;

    return gif_open(&gif_base);
}


void gd_next_frame(gd_GIF* gif)
{
    int32 has_next = gd_get_frame(gif);
    gif->is_finished = 0;
    if (has_next == 0) {
        gif->is_finished = 1;
        /*It was the last repeat*/
        if (gif->loop_count == 1) {
            //uint8 res = event_send(obj, EVENT_READY, NULL);
            //if (res != RES_OK) return;
        }
        else {
            //if (gif->loop_count > 1)  gif->loop_count--;
            gd_rewind(gif);
        }
    }
    //将mygif->canvas的数据拷贝到buff中
    //gd_render_frame(mygif, myimg.data);
}

//退出
void gd_quit(gd_GIF* gif)
{
    f_gif_close(gif);
    CV_free1(gif);
}



