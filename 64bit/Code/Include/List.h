/************************************************************************************
 * Copyright (C) 2013                                                               *
 * TETCOS, Bangalore. India                                                         *
 *                                                                                  *
 * Tetcos owns the intellectual property rights in the Product and its content.     *
 * The copying, redistribution, reselling or publication of any or all of the       *
 * Product or its content without express prior written consent of Tetcos is        *
 * prohibited. Ownership and / or any other right relating to the software and all *
 * intellectual property rights therein shall remain at all times with Tetcos.      *
 *                                                                                  *
 * Author:    Shashi Kant Suman                                                       *
 *                                                                                  *
 * ---------------------------------------------------------------------------------*/
#pragma once
#ifndef _NETSIM_LIST_H_
#define _NETSIM_LIST_H_
#ifdef  __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#ifdef _WIN32
#pragma comment(lib,"list.lib")
#endif

#define LIST_NEXT(ls) list_next(ls,ls->ele->offset)
#define LIST_ADD_LAST(ls,mem) list_add_last(ls,mem->ele->offset,mem)
#define LIST_FREE(ls,mem) list_free(ls,mem->ele->offset,mem)
#define LIST_ADD(ls,mem,checker) list_add(ls,mem,mem->ele->offset,checker)
#define LIST_REMOVE(ls,mem) list_remove(ls,mem->ele->offset,mem)
#define LIST_SORT(ls,check) list_sort(ls,(*ls)->ele->offset,check)
#define list_alloc(size,offset) list_alloc_dbg(size,offset,__LINE__,__FILE__)

#define BEGIN_LIST(LIST_NAME) typedef struct stru_##LIST_NAME
#define END_LIST(LIST_NAME) LIST_NAME,*ptr_##LIST_NAME; \
	static void* ##LIST_NAME##_ALLOC(){return list_alloc(sizeof(struct stru_##LIST_NAME),offsetof(struct stru_##LIST_NAME,ele));} \
	static struct stru_##LIST_NAME##* ##LIST_NAME##_NEXT(ptr_##LIST_NAME ls){return (ptr_##LIST_NAME)LIST_NEXT(ls);}

	typedef struct element {
		size_t offset;
		void* next;
		void *prev;
	} _ele,*_ptr_ele;



	__inline _ele* list_ele(void* list,size_t offset);
	void* list_alloc_dbg(size_t size,size_t offset,int line,char* file);
	__inline void* list_next(void* list,size_t offset);
	__inline void* list_prev(void* list,size_t offset);
	void list_add(void** list,void* mem,size_t offset,int (*check)(void* current,void* mem));
	void list_add_last(void** ls,size_t offset,void* mem);
	void list_add_top(void** list,size_t offset,void* mem);
	void list_free(void** list,size_t offset,void* mem);
	void list_sort(void** list,size_t offset,int (*check)(void* current, void* mem)); //Bubble sort
	void list_remove(void** list,size_t offset,void* mem);

#ifdef  __cplusplus
}
#endif
#endif
