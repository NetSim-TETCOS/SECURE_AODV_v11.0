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
#ifndef _NETSIM_PROTOCOL_H_
#define _NETSIM_PROTOCOL_H_
#ifdef  __cplusplus
extern "C" {
#endif
# define MAX_NAME_CHAR 50
# define MAX_CONTROL_PACKET 50
enum enum_isConfigured
{
	DISABLE=0,
	ENABLE=1,
};
enum dll_id
{
	libTCP=0,
	libIP,
	libUDP,
	libRIP,
	libOSPF,
	libDSR,
	libAODV,
	libWLAN,
	libEthernet,
	libZigbee,
	libGSM,
	libCDMA,
	libARP,
	libCR,
	libMobility,
	libApplication,
	libLTE,
	libTDMA,
	libDTDMA,
	libZRP,
	libOLSR,
	libAloha,
	libCSMACD,
	libToken_BR,
	libVanet,
	libWSMP,
	libRPL,
	libP2P,
	libSDN,
	nStaticProtocolCount, //Keep this at last
};
struct stru_ProtocolInfo
{
	int nComponentId;
	char dllName[MAX_NAME_CHAR];
	char protocolName[MAX_NAME_CHAR];
	int protocolId;
	ISCONFIGURED status;
	char pszControlPacketName[MAX_CONTROL_PACKET][MAX_NAME_CHAR];
};
#ifndef _NETSIM_CODE_
_declspec(dllimport) struct stru_ProtocolInfo* pstruProtocolInfo;
_declspec(dllimport) UINT nProtocolCount;
#else
#ifndef _NETSIM_PROTOCOL_INTERFACE_CODE_
extern struct stru_ProtocolInfo* pstruProtocolInfo;
extern UINT nProtocolCount;
#else
_declspec(dllexport) UINT nProtocolCount;
_declspec(dllexport) struct stru_ProtocolInfo* pstruProtocolInfo;
#endif
#endif
HINSTANCE* hProtocolDll;
/*****************************************************************************
	Common function for all protocol. These are the interface function present in 
	protocol dll by which stack interact with protocol.
******************************************************************************/
//Function Pointer for protocol main function
typedef int (*protocol_run)();		//Function name must be fn_NetSim_[Protocol Name]_run
protocol_run* fnpProtocol_Run;
//Function pointer for protocol initialization function
typedef int (*protocol_init)(const struct stru_NetSim_Network*,const struct stru_NetSim_EventDetails*,const char*,const char*,int);
protocol_init* fnpProtocol_Init; //Function name must be fn_NetSim_[Protocol Name]_init
//Function pointer for event trace
typedef char* (*protocol_trace)(int);	//Function name must be fn_NetSim_[Protocol Name]_trace
protocol_trace* fnpProtocol_Trace;
//Function pointer for copy packet
typedef int (*protocol_CopyPacket)(const NetSim_PACKET* pstruDestPacket,const NetSim_PACKET* pstruSrcPacket);
protocol_CopyPacket* fnpProtocol_CopyPacket;	//Function name must be fn_NetSim_[Protocol Name]_CopyPacket
//Function pointer for free packet
typedef int (*protocol_FreePacket)(const NetSim_PACKET* pstruPacket);
protocol_FreePacket* fnpProtocol_FreePacket;	//Function name must be fn_NetSim_[Protocol Name]_FreePacket
//Function pointer for metrics
typedef int (*protocol_Metrics)(const char* szMetricsFile);
protocol_Metrics* fnpProtocol_Metrics;		//Function name must be fn_NetSim_[Protocol Name]_Metrics
//Function pointer for finish
typedef int (*protocol_Finish)();
protocol_Finish* fnpProtocol_Finish;		//Function name must be fn_NetSim_[Protocol Name]_Finish
//Function pointer for configure
typedef int (*protocol_Configure)(void**);
protocol_Configure* fnpProtocol_Configure;		//Function name must be fn_NetSim_[Protocol Name]_Configure
//Function pointer to configure packet trace
typedef char* (*protocol_ConfigPacketTrace)(const void* xmlNetSimNode);
protocol_ConfigPacketTrace* fnpProtocol_ConfigPacketTrace;	//Function name must be fn_NetSim_[Protocol Name]_ConfigPacketTrace
//Function pointer to write packet trace
typedef int (*protocol_WritePacketTrace)(const NetSim_PACKET*,char**);
protocol_WritePacketTrace* fnpProtocol_WritePacketTrace;	//Function name must be fn_NetSim_[Protocol Name]_WritePacketTrace

_declspec(dllexport) int fnCallProtocol(int nProtocol);
_declspec(dllexport) int fn_NetSim_LoadProtocolDll(int nId,const char* name,const char* dllName);
#ifdef  __cplusplus
}
#endif
#endif


