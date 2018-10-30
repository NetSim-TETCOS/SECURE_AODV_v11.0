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

#include "main.h"
#include "AODV.h"
#include "List.h"
static unsigned int nRREQId=0;
double fn_NetSim_AODV_AddTimeOut(NETSIM_IPAddress dest,NetSim_EVENTDETAILS* pstruEventDetails,double time);
double fnGetTimeoutTime(int ttl,int count);
int fn_NetSim_AODV_InsertInRREQSeenTable(AODV_DEVICE_VAR* devVar,
	NETSIM_IPAddress orginator,
	unsigned int nRREQId,
	double dTime);
int fnIncreaseTTL(AODV_DEVICE_VAR* devVar,NETSIM_IPAddress ip);
int fn_NetSim_AODV_InterstInRREQSentTable(AODV_DEVICE_VAR* devVar,
	NETSIM_IPAddress dest,
	int ttl,
	double dTimout);
int fnDeleteRREQSentTable(AODV_DEVICE_VAR* devVar,AODV_RREQ_SENT_TABLE* table);
bool fnCheckRREQSeenTable(AODV_DEVICE_VAR* devVar,AODV_RREQ* rreq);
/**
This function is used to genereate an AODV Route Request Packet. It Adds the RREQ entry in the 
RREQ sent table.
*/
NetSim_PACKET* fn_NetSim_AODV_GenerateRREQ(NetSim_EVENTDETAILS* pstruEventDetails)
{
	double time;
	NETSIM_IPAddress Dest = pstruEventDetails->pPacket->pstruNetworkData->szDestIP;
	unsigned int nDestSeq = fnFindSequenceNumber(AODV_DEV_VAR(pstruEventDetails->nDeviceId),Dest);
	NetSim_PACKET* packet = fn_NetSim_AODV_GenerateCtrlPacket(pstruEventDetails->nDeviceId,
		0,
		0,
		pstruEventDetails->dEventTime,
		AODVctrlPacket_RREQ);
	AODV_RREQ* rreq = (AODV_RREQ*)calloc(1,sizeof* rreq);
	//Set the packet size
	packet->pstruNetworkData->dPacketSize = AODV_RREQ_SIZE(pstruEventDetails->nDeviceId);
	packet->pstruNetworkData->dOverhead = AODV_RREQ_SIZE(pstruEventDetails->nDeviceId);
	packet->pstruNetworkData->Packet_RoutingProtocol = rreq;
	rreq->DestinationIPAddress = IP_COPY(Dest);
	rreq->DestinationSequenceNumber = nDestSeq;
	rreq->HopCount = 0;
	if(!nDestSeq)
		rreq->JRGDU[4] = '1';
	rreq->OriginatorIPAddress = aodv_get_curr_ip();
	rreq->OriginatorSequenceNumber = ++AODV_DEV_VAR(pstruEventDetails->nDeviceId)->nSequenceNumber;
	rreq->RREQID = ++nRREQId;
	rreq->Type = 1;
	rreq->LastAddress = IP_COPY(rreq->OriginatorIPAddress);
	fn_NetSim_AODV_InsertInRREQSeenTable(AODV_DEV_VAR(pstruEventDetails->nDeviceId),
		rreq->OriginatorIPAddress,
		nRREQId,
		pstruEventDetails->dEventTime);
	//Set the TTL
	packet->pstruNetworkData->nTTL = fnIncreaseTTL(AODV_DEV_VAR(pstruEventDetails->nDeviceId),
		rreq->DestinationIPAddress)+1;
	packet->pstruNetworkData->nTTL += 1;
	time = fnGetTimeoutTime(packet->pstruNetworkData->nTTL-1,1);
	//Add rreq timeout event
	time = fn_NetSim_AODV_AddTimeOut(rreq->DestinationIPAddress,pstruEventDetails,time);

	fn_NetSim_AODV_InterstInRREQSentTable(AODV_DEV_VAR(pstruEventDetails->nDeviceId),
		rreq->DestinationIPAddress,
		packet->pstruNetworkData->nTTL-1,
		time);
	//update the metrics
	AODV_METRICS_VAR(pstruEventDetails->nDeviceId).rreqSent++;
	return packet;
}
/**
This function is used to find if a RREQ to the given ip is sent. This is searched in the 
RREQ_SENT_TABLE. If an entry is encountered, the corresponding table is returned. Else NULL
is returned.
*/
AODV_RREQ_SENT_TABLE* fnFindSentTable(AODV_DEVICE_VAR* devVar,NETSIM_IPAddress ip)
{
	AODV_RREQ_SENT_TABLE* table = devVar->rreqSentTable;
	while(table)
	{
		if(!IP_COMPARE(table->DestAddress,ip))
			return table;
		table=LIST_NEXT(table);
	}
	return NULL;
}
/**
This functions inserts a RREQ querry in the RREQ sent table
*/
int fn_NetSim_AODV_InterstInRREQSentTable(AODV_DEVICE_VAR* devVar,
	NETSIM_IPAddress dest,
	int ttl,
	double dTimout)
{
	AODV_RREQ_SENT_TABLE* table = fnFindSentTable(devVar,dest);
	if(!table)
	{
		table = RREQSENTTABLE_ALLOC();
		table->DestAddress = IP_COPY(dest);
		table->times = 1;
		LIST_ADD_LAST(&devVar->rreqSentTable,table);
	}
	if(ttl == AODV_NET_DIAMETER)
		table->times += 1;
	table->ttl = ttl;
	table->dTimeoutTime = dTimout;
	return 1;
}
/**
This function increases the TTL of the RREQ sent table
*/
int fnIncreaseTTL(AODV_DEVICE_VAR* devVar,NETSIM_IPAddress ip)
{
	AODV_RREQ_SENT_TABLE* table = fnFindSentTable(devVar,ip);
	if(!table)
		return AODV_TTL_START;
	if(table->ttl >= AODV_TTL_THRESHOLD)
		return AODV_NET_DIAMETER;
	if(table->ttl+AODV_TTL_INCREMENT > AODV_TTL_THRESHOLD)
		return AODV_TTL_THRESHOLD;
	return table->ttl+AODV_TTL_INCREMENT;

}
/**
This function inserts the data of a RREQ in the RREQ seen table. It contains the 
originator IP address. 
*/
int fn_NetSim_AODV_InsertInRREQSeenTable(AODV_DEVICE_VAR* devVar,
	NETSIM_IPAddress orginator,
	unsigned int nRREQId,
	double dTime)
{
	AODV_RREQ_SEEN_TABLE* table = RREQSEENTABLE_ALLOC();
	table->nRREQId = nRREQId;
	table->OrginatingNode = IP_COPY(orginator);
	table->time = dTime+AODV_PATH_DISCOVERY_TIME;
	LIST_ADD_LAST(&devVar->rreqSeenTable,table);
	return 1;
}
/**
This function adds the RREQ timeout event.
*/
double fn_NetSim_AODV_AddTimeOut(NETSIM_IPAddress dest,NetSim_EVENTDETAILS* pstruEventDetails,double time)
{
	NetSim_EVENTDETAILS pevent;
	pevent.dEventTime = pstruEventDetails->dEventTime+time;
	pevent.dPacketSize = 0;
	pevent.nApplicationId =0;
	pevent.nDeviceId=pstruEventDetails->nDeviceId;
	pevent.nDeviceType=pstruEventDetails->nDeviceType;
	pevent.nInterfaceId = aodv_get_curr_if();
	pevent.nEventType = TIMER_EVENT;
	pevent.nPacketId =0;
	pevent.nProtocolId = NW_PROTOCOL_AODV;
	pevent.nSegmentId=0;
	pevent.nSubEventType = AODVsubevent_RREQ_TIMEOUT;
	pevent.pPacket=NULL;
	pevent.szOtherDetails=IP_COPY(dest);
	fnpAddEvent(&pevent);
	return pevent.dEventTime;
}
/**
This function is used to retry an AODV route request. If the table is NULL or if the table
timeout is greater than present time or if the number of RREQ exceeds the RREQ_RETRY_LIMIT,
then RREQ is not sent again.

Else, a RREQ packet is generated and a NETWORK_OUT_EVENT is added.
*/
int fn_NetSim_AODV_RetryRREQ(NetSim_EVENTDETAILS* pstruEventDetails)
{
	unsigned int nDestSeq;
	NetSim_PACKET* packet;
	AODV_RREQ* rreq;
	double time;
	NETSIM_IPAddress Dest = pstruEventDetails->szOtherDetails;
	AODV_RREQ_SENT_TABLE* table = fnFindSentTable(AODV_DEV_VAR(pstruEventDetails->nDeviceId),
		Dest);
	if(!table)
	{
		IP_FREE(pstruEventDetails->szOtherDetails);
		return 0; //RREP already received
	}
	if(table->dTimeoutTime > pstruEventDetails->dEventTime)
	{
		IP_FREE(pstruEventDetails->szOtherDetails);
		return 0; //new RREQ sent
	}
	if(table->times > AODV_RREQ_RETRIES)
	{
		fnDeleteRREQSentTable(AODV_DEV_VAR(pstruEventDetails->nDeviceId),table);
		fnEmptyFIFOBuffer(AODV_DEV_VAR(pstruEventDetails->nDeviceId),Dest);
		IP_FREE(pstruEventDetails->szOtherDetails);
		return 1; //MAX retries limit reaches
	}

	nDestSeq = fnFindSequenceNumber(AODV_DEV_VAR(pstruEventDetails->nDeviceId),Dest);
	packet = fn_NetSim_AODV_GenerateCtrlPacket(pstruEventDetails->nDeviceId,
		0,
		0,
		pstruEventDetails->dEventTime,
		AODVctrlPacket_RREQ);
	rreq = calloc(1,sizeof* rreq);

	//Set the packet size
	packet->pstruNetworkData->dPacketSize = AODV_RREQ_SIZE(pstruEventDetails->nDeviceId);
	packet->pstruNetworkData->dOverhead = AODV_RREQ_SIZE(pstruEventDetails->nDeviceId);
	packet->pstruNetworkData->Packet_RoutingProtocol = rreq;
	rreq->DestinationIPAddress = IP_COPY(Dest);
	rreq->DestinationSequenceNumber = nDestSeq;
	rreq->HopCount = 0;
	if(!nDestSeq)
		rreq->JRGDU[4] = '1';
	rreq->OriginatorIPAddress = aodv_get_curr_ip();
	rreq->OriginatorSequenceNumber = ++AODV_DEV_VAR(pstruEventDetails->nDeviceId)->nSequenceNumber;
	rreq->RREQID = ++nRREQId;
	rreq->LastAddress = IP_COPY(rreq->OriginatorIPAddress);
	rreq->Type = 1;
	fn_NetSim_AODV_InsertInRREQSeenTable(AODV_DEV_VAR(pstruEventDetails->nDeviceId),
		rreq->OriginatorIPAddress,
		nRREQId,
		pstruEventDetails->dEventTime);
	//Set the TTL
	packet->pstruNetworkData->nTTL = fnIncreaseTTL(AODV_DEV_VAR(pstruEventDetails->nDeviceId),
		rreq->DestinationIPAddress)+1;
	time = fnGetTimeoutTime(packet->pstruNetworkData->nTTL,table->times);
	//Add rreq timeout event
	time = fn_NetSim_AODV_AddTimeOut(rreq->DestinationIPAddress,pstruEventDetails,time);

	fn_NetSim_AODV_InterstInRREQSentTable(AODV_DEV_VAR(pstruEventDetails->nDeviceId),
		rreq->DestinationIPAddress,
		packet->pstruNetworkData->nTTL-1,
		time);
	//Add Network out event to transmit RREQ
	pstruEventDetails->dPacketSize = 0;
	pstruEventDetails->nApplicationId = 0;
	pstruEventDetails->nEventType = NETWORK_OUT_EVENT;
	pstruEventDetails->nPacketId = 0;
	pstruEventDetails->nProtocolId = fn_NetSim_Stack_GetNWProtocol(pstruEventDetails->nDeviceId);
	pstruEventDetails->nSegmentId = 0;
	pstruEventDetails->nSubEventType = 0;
	pstruEventDetails->pPacket = packet;
	if(pstruEventDetails->szOtherDetails)
		IP_FREE(pstruEventDetails->szOtherDetails);
	pstruEventDetails->szOtherDetails = NULL;
	pstruEventDetails->pPacket->pstruNetworkData->szNextHopIp=NULL;
	fnpAddEvent(pstruEventDetails);
	//Update the metrics
	AODV_METRICS_VAR(pstruEventDetails->nDeviceId).rreqSent++;
	return 1;
}
/**
This function returns the traversal time
*/
double fnGetTimeoutTime(int ttl,int count)
{
	if(ttl!=AODV_NET_DIAMETER)
		return AODV_RING_TRAVERSAL_TIME(ttl);
	return count*AODV_NET_TRAVERSAL_TIME;
}
/**
This function deletes the RREQ snet table
*/
int fnDeleteRREQSentTable(AODV_DEVICE_VAR* devVar,AODV_RREQ_SENT_TABLE* table)
{
	LIST_FREE(&devVar->rreqSentTable,table);
	return 1;
}
/**
This function process the AODV RREQ. It checks for duplicate entry of the received RREQ packet.
If the RREQ packet was received previously, it frees the packet.
Else, it adds the RREQ in the AODV seen table, Inserts the reverse route in the Route table.
If the device has the Route to the target, it generates a RREP.
else it forwards the RREQ.
*/
int fn_NetSim_AODV_ProcessRREQ(NetSim_EVENTDETAILS* pstruEventDetails)
{
	NetSim_PACKET* packet = pstruEventDetails->pPacket;
	AODV_RREQ* rreq=(AODV_RREQ*)pstruEventDetails->pPacket->pstruNetworkData->Packet_RoutingProtocol;
	//Check for duplicate entry
	if(fnCheckRREQSeenTable(AODV_DEV_VAR(pstruEventDetails->nDeviceId),rreq))
	{
		fn_NetSim_Packet_FreePacket(packet);
		pstruEventDetails->pPacket=NULL;
		return 0;//RREQ already received
	}
	//Insert in RREQ seen table
	fn_NetSim_AODV_InsertInRREQSeenTable(AODV_DEV_VAR(pstruEventDetails->nDeviceId),
		rreq->OriginatorIPAddress,
		rreq->RREQID, 
		pstruEventDetails->dEventTime);
	//Add to precursos list
	AODV_INSERT_PRECURSOR(rreq->LastAddress);
	//Add reverse route in route table
	AODV_INSERT_ROUTE_TABLE(rreq->OriginatorIPAddress,
		rreq->OriginatorSequenceNumber,
		rreq->HopCount,
		rreq->LastAddress,
		pstruEventDetails->dEventTime+2*AODV_NET_TRAVERSAL_TIME-2*rreq->HopCount*AODV_NODE_TRAVERSAL_TIME);
	//Transmit fifo buffer
	AODV_TRANSMIT_FIFO(AODV_DEV_VAR(pstruEventDetails->nDeviceId));
	//Check for the destination
	if(!IP_COMPARE(aodv_get_curr_ip(),rreq->DestinationIPAddress))
	{
		//Generate the route reply
		AODV_GENERATE_RREP();
		//Free the rreq packet
		fn_NetSim_Packet_FreePacket(packet);
		pstruEventDetails->pPacket=NULL;
	}
	else
	{
		if(AODV_CHECK_ROUTE_FOUND(rreq->DestinationIPAddress) &&
			rreq->JRGDU[3] != '1' /* Destination only flag*/)
		{
			if(AODV_GENERATE_RREP_BY_IN())
			{
				fn_NetSim_Packet_FreePacket(packet);
				pstruEventDetails->pPacket=NULL;
			}
		}
		else
		{
			//Forward the rreq
			AODV_FORWARD_RREQ();
		}
	}
	return 1;
}
/**
This function checks if the RREQ is there in the AODV seen table
*/
bool fnCheckRREQSeenTable(AODV_DEVICE_VAR* devVar,AODV_RREQ* rreq)
{
	AODV_RREQ_SEEN_TABLE* table = devVar->rreqSeenTable;
	while(table)
	{
		if(!IP_COMPARE(table->OrginatingNode,rreq->OriginatorIPAddress) &&
			table->nRREQId == rreq->RREQID)
			return true;
		table=LIST_NEXT(table);
	}
	return false;
}
/**
This function forwards the RREQ. It creates, a control packet, adds the necessary header and 
adds the Network Out Event.
*/
int fn_NetSim_AODV_ForwardRREQ(NetSim_EVENTDETAILS* pstruEventDetails)
{
	unsigned int nSeq;
	NetSim_EVENTDETAILS pevent;
	AODV_RREQ* rreq = (AODV_RREQ*)pstruEventDetails->pPacket->pstruNetworkData->Packet_RoutingProtocol;
	if(pstruEventDetails->pPacket->pstruNetworkData->nTTL == 1)
	{
		fn_NetSim_Packet_FreePacket(pstruEventDetails->pPacket);
		return 1;//Don't forward the RREQ
	}
	rreq->HopCount++;
	nSeq = fnFindSequenceNumber(AODV_DEV_VAR(pstruEventDetails->nDeviceId),rreq->DestinationIPAddress);
	if(nSeq > rreq->DestinationSequenceNumber)
		rreq->DestinationSequenceNumber = nSeq;
	if(rreq->DestinationSequenceNumber)
		rreq->JRGDU[4]='1';
	if(rreq->LastAddress)
		IP_FREE(rreq->LastAddress);
	rreq->LastAddress = aodv_get_curr_ip();
	pstruEventDetails->pPacket->nTransmitterId = pstruEventDetails->nDeviceId;
	pstruEventDetails->pPacket->nReceiverId = 0;
	//Add network out event to forward the route req
	memcpy(&pevent,pstruEventDetails,sizeof pevent);
	pevent.nEventType = NETWORK_OUT_EVENT;
	pevent.nSubEventType = 0;
	pstruEventDetails->pPacket->pstruNetworkData->szNextHopIp=NULL;
	fnpAddEvent(&pevent);
	//update the metrics
	AODV_METRICS_VAR(pstruEventDetails->nDeviceId).rreqForwarded++;
	return 1;
}


