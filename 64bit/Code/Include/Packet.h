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
#ifndef _NETSIM_PACKET_H_
#define _NETSIM_PACKET_H_
#ifdef  __cplusplus
extern "C" {
#endif
/* Enumerator for video model*/
enum enum_VideoModel
{
	VidModel_NULL=0,
	VidModel_CONTINUOUS_NORMAL_VBR=1,
	VidModel_CONTINUOUS_STATE_AUTOREGRESSIVE_MARKOV,
	VidModel_QUANTIZED_STATE_CONTINUOUS_TIME_MARKOV,
	VidModel_SIMPLE_IPB_COMPOSITE_MODEL,
};
/* Enumerator for Suppression model*/
enum enum_SuppressionModel
{
	SuppModel_NULL=0,
	SuppModel_Deterministics=1,
	SuppModel_MarkovChain,
};
/* Enumeration for Distribution*/
enum enum_Distribution
{
	Distribution_NULL=0,
	Distribution_Exponential=1,
	Distribution_Uniform,
	Distribution_Triangular,
	Distribution_Weibull,
	Distribution_Constant,
	Distribution_Backlog,
	Distribution_Normal,
	Distribution_Gamma,
};
/* Enumeration for packet type*/
enum enum_PacketType
{
	PacketType_Voice=1,
	PacketType_CBR,
	PacketType_Custom,
	PacketType_DataBase,
	PacketType_Video,
	PacketType_FTP,
	PacketType_HTTP,
	PacketType_EMAIL,
	PacketType_Peer_to_Peer,
	PacketType_Erlang_Call,
	PacketType_Emulation,
	PacketType_Sensing,
	PacketType_BSM,
	PacketType_COAP,
	PacketType_Control,
};
/* Enumeration for Service type*/
enum enum_ServiceType
{
	ServiceType_NULL=0,
	ServiceType_CBR =1,
	ServiceType_VBR=2,
};
/* Enumeration for packet priority */
enum enum_PacketPriority
{
	Priority_Low=2,
	Priority_Normal=4,
	Priority_Medium=6,
	Priority_High=8,
};
/* Enumeration for QOS class */
enum enum_Packet_QOS
{
	QOS_UGS = 1,
	QOS_rtPS,
	QOS_ertPS,
	QOS_nrtPS,
	QOS_BE,

	//LTE QoS class
	QOS_GBR = QOS_UGS,
	QOS_NONGBR = QOS_BE,
};
/* Enumeration for packet status */
enum enum_PacketStatus
{
	PacketStatus_NoError=0,
	PacketStatus_Error=1,
	PacketStatus_Collided=2,
	PacketStatus_Buffer_Dropped=3,
	PacketStatus_TTL_Expired=4,
	PacketStatus_Dropped=5,
};

/* Enumeration for fragment */
enum enum_Segment
{
	Segment_FirstSegment=10,
	Segment_ContinuingFragment=11,
	Segment_LastFragment=01,
	Segment_Unfragment=00,
};
/* Enumerator for Buffer status */
enum enum_BUFFER
{
	Buffer_Underflow,
	Buffer_Overflow,
};

typedef enum
{
	IPPROTOCOL_NULL = 0x00,
	IPPROTOCOL_ICMP = 0x01,
	IPPROTOCOL_IGMP = 0x02,
	IPPROTOCOL_TCP = 0x06,
	IPPROTOCOL_UDP = 0x11,
	IPPROTOCOL_DSR = 0x30,
	IPPROTOCOL_OSPF = 0x59,
	IPPROTOCOL_PIM = 0x67,
}IP_PROTOCOL_NUMBER;

/*Enumeration for traffic type*/
typedef enum enum_ApplicationType
{
	TRAFFIC_NULL = 0,
	TRAFFIC_DATA = 1,
	TRAFFIC_CBR,
	TRAFFIC_VOICE,
	TRAFFIC_VIDEO,
	TRAFFIC_FTP,
	TRAFFIC_HTTP,
	TRAFFIC_DATABASE,
	TRAFFIC_CUSTOM,
	TRAFFIC_EMAIL,
	TRAFFIC_PEER_TO_PEER,
	TRAFFIC_ERLANG_CALL,
	TRAFFIC_EMULATION,
	TRAFFIC_SENSING,
	TRAFFIC_BSM_APP,
	TRAFFIC_SENSOR_EMULATION,
	TRAFFIC_COAP,
}APPLICATION_TYPE;

/* App layer of packet*/
struct stru_NetSim_Packet_AppLayer
{
	int nSegmentId;//Unique id of segment
	NETSIM_ID nApplicationId; //Unique id of application
	PACKET_FRAGMENT nPacketFragment; //Fragment
	double dArrivalTime; //Arrival time in application layer
	double dStartTime; //Start time in application layer
	double dEndTime; //End time in application layer
	double dPacketSize; //Packet in app layer
	double dPayload; //Payload in app layer
	double dOverhead; //Overhead due to app layer
	int nAppEndFlag; // 1 for last packet of the application
	APPLICATION_TYPE nAppType;
	APPLICATION_LAYER_PROTOCOL nApplicationProtocol; //App layer protocol
	void* Packet_AppProtocol; //Pointer to app layer protocol overhead
};
/* Transport layer of packet*/
struct stru_NetSim_Packet_TransportLayer
{
	double dArrivalTime; //Arrival time in transport layer
	double dStartTime; //Start time from transport layer
	double dEndTime; //End time from transport layer
	double dPacketSize; //packet size in transport layer
	double dPayload; //Payload in transport layer
	double dOverhead; //Overhead due to transport layer
	unsigned short int nSourcePort;	//Source port
	unsigned short int nDestinationPort; //Destination port
	TRANSPORT_LAYER_PROTOCOL nTransportProtocol; //Transport layer protocol id
	void* Packet_TransportProtocol; //Pointer to transport layer overhead
};
/* Network layer of packet*/
struct stru_NetSim_Packet_NetworkLayer
{
	double dArrivalTime; //Arrival time in network layer
	double dStartTime; //Start time from network layer
	double dEndTime; //End time from network layer
	double dPacketSize; //Packet size in network layer
	double dPayload;//Payload in network layer
	double dOverhead; //Overhead due to network layer
	NETSIM_IPAddress szSourceIP;//IP address of source device
	NETSIM_IPAddress szDestIP;//IP address of dest device
	NETSIM_IPAddress szGatewayIP; //IP address of the gate way
	NETSIM_IPAddress szNextHopIp;//IP address of next hop
	unsigned int nTTL:8;	//Time to live
	IP_PROTOCOL_NUMBER IPProtocol;
	NETWORK_LAYER_PROTOCOL nNetworkProtocol; //Network layer protocol id
	NETWORK_LAYER_PROTOCOL nRoutingProtocol; //Network layer routing protocol
	void* Packet_NetworkProtocol; //Pointer to network layer overhead
	void* Packet_RoutingProtocol; //Pointer to routing layer overhead
	unsigned int nPacketFlag;
	struct stru_NetSim_Packet_NetworkLayer* next;
};
/* Mac layer of packet*/
struct stru_NetSim_Packet_MACLayer
{
	double dArrivalTime; //Arrival time in Mac layer
	double dStartTime; //Start time from Mac layer
	double dEndTime; //End time from Mac layer
	double dPacketSize; //Packet size in Mac layer
	double dPayload;//payload in Mac layer
	double dOverhead; //Overhead dur to Mac layer
	PNETSIM_MACADDRESS szSourceMac;//MAC address of source device
	PNETSIM_MACADDRESS szDestMac;//MAC address of dest device
	PNETSIM_MACADDRESS szNextHopMac;//MAC address of next hop
	MAC_LAYER_PROTOCOL nMACProtocol; //Mac layer protocol id
	void* Packet_MACProtocol; //Pointer to Mac layer overhead
	bool dontFree; // Signal other protocol to don't free mac overhead or change
};
/* Physical layer of packet*/
struct stru_NetSim_Packet_PhyLayer
{
	double dArrivalTime; //Arrival time in Phy layer
	double dStartTime; //Start time from Phy layer
	double dEndTime; //End time from Phy layer
	double dPacketSize; //Packet size in Phy layer
	double dPayload;//Payload in phy layer
	double dOverhead; //Overhead due to Phy layer
	PACKET_STATUS nPacketErrorFlag;
	PHYSICAL_LAYER_MEDIUM nPhyMedium; //Phy layer medium
	void* Packet_PhyData; //Pointer to phy layer overhead
};

#define MAXBUF  0x800
typedef struct stru_packet_info
{
	unsigned char packet[MAXBUF];
	unsigned int packet_len;
	void* addr;
	int flag;
	long long int captureTime;
	long long int reinjectTime;
}PACKET_INFO,*PPACKET_INFO;

#define MAX_PACKET_TYPE_LEN 50
/* Structure for packet*/
struct stru_NetSim_Packet
{
	long long int nPacketId; //Unique id of packet
	PACKET_TYPE nPacketType; //Packet type 
	unsigned int nControlDataType; //Control packet type
	SERVICE_TYPE nServiceType; //Service type
	PACKET_PRIORITY nPacketPriority; //Packet priority
	QUALITY_OF_SERVICE nQOS;	//Quality of service
	double dEventTime; //event time
	NETSIM_ID nSourceId; //Unique id of source
	void* destInfo;
	NETSIM_ID nTransmitterId; //Unique id of transmitter (As per IEEE802.11b) 
	NETSIM_ID nReceiverId; //Unique id of receiver (As per IEEE802.11b)
						  //Next hop in Routing (RIP)
						  //Address[n] in DSR
	PPACKET_INFO szPayload;//Actual payload of packet.
	void* szOverhead;//Actual overhead of packet.
	char szPacketType[MAX_PACKET_TYPE_LEN];
	PACKET_STATUS nPacketStatus; //0 - No error, 1- Error, 2- collided, 3- Dropped
	struct stru_NetSim_Packet_AppLayer* pstruAppData; //App layer details
	struct stru_NetSim_Packet_TransportLayer* pstruTransportData; //Transport layer details
	struct stru_NetSim_Packet_NetworkLayer* pstruNetworkData; //Network layer details
	struct stru_NetSim_Packet_MACLayer* pstruMacData; //MAC layer details
	struct stru_NetSim_Packet_PhyLayer* pstruPhyData; //Physical layer details
	struct stru_NetSim_Packet* pstruNextPacket; //Pointer to next packet

	//Function pointer for notification used for lower layer to higher layer notification
	int (*DropNotification)(struct stru_NetSim_Packet* packet);
	int (*ReceiveAckNotification)(struct stru_NetSim_Packet* ack);
};

/* PACKET API*/
/*MACROS*/
# define PACKET_ID(pPacket)					(pPacket)->nPacketId
# define PACKET_SERVICETYPE(pPacket)		(pPacket)->nServiceType
# define PACKET_EVENTTIME(pPacket)			(pPacket)->dEventTime
# define PACKET_SOURCE(pPacket)				(pPacket)->nSourceId
# define PACKET_DESTINATION(pPacket)		(pPacket)->nDestinationId
# define PACKET_TRANSMITTER(pPacket)		(pPacket)->nTransmitterId
# define PACKET_RECEIVER(pPacket)			(pPacket)->nReceiverId
# define PACKET_APPDATA(pPacket)			(pPacket)->pstruAppData
# define PACKET_TRXDATA(pPacket)			(pPacket)->pstruTransportData
# define PACKET_NWDATA(pPacket)				(pPacket)->pstruNetworkData
# define PACKET_MACDATA(pPacket)			(pPacket)->pstruMacData
# define PACKET_PHYDATA(pPacket)			(pPacket)->pstruPhyData
# define PACKET_PHYPROTOCOLDATA(pPacket)	PACKET_PHYDATA(pPacket)->Packet_PhyData
# define PACKET_MACPROTOCOLDATA(pPacket)	PACKET_MACDATA(pPacket)->Packet_MACProtocol
# define PACKET_NWPROTOCOLDATA(pPacket)		PACKET_NWDATA(pPacket)->Packet_NetworkProtocol
# define PACKET_TRXPROTOCOLDATA(pPacket)	PACKET_TRXDATA(pPacket)->Packet_TransportProtocol
# define PACKET_APPPROTOCOLDATA(pPacket)	PACKET_APPDATA(pPacket)->Packet_AppProtocol
# define PACKET_NEXTPACKET(pPacket)			(pPacket)->pstruNextPacket

/* PACKET API */
//Used to create a new packet of any layer
_declspec(dllexport) NetSim_PACKET* fn_NetSim_Packet_CreatePacket_dbg(int nLayer,int line,const char* file);
//Used to free mac layer protocol data
_declspec(dllexport) int fn_NetSim_Packet_FreeMacProtocolData(NetSim_PACKET* pstruPacket);
//Used to free the packet
_declspec(dllexport) int fn_NetSim_Packet_FreePacket_dbg(NetSim_PACKET** pstruPacket,int line,char* file);
//Used to add the packet to list
_declspec(dllexport) int fn_NetSim_Packet_AddPacketToList_dbg(NetSim_BUFFER* pstruBuffer,NetSim_PACKET* pstruPacket, int nInsertionType /* 1- head,2-Tail,3-Order of event time*/,int line,const char* file);
/*Used to check whether any packet is there in buffer or not
* Return 1 if there is a packet in buffer
* Return 0 if there is no packet in buffer
*/
_declspec(dllexport) int fn_NetSim_GetBufferStatus(NetSim_BUFFER* pstruBuffer);
/*Used to get a packet from list
 * pass nFlag = 1 if want to move buffer
 * pass nFlag = 0 if don't want to move buffer
 */
_declspec(dllexport) NetSim_PACKET* fn_NetSim_Packet_GetPacketFromBuffer(NetSim_BUFFER* pstruBuffer,int nFlag);
//Used to create error in packet
_declspec(dllexport) PACKET_STATUS fn_NetSim_Packet_DecideError(double dBER, long double dPacketSize);
//Used to copy a packet into a new packet
_declspec(dllexport) NetSim_PACKET* fn_NetSim_Packet_CopyPacket_dbg(const NetSim_PACKET* pstruPacket,int line,const char* file);
//Used to copy a list of packet
_declspec(dllexport) NetSim_PACKET* fn_NetSim_Packet_CopyPacketList_dbg(const NetSim_PACKET* pstruPacket,int line,const char* file);
_declspec(dllexport) int fn_NetSim_Packet_CopyPhyData(NetSim_PACKET* pstruDestPacket,const NetSim_PACKET* pstruSrcPacket,int,const char*);
//Used to copy mac/data link layer data of a packet to another packet
_declspec(dllexport) int fn_NetSim_Packet_CopyMacData(NetSim_PACKET* pstruDestPacket,const NetSim_PACKET* pstruSrcPacket,int,const char*);
//Used to copy network layer data of a packet to another packet
_declspec(dllexport) int fn_NetSim_Packet_CopyNWData(NetSim_PACKET* pstruDestPacket, NetSim_PACKET* pstruSrcPacket);
//Used to copy transport layer data of a packet to another packet
_declspec(dllexport) int fn_NetSim_Packet_CopyTransportData(NetSim_PACKET* pstruDestPacket,const NetSim_PACKET* pstruSrcPacket,int,const char*);
//Used to copy application layer data of a packet to another packet
_declspec(dllexport) int fn_NetSim_Packet_CopyAppData(NetSim_PACKET* pstruDestPacket,const NetSim_PACKET* pstruSrcPacket,int,const char*);
//Used to write packet trace
_declspec(dllexport) int fn_NetSim_WritePacketTrace_dbg(const NetSim_PACKET* pstruPacket,int line,const char* file);
//Used to validate a packet
_declspec(dllexport) int fnValidatePacket(const NetSim_PACKET* pstruPacket);
//Used to fragment the packet
_declspec(dllexport) int fn_NetSim_Stack_FragmentPacket(NetSim_PACKET* pstruPacket,int nMSS);
//Used to get packet size
_declspec(dllexport) double fnGetPacketSize(NetSim_PACKET* pstruData);
//Get packet type as string
_declspec(dllexport) char* fn_NetSim_Config_GetPacketTypeAsString(PACKET_TYPE nPacketType);
_declspec(dllexport) char* fn_NetSim_Config_GetControlPacketType(const NetSim_PACKET*,char* type);

_declspec(dllexport) int fn_NetSim_Dispatch_to_emulator(NetSim_PACKET* packet);
_declspec(dllexport) unsigned int getPacketInfoSize();

//Destination
_declspec(dllexport) void add_dest_to_packet(NetSim_PACKET* packet, NETSIM_ID dest);
_declspec(dllexport) void add_destlist_to_packet(NetSim_PACKET* packet, NETSIM_ID* destlist, UINT count);
_declspec(dllexport) NETSIM_ID* get_dest_from_packet(NetSim_PACKET* packet, UINT* count);
_declspec(dllexport) NETSIM_ID get_first_dest_from_packet(NetSim_PACKET* packet);
_declspec(dllexport) void remove_dest_from_packet(NetSim_PACKET* packet, NETSIM_ID dest);
_declspec(dllexport) bool isDestFoundinPacket(NetSim_PACKET* packet, NETSIM_ID dest);
_declspec(dllexport) bool isBroadcastPacket(NetSim_PACKET* packet);
_declspec(dllexport) bool isMulticastPacket(NetSim_PACKET* packet);

// Send to HWI
_declspec(dllexport) bool fn_NetSim_HWI_Send(NetSim_PACKET* packet);
#ifdef  __cplusplus
}
#endif
#endif

