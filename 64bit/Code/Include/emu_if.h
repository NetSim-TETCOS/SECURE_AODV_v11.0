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
* Author:    Shashi Kant Suman                                                     *
*                                                                                  *
* ---------------------------------------------------------------------------------*/
#ifndef _EMU_IF_H_
#define _EMU_IF_H_

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

# define _CRTDBG_MAP_ALLOC

#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "IPHLPAPI.lib") //IP helper library
#pragma comment(lib,"ws2_32.lib") //Winsock Library
#ifndef _EMU_LIB_CODE_
#pragma comment(lib,"emu_lib.lib") //Socket interface library
#endif

#ifdef  __cplusplus
extern "C" {
#endif

WSADATA wsaData; //variable to hold WSA

//Function prototype
void emu_socket_init();
SOCKET emu_create_socket();
void emu_start_server(SOCKET s,SOCKADDR_IN* con,unsigned short port);
int recvData(SOCKET s,char *recvbuf,int size);
int sendData(SOCKET s,char *sendbuf);
void closeConnection(SOCKET s);
void waitForClient(SOCKET* s,SOCKADDR* clientaddr,int* clientlen);
void emu_socket_close();

int emu_connectServer(SOCKET s, char *ip,int port);

#ifdef  __cplusplus
}
#endif
#endif //_EMU_IF_H_