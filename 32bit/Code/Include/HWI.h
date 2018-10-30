/************************************************************************************
* Copyright (C) 2018                                                               *
* TETCOS, Bangalore. India                                                         *
*                                                                                  *
* Tetcos owns the intellectual property rights in the Product and its content.     *
* The copying, redistribution, reselling or publication of any or all of the       *
* Product or its content without express prior written consent of Tetcos is        *
* prohibited. Ownership and / or any other right relating to the software and all  *
* intellectual property rights therein shall remain at all times with Tetcos.      *
*                                                                                  *
* Author: Shashi Kant Suman
*																					*
* ---------------------------------------------------------------------------------*/
#pragma once

#define RCVSIZ 8192 //Bytes

typedef struct stru_HWI_CallBack
{
	int(*fnHWIConfigure)(void* xmlNode);
	int(*fnHWIStart)();
	int(*fnHWIStop)();
	bool(*fnHWISend)(NetSim_PACKET*);
	int(*fnCleanup)();
}HWICALLBACK,*ptrHWICALLBACK;
EXPORTED ptrHWICALLBACK hwiCallback;

//Function prototype
int fn_NetSim_Stack_RegisterHWI();
