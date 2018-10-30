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

#include "main.h"
#include "AODV.h"
#include "List.h"

/**
This function generates a route reply
*/
int fn_NetSim_AODV_GenerateRREP(NetSim_EVENTDETAILS* pstruEventDetails)
{
	NETSIM_ID id;
	NetSim_EVENTDETAILS pevent;
	AODV_RREQ* rreq = (AODV_RREQ*)pstruEventDetails->pPacket->pstruNetworkData->Packet_RoutingProtocol;
	NetSim_PACKET* packet = fn_NetSim_AODV_GenerateCtrlPacket(pstruEventDetails->nDeviceId,
		fn_NetSim_Stack_GetDeviceId_asIP(rreq->OriginatorIPAddress,&id),
		fn_NetSim_Stack_GetDeviceId_asIP(rreq->LastAddress,&id),
		pstruEventDetails->dEventTime,
		AODVctrlPacket_RREP);
	AODV_RREP* rrep = (AODV_RREP*)calloc(1,sizeof* rrep);
	packet->pstruNetworkData->Packet_RoutingProtocol = rrep;
	packet->pstruNetworkData->dPacketSize = AODV_RREP_SIZE(pstruEventDetails->nDeviceId);
	packet->pstruNetworkData->dOverhead = AODV_RREP_SIZE(pstruEventDetails->nDeviceId);
	packet->pstruNetworkData->nTTL = rreq->HopCount+2;
	rrep->DestinationIPaddress = IP_COPY(rreq->DestinationIPAddress);
	rrep->DestinationSequenceNumber = ++AODV_DEV_VAR(pstruEventDetails->nDeviceId)->nSequenceNumber;
	rrep->HopCount = 1;
	rrep->Lifetime = AODV_MY_ROUTE_TIMEOUT;
	rrep->OriginatorIPaddress = IP_COPY(rreq->OriginatorIPAddress);
	rrep->PrefixSz = 0;
	rrep->Type = 2;
	rrep->LastAddress = aodv_get_curr_ip();
	//Add network out event
	memcpy(&pevent,pstruEventDetails,sizeof pevent);
	pevent.dPacketSize = AODV_RREP_SIZE(pstruEventDetails->nDeviceId);

	#ifdef SAODV_ENABLE
		get_saodv_ctrl_packet(packet);
	#endif

	pevent.nEventType = NETWORK_OUT_EVENT;
	pevent.nSubEventType = 0;
	pevent.pPacket = packet;
	pstruEventDetails->pPacket->pstruNetworkData->szNextHopIp=NULL;
	fnpAddEvent(&pevent);
	//Update the metrics
	AODV_METRICS_VAR(pstruEventDetails->nDeviceId).rrepSent++;
	return 1;
}
/**
This function processes a route reply.
It adds the entry of the next hop in Route Table, transmits the packets in FIFO Buffer and
inserts precursor.
Deletes the RREQ entry from sent table and forwards the rrep if the device is not 
the source node.
*/
int fn_NetSim_AODV_ProcessRREP(NetSim_EVENTDETAILS* pstruEventDetails)
{
	AODV_RREP* rrep = (AODV_RREP*)pstruEventDetails->pPacket->pstruNetworkData->Packet_RoutingProtocol;
	//Update the routing table
	if(rrep->DestinationIPaddress == aodv_get_curr_ip())
		return 0;

	AODV_INSERT_ROUTE_TABLE(rrep->DestinationIPaddress,
		rrep->DestinationSequenceNumber,
		rrep->HopCount,
		rrep->LastAddress,
		pstruEventDetails->dEventTime+AODV_ACTIVE_ROUTE_TIMEOUT);
	//Transmit the buffer
	AODV_TRANSMIT_FIFO(AODV_DEV_VAR(pstruEventDetails->nDeviceId));
	//Update the precursor list
	AODV_INSERT_PRECURSOR(rrep->LastAddress);
	AODV_UPDATE_ROUTE_TABLE(rrep->LastAddress,rrep->Lifetime);
	if(!IP_COMPARE(aodv_get_curr_ip(),rrep->OriginatorIPaddress))
	{
		//Delete entry from sent table
		AODV_RREQ_SENT_TABLE* table = AODV_DEV_VAR(pstruEventDetails->nDeviceId)->rreqSentTable;
		while(table)
		{
			if(!IP_COMPARE(table->DestAddress,rrep->DestinationIPaddress))
			{
				IP_FREE(table->DestAddress);
				LIST_FREE((void**)&AODV_DEV_VAR(pstruEventDetails->nDeviceId)->rreqSentTable,table);
				break;
			}
			table = (AODV_RREQ_SENT_TABLE*)LIST_NEXT(table);
		}
		//Drop the packet
		fn_NetSim_Packet_FreePacket(pstruEventDetails->pPacket);
	}
	else
	{
		//Forward the route reply
		AODV_FORWARD_RREP();
	}
	return 1;
}
/**
This functiopn is used to forward the RREP packet to the previous hop, else the 
packet is freed.
*/
int fn_NetSim_AODV_ForwardRREP(NetSim_EVENTDETAILS* pstruEventDetails)
{
	AODV_RREP* rrep = (AODV_RREP*)pstruEventDetails->pPacket->pstruNetworkData->Packet_RoutingProtocol;
	if(AODV_CHECK_ROUTE_FOUND(rrep->OriginatorIPaddress) &&
	   !isBroadcastPacket(pstruEventDetails->pPacket)/*For hello packet*/)
	{
		NetSim_EVENTDETAILS pevent;
		NETSIM_ID port;
		NETSIM_IPAddress nextHop = IP_COPY(AODV_FIND_NEXT_HOP(AODV_DEV_VAR(pstruEventDetails->nDeviceId),rrep->OriginatorIPaddress));
		rrep->HopCount+=1;
		rrep->LastAddress = aodv_get_curr_ip();
		pstruEventDetails->pPacket->nReceiverId = fn_NetSim_Stack_GetDeviceId_asIP(nextHop,&port);
		pstruEventDetails->pPacket->nTransmitterId = pstruEventDetails->nDeviceId;
		pstruEventDetails->pPacket->pstruNetworkData->szNextHopIp = (nextHop);

		#ifdef SAODV_ENABLE
			get_saodv_ctrl_packet(pstruEventDetails->pPacket);
		#endif

		//Add network out event to transmit RREP
		memcpy(&pevent,pstruEventDetails,sizeof pevent);
		pevent.nEventType = NETWORK_OUT_EVENT;
		pstruEventDetails->pPacket->pstruNetworkData->szNextHopIp=NULL;
		fnpAddEvent(&pevent);
		//Update the metrics
		if(pevent.pPacket->pstruNetworkData->nTTL>1)
			AODV_METRICS_VAR(pevent.nDeviceId).rrepForwarded++;
	}
	else
		fn_NetSim_Packet_FreePacket(pstruEventDetails->pPacket);
	return 1;
}
/** 
This function generates a RREP by an intermediate node if the intermediate node has 
a route to the target entered in the route table.
*/
int fn_NetSim_AODV_GenerateRREPByIntermediate(NetSim_EVENTDETAILS* pstruEventDetails)
{
	NETSIM_ID id;
	NetSim_EVENTDETAILS pevent;
	AODV_RREQ* rreq = (AODV_RREQ*)pstruEventDetails->pPacket->pstruNetworkData->Packet_RoutingProtocol;
	AODV_ROUTETABLE* table = fnFindRouteTable(AODV_DEV_VAR(pstruEventDetails->nDeviceId)->routeTable,rreq->DestinationIPAddress);
	NetSim_PACKET* packet = fn_NetSim_AODV_GenerateCtrlPacket(pstruEventDetails->nDeviceId,
		fn_NetSim_Stack_GetDeviceId_asIP(rreq->OriginatorIPAddress,&id),
		fn_NetSim_Stack_GetDeviceId_asIP(rreq->LastAddress,&id),
		pstruEventDetails->dEventTime,
		AODVctrlPacket_RREP);
	AODV_RREP* rrep = (AODV_RREP*)calloc(1,sizeof* rrep);

	if(!IP_COMPARE(table->NextHop,rreq->LastAddress))
	{
		AODV_FORWARD_RREQ();
		return 0;
	}

	packet->pstruNetworkData->Packet_RoutingProtocol = rrep;
	packet->pstruNetworkData->dPacketSize = AODV_RREP_SIZE(pstruEventDetails->nDeviceId);
	packet->pstruNetworkData->dOverhead = AODV_RREP_SIZE(pstruEventDetails->nDeviceId);
	packet->pstruNetworkData->nTTL = AODV_NET_DIAMETER;
	rrep->DestinationIPaddress = IP_COPY(rreq->DestinationIPAddress);
	rrep->DestinationSequenceNumber = fnFindSequenceNumber(AODV_DEV_VAR(pstruEventDetails->nDeviceId),
		rreq->DestinationIPAddress);
	rrep->HopCount = table->HopCount;
	rrep->Lifetime = (unsigned int)(table->Lifetime-pstruEventDetails->dEventTime);
	rrep->OriginatorIPaddress = IP_COPY(rreq->OriginatorIPAddress);
	rrep->PrefixSz = 0;
	rrep->Type = 2;
	rrep->LastAddress = aodv_get_curr_ip();
	#ifdef SAODV_ENABLE
		get_saodv_ctrl_packet(packet);
	#endif
	//Add network out event
	memcpy(&pevent,pstruEventDetails,sizeof pevent);
	pevent.dPacketSize = AODV_RREP_SIZE(pstruEventDetails->nDeviceId);
	pevent.nEventType = NETWORK_OUT_EVENT;
	pevent.nSubEventType = 0;
	pevent.pPacket = packet;
	pstruEventDetails->pPacket->pstruNetworkData->szNextHopIp=NULL;
	fnpAddEvent(&pevent);
	//Update the metrics
	AODV_METRICS_VAR(pevent.nDeviceId).rrepSent++;
	return 1;
}

