#ifndef YMCV_Mem_H
#define YMCV_Mem_H

#include "../CONFIG/YMCV_PubType.h"

void * CV_malloc0(size_t size);//小数据缓冲区申请，在较高速度的缓冲区申请
void* CV_realloc0(void* ptr, size_t size);//重置内存长度
void CV_free0(void * ptr);//释放内存

void * CV_malloc1(size_t size);//大数据缓冲区申请，在较低速度的缓冲区申请
void* CV_realloc1(void* ptr, size_t size);//重置内存长度
void CV_free1(void * ptr);//释放内存

void CV_memset(void* ptr, int val, size_t size);//内存值设置
void CV_memcpy(void* dst, const void* src, size_t size);//内存拷贝

#endif
