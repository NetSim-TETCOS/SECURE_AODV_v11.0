#pragma once
/************************************************************************************
* Copyright (C) 2018                                                               *
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
#ifndef _NETSIM_CLIINTERFACE_H_
#define _NETSIM_CLIINTERFACE_H_
#ifdef  __cplusplus
extern "C" {
#endif

#ifndef _DONT_INCLUDE_CLI_LIB_
#pragma comment(lib,"libCLI.lib")
#endif

#ifndef NETSIM_ID
#define NETSIM_ID UINT
#endif

	typedef struct stru_cli_info
	{
		char* inputFileName;
		int(*fnHandleTimerEvent)();
	}CLIINFO, *ptrCLIINFO;

	/*
	A Struct for hold 2D-array with count items
	*/
	typedef struct stru_commandArray {
		char* originalCommand;
		char **commands;
		int length;
	} COMMANDARRAY, *ptrCOMMANDARRAY;

	typedef void* CLIHANDLE;
	_declspec(dllexport) ptrCOMMANDARRAY CLI_GET_CMDARRAY_FROM_HANDLE(CLIHANDLE handle);
	_declspec(dllexport) void CLI_SEND_MESSAGE(CLIHANDLE handle,
											   char* msg,
											   ...);
	_declspec(dllexport) void CLI_STOP_WAITING(CLIHANDLE handle);
	_declspec(dllexport) ptrCOMMANDARRAY remove_first_word_from_commandArray(ptrCOMMANDARRAY c);
	_declspec(dllexport) char* CLI_EXECUTE_COMMAND(ptrCOMMANDARRAY cmd,
												   NETSIM_ID d,
												   int* len,
												   bool(*multResp)(void*, char* msg, int len, bool isMore),
												   void* arg,
												   bool* isMore);
	_declspec(dllexport) void CLI_PRINT_MESSAGE(CLIHANDLE handle, char* msg, int len);

#ifdef  __cplusplus
}

#endif
#endif //_NETSIM_CLIINTERFACE_H_