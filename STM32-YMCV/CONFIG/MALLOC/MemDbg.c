#include "MemDbg.h"

//三块逐级增加的内存空间
__align(32) uint8 Memaddr0[MemTolSize0] __attribute__((at(0X10000000)));//内部CCM内存池
__align(32) uint8 Memaddr1[MemTolSize1] ;//内部SRAM内存池
__align(32) uint8 Memaddr2[MemTolSize2] __attribute__((at(0X68000000)));//外部SRAM内存池

//内存管理器
struct _mem_ctrol mem_ctrol=
{
	.init= YMem_init,//初始化
	.used= YMem_used,//使用率
	.Membase = {Memaddr0,Memaddr1,Memaddr2},//内存基地址
	.mem_size = {Mc0_size*Block_size,Mc1_size*Block_size,Mc2_size*Block_size},//管理内存的大小（字节）
	.mem_end = {Mem0_end,Mem1_end,Mem2_end},//内存结束地址
	.map_size = {Mc0_size,Mc1_size,Mc2_size},//管理状态表的大小
	.Memmap = {(uint16*)Mc0_ubase,(uint16*)Mc1_ubase,(uint16*)Mc2_ubase},//管理状态表地址  //单次最大分配 65536/ 32  = 2048 KB 2M 空间
	.Memflg = {NULL,NULL,NULL},//管理表状态标志
};

//内存初始化
void YMem_init()
{
//	//清空内存空间
//	YMem_memset_uint8(Memaddr0, 0, MemTolSize0);
//	YMem_memset_uint8(Memaddr1, 0, MemTolSize1);
//	YMem_memset_uint8(Memaddr2, 0, MemTolSize2);
	
	//仅清除管理状态表就够了
	YMem_memset_uint16(Mc0_ubase, 0, Mc0_size);
	YMem_memset_uint16(Mc1_ubase, 0, Mc1_size);
	YMem_memset_uint16(Mc2_ubase, 0, Mc2_size);
	
	//初始化成功
	for (int i = 0; i < Mem_Num; i++)
		mem_ctrol.Memflg[i]=1;
}
//内存使用率
//witchmem: 第几个内存块
//返回值：使用率（0-100）
uint8 YMem_used(uint8 witchmem)
{
	uint32 used = 0;
	uint32 i;
	for (i = 0; i < mem_ctrol.map_size[witchmem]; i++)
	{
		if (mem_ctrol.Memmap[witchmem][i])
			used++;
	}
	return (used * 100) / (mem_ctrol.map_size[witchmem]);

}

//内存分配
//memx:所属内存块
//size:要分配的字节数
//错误返回:NULL,否则返回地址 
void * YMem_malloc_nt(uint8 memx, uint32 size)
{
	signed long offset = 0;
	uint32 nmmap;	//所需内存块数量
	uint32 cmap = 0;//连续空的内存块累计数
	uint32 i;

	if (size == 0)return NULL;//无需分配

	nmmap = size / Block_size;  	//获取所需内存块数量
	if ((size%Block_size) != 0)nmmap++;

	for (offset = mem_ctrol.map_size[memx] - 1; offset >= 0; offset--)//逆向搜索全部管理表
	{
		if (!mem_ctrol.Memmap[memx][offset])//管理表未标志，内存未分配
			cmap++;
		else
			cmap = 0;

		if (cmap == nmmap)							//连续空的内存空间满足需求
		{
			for (i = 0; i < cmap; i++)  					//管理表 标志 分配
			{
				mem_ctrol.Memmap[memx][offset + i] = nmmap;
			}
			return (mem_ctrol.Membase[memx] + offset * Block_size);//返回地址  
		}
	}
	return NULL;//未找到符合条件的内存块
}

//内存释放（内部调用）
//memx:所属内存块
//ptr:内存地址
void YMem_free_nt(uint8 memx, void * ptr)
{
	int i;
	if (ptr == NULL)
		return;

	if (((void*)mem_ctrol.Membase[memx] <= ptr) && (ptr < (void*)mem_ctrol.mem_end[memx]))//偏移范围在内存池中
	{
		int index = ((uint32)ptr - (uint32)mem_ctrol.Membase[memx]) / Block_size;			//偏移的内存块数  
		int nmemb = mem_ctrol.Memmap[memx][index];	//连续偏移数
		for (i = 0; i < nmemb; i++)  				//重新标记 清零
		{
			mem_ctrol.Memmap[memx][index + i] = 0;
		}
	}
}
//连续内存分配并初始化为0
//memx:所属内存块
//num:对象个数
//size:对象占据的字节数
//错误返回:NULL,否则返回地址 
void *YMem_calloc_nt(uint8 memx, uint32 num, uint32 size)
{
	void * ptr;

	ptr = YMem_malloc_nt(memx, num*size);
	if (ptr == NULL)
		return NULL;
	else
	{
		YMem_memset_uint8(ptr, 0, num*size);
		return ptr;
	}
}
//内存重新分配并初始化
//memx:所属内存块
//ptr:旧地址
//size:对象占据的字节数
//错误返回:NULL,否则返回地址 
void *YMem_realloc_nt(uint8 memx, void *oldptr, uint32 size)
{
	void * ptr;

	ptr = YMem_malloc_nt(memx, size);

	if ((ptr == NULL) || (oldptr == NULL))
		return ptr;
	else
	{
		YMem_memcpy_uint8( ptr,oldptr, size);
		YMem_free_nt(memx, oldptr);
		return ptr;
	}
}

//内存分配
//memx:所属内存块
//size:要分配的字节数
//错误返回:NULL,否则返回地址 
void * YMem_malloc_t(uint8 memx, uint32 size)
{
	signed long offset = 0;
	uint32 nmmap;	//所需内存块数量
	uint32 cmap = 0;//连续空的内存块累计数
	uint32 i;

	if (size == 0)return NULL;//无需分配

	nmmap = size / Block_size;  	//获取所需内存块数量
	if (size%Block_size)nmmap++;
	
	for (offset = mem_ctrol.map_size[memx] - 1; offset >= 0; offset--)//逆向搜索全部管理表
	{
		if (!mem_ctrol.Memmap[memx][offset])//管理表未标志，内存未分配
			cmap++;
		else 
			cmap = 0;	

		if (cmap == nmmap)							//连续空的内存空间满足需求
		{
			for (i = 0; i < cmap; i++)  					//管理表 标志 分配
			{
				mem_ctrol.Memmap[memx][offset + i] = nmmap;
			}
#if _Mem_Debug_==1  //开启内存调试
			if (Mem_debug.encflg)
			{
				mdebug_check();
				mdebug_add_a_mem(memx, mem_ctrol.Membase[memx] + offset * Block_size);
			}
#endif // _Mem_Debug_==1
			return (mem_ctrol.Membase[memx] + offset*Block_size);//返回地址  
		}
	}
	return NULL;//未找到符合条件的内存块
}

//内存释放（内部调用）
//memx:所属内存块
//ptr:内存地址
void YMem_free_t(uint8 memx, void * ptr)
{
	int i;
	if(ptr ==NULL)
		return ;
#if _Mem_Debug_==1  //开启内存调试
	if (Mem_debug.encflg)
	{
		mdebug_free_a_mem(memx, ptr);
	}
#endif // _Mem_Debug_==1

	if (((void*)mem_ctrol.Membase[memx] <= ptr)&&(ptr < (void*)mem_ctrol.mem_end[memx]))//偏移范围在内存池中
	{
		int index =((uint32)ptr-(uint32)mem_ctrol.Membase[memx])/ Block_size;			//偏移的内存块数  
		int nmemb = mem_ctrol.Memmap[memx][index];	//连续偏移数
		for (i = 0; i < nmemb; i++)  				//重新标记 清零
		{
			mem_ctrol.Memmap[memx][index + i] = 0;
		}
	}
}

//连续内存分配并初始化为0
//memx:所属内存块
//num:对象个数
//size:对象占据的字节数
//错误返回:NULL,否则返回地址 
void *YMem_calloc_t(uint8 memx, uint32 num, uint32 size)
{
	void * ptr;

	ptr = YMem_malloc_t(memx, num*size);
	if (ptr == NULL)
		return NULL;
	else
	{
		YMem_memset_uint8(ptr, 0, num*size);
		return ptr;
	}
}

//内存重新分配并初始化
//memx:所属内存块
//ptr:旧地址
//size:对象占据的字节数
//错误返回:NULL,否则返回地址 
void *YMem_realloc_t(uint8 memx, void *oldptr, uint32 size)
{
	void * ptr;

	ptr = YMem_malloc_t(memx, size);

	if ((ptr == NULL)||(oldptr == NULL))
		return ptr;
	else
	{
		YMem_memcpy_uint8( ptr, oldptr, size);
		YMem_free_t(memx, oldptr);
		return ptr;
	}
}
//复制内存
//*des:目的地址
//*src:源地址
//n:需要复制的内存长度(字节为单位)
void YMem_memcpy_uint8(void *des, const void *src, uint32 n)
{
	uint8 *xdes = des;
	uint8 *xsrc = (uint8*)src;
	while (n--)*xdes++ = *xsrc++;
}

void YMem_memcpy_uint16(void *des, const void *src, uint32 n)
{
	uint16 *xdes = des;
	uint16 *xsrc = (uint16*)src;
	while (n--)*xdes++ = *xsrc++;
}

void YMem_memcpy_uint32(void *des, const void *src, uint32 n)
{
	uint32 *xdes = des;
	uint32 *xsrc = (uint32*)src;
	while (n--)*xdes++ = *xsrc++;
}

void YMem_memcpy_float32(void *des, const void *src, uint32 n)
{
	float32 *xdes = des;
	float32 *xsrc = (float32*)src;
	while (n--)*xdes++ = *xsrc++;
}

//设置内存
//addr:地址
//value:设置的值
//count:字节数
void YMem_memset_uint8(void *addr, uint8 value, uint32 count)
{
	uint8 *xdr = addr;
	while (count--)
		*xdr++ = value;
}
void YMem_memset_uint16(void *addr, uint16 value, uint32 count)
{
	uint16 *xdr = addr;
	while (count--)
		*xdr++ = value;
}

void YMem_memset_uint32(void *addr, uint32 value, uint32 count)
{
	uint32 *xdr = addr;
	while (count--)
		*xdr++ = value;
}

void YMem_memset_float32(void *addr, float32 value, uint32 count)
{
	float32 *xdr = addr;
	while (count--)
		*xdr++ = value;
}
//内存搬运
// des:目标地址
// src:源地址
// num: 需要复制的字节数
void YMem_memmove(void *des, const void *src, uint32 num)
{
	uint8 *xdes = des;
	uint8 *xsrc = (uint8*)src;

	if (xsrc > xdes || (xsrc + num) <= xdes)//正序复制
	{
		//src :    xxxxxxxxxxxxx
		//des :  xxxxxxxxxxxxxxxxxx
		while (num--)*xdes++ = *xsrc++;
	}
	else//逆序复制
	{
		//src :    xxxxxxxxxxxxx
		//des :        xxxxxxxxxxxxxxxxxx
		xdes += num - 1;
		xsrc += num - 1;
		while (num--)*xdes-- = *xsrc--;
	}
}
//字符地址检测
//str: 搜索源地址
//c: 待搜索字符
//num :最大搜索字节数
void *YMem_memchr(const void *str, int c, size_t num)
{
	char* src = (char*)str;
	char vc1 = c;
	while (num--)
	{
		if (*src == vc1)
			return (void *)src;
		src++;
	}
	return NULL;
}
//内存比较
// des:目标地址
// src:源地址
// num: 需要比较的字节数
int YMem_memcmp(const void *des, const void *src, size_t num)
{
	const uint8 * pdes = (uint8*)des;
	const uint8 * psrc = (uint8*)src;
	int a;
	while (*pdes == *psrc && (--num > 0)) //跳过值相同的区域
	{
		pdes++;
		psrc++;
	}
	a = *pdes - *psrc; //计算值哪个大
	if (a > 0)return 1;
	else if (a < 0) return -1;
	else return 0;
}


//内存分配
//memx:所属内存块
//size:要分配的字节数
mem_type YMem_malloc(uint8 memx, uint32 size)
{
	signed long offset = 0;
	uint32 nmmap;	//所需内存块数量
	uint32 cmap = 0;//连续空的内存块累计数
	uint32 i;
	mem_type mymemt =
	{
		.ptr = NULL,
		.memx = memx,
		.free = YMem_free,
	};

	if (size == 0) return mymemt;//无需分配

	nmmap = size / Block_size;  	//获取所需内存块数量
	if (size%Block_size)nmmap++;

	for (offset = mem_ctrol.map_size[memx] - 1; offset >= 0; offset--)//逆向搜索全部管理表
	{
		if (!mem_ctrol.Memmap[memx][offset])//管理表未标志，内存未分配
			cmap++;
		else
			cmap = 0;

		if (cmap == nmmap)							//连续空的内存空间满足需求
		{
			for (i = 0; i < cmap; i++)  					//管理表 标志 分配
			{
				mem_ctrol.Memmap[memx][offset + i] = nmmap;
			}

			mymemt.ptr = mem_ctrol.Membase[memx] + offset * Block_size;//得到地址
			return mymemt;//返回  
		}
	}

	return mymemt;//未找到符合条件的内存块
}

//内存释放（内部调用）
//memx:所属内存块
//ptr:内存地址
void YMem_free(mem_type mem)
{
	int i;
	if (mem.ptr == NULL)
		return;

	if (((void*)mem_ctrol.Membase[mem.memx] <= mem.ptr) && (mem.ptr < (void*)mem_ctrol.mem_end[mem.memx]))//偏移范围在内存池中
	{
		int index = ((uint32)mem.ptr - (uint32)mem_ctrol.Membase[mem.memx]) / Block_size;			//偏移的内存块数  
		int nmemb = mem_ctrol.Memmap[mem.memx][index];	//连续偏移数
		for (i = 0; i < nmemb; i++)  				//重新标记 清零
		{
			mem_ctrol.Memmap[mem.memx][index + i] = 0;
		}
	}
}

//连续内存分配并初始化为0
//memx:所属内存块
//num:对象个数
//size:对象占据的字节数
//错误返回:NULL,否则返回地址 
mem_type YMem_calloc(uint8 memx, uint32 num, uint32 size)
{
	mem_type mymem;

	mymem = YMem_malloc(memx, num*size);
	if (mymem.ptr == NULL)
		return mymem;
	else
	{
		YMem_memset_uint8(mymem.ptr, 0, num*size);
		return mymem;
	}
}

//内存重新分配并初始化
//memx:所属内存块
//ptr:旧地址
//size:对象占据的字节数
//错误返回:NULL,否则返回地址 
mem_type YMem_realloc(mem_type oldpmem, uint32 size)
{
	mem_type mymem;

	mymem = YMem_malloc(oldpmem.memx,size);
	if ((mymem.ptr == NULL) || (oldpmem.ptr == NULL))
		return mymem;
	else
	{
		YMem_memcpy_uint8(mymem.ptr, oldpmem.ptr, size);
		YMem_free(oldpmem);
		return mymem;
	}
}

mem_list Mem_debug_list = {
		.ptr = NULL,
		.memx = 0,
		.times = 0,
		.next = NULL,
		.last = NULL,
};

mem_debug Mem_debug =
{
	.init = YMem_debug_init,
	.mymem = NULL,
	.start = YMem_debug_start,
	.end = YMem_debug_end,
	.tflg=0,
	.encflg=0,
	.times=0,
};


void mdebug_add_a_mem(uint8 memx, void* ptr)
{
	mem_list* rmemp;
	rmemp = (mem_list*)YMem_calloc_nt(memx, 1, sizeof(mem_list)); //申请内存
	if (rmemp)
	{
		//入栈
		rmemp->last = Mem_debug.mymem;
		if(Mem_debug.mymem!=NULL)
			Mem_debug.mymem->next = rmemp;

		Mem_debug.mymem = rmemp;

		rmemp->ptr = ptr;
		rmemp->memx = memx;
		rmemp->times = Mem_debug.times ++;

		printf("+%d :add ptr: %d ，\n", rmemp->times,(uint32)ptr);
	}
}
void mdebug_free_a_mem(uint8 memx, void* ptr)
{
	mem_list* rmemp,*lst,*nxt;
	rmemp = Mem_debug.mymem;
	if (rmemp == NULL) return;

	while (rmemp->ptr != ptr)
	{
		//mem is not in enc
		if (rmemp->last == NULL)
			break;
		else
			rmemp = rmemp->last;
	}	
	if ((rmemp->ptr == ptr)&&(rmemp->memx == memx))
	{
		//mem move
		if (rmemp == Mem_debug.mymem)//表头
		{
			Mem_debug.mymem = rmemp->last;
			YMem_free_nt(rmemp->memx, rmemp);
		}
		else //表身
		{
			lst = rmemp->last;
			nxt = rmemp->next;

			if (lst != NULL) lst->next = nxt;
			if (nxt !=NULL) nxt->last = lst;
		}

		YMem_free_nt(rmemp->memx, rmemp);
		printf("-%d :move ptr: %d\n", rmemp->times, ptr);
	}
}
void mdebug_check()
{
	mem_list * mylptr;
	if (Mem_debug.tflg == 2)
	{
		mylptr = Mem_debug.mymem;
		if (mylptr == NULL) return;

		while (mylptr->times != Mem_debug.times)
		{
			if (mylptr->last == NULL)
				break;
			else
				mylptr = mylptr->last;
		}

		if (mylptr->times == Mem_debug.times)
		{
			printf("+%d :this mem would not be free!\n", mylptr->times);
			while (1);
		}
	}
}


void YMem_debug_init()
{
#if _Mem_Debug_==1  //开启内存调试

	mem_list * mylptr;

	//释放残余内存
	if (Mem_debug.mymem != NULL)
	{
		while (Mem_debug.mymem->last != NULL)//last is ! null
		{
			mylptr = Mem_debug.mymem->last;
			Mem_debug.mymem->last = mylptr->last;
			//free mylptr
			YMem_free_t(mylptr->memx, mylptr->ptr);
			YMem_free_t(mylptr->memx, mylptr);
		};
		while (Mem_debug.mymem->next != NULL)
		{
			mylptr = Mem_debug.mymem->next;
			Mem_debug.mymem->next = mylptr->next;
			//free mylptr
			YMem_free_t(mylptr->memx, mylptr->ptr);
			YMem_free_t(mylptr->memx, mylptr);
		};
		YMem_free_t(Mem_debug.mymem->memx, Mem_debug.mymem->ptr);
		YMem_free_t(Mem_debug.mymem->memx, Mem_debug.mymem);
	}
	Mem_debug.times = 0;
	Mem_debug.encflg = 0;
	Mem_debug.tflg = 0;

#endif // _Mem_Debug_==1
}

void YMem_debug_start()
{
#if _Mem_Debug_==1  //开启内存调试

	Mem_debug.times =0;
	Mem_debug.encflg=1;
	Mem_debug.tflg++;

	printf("\r\n mem check::  %d \r\n", Mem_debug.tflg);
	if (Mem_debug.tflg == 3)//第二次进入调试
	{
		if (Mem_debug.mymem == NULL)
		{
			printf("mem is nothing problem !\n");
		}
		while (1)
		{

		};
	}

#endif // _Mem_Debug_==1
}

void YMem_debug_end()
{
#if _Mem_Debug_==1  //开启内存调试

	Mem_debug.encflg = 0;
	printf("mem check::  %d is endl\r\n", Mem_debug.tflg);

#endif // _Mem_Debug_==1
}

