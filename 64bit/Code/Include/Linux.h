/************************************************************************************
 * Copyright (C) 2015                                                               *
 * TETCOS, Bangalore. India                                                         *
 *                                                                                  *
 * Tetcos owns the intellectual property rights in the Product and its content.     *
 * The copying, redistribution, reselling or publication of any or all of the       *
 * Product or its content without express prior written consent of Tetcos is        *
 * prohibited. Ownership and / or any other right relating to the software and all  *
 * intellectual property rights therein shall remain at all times with Tetcos.      *
 *                                                                                  *
 * Author: Shashi kant suman                                                        *
 *                                                                                  *
 * ---------------------------------------------------------------------------------*/

#ifndef LINUX_HEADER
#define LINUX_HEADER

#include <string.h>
#include <errno.h>
#include <math.h>
#include <ctype.h>
#include <dlfcn.h>
#include <pthread.h>
#include <time.h>
#include <malloc.h>// _msize
#include <unistd.h> //for close() 
#include <sys/time.h> //gettimeofday
#include <sys/types.h>

#define INT8    int8_t
#define UINT8   uint8_t
#define INT16   int16_t
#define UINT16  uint16_t
#define INT32   int32_t
#define UINT32  uint32_t
#define INT64   int64_t
#define UINT64  uint64_t
#define UINT	unsigned int

#define _declspec(dllexport) extern
#define _declspec(dllimport) extern
#define GetLastError() errno
//#define _putenv putenv
#define _getch();
#define getch();
#define PVOID void*
# define HINSTANCE void*
# define LoadLibraryA(x) dlopen(x,RTLD_NOW)
# define LoadLibrary(x) dlopen(x,RTLD_NOW)
# define GetProcAddress(x,y) dlsym(x,y)
# define FreeLibrary dlclose 
# define MessageBoxA(a,b,c,d) printf("%s\t%s\n",b,c);
# define MessageBox(a,b,c,d) printf("%s\t%s\n",b,c);
# define fnDlError(x){printf("Error: %s\n",dlerror());}
# define fnSystemError(x,...){printf("Error No:%d in %s line no %d: %s\n",errno,__FILE__,__LINE__,x);}
# define fnNetSimError(x,...){printf("In %s line no %d: %s",__FILE__,__LINE__,x);}
# define GetCurrentThreadId() pthread_self()
# define WriteFile(file,str,size,a,b) write(file,str,size)
#define BOOLEAN BOOL

typedef enum 
{
	FALSE=0,
	TRUE=1,
}BOOL;


typedef struct _LARGE_INTEGER {
  long long QuadPart;
} LARGE_INTEGER;
static const char pathSeperator[5] = "/";
// static const char libext[10] = ".so"; 
#define HANDLE void *
#define HMODULE void*
#define pthread_mutex_destroy(a) ReleaseMutex(a)
#define FALSE 0
#define TRUE 1
#define WORD unsigned short 
#define DWORD unsigned int 
#define WINAPI
#define LPVOID void *
#define INFINITE INFINITY
#define WaitForSingleObject(mutexObject,b) pthread_mutex_lock( mutexObject )
#define ReleaseMutex(mutexObject) pthread_mutex_unlock(mutexObject)
typedef struct _SYSTEMTIME {
  WORD wYear;
  WORD wMonth;
  WORD wDayOfWeek;
  WORD wDay;
  WORD wHour;
  WORD wMinute;
  WORD wSecond;
  WORD wMilliseconds;
} SYSTEMTIME;
#define _popen(a,b) popen(a,b) 
#define _pclose(a) pclose(a)
#define CRITICAL_SECTION pthread_mutex_t;
#define FARPROC void *
#define EnterCriticalSection(a) pthread_mutex_lock (a);
#define LeaveCriticalSection(a) pthread_mutex_unlock (a);
#define DeleteCriticalSection(a) pthread_mutex_destroy (a);
#define CreateMutexA CreateMutex
#define CloseHandle(mutex) pthread_mutex_destroy(mutex);
#define Sleep(x) usleep(1000*x) //usleep takes in microscecond
#define _fcloseall() fcloseall()
#define min(a,b) !(b<a)?a:b 
#define max(a,b) !(b>a)?a:b
#define __debugbreak()
typedef unsigned char   u_char;
typedef unsigned short  u_short;
typedef unsigned int    u_int;
typedef unsigned long   u_long;
#define __in 
#define __out

//Thread function
void* CreateMutex(void* att,int a,char* name);
void  WaitForMultipleObjects(DWORD nCount,const pthread_mutex_t *lpHandles,int bWaitAll, DWORD dwMilliseconds);


//String macros
#define _strrev strrev
#ifdef _strdup
#undef _strdup
#endif
#define _strdup(x) x==NULL?NULL:strdup(x)
#define _stricmp strcasecmp
#define stricmp strcasecmp

//String function
char *_strrev(char *str);
char* _strupr( char* s );




//Time function
void GetSystemTime(struct _SYSTEMTIME *lp);
DWORD GetTickCount();


//Memory function
int _msize(void* p);

//error function
#define SetLastError(x) errno=x

//environment function
void _putenv(char* s);
#endif //LINUX_HEADER
