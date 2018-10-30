/************************************************************************************
 * Copyright (C) 2014                                                               *
 * TETCOS, Bangalore. India                                                         *
 *                                                                                  *
 * Tetcos owns the intellectual property rights in the Product and its content.     *
 * The copying, redistribution, reselling or publication of any or all of the       *
 * Product or its content without express prior written consent of Tetcos is        *
 * prohibited. Ownership and / or any other right relating to the software and all  *
 * intellectual property rights therein shall remain at all times with Tetcos.      *
 *                                                                                  *
 * Author:    Shashi Kant Suman                                                     *
 *                                                                                  *
 * ---------------------------------------------------------------------------------*/
#ifdef _WIN32
#pragma comment(lib,"MemoryBlock.lib")
#endif
#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)") : Warning Msg: "
#ifdef _DEBUG
#undef _strdup
#undef calloc
#undef malloc
#undef free
#undef realloc
#undef _msize
#endif
#define _strdup(x) _strdup_dbg1(x,__LINE__,__FILE__)
#define calloc(c,s) fnpAllocateMemory(c,s)
#define malloc(s) fnpAllocateMemory(1,s)
#define free(p) fnpFreeMemory(p)
#define realloc(p,s) fnpReAllocateMemory(p,s)
#define _msize(memory) fnmsize(memory)
# define fnpAllocateMemory(count,size) fnpAllocateMemory_dbg(count,size,__LINE__,__FILE__)
# define fnpReAllocateMemory(p,size) fnpReAllocateMemory_dbg(p,size,__LINE__,__FILE__)
# define fnpFreeMemory(p) fnpFreeMemory_dbg(p,sizeof *(p),__LINE__,__FILE__)


_declspec(dllexport) void* fnpAllocateMemory_dbg(size_t count, size_t size,int line,const char* file);
_declspec(dllexport) void* fnpReAllocateMemory_dbg(void* p, size_t size,int line,const char* file);
_declspec(dllexport) int fnpFreeMemory_dbg(void* p,unsigned int size,int line,const char* file);
_declspec(dllexport) char* _strdup_dbg1(const char* str,int nline,const char* file);
_declspec(dllexport) size_t fnmsize(void* memory);

