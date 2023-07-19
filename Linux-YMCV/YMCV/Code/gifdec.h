#ifndef GIFDEC_H
#define GIFDEC_H

#include "../CONFIG/YMCV_PubDefine.h"
#include "../CONFIG/YMCV_PubType.h"
#include "../CONFIG/YMCV_Mem.h"
#include "../DEBUG/YMCV_Debug.h"

#define GIF_COLOR_RGB888 24
#define GIF_COLOR_RGB565 16
#define GIF_COLOR_GRAY 8

//设置输出为rgb565格式
#define GIF_COLOR_OUT_TYPE   GIF_COLOR_RGB565

typedef struct gd_Palette {
    int32 size;
    uint8 colors[256 * 3];
} gd_Palette;

typedef struct gd_GCE {
    uint16 delay;
    uint8 tindex;
    uint8 disposal;
    int32 input;
    int32 transparency;
} gd_GCE;

typedef struct gd_GIF {
    void* device;
    const char * data;
    uint8 is_file;
    uint8 is_finished;
    uint8 have_gct;//存在全局颜色表
    uint32 f_rw_p;
    int32 anim_start;
    uint16 width, height;
    uint16 depth;
    uint16 loop_count;
    gd_GCE gce;
    gd_Palette *palette;
    gd_Palette lct, gct;
    void (*plain_text)(
        struct gd_GIF *gif, uint16 tx, uint16 ty,
        uint16 tw, uint16 th, uint8 cw, uint8 ch,
        uint8 fg, uint8 bg
    );
    void (*comment)(struct gd_GIF *gif);
    void (*application)(struct gd_GIF *gif, char id[8], char auth[3]);
    uint16 fx, fy, fw, fh;
    uint8 bgindex;
    uint8 *canvas, *frame;
} gd_GIF;


//void gd_render_frame(gd_GIF *gif, uint8 *buffer);
//int32 gd_get_frame(gd_GIF *gif);
//void gd_rewind(gd_GIF *gif);

//准备解码器
gd_GIF* gd_prepare_from_data(const void* data);
gd_GIF* gd_prepare_from_file(const char* fname);
//获取下一帧图像
void gd_next_frame(gd_GIF* gif);
//退出解码器
void gd_quit(gd_GIF *gif);

#endif /* GIFDEC_H */
