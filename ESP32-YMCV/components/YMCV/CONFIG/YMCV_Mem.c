#include <stdlib.h>
#include <string.h>
#include "esp_heap_caps.h"
#include "./YMCV_Mem.h"

//小型数据用这个
//在较高速度的缓冲区申请
void * CV_malloc0(size_t size)
{
	void* myptr = malloc(size);
  
	return myptr;
}
//重新分配内存长度
void* CV_realloc0(void* ptr, size_t size)
{
	void* myptr = realloc(ptr, size);

	return myptr;
}
//释放内存
void CV_free0(void * ptr)
{
	free(ptr);
}

//大型数据用这个
//在较低速度的缓冲区申请
void * CV_malloc1(size_t size)
{
	void* myptr = heap_caps_malloc(size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);//表示分配 8 位的 PSRAM 内存

	return myptr;
}

//重新分配内存长度
void* CV_realloc1(void* ptr ,size_t size)
{
	void* myptr = heap_caps_realloc(ptr,size,MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

	return myptr;
}

//释放内存
void CV_free1(void * ptr)
{
	heap_caps_free(ptr);
}

//内存值设置
void CV_memset(void* ptr, int val , size_t size)
{
	memset(ptr, val, size);
}

//内存值拷贝
void CV_memcpy(void* dst, const void* src, size_t size)
{
	memcpy(dst, src, size);
}
