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
#ifndef _NETSIM_STACK_H_
#define _NETSIM_STACK_H_
#ifdef  __cplusplus
extern "C" {
#endif
#include "List.h"
#include "Wireless.h"

#ifdef _WIN32
#ifdef _NETSIM_CODE_
#define EXPORTED _declspec(dllexport)
#else
#define EXPORTED _declspec(dllimport)
#endif
#ifdef _NETSIM_METRIC_CODE_
#undef EXPORTED
#define EXPORTED
#endif
#else
#ifdef _NETSIM_CODE_
#define EXPORTED
#else
#define EXPORTED extern
#endif
#ifdef _NETSIM_METRIC_CODE_
#undef EXPORTED
#define EXPORTED
#endif

#endif
EXPORTED int nContinueFlag;

//Time Definition
#define MICROSECOND (1)
#define NANOSECOND	(MICROSECOND/1000.0)
#define MILLISECOND	(1000*MICROSECOND)
#define SECOND		(1000*MILLISECOND)
#define MINUTE		(60*SECOND)
#define HOUR		(60*MINUTE)


#ifndef INFINITY
#define INFINITY (0xFFFFFFFF)
#endif
#ifndef NEGATIVE_INFINITY
#define NEGATIVE_INFINITY (-1.0*INFINITY)
#endif


/* Enumeration for flag*/
enum enum_Flag
{
	ZERO,
	ONE,
};

typedef enum enum_sim_state
{
	SIMSTATE_NOTSTARTED,
	SIMSTATE_PAUSED,
	SIMSTATE_RUNNING,
	SIMSTATE_STOPPED,
}SIMSTATE;
_declspec(dllexport) void netsim_set_simstate(SIMSTATE state);
_declspec(dllexport) SIMSTATE netsim_get_simstate();

/* Enumeration for Layer type*/
enum enum_LayerType
{
	LAYER_NULL=0,
	PHYSICAL_LAYER = 1, //Interface layer in linux
	MAC_LAYER,			//Protocol layer in linux
	NETWORK_LAYER,		//Socket layer in linux
	TRANSPORT_LAYER,	//Socket layer in linux
	APPLICATION_LAYER,	//Process layer in linux
};
/* Enumeration for event type*/
enum enum_EventType
{
	PHYSICAL_OUT_EVENT = PHYSICAL_LAYER*10+1,
	PHYSICAL_IN_EVENT = PHYSICAL_LAYER*10+2,
	MAC_OUT_EVENT = MAC_LAYER*10+1,
	MAC_IN_EVENT = MAC_LAYER*10+2,
	NETWORK_OUT_EVENT = NETWORK_LAYER*10+1,
	NETWORK_IN_EVENT = NETWORK_LAYER*10+2,
	TRANSPORT_OUT_EVENT	= TRANSPORT_LAYER*10+1,
	TRANSPORT_IN_EVENT = TRANSPORT_LAYER*10+2,
	APPLICATION_OUT_EVENT =	APPLICATION_LAYER*10+1,
	APPLICATION_IN_EVENT = APPLICATION_LAYER*10+2,
	TIMER_EVENT = 1,
	INTERNAL_NETSIM_EVENT=2,
};

typedef enum enum_common_subevent
{
	SUBEVENT_GETLINKQUALITY= TIMER_EVENT*100+1, //Must be provided by link layer protocol to get link quality
	SUBEVENT_JOIN_MULTICAST_GROUP,
	SUBEVENT_LINKUP,
	SUBEVENT_LINKDOWN,
	SUBEVENT_HWIRECV,
	SUBEVENT_HWI_CLEANUP,
	SUBEVENT_PAUSESIMULATION,
	SUBEVENT_EXECUTECOMMAND,
	SUBEVNET_LAST,
}COMMON_SUBEVENT;

static char subeventName[SUBEVNET_LAST][100] = { "GET_LINK_QUALITY",
"JOIN_MULTICAST_GROUP",
"LINKUP",
"LINKDOWN",
"HWIRECV",
"HWI_CLEANUP",
"PAUSE_SIMULATION",
"EXECUTE_COMMAND"};

///Internal module
#define PROTOCOL_METRICS		2
#define PROTOCOL_MOBILITY		3
#define PROTOCOL_APPLICATION	4
#define PROTOCOL_LINK			5
#define PROTOCOL_HWI			6
#define PROTOCOL_CLI			7

/* Enumeration for Application layer protocol*/
enum enum_AppProtocol
{
	APP_PROTOCOL_NULL = 0,

	// Routing protocol
	APP_PROTOCOL_RIP = APPLICATION_LAYER*100+1,//=501,
	APP_PROTOCOL_OSPF,

	//Application
	APP_PROTOCOL_OPENFLOW,
	APP_PROTOCOL_CBR=PROTOCOL_APPLICATION,
	APP_PROTOCOL_VOICE=PROTOCOL_APPLICATION,
	APP_PROTOCOL_FTP=PROTOCOL_APPLICATION,
	APP_PROTOCOL_HTTP=PROTOCOL_APPLICATION,
	APP_PROTOCOL_BSM=PROTOCOL_APPLICATION,
	APP_PROTOCOL_COAP=PROTOCOL_APPLICATION,
	APP_PROTOCOL_DATABASE=PROTOCOL_APPLICATION,
	APP_PROTOCOL_CUSTOM=PROTOCOL_APPLICATION,
	APP_PROTOCOL_EMAIL=PROTOCOL_APPLICATION,
	APP_PROTOCOL_ERLANGCALL=PROTOCOL_APPLICATION,
	APP_PROTOCOL_VIDEO=PROTOCOL_APPLICATION,
	APP_PROTOCOL_EMULATION=PROTOCOL_APPLICATION,
};

/* Enumeration for transport layer protocol*/
enum enum_TrxProtocol
{
	TX_PROTOCOL_NULL = 0,
	TX_PROTOCOL_TCP = TRANSPORT_LAYER*100+1,
	TX_PROTOCOL_UDP,
	TX_PROTOCOL_WSMP,  //IEEE1609.3
};

/* Enumeration for network layer protocol*/
enum enum_NWProtocol
{
	/* IP protocol*/
	NW_PROTOCOL_IPV4 = NETWORK_LAYER*100+1,
	NW_PROTOCOL_IPV6,
	NW_PROTOCOL_ARP,
	NW_PROTOCOL_ICMP,
	NW_PROTOCOL_IGMP,

	/* Routing Protocol*/
	NW_PROTOCOL_DSR,
	NW_PROTOCOL_AODV,
	NW_PROTOCOL_ZRP,
	NW_PROTOCOL_OLSR,
	NW_PROTOCOL_RPL, //RFC 6550
};

/* Enumeration for MAC layer protocol*/
enum enum_MACProtocol
{
	MAC_PROTOCOL_NULL=0,
	/***WIRELESS***/
	MAC_PROTOCOL_IEEE802_11 = MAC_LAYER*100+1,	//WLAN
	MAC_PROTOCOL_IEEE802_15_4,		//Zigbee
	MAC_PROTOCOL_IEEE802_22,		//Cognitive Radio

	/***WIRED***/
	MAC_PROTOCOL_IEEE802_3,			//Ethernet
	MAC_PROTOCOL_P2P,				//Point to point link

	/***Cellular Protocol***/
	MAC_PROTOCOL_CDMA,				//CDMA
	MAC_PROTOCOL_GSM,				//GSM
	MAC_PROTOCOL_LTE,				//LTE

	/***/
	MAC_PROTOCOL_TDMA,				//TDMA
	MAC_PROTOCOL_DTDMA,				//Dynamic TDMA

	MAC_PROTOCOL_ALOHA,
	MAC_PROTOCOL_CSMACD,
	MAC_PROTOCOL_TOKEN_RING_BUS,

	MAC_PROTOCOL_IEEE1609,				//IEEE 1609, WAVE, VANET
};


/*Enumeration for physical medium type. Must be compatible with MAC layer*/
enum enum_PhyMedium
{
	PHY_MEDIUM_NULL = 0,
	PHY_MEDIUM_WIRELESS = 1,
	PHY_MEDIUM_WIRED = 2,
};

/*Enumeration for interface type. Must be compatible with network layer, MAC layer and physical medium*/
enum enum_InterfaceType
{
	INTERFACE_NULL=0,
	INTERFACE_LAN_802_3=1,	//Ethernet
	INTERFACE_LAN_802_11,	//WLAN b	
	INTERFACE_LAN_802_15_4,	//Zigbee
	INTERFACE_LAN_802_22,	//Cognitive radio
	INTERFACE_WAN_ROUTER,	//WAN
	INTERFACE_VIRTUAL,		//Virtual interface
	INTERFACE_GSM,			//GSM Interface
	INTERFACE_CDMA,			//CDMA Interface
	INTERFACE_LTE,			//LTE Interface
	INTERFACE_ALOHA,		// Aloha Interface
	INTERFACE_TOKEN_RING,	//Token ring interface
	INTERFACE_TOKEN_BUS,	//Token bus interface
	INTERFACE_IEEE1609,		// WAVE, VANET Interface
};

/* Enumeration for device type*/
enum enum_DeviceType
{
	Not_Device=0,
	/* Node 5-layer device */
	NODE=1,			//Wired or wireless
	CPE=1,			//Wired
	MOBILESTATION=1,//Wireless
	UE=1,			//Wireless LTE user equipment
	SENSOR=1,		//Wireless
	SINKNODE=1,		//Wireless
	PANCOORDINATOR=1,	//Wireless
	RSU=1,			// Road side Unit, wireless

	/***Intermediate Device***/
	/* 2-layer device*/
	SWITCH=2,		//Wired
	HUB=3,			//Wired
	CONCENTRATOR=3,
	BASESTATION=4,	//Wireless
	eNB=4,			//Wireless LTE base station
	ACCESSPOINT=5,	//Wireless

	/*3-Layer device*/
	ROUTER=6,		//Wired and/or wireless
	MME=6,			//LTE Mobile management entity
	HNB_GATEWAY=6,  //LTE Femtocell Gateway

	RELAY=7,		// LTE Relay
	MSC=8,			//Mobile Switching center
};

/* Enumeration for link type */
enum enum_LinkType
{
	LinkType_NULL=0,
	LinkType_P2P=1,	//Point to point
	LinkType_P2MP=2,//Point to multipoint
	LinkType_MP2MP=3,//multipoint to multipoint
};

/* Enumeration for link state */
enum enum_LinkState
{
	LINKSTATE_UP,
	LINKSTATE_DOWN,
};

/* Enumerator for link mode */
enum enum_LinkMode
{
	LinkMode_NULL=0,
	LinkMode_HalfDuplex=1,
	LinkMode_FullDuplex=2,
};

/* Enumeration for Simulation exit type*/
enum enum_SimulationEndType
{
	SimEndType_TIME=1,			//End simulation after specified time
	
	//NOT SUPPORTED
	SimEndType_NUMOFEVENT,		//End simulation after specified number of event execution
	SimEndType_NUMOFPACKET,	//End simulation after specified number of packet transfered
};

/*Enumeration for scheduling type*/
enum enum_SchedulingType
{
	SCHEDULING_NONE=0,
	SCHEDULING_FIFO=1,
	SCHEDULING_PRIORITY=2,
	SCHEDULING_ROUNDROBIN=3,
	SCHEDULING_WFQ=4,
};
enum enum_TransmissionType
{
	UNICAST,
	BROADCAST,
	MULTICAST,
};

/* enumeration for wireshark option */
enum emum_wireshark_option
{
	wireshark_disable=0,
	wireshark_online,
	wireshark_offline,
};

/*enumeration for Node action */
typedef enum enum_NodeAction
{
	NOT_CONNECTED,
	CONNECTED,
	JOIN,
	LEAVE,	
}NODE_ACTION;

/* Structure of mac address*/
typedef struct stru_macaddress
{
	unsigned long long int nmacaddress;
	char szmacaddress[13];
	char szBinary[50];
}NETSIM_MACADDRESS,*PNETSIM_MACADDRESS;

EXPORTED PNETSIM_MACADDRESS BROADCAST_MAC;

/*Coordinate of point in 3-d space*/
struct stru_NetSim_Coordinates
{
	bool ismap;
	double lat;
	double lon;
	double X;
	double Y;
	double Z;
};

/*Store the simulation parameter*/
struct stru_NetSim_SimulationParameter
{
	SIMULATION_END_TYPE nSimulationExitType;//Simulation exit type
	double dVal;	//Value at which simulation end occurs for a particular simulation end type.
					//For example Time = 100000 Sec.
};

/* Socket Interface*/
typedef struct stru_NetSim_SocketInterface
{
	APPLICATION_LAYER_PROTOCOL appProtocol;
	TRANSPORT_LAYER_PROTOCOL trxProtocol;
	UINT16 localPortNumber;
	UINT16 remotePortNumber;
	struct stru_NetSim_Device_Buffer* pstruSocketBuffer;
	struct stru_NetSim_SocketInterface* next;
}SOCKETINTERFACE, *ptrSOCKETINTERFACE;
_declspec(dllexport) ptrSOCKETINTERFACE fn_NetSim_Socket_CreateNewSocket(NETSIM_ID deviceId,
																		 int appProtocol,
																		 TRANSPORT_LAYER_PROTOCOL trxProtocol,
																		 UINT16 localPort,
																		 UINT16 remotePort);
_declspec(dllexport) ptrSOCKETINTERFACE fn_NetSim_Socket_GetSocketInterface(NETSIM_ID deviceId,
																			APPLICATION_LAYER_PROTOCOL appProtocol,
																			TRANSPORT_LAYER_PROTOCOL trxProtocol,
																			UINT16 localPort,
																			UINT16 remotePort);
_declspec(dllexport) void fn_NetSim_Socket_PassPacketToInterface(NETSIM_ID deviceId,
																 NetSim_PACKET* packet,
																 ptrSOCKETINTERFACE socketInterface);


/* Stores the application layer of device*/
struct stru_NetSim_ApplicationLayer
{
	LAYER_TYPE nLayerType; //Layer type
	UINT nappProtocolCount;
	int* nAppProtocol; //Application layer protocol list
	void** appVar;	//Application protocol variable
};
_declspec(dllexport) void* fn_NetSim_Stack_RegisterNewApplicationProtocol(NETSIM_ID deviceId,
																		  UINT appProtocol);
_declspec(dllexport) void* fn_NetSim_Stack_GetAppProtocolData(NETSIM_ID deviceId,
															  UINT appProtocol);
_declspec(dllexport) void fn_NetSim_Stack_SetAppProtocolData(NETSIM_ID deviceId,
															  UINT appProtocol,
															  void* data);
_declspec(dllexport) bool fn_NetSim_Socket_GetBufferStatus(ptrSOCKETINTERFACE s);
_declspec(dllexport) NetSim_PACKET* fn_NetSim_Socket_GetPacketFromInterface(ptrSOCKETINTERFACE s,
																			int moveFlag);

/* Stores the transport layer of device*/
struct stru_NetSim_TransportLayer
{
	unsigned int isTCP:1;	//Flag to enable or disable TCP
	unsigned int isUDP:1;	//Flag to enable or disable UDP;
	TRANSPORT_LAYER_PROTOCOL nTransportLayerProtocol; //Other than TCP and UDP
	LAYER_TYPE nLayerType; //Layer type	
	double dMSS; //Updated by tx layer protocol
	void* TCPVar; //Pointer for TCP variable
	void* UDPVar;//Pointer for UDP variable
	void* TransportProtocolVar; //Pointer to transport layer protocol
};

typedef struct stru_ip_wrapper
{
	bool isInit;
	void* handle; //Animation handle
	void* table; //IP routing tables
}IP_WRAPPER, *ptrIP_WRAPPER;

/*Stores the network layer of device*/
struct stru_NetSim_NetworkLayer
{
	LAYER_TYPE nLayerType; //Layer type
	NETWORK_LAYER_PROTOCOL nRoutingProtocolId; //Routing protocol id
	void* RoutingVar; //Routing variable
	int (*routerFunction)(); //Routing function used by app layer routing protocol
							 // like RIP, OSPF	
	ptrIP_WRAPPER ipWrapper;
	void* ipVar; //IP configuration variable
	double dMaxPacketLen;
};

/*Stores the MAC layer of device*/
struct stru_NetSim_MacLayer
{
	LAYER_TYPE nLayerType;	//Layer type
	MAC_LAYER_PROTOCOL nMacProtocolId;	//Protocol id
	PNETSIM_MACADDRESS szMacAddress; //MAC address of the device
	bool isWiresharkWriter;
	double dFragmentSize; //Updated by mac layer protocol
	void* MacVar;	//MAC protocol variable
};

/*Stores the physical layer of device*/
struct stru_NetSim_PhysicalLayer
{
	LAYER_TYPE nLayerType; //Layer type
	PHYSICAL_LAYER_MEDIUM nPhyMedium;	//Physical medium type. Must be compatible with mac layer
	NETSIM_ID nLinkId;	//Connected link id
	struct stru_NetSim_Links* pstruNetSimLinks; //Pointer to connected link
	void* phyVar; //Phy layer variable
	double dLastPacketEndTime; //Last packet end time

	union
	{
		//For wireless
		PHYPROP_WIRELESS wireless;
		//For Wired
		struct 
		{
			int noProperty;
		}WIRED;
	}phyProp;
};

/* Stores the buffer information of device*/
struct stru_NetSim_Device_Buffer
{
	double dMaxBufferSize;	//Max buffer size in bytes
	double dCurrentBufferSize;	//Current buffer size in bytes
	SCHEDULING_TYPE nSchedulingType;	//Scheduling type
	unsigned int nQueuedPacket;
	unsigned int nDequeuedPacket;
	unsigned int nDroppedPacket;
	struct stru_NetSim_Packet* pstruPacketlist;	//Packet list
	struct stru_NetSim_Packet* last;
};

/* Stores the MAC-NW interface of device*/
struct stru_NetSim_Access_Interface
{
	struct stru_NetSim_Device_Buffer* pstruAccessBuffer; //MAC-NW buffer
};

/* structure for node join profile */
typedef struct stru_NodeJoinProfile
{
	NETSIM_ID NodeId;
	double time;
	NODE_ACTION action;
	_ele* ele;
}NODE_JOIN_PROFILE,*PNODE_JOIN_PROFILE;
#define JOIN_PROFILE_ALLOC()				(PNODE_JOIN_PROFILE)list_alloc(sizeof(NODE_JOIN_PROFILE),offsetof(NODE_JOIN_PROFILE,ele))
#define JOIN_PROFILE_NEXT(profile)			profile=(PNODE_JOIN_PROFILE)LIST_NEXT(profile)
#define JOIN_PROFILE_ADD(lprofile,profile)	LIST_ADD_LAST((void**)lprofile,profile)

/* stores the links information*/
struct stru_NetSim_Links
{
	NETSIM_ID nConfigLinkId; //Link id setted in config file
	NETSIM_ID nLinkId;
	LINK_TYPE nLinkType;
	PHYSICAL_LAYER_MEDIUM nLinkMedium;
	LINK_MODE nLinkMode;
	union uni_NetSim_MediumProp
	{
		struct stru_NetSim_WiredLink
		{
			double dDistance;
			double dPropagationDelayUp;
			double dDataRateUp;
			double dErrorRateUp;
			double dPropagationDelayDown;
			double dDataRateDown;
			double dErrorRateDown;
		}pstruWiredLink;
		struct stru_NetSim_wirelessLink
		{
			double dDataRateUp;
			double dDataRateDown;
			PPROPAGATION propagation;
		}pstruWirelessLink;
	}puniMedProp;
	union uni_NetSim_Link_DeviceList
	{
		struct stru_NetSim_P2P
		{
			NETSIM_ID nFirstDeviceId;
			NETSIM_ID nFirstInterfaceId;
			NETSIM_ID nSecondDeviceId;
			NETSIM_ID nSecondInterfaceId;
		}pstruP2P;
		struct stru_NetSim_P2MP
		{
			NETSIM_ID nConnectedDeviceCount;//Count of connected device through this wireless link
			NETSIM_ID nCenterDeviceId;
			NETSIM_ID nCenterInterfaceId;
			NETSIM_ID* anDevIds;	//List of all the device belong to this wireless medium
			NETSIM_ID* anDevInterfaceIds;
		}pstrup2MP;
		struct stru_NetSim_MP2MP
		{
			NETSIM_ID nConnectedDeviceCount;//Count of connected device through this wireless link
			NETSIM_ID* anDevIds;	//List of all the device belong to this shared medium
			NETSIM_ID* anDevInterfaceIds;
		}pstruMP2MP;
	}puniDevList;
	bool isLinkFailureMode;
	struct stru_linkFailureModel
	{
		LINK_STATE linkState;
		unsigned int nLinkUpCount;
		double* dLinkUPTime;
		unsigned int nLinkDownCount;
		double* dLinkDownTime;
		int(*fnLinkFailureModelCallBack)(NETSIM_ID, LINK_STATE);
	}struLinkFailureModel;
	struct stru_NetSim_Links* pstruNextLink;
	struct stru_NetSim_Links* pstruPreviousLink;
};
/*Stores the interface information of device*/
struct stru_NetSim_Interface
{
	NETSIM_ID nInterfaceId; //Interface id. Unique for each interface for a device.
	NETSIM_ID nConfigInterfaceId;
	INTERFACE_TYPE nInterfaceType; //Interface type
	NETWORK_LAYER_PROTOCOL nProtocolId; //IP protocol id
	NETSIM_IPAddress szAddress; //IP address of the interface
	NETSIM_IPAddress publicIP;
	NETSIM_IPAddress szSubnetMask; //Subnet mask of the interface in case of IPV4
	unsigned int prefix_len; //Prefix length in case of IPV6
	NETSIM_IPAddress szDefaultGateWay; //Default gateway
	void* ipVar;
	void* interfaceVar;	//Variable used to store the common interface variable.
	NETWORK_LAYER_PROTOCOL nLocalNetworkProtocol;
	struct stru_NetSim_Access_Interface* pstruAccessInterface; //Network-MAC interface
	struct stru_NetSim_MacLayer* pstruMACLayer; //MAC layer of interface
	struct stru_NetSim_PhysicalLayer* pstruPhysicalLayer; //Physical layer of interface
	struct stru_NetSim_Interface* pstruNextInterface;
	struct stru_NetSim_Interface* pstruPrevInterface;
};

#define SEED_COUNT 2
/* Stores the device information in network*/
struct stru_NetSim_Device
{
	NETSIM_ID nConfigDeviceId; //Device id setted in the config file
	NETSIM_ID nDeviceId;		//Unique id for all type of device. Auto assigned by backend.This is mapped to device name from front end
	netsimDEVICE_TYPE nDeviceType;	//Type of device
	NETSIM_Name type;
	NETSIM_Name szDeviceName;			//Name of the device in GUI
	NETSIM_ID nNumOfInterface;	//Number of interface in device
	unsigned long ulSeed[SEED_COUNT];//Unique seed
	void* deviceVar;	//Variable used to store to device specific variable. Common for all interface
	struct stru_NetSim_Interface** ppstruInterfaceList; //Interface list
	struct stru_NetSim_ApplicationLayer* pstruApplicationLayer;//Application layer of device
	ptrSOCKETINTERFACE pstruSocketInterface; // Socket interface of device
	struct stru_NetSim_TransportLayer* pstruTransportLayer;	//Transport layer of device
	struct stru_NetSim_NetworkLayer* pstruNetworkLayer; //Network layer of device
	struct stru_NetSim_Coordinates* pstruDevicePosition;	//Position of device in simulation workspace
	struct stru_NetSim_Mobility* pstruDeviceMobility;		//Mobility information of mobile device
	WIRESHARK_OPTION wireshark_option;
	PNODE_JOIN_PROFILE node_join_profile;
	NODE_ACTION node_status;
	struct stru_NetSim_Device* pstruNextDevice;
	struct stru_NetSim_Device* pstruPrevDevice;
};

/* Store the Network structure*/
struct stru_NetSim_Network
{
	UINT nDeviceCount;	//Total number of device
	UINT nLinkCount;	//Total number of links in network
	UINT nApplicationCount; //Total number of application running
	struct stru_NetSim_Device** ppstruDeviceList;	//List of all device in network
	struct stru_NetSim_Links** ppstruNetSimLinks;
	void** appInfo;	//Information of application. Refer ../ApplicationDll/Application.h
	struct stru_NetSim_SimulationParameter* pstruSimulationParameter;//Simulation parameter for end type and value
};
EXPORTED struct stru_NetSim_Network* NETWORK;
struct stru_NetSim_EventDetails
{
	unsigned long long int nEventId; //Unique id of event
	double dEventTime; //Event time when packet is processed.
	EVENT_TYPE nEventType; //Type of event
	NETSIM_ID nProtocolId;
	netsimDEVICE_TYPE nDeviceType; //Type of device
	NETSIM_ID nDeviceId; //Unique id of device
	NETSIM_ID nInterfaceId; //Unique id of interface
	NETSIM_ID nApplicationId; //Unique id of application
	double dPacketSize; //Packet size of current event
	struct stru_NetSim_Packet *pPacket; //Packet for current event
	long long int nPacketId; //Unique id for packet
	int nSegmentId;
	NETSIM_ID nSubEventType; //Sub event type
	void* szOtherDetails; //Details specific to protocol.
	unsigned long long int nPrevEventId;
	int line;
	char* file;
};

/* Stack API */

/*MACROS*/
#define DEVICE(DeviceId)							NETWORK->ppstruDeviceList[DeviceId-1]
 #ifdef DEVICE_TYPE
 #undef DEVICE_TYPE
 #endif
#define DEVICE_TYPE(DeviceId)							DEVICE(DeviceId)->nDeviceType
#define DEVICE_NAME(DeviceId)							DEVICE(DeviceId)->szDeviceName
#define DEVICE_VAR(DeviceId)							DEVICE(DeviceId)->deviceVar
#define DEVICE_SEED0(DeviceId)							&(DEVICE(DeviceId)->ulSeed[0])
#define DEVICE_SEED1(DeviceId)							&(DEVICE(DeviceId)->ulSeed[1])
#define DEVICE_SEED(DeviceId)							DEVICE_SEED0(DeviceId),DEVICE_SEED1(DeviceId)
#define DEVICE_CONFIGID(DeviceId)						DEVICE(DeviceId)->nConfigDeviceId
#define DEVICE_INTERFACE(DeviceId,InterfaceId)			DEVICE(DeviceId)->ppstruInterfaceList[InterfaceId-1]
#define DEVICE_INTERFACE_CONFIGID(DeviceId,InterfaceId)	DEVICE_INTERFACE(DeviceId,InterfaceId)->nConfigInterfaceId
#define DEVICE_APPLAYER(DeviceId)						DEVICE(DeviceId)->pstruApplicationLayer
#define DEVICE_APPVAR(DeviceId,protocolId)				(fn_NetSim_Stack_GetAppProtocolData(DeviceId,protocolId))
#define DEVICE_TRXLayer(DeviceId)					DEVICE(DeviceId)->pstruTransportLayer
#define DEVICE_NWLAYER(DeviceId)					DEVICE(DeviceId)->pstruNetworkLayer
#define DEVICE_MACLAYER(DeviceId,InterfaceId)		DEVICE_INTERFACE(DeviceId,InterfaceId)->pstruMACLayer
#define DEVICE_PHYLAYER(DeviceId,InterfaceId)		DEVICE_INTERFACE(DeviceId,InterfaceId)->pstruPhysicalLayer
#define DEVICE_LINK(DeviceId,InterfaceId)			(DEVICE_PHYLAYER(DeviceId,InterfaceId)->pstruNetSimLinks)
#define DEVICE_POSITION(DeviceId)					DEVICE(DeviceId)->pstruDevicePosition
#define DEVICE_MOBILITY(DeviceId)					DEVICE(DeviceId)->pstruDeviceMobility
#define DEVICE_SOCKETINTERFACE(DeviceId)			DEVICE(DeviceId)->pstruSocketInterface
#define DEVICE_MAC_NW_INTERFACE(DeviceId,InterfaceId) DEVICE_INTERFACE(DeviceId,InterfaceId)->pstruAccessInterface
#define DEVICE_ACCESSBUFFER(DeviceId,InterfaceId)	DEVICE_MAC_NW_INTERFACE(DeviceId,InterfaceId)->pstruAccessBuffer
#define DEVICE_MYACCESSBUFFER()						DEVICE_ACCESSBUFFER(pstruEventDetails->nDeviceId,pstruEventDetails->nInterfaceId)
#define DEVICE_PHYMEDIUM(DeviceId,InterfaceId)		DEVICE_PHYLAYER(DeviceId,InterfaceId)->nPhyMedium
#define	DEVICE_PHYVAR(DeviceId,InterfaceId)			DEVICE_PHYLAYER(DeviceId,InterfaceId)->phyVar
#define DEVICE_MACVAR(DeviceId,InterfaceId)			DEVICE_MACLAYER(DeviceId,InterfaceId)->MacVar
#define DEVICE_HWADDRESS(DeviceId,InterfaceId)		DEVICE_MACLAYER(DeviceId,InterfaceId)->szMacAddress
#define DEVICE_NWROUTINGVAR(DeviceId)				DEVICE_NWLAYER(DeviceId)->RoutingVar
#define IP_WRAPPER_GET(DeviceId)					((ptrIP_WRAPPER)(DEVICE_NWLAYER(DeviceId)->ipWrapper))
#define IP_TABLE_GET(DeviceId)						((ptrIP_ROUTINGTABLE)(IP_WRAPPER_GET(DeviceId)->table))
#define PIP_TABLE_GET(DeviceId)						(&IP_TABLE_GET(DeviceId))
#define DEVICE_NWPROTOCOL(DeviceId,InterfaceId)		DEVICE_INTERFACE(DeviceId,InterfaceId)->nProtocolId
#define DEVICE_NWADDRESS(DeviceId,InterfaceId)		DEVICE_INTERFACE(DeviceId,InterfaceId)->szAddress
#define DEVICE_MYIP()								DEVICE_NWADDRESS(pstruEventDetails->nDeviceId,pstruEventDetails->nInterfaceId)
#define DEVICE_SUBNETMASK(DeviceId,InterfaceId)		DEVICE_INTERFACE(DeviceId,InterfaceId)->szSubnetMask
#define DEVICE_MYMASK()								DEVICE_SUBNETMASK(pstruEventDetails->nDeviceId,pstruEventDetails->nInterfaceId)
#define DEVICE_PUBLICIP(DeviceId,InterfaceId)		DEVICE_INTERFACE(DeviceId,InterfaceId)->publicIP
#define DEVICE_FIRST_PUBLICIP(DeviceId)				DEVICE_PUBLICIP(DeviceId,1)
#define DEVICE_TCPVAR(DeviceId)						DEVICE_TRXLayer(DeviceId)->TCPProtocol
#define DEVICE_UDPVAR(DeviceId)						DEVICE_TRXLayer(DeviceId)->UDPProtocol
#define DEVICE_APPINFO(DeviceId)					DEVICE_APPLAYER(DeviceId)->pstruApplicationInfo
#define DEVICE_DISTANCE(d1,d2)						(fn_NetSim_Utilities_CalculateDistance(DEVICE_POSITION(d1), DEVICE_POSITION(d2)))
/* Stack API end*/

/*Event API*/
#define EVENT_TYPE(EventDetails)					(EventDetails)->nEventType
#define EVENT_TIME(EventDetails)					(EventDetails)->dEventTime
#define EVENT_DEVICETYPE(EventDetails)				(EventDetails)->nDeviceType
#define EVENT_DEVICEID(EventDetails)				(EventDetails)->nDeviceId
#define EVENT_INTERFACEID(EventDetails)				(EventDetails)->nInterfaceId
#define EVENT_APPID(EventDetails)					(EventDetails)->nApplicationId
#define EVENT_PACKETSIZE(EventDetails)				(EventDetails)->dPacketSize
#define EVENT_PACKET(EventDetails)					(EventDetails)->pPacket
#define EVENT_PACKETID(EventDetails)				(EventDetails)->nPacketId
#define EVENT_SUBEVENTTYPE(EventDetails)			(EventDetails)->nSubEventType
#define EVENT_OTHERDETAILS(EventDetails)			(EventDetails)->szOtherDetails
/*Event API End*/

//Simulation kernel api
typedef long long int (*fn_NetSim_Utilities_AddEvent)(struct stru_NetSim_EventDetails*,int,const char*); /* Add an event to event list.*/
fn_NetSim_Utilities_AddEvent fnpAddEvent;
_declspec(dllexport) int fnDeleteEvent(unsigned long long int nEventId);
/* Stack Variable declaration*/
EXPORTED struct stru_NetSim_EventDetails* pstruEventDetails; //Stores the event details. Only one time memory is allocated. Most used variable
EXPORTED double ldEventTime;		//Stores the current event time
/*EXPORTED*/ EVENT_TYPE nEventType;	//Stores the current event type
EXPORTED char* pszAppPath;		//Stores the application path

EXPORTED char* pszIOPath;		//Stores the I/O path
EXPORTED NETSIM_VERSIONTYPE nVersionType;		//Stores the version type of netsim
EXPORTED int nDbgFlag; //Stores the debug flag

EXPORTED int wireshark_flag;

#define NETSIM_RAND_MAX (double)10000000000

_declspec(dllexport) bool isHost(NETSIM_ID d);
_declspec(dllexport) bool isRouter(NETSIM_ID d);
_declspec(dllexport) bool isL3Device(NETSIM_ID d);

//Calculate distance
_declspec(dllexport) double fn_NetSim_Utilities_CalculateDistance(NetSim_COORDINATES* coordinate1,NetSim_COORDINATES* coordinates2);
//Random Number Generator
_declspec(dllexport) double fn_NetSim_Utilities_GenerateRandomNo(unsigned long *uSeed, unsigned long *uSeed1);
#define NETSIM_RAND() fn_NetSim_Utilities_GenerateRandomNo(DEVICE_SEED(1))
#define NETSIM_RAND_01() (NETSIM_RAND()/NETSIM_RAND_MAX)
#define NETSIM_RAND_RN(max,min) (min+NETSIM_RAND_01()*(max-min))

//Con cat string
_declspec(dllexport) int fn_NetSim_Utilities_ConcatString(int num,char* arg1,const char* arg2,...);


//Used to generate number based on distribution.
_declspec(dllexport) int fn_NetSim_Utils_Distribution(int nDistributionType, double *fDistOut,unsigned long *uSeed, unsigned long *uSeed1,double fFirstArg,double fSecondArg);

//return the device id. This function take input as device name and return the device is of the device if found else return 0. 
_declspec(dllexport) NETSIM_ID fn_NetSim_Stack_GetDeviceId_asName(const char* name);

//Device Id by IP address. This function take input as IP address and return the device id and also update the interface id(passed as reference)
_declspec(dllexport) NETSIM_ID fn_NetSim_Stack_GetDeviceId_asIP(NETSIM_IPAddress ip,NETSIM_ID* nInterfaceId);

//Device Id by Mac address. This function take the MAC address as input and return the device id and also update the interface id (Passed as reference)
_declspec(dllexport) NETSIM_ID fn_NetSim_Stack_GetDeviceId_asMac(PNETSIM_MACADDRESS mac,NETSIM_ID* nInterfaceId);

//Wireless interface of a device. This function takes the link id and device id as input and return the interface id of wireless port if connected else return 0.
_declspec(dllexport) NETSIM_ID fn_NetSim_Stack_GetWirelessInterface(NETSIM_ID nLinkId,NETSIM_ID nDevId);

//Used to get interface id from IP address of a device.
_declspec(dllexport) NETSIM_ID fn_NetSim_Stack_GetInterfaceIdFromIP(NETSIM_ID nDeviceId,NETSIM_IPAddress ip);

//Used to get all interface id by running mac protocol. Caller must free return value.
_declspec(dllexport) NETSIM_ID* fn_NetSim_Stack_GetInterfaceIdbyMacProtocol(NETSIM_ID devId,MAC_LAYER_PROTOCOL protocol,unsigned int* count);

//Used to get all interface id by interface type. Caller must free return value.
_declspec(dllexport) NETSIM_ID* fn_NetSim_Stack_GetInterfaceIdbyInterfaceType(NETSIM_ID devId,INTERFACE_TYPE ifType,unsigned int* count);

/* Function used to get connection.
 * Take four parameter
 * 1. Device id
 * 2. Interface id
 * 3. pointer to connected device id
 * 4. pointer to connected interface id
 * return link id.
 * Update connected device id and connected interface id.
 * return 0 in case of no connection found.
 */
_declspec(dllexport) NETSIM_ID fn_NetSim_Stack_GetConnectedDevice(NETSIM_ID nDeviceId,NETSIM_ID nInterfaceId,NETSIM_ID* nConnectedDevId,NETSIM_ID* nConnectedInterfaceId);
_declspec(dllexport) NETSIM_ID fn_NetSim_Stack_GetFirstConnectedDevice(NETSIM_ID nDeviceId, NETSIM_ID nInterfaceId, NETSIM_ID* nConnectedDevId, NETSIM_ID* nConnectedInterfaceId);

// Find the connected device interface id
_declspec(dllexport) NETSIM_ID fn_NetSim_Stack_GetConnectedInterface(NETSIM_ID devId,NETSIM_ID ifid,NETSIM_ID conId);

//Used to remove a device from link. Call only for P2MP or MP2MP link
_declspec(dllexport) NetSim_LINKS* fn_NetSim_Stack_RemoveDeviceFromlink(NETSIM_ID nDeviceId,NETSIM_ID nInterfaceId,NETSIM_ID nLinkId);

//Used to add a device to link. Call only for P2MP or MP2MP link
_declspec(dllexport) NetSim_LINKS* fn_NetSim_Stack_AddDeviceTolink(NETSIM_ID nDeviceId,NETSIM_ID nInterfaceId,NETSIM_ID nLinkId);

//Used to get the MAC protocol running on interface of a device 
_declspec(dllexport) MAC_LAYER_PROTOCOL fn_NetSim_Stack_GetMacProtocol(NETSIM_ID nDeviceId,NETSIM_ID nInterfaceId);
//Used to get Network layer protocol running on the device
_declspec(dllexport) NETWORK_LAYER_PROTOCOL fn_NetSim_Stack_GetNWProtocol(NETSIM_ID nDeviceId);
//Used to get network layer routing protocol running on device
_declspec(dllexport) NETWORK_LAYER_PROTOCOL fn_NetSim_Stack_GetNWRoutingProtocol(NETSIM_ID nDeviceId);
//used to get Transport layer protocol based on the packet type.
_declspec(dllexport) TRANSPORT_LAYER_PROTOCOL fn_NetSim_Stack_GetTrnspProtocol(NETSIM_ID nDeviceId, const struct stru_NetSim_Packet*);

/* Configuration API*/
//Used to get an attributes value from an xml tag
_declspec (dllexport) char* fn_NetSim_xmlConfig_GetVal(void* xmlNetSimNode,const char* szName,int flag);
//Used to configure packet trace field
_declspec (dllexport) char* fn_NetSim_xmlConfigPacketTraceField(const void* xmlNetSimNode,const char* name);
//Used to write error log in config log file
_declspec(dllexport) int NetSimxmlError(const char* msg, char* val, const void* xmlNetSimNode);
//Used to get child node of any node
_declspec(dllexport) void* fn_NetSim_xmlGetChildElement(void* xmlNetSimNode,const char* childName,int count);
//Used to iterate over child element
_declspec(dllexport) void fn_NetSim_xmlIterateOverChildElement(void* xmlNetSimNode, const char* childName,
															   int(*fun)(void* childNode, int argc, char* argv[]),
															   int argc,
															   char* argv[]);
//convert string link type to enum
_declspec(dllexport) LINK_TYPE fn_NetSim_Config_GetLinkType(const char* value);
//Covert string suppression model to enum
_declspec(dllexport) SUPPRESSION_MODEL fn_NetSim_Config_GetSuppressionModel(char* model);
//Convert string service type to enum
_declspec(dllexport) SERVICE_TYPE fn_NetSim_Config_GetServiceType(char* type);
//FConvert string device type to enum
_declspec(dllexport) netsimDEVICE_TYPE fn_NetSim_Config_GetDeviceType(const char* type);
//convert the string video model to enum
_declspec(dllexport) VIDEO_MODEL fn_NetSim_Config_GetVideoModel(const char* value);
//Convert the string distribution to enum
_declspec(dllexport) DISTRIBUTION fn_NetSim_Config_GetDistribution(const char* value);
//Convert the string interface type to enum
_declspec(dllexport) INTERFACE_TYPE fn_NetSim_Config_GetInterfaceType(const char* value);
//convert string protocol to enum
_declspec(dllexport) int fn_NetSim_Config_GetProtocol(const char* value);
//Convert string propagation model to enum
_declspec(dllexport) PROPAGATION_MODEL fn_NetSim_Config_GetPropagationModel(char* model);
_declspec(dllexport) SHADOW_MODEL fn_NetSim_Config_GetShadowModel(char* model);
_declspec(dllexport) FADING_MODEL fn_NetSim_Config_GetFadingModel(char* model);
_declspec(dllexport) PATHLOSS_MODEL fn_NetSim_Config_GetPathLossModel(char* model);

//Used to get the IP address from name and interface id
_declspec(dllexport) NETSIM_IPAddress fn_NetSim_Stack_GetIPAddressByName(const NETSIM_Name szDeviceName,NETSIM_ID nInterfaceId);
//Used to get the mac address from IP address
_declspec(dllexport) PNETSIM_MACADDRESS fn_NetSim_Stack_GetMacAddressFromIP(const NETSIM_IPAddress szIPAddress);
//used to get the IP address from MAC address
_declspec(dllexport) NETSIM_IPAddress fn_NetSim_Stack_GetIPAddressFromMac(const PNETSIM_MACADDRESS szMacAddress);
//Used to get the first configure interface ip address of a device
_declspec(dllexport) NETSIM_IPAddress fn_NetSim_Stack_GetFirstIPAddressAsId(NETSIM_ID nDeviceId,unsigned int type);
//used to get the ip address from device id and interface id
_declspec(dllexport) NETSIM_IPAddress fn_NetSim_Stack_GetIPAddressAsId(NETSIM_ID nDeviceId,NETSIM_ID nInterfaceId);
//Used to get the first configured interface ip address of a device
_declspec(dllexport) NETSIM_IPAddress fn_NetSim_Stack_GetFirstIPAddressByName(const NETSIM_Name szDeviceName);
//Used to get config id of link by back-end id of link
_declspec(dllexport) NETSIM_ID fn_NetSim_Stack_GetConfigIdOfLinkById(NETSIM_ID nLinkId);
_declspec(dllexport) NETSIM_ID fn_NetSim_Stack_GetLinkIdByConfigId(NETSIM_ID nLinkId);
//Used to get config id of device by back-end id of device
_declspec(dllexport) NETSIM_ID fn_NetSim_Stack_GetConfigIdOfDeviceById(NETSIM_ID nDeviceId);
// Used to get the device id from config id
_declspec (dllexport) NETSIM_ID fn_NetSim_GetDeviceIdByConfigId(NETSIM_ID id);

_declspec(dllexport) bool isIPForSameDevice(NETSIM_IPAddress ip, NETSIM_ID d);

_declspec(dllexport) NETSIM_IPAddress get_ip_from_same_subnet(NETSIM_ID d, NETSIM_IPAddress ip);

//Used to get Application by config id
_declspec(dllexport) NETSIM_ID fn_NetSim_Stack_GetApplicationIdByConfigId(NETSIM_ID id);
//Used to get interface id by config id
_declspec(dllexport) NETSIM_ID fn_NetSim_GetInterfaceIdByConfigId(NETSIM_ID devId,NETSIM_ID id);
//Convert Connection medium string to enum
_declspec(dllexport) PHYSICAL_LAYER_MEDIUM fn_NetSim_Cofig_GetConnectionMedium(char* medium);
//Used to configure protocol
_declspec(dllexport) int fn_NetSim_xmlConfigureProtocolProperty(void* xmlChildNode, NETSIM_ID nDeviceId, NETSIM_ID nInterfaceId, LAYER_TYPE nLayerType, int nProtocolId);
_declspec(dllexport) bool fn_NetSim_Stack_isProtocolConfigured(NETSIM_ID d,
															   LAYER_TYPE layer,
															   int protocol);
/* Metrics */
_declspec(dllexport) int fn_NetSim_Metrics_Add(const NetSim_PACKET* pPacket);

#define DEFAULT_MAC_FRAGMENT_SIZE 1500	//Bytes
#define DEFAULT_TRX_MSS	1472	//Bytes
#define DEFAULT_NW_LAYER_SIZE 1480 //Bytes
_declspec(dllexport) double fn_NetSim_Stack_GetMSS(const NetSim_PACKET* pPacket);

//Interlayer communication function
//Register call back back when an ICMP error arrive
_declspec(dllexport) int fn_NetSim_Stack_RegisterICMPCallback(int (*fnCallBack)(NetSim_PACKET* errPacket,NETSIM_ID nDeviceId,unsigned int nICMPErr));
//Called by IP layer when an ICMP error arrives
_declspec(dllexport) int fn_NetSim_Stack_CallICMPErrorFun(NetSim_PACKET* packet,NETSIM_ID nDeviceId,unsigned int nICMPErr);
//Called by upper layer to get link quality from phy layer. Must be implemented by phy layer protocol
_declspec(dllexport) double fn_NetSim_stack_get_link_quality(NETSIM_ID trx, NETSIM_ID tri, NETSIM_ID rx, NETSIM_ID ri);

//Function pointer called to resolve id to ip
typedef NETSIM_IPAddress (*fndns_query)(NETSIM_ID nDeviceId,NETSIM_ID id);
EXPORTED fndns_query fnDNS;
_declspec(dllexport) NETSIM_IPAddress DNS_QUERY(NETSIM_ID nDeviceId,NETSIM_ID id);

//Callback function for mobility. Must include Mobility.lib
typedef int (*_fnMobilityCallBack)(NETSIM_ID nDeviceId);
_fnMobilityCallBack* fnMobilityCallBack;
_declspec(dllexport) int fnMobilityRegisterCallBackFunction(_fnMobilityCallBack fnCallBack);

//Callback function for node join. Must include Mobility.lib
typedef int (*_fnNodeJoinCallBack)(NETSIM_ID nDeviceId,double time,NODE_ACTION);
_fnNodeJoinCallBack* fnNodeJoinCallBack;
_declspec(dllexport) int fnNodeJoinRegisterCallBackFunction(_fnNodeJoinCallBack fnCallBack);

//Xml configuration automatic reading
typedef enum xmlDataType
{
	_INT = 1,
	_DOUBLE,
	_FLOAT,
	_STRING,
	_BOOL,
	_CHAR,
	_SHORT,
	_UINT,
	_UCHAR,
	_USHORT,
	_UINT8,
	_UINT16,
}XMLDATATYPE;
_declspec(dllexport) int GetXmlVal(void* var,char* name,void* xmlNode,int flag, XMLDATATYPE type);
#define getXmlVar(var,name,xmlNode,flag,type,protocol) if(!GetXmlVal(var,#name,xmlNode,flag,type)) *(var)= protocol##_##name##_DEFAULT

//Mac address API
_declspec(dllexport) PNETSIM_MACADDRESS STR_TO_MAC(char* mac);
_declspec(dllexport) char* MAC_TO_STR(PNETSIM_MACADDRESS mac);
_declspec(dllexport) PNETSIM_MACADDRESS MAC_COPY(PNETSIM_MACADDRESS mac);
_declspec(dllexport) int MAC_COMPARE(PNETSIM_MACADDRESS mac1, PNETSIM_MACADDRESS mac2);
_declspec(dllexport) void MAC_FREE(PNETSIM_MACADDRESS mac);
_declspec(dllexport) void MAC_FREE_ALL();
_declspec(dllexport) PNETSIM_MACADDRESS multicastIP_to_Mac(NETSIM_IPAddress multicastIP);
_declspec(dllexport) bool isBroadcastMAC(PNETSIM_MACADDRESS mac);
_declspec(dllexport) bool isMulticastMAC(PNETSIM_MACADDRESS mac);

//Stack Utility
_declspec(dllexport) void sim_time_to_string(double time, char* strTime/*Min 256 char*/);

//Emulation
_declspec(dllexport) void set_emulation(bool isCustom,
										int(*load_emulation)(void* emuInterface));

// Interface state
typedef int(*fun)(NETSIM_ID, NETSIM_ID, bool);
_declspec(dllexport) void register_interface_state_change(NETSIM_ID d,
														  NETSIM_ID in,
														  fun foo);
_declspec(dllexport) void notify_interface_up(NETSIM_ID d, NETSIM_ID in);
_declspec(dllexport) void notify_interface_down(NETSIM_ID d, NETSIM_ID in);

//Link failure model
_declspec(dllexport) int fn_NetSim_Link_RegisterLinkFailureCallback(NETSIM_ID linkId,
																	int(*fnLinkCallBack)(NETSIM_ID, LINK_STATE));

#ifdef  __cplusplus
}
#endif
#endif //_NETSIM_STACK_H_
