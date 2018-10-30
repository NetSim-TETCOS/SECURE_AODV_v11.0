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


/************************************************************************************
RFC 3561          Ad hoc On-Demand Distance Vector (AODV) Routing      July 2003
************************************************************************************/

#ifndef _NETSIM_AODV_H_
#define _NETSIM_AODV_H_
#ifdef  __cplusplus
extern "C" {
#endif

#define AODV_ACTIVE_ROUTE_TIMEOUT		3000* MILLISECOND
#define AODV_ALLOWED_HELLO_LOSS			2
#define AODV_BLACKLIST_TIMEOUT			AODV_RREQ_RETRIES * AODV_NET_TRAVERSAL_TIME
#define AODV_DELETE_PERIOD				K * max (AODV_ACTIVE_ROUTE_TIMEOUT, AODV_HELLO_INTERVAL)
#define K 5
#define AODV_HELLO_INTERVAL				1000* MILLISECOND
#define AODV_LOCAL_ADD_TTL				2
#define AODV_MAX_REPAIR_TTL				0.3 * AODV_NET_DIAMETER
#define AODV_MIN_REPAIR_TTL				//see note below
#define AODV_MY_ROUTE_TIMEOUT			2 * AODV_ACTIVE_ROUTE_TIMEOUT
#define AODV_NET_DIAMETER				35
#define AODV_NET_TRAVERSAL_TIME			2 * AODV_NODE_TRAVERSAL_TIME * AODV_NET_DIAMETER
#define AODV_NEXT_HOP_WAIT				AODV_NODE_TRAVERSAL_TIME + 10
#define AODV_NODE_TRAVERSAL_TIME		40 * MILLISECOND
#define AODV_PATH_DISCOVERY_TIME		2 * AODV_NET_TRAVERSAL_TIME
#define AODV_RERR_RATELIMIT				10
#define AODV_RING_TRAVERSAL_TIME(ttl)	2 * AODV_NODE_TRAVERSAL_TIME *(ttl + AODV_TIMEOUT_BUFFER)
#define AODV_RREQ_RETRIES				2
#define AODV_RREQ_RATELIMIT				10
#define AODV_TIMEOUT_BUFFER				2
#define AODV_TTL_START					1
#define AODV_TTL_INCREMENT				2
#define AODV_TTL_THRESHOLD				7

	//Packet size
#define AODV_RREQ_SIZE_IPV4		24 ///< AODV route request size in bytes
#define AODV_RREQ_SIZE_IPV6		72
#define AODV_RREQ_SIZE(dev)		DEVICE_NWPROTOCOL(dev,1) == NW_PROTOCOL_IPV4 ? AODV_RREQ_SIZE_IPV4:AODV_RREQ_SIZE_IPV6
#define AODV_RREP_SIZE_IPV4		20 ///< AODV route reply size in bytes
#define AODV_RREP_SIZE_IPV6		56
#define AODV_RREP_SIZE(dev)		DEVICE_NWPROTOCOL(dev,1) == NW_PROTOCOL_IPV4 ? AODV_RREP_SIZE_IPV4:AODV_RREP_SIZE_IPV6
#define AODV_RERR_SIZE_FIXED	4 ///< AODV route error size in bytes


	//Typedef
	typedef enum enum_AODV_Ctrl_Packet AODV_CONTROL_PACKET;
	typedef struct stru_AODV_Primitives AODV_PRIMITIVES;
	typedef struct stru_NetSim_AODV_RouteTable AODV_ROUTETABLE;
	typedef struct stru_AODV_FIFO AODV_FIFO;
	typedef struct stru_AODV_DeviceVariable AODV_DEVICE_VAR;
	typedef struct stru_AODV_RREQ_Seen_Table AODV_RREQ_SEEN_TABLE;
	typedef struct stru_AODV_RREQ_Sent_Table AODV_RREQ_SENT_TABLE;
	typedef struct stru_NetSim_AODV_PrecursorsList AODV_PRECURSORS_LIST;
	typedef struct stru_AODV_Metrics AODV_METRICS;

	//packet typedef
	typedef struct stru_NetSim_AODV_RREQ AODV_RREQ;
	typedef struct stru_NetSim_AODV_RREP AODV_RREP;
	typedef struct stru_NetSim_AODV_RERR AODV_RERR;
	/** Enumeration for AODV control packets */
	enum enum_AODV_Ctrl_Packet
	{
		AODVctrlPacket_RREQ=NW_PROTOCOL_AODV*100+1,
		AODVctrlPacket_RREP,
		AODVctrlPacket_RERR,
	};
	/** Enumeration for AODV sub events */
	typedef enum
	{
		AODVsubevent_RREQ_TIMEOUT=NW_PROTOCOL_AODV*100+1,
		AODVsubevent_TRANSMIT_HELLO,
		AODVsubevent_ACTIVE_ROUTE_TIMEOUT,
	}AODV_SUB_EVENT;

	/**
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	5.1. Route Request (RREQ) Message Format

	0                   1                   2                   3
	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|     Type      |J|R|G|D|U|   Reserved          |   Hop Count   |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                            RREQ ID                            |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                    Destination IP Address                     |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                  Destination Sequence Number                  |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                    Originator IP Address                      |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                  Originator Sequence Number                   |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	*/
	struct stru_NetSim_AODV_RREQ
	{
		unsigned int Type:8; //1
		char JRGDU[6];	/**<
						~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
						J              Join flag; reserved for multicast.

						R              Repair flag; reserved for multicast.

						G              Gratuitous RREP flag; indicates whether a
						gratuitous RREP should be unicast to the node
						specified in the Destination IP Address field (see
						sections 6.3, 6.6.3).

						D              Destination only flag; indicates only the
						destination may respond to this RREQ (see
						section 6.5).

						U              Unknown sequence number; indicates the destination
						sequence number is unknown (see section 6.3).
						~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
						*/					
		unsigned int Reserved:11; ///< Sent as 0; ignored on reception.		
		unsigned int HopCount:8; /**<
								 The number of hops from the Originator IP Address
								 to the node handling the request.
								 */		
		unsigned int RREQID; /**<
							 A sequence number uniquely identifying the
							 particular RREQ when taken in conjunction with the
							 originating node's IP address.
							 */
		NETSIM_IPAddress DestinationIPAddress;/**< The IP address of the destination for which a route is desired.*/
		unsigned int DestinationSequenceNumber;/**< 
											   The latest sequence number received in the past by the originator for any route towards the
											   destination.
											   */		
		NETSIM_IPAddress OriginatorIPAddress;/**< The IP address of the node which originated the Route Request.*/		
		unsigned int OriginatorSequenceNumber;/**<
											  The current sequence number to be used in the route
											  entry pointing towards the originator of the route
											  request.
											  */
		NETSIM_IPAddress LastAddress;//NetSim Specific
	};


	/**
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	5.2. Route Reply (RREP) Message Format

	0                   1                   2                   3
	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|     Type      |R|A|    Reserved     |Prefix Sz|   Hop Count   |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                     Destination IP address                    |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                  Destination Sequence Number                  |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                    Originator IP address                      |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                           Lifetime                            |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	*/
	struct stru_NetSim_AODV_RREP
	{
		unsigned int Type:8;//2
		char RA[3]; /**<
					~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
					R             Repair flag; used for multicast.

					A             Acknowledgment required; see sections 5.4 and 6.7.
					~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
					*/		
		unsigned int Reserved:9; ///< Sent as 0; ignored on reception.	
		unsigned int PrefixSz:5; /**<
								 If nonzero, the 5-bit Prefix Size specifies that the
								 indicated next hop may be used for any nodes with
								 the same routing prefix (as defined by the Prefix
								 Size) as the requested destination.
								 */		
		unsigned int HopCount:8; /**<
								 The number of hops from the Originator IP Address
								 to the Destination IP Address.  For multicast route
								 requests this indicates the number of hops to the
								 multicast tree member sending the RREP.
								 */		
		NETSIM_IPAddress DestinationIPaddress;////< The IP address of the destination for which a route is supplied.
		unsigned int DestinationSequenceNumber;///< The destination sequence number associated to the route.
		NETSIM_IPAddress OriginatorIPaddress;///< The IP address of the node which originated the RREQ for which the route is supplied.
		unsigned int Lifetime;///< The time in milliseconds for which nodes receiving the RREP consider the route to be valid.
		NETSIM_IPAddress LastAddress; //NetSim-specific
	};

	/**
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	5.3. Route Error (RERR) Message Format

	0                   1                   2                   3
	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|     Type      |N|          Reserved           |   DestCount   |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|            Unreachable Destination IP Address (1)             |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|         Unreachable Destination Sequence Number (1)           |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-|
	|  Additional Unreachable Destination IP Addresses (if needed)  |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|Additional Unreachable Destination Sequence Numbers (if needed)|
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	*/
	struct stru_NetSim_AODV_RERR
	{
		unsigned int Type:8;//3
		char N;/**<
			   No delete flag; set when a node has performed a local
			   repair of a link, and upstream nodes should not delete
			   the route.
			   */	
		unsigned int Reserved:15; ///< Sent as 0; ignored on reception.
		unsigned int DestCount:8;/**<
								 The number of unreachable destinations included in the
								 message; MUST be at least 1.
								 */

		NETSIM_IPAddress* UnreachableDestinationIPAddress;/**<
														  The IP address of the destination that has become
														  unreachable due to a link break.
														  */	

		unsigned int* UnreachableDestinationSequenceNumber;/**<
														   The sequence number in the route table entry for
														   the destination listed in the previous Unreachable
														   Destination IP Address field.
														   */
	};

	/**
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	5.4. Route Reply Acknowledgment (RREP-ACK) Message Format

	The Route Reply Acknowledgment (RREP-ACK) message MUST be sent in
	response to a RREP message with the 'A' bit set (see section 5.2).
	This is typically done when there is danger of unidirectional links
	preventing the completion of a Route Discovery cycle (see section
	6.8).

	0                   1
	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|     Type      |   Reserved    |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	Type        4

	Reserved    Sent as 0; ignored on reception.

	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	*/
	struct stru_NetSim_AODV_RREP_ACK
	{
		unsigned int Type:8;
		unsigned int Reserved:8;
	};


	typedef enum
	{
		AODV_RoutingFlag_Valid,
		AODV_RoutingFlag_InValid,
		AODV_RoutingFlag_Repairable,
		AODV_RoutingFlag_beingRepaired,
	}AODV_RoutingFlag;
	struct stru_NetSim_AODV_PrecursorsList
	{
		NETSIM_IPAddress* list;
		int count;
	};
	/**
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	AODV is a routing protocol, and it deals with route table management.
	Route table information must be kept even for short-lived routes,
	such as are created to temporarily store reverse paths towards nodes
	originating RREQs.  AODV uses the following fields with each route
	table entry:

	-  Destination IP Address
	-  Destination Sequence Number
	-  Valid Destination Sequence Number flag
	-  Other state and routing flags (e.g., valid, invalid, repairable,
	being repaired)
	-  Network Interface
	-  Hop Count (number of hops needed to reach destination)
	-  Next Hop
	-  List of Precursors (described in Section 6.2)
	-  Lifetime (expiration or deletion time of the route)
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	*/
	struct stru_NetSim_AODV_RouteTable
	{
		NETSIM_IPAddress DestinationIPAddress;
		unsigned int DestinationSequenceNumber;
		bool ValidDestinationSequenceNumberflag;
		AODV_RoutingFlag routingFlags;
		NETSIM_ID NetworkInterface;
		unsigned int HopCount;
		NETSIM_IPAddress NextHop;
		AODV_PRECURSORS_LIST* ListofPrecursors;
		double Lifetime;
		struct element* ele;
	};
	/**
	This is a buffer to which packets are added after if no route is present to the target.
	*/		
	struct stru_AODV_FIFO
	{
		NETSIM_IPAddress destination; ///< Destination - destination ip address.	
		NetSim_PACKET* packetList;	///< PacketList - List of packets added to the FIFO buffer.
		double time;
		struct element* ele;	///< ele - next buffer
	};
	///Structure of RREQ Seen Table in which entry is made of the RREQ received
	struct stru_AODV_RREQ_Seen_Table
	{
		NETSIM_IPAddress OrginatingNode;	///< IP address of a node originating RREQ		
		unsigned int nRREQId;	///< RREQ identification no
		double time;
		struct element* ele;	///< Next AODV_RREQ_Seen_Table	
	};
	///Structure of RREQ sent Table in which entry of RREQ sent is made
	struct stru_AODV_RREQ_Sent_Table
	{
		NETSIM_IPAddress DestAddress;	///< Destination IP Address
		int ttl;		///< Time to live
		int times;
		double dTimeoutTime;
		struct element* ele;	///< Next AODV_RREQ_Sent_Table
	};
	/// Stucture for AODV Metrics
	struct stru_AODV_Metrics
	{
		unsigned int rreqSent;
		unsigned int rreqForwarded;
		unsigned int rrepSent;
		unsigned int rrepForwarded;
		unsigned int rerrSent;
		unsigned int rerrForwarded;
		unsigned int routeBreak;
		unsigned int packetTransmitted;
		unsigned int packetOrginated;
		unsigned int packetReceived;
		unsigned int packetDropped;
	};
	/**
	This is the AODV DeviceVariable Structure which contains - 
	FIFO - a packet is added in FIFO buffer if the device does not have route to the target<br\>
	routeTable - this contains the next HOP ip of the routes to the target<br\>
	RREQ_SEEN_TABLE - this contains list differnet RREQ a device encounters.
	*/
	struct stru_AODV_DeviceVariable
	{
		unsigned int nSequenceNumber;
		AODV_FIFO* fifo;
		AODV_ROUTETABLE* routeTable;
		AODV_RREQ_SEEN_TABLE* rreqSeenTable;
		AODV_RREQ_SENT_TABLE* rreqSentTable;
		AODV_PRECURSORS_LIST* precursorsList;
		double dLastBroadcastTime;
		unsigned int nRerrCount;
		double dFirstRerrTime;
		AODV_METRICS aodvMetrics;
	};

#define AODV_PACKET_PROCESSING() fn_NetSim_AODV_GeneralPacketProcessing(pstruEventDetails)
#define AODV_CHECK_ROUTE_FOUND(destIP) fn_NetSim_AODV_CheckRouteFound(destIP,pstruEventDetails)
#define AODV_ADD_TO_FIFO(packet,fifoBuffer,time) fn_NetSim_AODV_AddToFIFOBuffer(packet,fifoBuffer,time)
#define AODV_GEN_RREQ() fn_NetSim_AODV_GenerateRREQ(pstruEventDetails)
#define AODV_RETRY_RREQ() fn_NetSim_AODV_RetryRREQ(pstruEventDetails)
#define AODV_PROCESS_RREQ() fn_NetSim_AODV_ProcessRREQ(pstruEventDetails)
#define AODV_INSERT_ROUTE_TABLE(ip,seq,hopcount,nexthop,lifeTime) fn_NetSim_AODV_InsertInRouteTable(ip,seq,hopcount,nexthop,lifeTime,pstruEventDetails)
#define AODV_INSERT_PRECURSOR(ip) fn_NetSim_AODV_InsertInPrecursorsList(ip,pstruEventDetails)
#define AODV_FORWARD_RREQ() fn_NetSim_AODV_ForwardRREQ(pstruEventDetails)
#define AODV_GENERATE_RREP() fn_NetSim_AODV_GenerateRREP(pstruEventDetails)
#define AODV_TRANSMIT_FIFO(devVar) fn_NetSim_AODV_TransmitFIFOBuffer(devVar,pstruEventDetails)
#define AODV_PROCESS_RREP() fn_NetSim_AODV_ProcessRREP(pstruEventDetails)
#define AODV_FORWARD_RREP() fn_NetSim_AODV_ForwardRREP(pstruEventDetails)
#define AODV_FIND_NEXT_HOP(devVar,dest) fn_NetSim_AODV_FindNextHop(devVar,dest,pstruEventDetails)
#define AODV_GENERATE_RREP_BY_IN() fn_NetSim_AODV_GenerateRREPByIntermediate(pstruEventDetails)
#define AODV_TRANSMIT_HELLO() fn_NetSim_AODV_TransmitHelloMessage(pstruEventDetails)
#define AODV_UPDATE_ROUTE_TABLE(ip,lifetime) fn_NetSim_AODV_UpdateRouteTable(ip,lifetime,pstruEventDetails)
#define AODV_ACTIVE_ROUTE_TIMEOUT_EVENT() fn_NetSim_AODV_ActiveRouteTimeout(pstruEventDetails)
#define AODV_GENERATE_RERR(DevId,UnreachableIP) fn_NetSim_AODV_GenerateRERR(DevId,UnreachableIP,pstruEventDetails)
#define AODV_PROCESS_RERR() fn_NetSim_AODV_ProcessRERR(pstruEventDetails)


#define AODV_DEV_VAR(devId) ((AODV_DEVICE_VAR*)NETWORK->ppstruDeviceList[devId-1]->pstruNetworkLayer->RoutingVar)
#define AODV_METRICS_VAR(devId) (AODV_DEV_VAR(devId)->aodvMetrics)
#define FIFO_ALLOC() (AODV_FIFO*)list_alloc(sizeof(AODV_FIFO),offsetof(AODV_FIFO,ele))
#define RREQSEENTABLE_ALLOC() (AODV_RREQ_SEEN_TABLE*)list_alloc(sizeof(AODV_RREQ_SEEN_TABLE),offsetof(AODV_RREQ_SEEN_TABLE,ele))
#define RREQSENTTABLE_ALLOC() (AODV_RREQ_SENT_TABLE*)list_alloc(sizeof(AODV_RREQ_SENT_TABLE),offsetof(AODV_RREQ_SENT_TABLE,ele))
#define ROUTETABLE_ALLOC() (AODV_ROUTETABLE*)list_alloc(sizeof(AODV_ROUTETABLE),offsetof(AODV_ROUTETABLE,ele))

	void set_aodv_curr();
	NETSIM_IPAddress aodv_get_curr_ip();
	NETSIM_ID aodv_get_curr_if();
	NETSIM_IPAddress aodv_get_dev_ip(NETSIM_ID d);
	bool isAodvConfigured(NETSIM_ID d, NETSIM_ID in);

	//Function prototype
	unsigned int fnFindSequenceNumber(AODV_DEVICE_VAR* devVar,NETSIM_IPAddress ip);
	int fnEmptyFIFOBuffer(AODV_DEVICE_VAR* devVar,NETSIM_IPAddress dest);
	AODV_ROUTETABLE* fnFindRouteTable(AODV_ROUTETABLE* table,NETSIM_IPAddress dest);
	NetSim_PACKET* fn_NetSim_AODV_GenerateCtrlPacket(NETSIM_ID src,
		NETSIM_ID dest,
		NETSIM_ID recv,
		double dTime,
		AODV_CONTROL_PACKET type);
	int fn_NetSim_AODV_InsertInRouteTable(NETSIM_IPAddress ip,
		unsigned int seqNumber,
		unsigned int hopCount,
		NETSIM_IPAddress nextHop,
		double lifeTime,
		NetSim_EVENTDETAILS* pstruEventDetails);
	int fn_NetSim_AODV_TransmitFIFOBuffer(AODV_DEVICE_VAR* devVar,NetSim_EVENTDETAILS* pstruEventDetails);
	int fn_NetSim_AODV_InsertInPrecursorsList(NETSIM_IPAddress ip,NetSim_EVENTDETAILS* pstruEventDetails);
	int fn_NetSim_AODV_UpdateRouteTable(NETSIM_IPAddress ip,double lifetime,NetSim_EVENTDETAILS* pstruEventDetails);
	int fn_NetSim_AODV_ForwardRREP(NetSim_EVENTDETAILS* pstruEventDetails);
	bool fn_NetSim_AODV_CheckRouteFound(NETSIM_IPAddress ip, NetSim_EVENTDETAILS* pstruEventDetails);
	NETSIM_IPAddress fn_NetSim_AODV_FindNextHop(AODV_DEVICE_VAR* devVar,
		NETSIM_IPAddress dest,
		NetSim_EVENTDETAILS* pstruEventDetails);
	int fn_NetSim_AODV_ForwardRREQ(NetSim_EVENTDETAILS* pstruEventDetails);
	int fn_NetSim_AODV_GeneralPacketProcessing(NetSim_EVENTDETAILS* pstruEventDetails);

	bool fn_NetSim_AODV_CheckRouteFound(NETSIM_IPAddress destIP,
		NetSim_EVENTDETAILS* pstruEventDetails);

	bool fn_NetSim_AODV_AddToFIFOBuffer(NetSim_PACKET* packet,
		AODV_FIFO** fifoBuffer,
		double time);

	NetSim_PACKET* fn_NetSim_AODV_GenerateRREQ(NetSim_EVENTDETAILS* pstruEventDetails);

	int fn_NetSim_AODV_RetryRREQ(NetSim_EVENTDETAILS* pstruEventDetails);

	int fn_NetSim_AODV_ProcessRREQ(NetSim_EVENTDETAILS* pstruEventDetails);

	int fn_NetSim_AODV_InsertInRouteTable(NETSIM_IPAddress ip,
		unsigned int seqNumber,
		unsigned int hopCount,
		NETSIM_IPAddress nextHop,
		double lifeTime,
		NetSim_EVENTDETAILS* pstruEventDetails);

	int fn_NetSim_AODV_InsertInPrecursorsList(NETSIM_IPAddress ip,
		NetSim_EVENTDETAILS* pstruEventDetails);

	int fn_NetSim_AODV_ForwardRREQ(NetSim_EVENTDETAILS* pstruEventDetails);

	int fn_NetSim_AODV_GenerateRREP(NetSim_EVENTDETAILS* pstruEventDetails);

	int fn_NetSim_AODV_ProcessRREP(NetSim_EVENTDETAILS* pstruEventDetails);
	int fn_NetSim_AODV_ForwardRREP(NetSim_EVENTDETAILS* pstruEventDetails);
	NETSIM_IPAddress fn_NetSim_AODV_FindNextHop(AODV_DEVICE_VAR* devVar,
		NETSIM_IPAddress dest,
		NetSim_EVENTDETAILS* pstruEventDetails);

	int fn_NetSim_AODV_GenerateRREPByIntermediate(NetSim_EVENTDETAILS* pstruEventDetails);

	int fn_NetSim_AODV_TransmitHelloMessage(NetSim_EVENTDETAILS* pstruEventDetails);

	int fn_NetSim_AODV_ReceiveHelloMessage(NetSim_EVENTDETAILS* pstruEventDetails);

	int fn_NetSim_AODV_UpdateRouteTable(NETSIM_IPAddress nextHop,
		double lifetime,
		NetSim_EVENTDETAILS* pstruEventDetails);

	int fn_NetSim_AODV_ActiveRouteTimeout(NetSim_EVENTDETAILS* pstruEventDetails);

	int fn_NetSim_AODV_GenerateRERR(NETSIM_ID nDeviceId,
		NETSIM_IPAddress UnreachableIP,
		NetSim_EVENTDETAILS* pstruEventDetails);
	int fn_NetSim_AODV_ProcessRERR(NetSim_EVENTDETAILS* pstruEventDetails);


#ifdef  __cplusplus
}
#endif
#endif
