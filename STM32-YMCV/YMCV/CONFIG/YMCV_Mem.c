#include <stdlib.h>
#include <string.h>
#include "MemDbg.h"
#include "./YMCV_Mem.h"

//小型数据用这个
//在较高速度的缓冲区申请
void * CV_malloc0(size_t size)
{
	void* myptr = YMem_malloc_nt(0,size);
  
	return myptr;
}
//重新分配内存长度
void* CV_realloc0(void* ptr, size_t size)
{
	void* myptr = YMem_realloc_nt(0,ptr, size);

	return myptr;
}
//释放内存
void CV_free0(void * ptr)
{
	YMem_free_nt(0,ptr);
}

//大型数据用这个
//在较低速度的缓冲区申请
void * CV_malloc1(size_t size)
{
	void* myptr = YMem_malloc_nt(2,size);

	return myptr;
}

//重新分配内存长度
void* CV_realloc1(void* ptr ,size_t size)
{
	void* myptr = YMem_realloc_nt(2,ptr,size);

	return myptr;
}

//释放内存
void CV_free1(void * ptr)
{
	YMem_free_nt(2,ptr);
}

//内存值设置
void CV_memset(void* ptr, int val , size_t size)
{
	//memset(ptr, val, size);
	YMem_memset_uint8(ptr, val, size);
}

//内存值拷贝
void CV_memcpy(void* dst, const void* src, size_t size)
{
	//memcpy(dst, src, size);
	YMem_memcpy_uint8(dst, src, size);
}
