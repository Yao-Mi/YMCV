#ifndef _MEMDBG_H
#define _MEMDBG_H

#include <stdio.h>
#include "Pubtype.h"

typedef uint32 size_t;

#define _Mem_Debug_ 1  //启用内存调试

/****************************************
*
*   stm32中，CCM内存访问速度最快，但只能被CPU访问，只能定义数据变量，且不能用DMA，可作为栈或者高速缓冲，
*   Memaddr0：CCM
*   Memaddr1：内部RAM
*	  Memaddr2：外部SRAM
*
*****************************************/
#define NULL 0

//管理的内存数量
#define Mem_Num  3

//管理内存大小单位字节数 BYTE
#define Mem0_size 1024*35      //max < 40KB
#define Mem1_size 1024*10    //max < 100KB
#define Mem2_size 1024*960   //max < 1024KB

//分块大小
#define Block_size 32  //每块大小 32 Byte
#define Save_size 4 //预留分隔带
//管理表的大小
#define Mc0_size (Mem0_size/Block_size)
#define Mc1_size (Mem1_size/Block_size)
#define Mc2_size (Mem2_size/Block_size) 

//物理内存空间大小 : 内存+分隔带+管理表(u16)
#define MemTolSize0  (Mc0_size * Block_size + Save_size + (2 * Mc0_size))
#define MemTolSize1	 (Mc1_size * Block_size + Save_size + (2 * Mc1_size))
#define MemTolSize2  (Mc2_size * Block_size + Save_size + (2 * Mc2_size))

#if (MemTolSize0 > 40*1024)
#error CCM内存不能超过40KB
#endif
#if (MemTolSize1 > 100*1024)
#error 内部SRAM不能超过100KB
#endif
#if (MemTolSize2 > 1024*1024)
#error 外部SRAM不能超过1024KB（1MB）
#endif

extern uint8 Memaddr0[MemTolSize0];
extern uint8 Memaddr1[MemTolSize1];
extern uint8 Memaddr2[MemTolSize2];

//将管理状态表放置于数组尾部位置
//管理表基地址
#define Mc0_ubase (Memaddr0 + Mc0_size*Block_size + Save_size)
#define Mc1_ubase (Memaddr1 + Mc1_size*Block_size + Save_size)
#define Mc2_ubase (Memaddr2 + Mc2_size*Block_size + Save_size)

//管理空间结束地址
#define Mem0_end (Memaddr0 + Mc0_size*Block_size)
#define Mem1_end (Memaddr1 + Mc1_size*Block_size)
#define Mem2_end (Memaddr2 + Mc2_size*Block_size)

//内存控制器
struct _mem_ctrol
{
	void(*init)();					    //内存初始化
	uint8(*used)(uint8);		  	    	//内存使用率
	uint8 	*Membase[Mem_Num];				//内存基地址
	size_t mem_size[Mem_Num];			//管理内存池的大小（字节）
	uint8 	*  mem_end[Mem_Num];			//内存结束地址
	size_t map_size[Mem_Num];			//管理状态表的大小（字节）
	uint16 *Memmap[Mem_Num]; 				//管理状态表地址  //单次最大分配 65536/ 32  = 2048 KB 2M 空间
	uint8  Memflg[Mem_Num]; 				//管理表状态标志
};
extern struct _mem_ctrol mem_ctrol;

void YMem_init(void);//内存初始化
uint8 YMem_used(uint8 witchmem);//内存使用率

//与debug无关，无法使用debug功能
void * YMem_malloc_nt(uint8 memx, uint32 size);
void YMem_free_nt(uint8 memx, void * ptr);
void *YMem_calloc_nt(uint8 memx, uint32 num, uint32 size);
void *YMem_realloc_nt(uint8 memx, void *oldptr, uint32 size);

//可以选择开启使用debug功能
void * YMem_malloc_t(uint8 memx, uint32 size);
void YMem_free_t(uint8 memx, void * ptr);
void *YMem_calloc_t(uint8 memx, uint32 num, uint32 size);
void *YMem_realloc_t(uint8 memx, void *oldptr, uint32 size);

void YMem_memcpy_uint8(void *des, const void *src, uint32 n);//复制
void YMem_memcpy_uint16(void *des, const void *src, uint32 n);
void YMem_memcpy_uint32(void *des, const void *src, uint32 n);
void YMem_memcpy_float32(void *des, const void *src, uint32 n);

void YMem_memset_uint8(void *addr, uint8 value, uint32 count);//设置
void YMem_memset_uint16(void *addr, uint16 value, uint32 count);
void YMem_memset_uint32(void *addr, uint32 value, uint32 count);
void YMem_memset_float32(void *addr, float32 value, uint32 count);

void YMem_memmove(void *des, const void *src, uint32 num);//复制
void *YMem_memchr(const void *str, int c, size_t num);//字符检测
int YMem_memcmp(const void *des, const void *src, size_t num);//比较


typedef struct _mem_type
{
	void* ptr;
	uint8 memx;
	void(*free)(struct _mem_type mymem);
}mem_type;

mem_type YMem_malloc(uint8 memx, uint32 size);
void YMem_free(mem_type mem);
mem_type YMem_calloc(uint8 memx, uint32 num, uint32 size);
mem_type YMem_realloc(mem_type oldpmem, uint32 size);

typedef struct _mem_list
{
	void* ptr;
	uint8 memx;
	int times;
	struct _mem_list* next;
	struct _mem_list* last;
}mem_list;

typedef struct _mem_debug
{
	void (*init)();
	void (*start)();
	void (*end)();
	int times;
	int encflg;
	int tflg;
	struct _mem_list* mymem;
}mem_debug;

extern mem_debug Mem_debug;
void YMem_debug_init(void);
void YMem_debug_start(void);
void YMem_debug_end(void);

void mdebug_add_a_mem(uint8 memx, void* ptr);
void mdebug_free_a_mem(uint8 memx, void* ptr);
void mdebug_check(void);

#endif

