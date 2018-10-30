 /************************************************************************************
 * Copyright (C) 2013                                                               *
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


#ifndef main_header_
#define main_header_
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#define _LICENSE_CHECK
#pragma warning(disable:4459)
#ifndef _DEBUG
#pragma warning(disable:4100)
#endif
/* Macro definition for memory leak testing. Must use visual studio.*/
# define _NETSIM_MEMORY_LEAK_TEST_ //Uncomment this line for memory leak testing
# define _CRTDBG_MAP_ALLOC
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
# include <errno.h>
# include <stdint.h>
#include <stdbool.h>
#include<malloc.h>
#include<assert.h>
#ifdef _WIN32
#ifdef _DEBUG
#include<crtdbg.h>
#endif
#include <windows.h>
#include <conio.h>
#include <process.h>
#else
#include <dlfcn.h>
#include "Linux.h"
#endif
#ifndef _NETSIM_MAIN_H_
#define _NETSIM_MAIN_H_
#ifdef  __cplusplus
extern "C" {
#endif
/*Global variable specific to OS -- Used for poratability*/
#ifdef _WIN32
# include <windows.h>
#ifndef _MSC_VER
#define _declspec(dllexport)
#endif
# define fnDlError(x,...) ReportError(x,__LINE__,__FILE__,1,__VA_ARGS__)
#define fnSystemError(x,...) ReportError(x,__LINE__,__FILE__,1,__VA_ARGS__)
# define fnNetSimError(x,...) ReportError(x,__LINE__,__FILE__,0,__VA_ARGS__)
static const char pathSeperator[]="\\";
static const char libext[]=".dll";
#ifdef _cplusplus
# define _declspec(dllexport) extern "C" _declspec(dllexport)
#endif
#else
#include <dlfcn.h>
#include<errno.h>
# define HINSTANCE void*
static const char libext[] = ".so";
#endif

#ifndef VERSION
#define VERSION 110
#endif

#define __CALLBACK__	// Indication of callback function
#define __IN__			// Indication of Input in function args
#define __OUT__			// Indication of Output in function args

#include "Memory.h"
#include "IP_Addressing.h"

#ifndef _NETSIM_IP_ADDRESSING_CODE_
#pragma comment(lib,"IP_Addressing.lib")
#endif

/* Typedef declaration of structure */
typedef struct stru_NetSim_SimulationParameter SIMULATIONPARAMETER;
typedef struct stru_NetSim_Device NetSim_DEVICE;
typedef struct stru_NetSim_Interface NetSIm_DEVICEINTERFACE;
typedef struct stru_NetSim_ApplicationLayer NetSim_APPLICATIONLAYER;
typedef struct stru_NetSim_SocketInterface NetSim_SOCKETINTERFACE;
typedef struct stru_NetSim_TransportLayer NetSim_TRANSPORTLAYER;
typedef struct stru_NetSim_Coordinates NetSim_COORDINATES;
typedef struct stru_NetSim_NetworkLayer NetSim_NETWORKLAYER;
typedef struct stru_NetSim_MAC_NW_Interface NetSim_MAC_NW_INTERFACE;
typedef struct stru_NetSim_MacLayer NetSim_MACLAYER;
typedef struct stru_NetSim_PhysicalLayer NetSim_PHYSICALLAYER;
typedef struct stru_ApplicationInformation NetSim_APPLICATIONINFO;
typedef struct stru_NetSim_Device_Buffer NetSim_BUFFER;
typedef struct stru_NetSim_Packet NetSim_PACKET;
typedef struct stru_NetSim_Packet_PhyLayer NetSim_PACKET_PHYLAYER;
typedef struct stru_NetSim_Packet_MACLayer NetSim_PACKET_MACLAYER;
typedef struct stru_NetSim_Packet_NetworkLayer NetSim_PACKET_NETWORKLAYER;
typedef struct stru_NetSim_Packet_TransportLayer NetSim_PACKET_TRANSPORTLAYER;
typedef struct stru_NetSim_Packet_AppLayer NetSim_PACKET_APPLAYER;
typedef struct stru_NetSim_EventDetails NetSim_EVENTDETAILS;
typedef struct stru_NetSim_Links NetSim_LINKS;

/* typedef for enumerator */
typedef enum enum_VideoModel VIDEO_MODEL;
typedef enum enum_Distribution DISTRIBUTION;
typedef enum enum_PacketType PACKET_TYPE;
typedef enum enum_SuppressionModel SUPPRESSION_MODEL;
typedef enum enum_ServiceType SERVICE_TYPE;
typedef enum enum_PacketPriority PACKET_PRIORITY;
typedef enum enum_Packet_QOS QUALITY_OF_SERVICE;
typedef enum enum_PacketStatus PACKET_STATUS;
typedef enum enum_Segment PACKET_FRAGMENT;
typedef enum enum_EventType EVENT_TYPE;
typedef enum enum_AppProtocol APPLICATION_LAYER_PROTOCOL;
typedef enum enum_TrxProtocol TRANSPORT_LAYER_PROTOCOL;
typedef enum enum_NWProtocol NETWORK_LAYER_PROTOCOL;
typedef enum enum_MACProtocol MAC_LAYER_PROTOCOL;
typedef enum enum_PhyMedium PHYSICAL_LAYER_MEDIUM;
typedef enum enum_LayerType LAYER_TYPE;
typedef enum enum_InterfaceType INTERFACE_TYPE;
typedef enum enum_DeviceType netsimDEVICE_TYPE; //Because windows.h has already have the definition of DEVICE_TYPE
typedef enum enum_LinkType LINK_TYPE;
typedef enum enum_LinkMode LINK_MODE;
typedef enum enum_LinkState LINK_STATE;
typedef enum enum_SimulationEndType SIMULATION_END_TYPE;
typedef enum enum_SchedulingType SCHEDULING_TYPE;
typedef enum enum_TransmissionType TRANSMISSION_TYPE;
typedef enum enum_isConfigured ISCONFIGURED;
typedef enum emum_wireshark_option WIRESHARK_OPTION;

//typedef for netsim special variable type
#ifndef NETSIM_ID
	typedef unsigned int NETSIM_ID; //unique id
#endif 
typedef char* NETSIM_Name;	//Unique name
typedef char* string;
typedef enum enum_Flag FLAG;

//Definition of version type
typedef enum
{
	NETSIM_PRO=VERSION*10+5,
	NETSIM_STANDARD=VERSION*10+4,
	NETSIM_ACEDEMIC=VERSION*10+3,
	NETSIM_DEMO=VERSION*10+2,
	NETSIM_COMER=VERSION*10+1,
}NETSIM_VERSIONTYPE;

# include "Stack.h"
# include "Packet.h"
# include "MetricsWriter.h"
#define fn_NetSim_LoadAppDllFunction(hsharedlib,path,sharedlibName,funName) fn_NetSim_LoadAppDllFunction_dbg(hsharedlib,path,sharedlibName,funName,__LINE__,__FILE__);
_declspec(dllexport) FARPROC fn_NetSim_LoadAppDllFunction_dbg(HINSTANCE* hdll, const char* path, const char* dllName, const char* funName, int line, char* file);

# define fn_NetSim_Utilities_LoadSharedlibFunction(hsharedlib,sharedlibName,funName) fn_NetSim_Utilities_LoadSharedlibFunction_dbg(hsharedlib,sharedlibName,funName,__LINE__,__FILE__)
_declspec(dllexport) FARPROC fn_NetSim_Utilities_LoadSharedlibFunction_dbg(HINSTANCE* hsharedlib,const char* sharedlibName,const char* funName,int line,const char* file);

_declspec(dllexport) int ReportError(const char* msg,int line,const char* file,int flag,...);
#pragma warning(disable:4213)
#define fn_NetSim_Packet_FreePacket(pstruPacket) fn_NetSim_Packet_FreePacket_dbg(&(pstruPacket),__LINE__,__FILE__)

#define fn_NetSim_Packet_AddPacketToList(pstruBuffer,pstruPacket,nInsertionType /* 1- head,2-Tail,3-Order of event time*/) fn_NetSim_Packet_AddPacketToList_dbg(pstruBuffer,pstruPacket,nInsertionType /* 1- head,2-Tail,3-Order of event time*/,__LINE__,__FILE__)
_declspec(dllexport) int fn_NetSim_Packet_AddPacketToList_dbg(NetSim_BUFFER* pstruBuffer,NetSim_PACKET* pstruPacket, int nInsertionType /* 1- head,2-Tail,3-Order of event time*/,int line,const char* file);

#define fn_NetSim_Packet_CopyPacket(pstruPacket) fn_NetSim_Packet_CopyPacket_dbg(pstruPacket,__LINE__,__FILE__)

#define fn_NetSim_Packet_CopyPacketList(pstruPacket) fn_NetSim_Packet_CopyPacketList_dbg(pstruPacket,__LINE__,__FILE__)

#define fn_NetSim_Packet_CreatePacket(layer) fn_NetSim_Packet_CreatePacket_dbg(layer,__LINE__,__FILE__)

#define fn_NetSim_WritePacketTrace(pstruPacket) fn_NetSim_WritePacketTrace_dbg(pstruPacket,__LINE__,__FILE__)

#define fnpAddEvent(pstruEvent) fnAddEvent_F(pstruEvent,__LINE__,__FILE__)
_declspec(dllexport) long long int fnAddEvent_F(struct stru_NetSim_EventDetails* pstruEvent,int line,const char* file);

_declspec(dllexport) bool fn_NetSim_Stack_RegisterCallBackFunction(int nSubeventId,int (*_fnCallBack)());

_declspec(dllexport) int fn_NetSim_Anim_MoveNode(NETSIM_ID src,double x,double y,double time);

//Used to convert Lat/Lon to X,Y,Z
_declspec(dllexport) int convert_lat_lon_to_3D(double lat,double lon,NetSim_COORDINATES* pos);
_declspec(dllexport) int convert_3D_to_lat_lon(NetSim_COORDINATES* pos);

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define ANSI_BOLD_TEXT		"\x1b[1m"

_declspec(dllexport) bool CHECK_FOR_USER_DLL(const char* dllName);


#ifdef _WIN32
#ifndef _NETSIM_CODE_
#define printf(x,...) {printf("\n%s:%d:  ",__FILE__,__LINE__);printf(x,__VA_ARGS__);}
#endif
#endif
#endif

#ifdef  __cplusplus
}
#endif
#endif //_NETSIM_MAIN_H_
