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
This function Generates a route error and sends it to the previous HOP.
*/
int fn_NetSim_AODV_GenerateRERR(NETSIM_ID nDeviceId,
	NETSIM_IPAddress UnreachableIP,
	NetSim_EVENTDETAILS* pstruEventDetails)
{
	int DestCount=0;
	NETSIM_IPAddress* DestinationList=NULL;
	unsigned int* DestinationSequence=NULL;
	AODV_DEVICE_VAR* pstruDeviceVar = AODV_DEV_VAR(nDeviceId);
	AODV_ROUTETABLE* routeTable = pstruDeviceVar->routeTable;
	AODV_PRECURSORS_LIST* precursorList = pstruDeviceVar->precursorsList;
	while(routeTable)
	{
		if(!IP_COMPARE(routeTable->NextHop,UnreachableIP))
		{
			routeTable->routingFlags = AODV_RoutingFlag_InValid;
			routeTable->Lifetime = pstruEventDetails->dEventTime+AODV_DELETE_PERIOD;
			DestCount++;
			DestinationList = realloc(DestinationList,DestCount*(sizeof* DestinationList));
			DestinationSequence = realloc(DestinationSequence,DestCount*(sizeof* DestinationSequence));
			DestinationList[DestCount-1] = IP_COPY(routeTable->DestinationIPAddress);
			DestinationSequence[DestCount-1] = routeTable->DestinationSequenceNumber;
		}
		routeTable = LIST_NEXT(routeTable);
	}
	if(precursorList->count)
	{
		int loop;
		bool flag=false;
		for(loop=0;loop<precursorList->count;loop++)
		{
			if(!IP_COMPARE(precursorList->list[loop],UnreachableIP))
			{
				IP_FREE(precursorList->list[loop]);
				precursorList->count--;
				flag = true;
			}
			if(flag)
				precursorList->list[loop] = precursorList->list[loop+1];
		}
	}
	if(DestCount)
	{
		NetSim_EVENTDETAILS pevent;
		AODV_RERR* rerr;
		NetSim_PACKET* packet = fn_NetSim_AODV_GenerateCtrlPacket(nDeviceId,
			0,0,
			pstruEventDetails->dEventTime,
			AODVctrlPacket_RERR);
		if(DEVICE_NWPROTOCOL(pstruEventDetails->nDeviceId,1) == NW_PROTOCOL_IPV4)
		{
			packet->pstruNetworkData->dOverhead = AODV_RERR_SIZE_FIXED+8*DestCount;
			packet->pstruNetworkData->dPacketSize = AODV_RERR_SIZE_FIXED+8*DestCount;
		}
		else
		{
			packet->pstruNetworkData->dOverhead = AODV_RERR_SIZE_FIXED+20*DestCount;
			packet->pstruNetworkData->dPacketSize = AODV_RERR_SIZE_FIXED+20*DestCount;
		}
		packet->pstruNetworkData->nTTL = AODV_NET_DIAMETER;
		rerr = calloc(1,sizeof* rerr);
		packet->pstruNetworkData->Packet_RoutingProtocol = rerr;
		rerr->DestCount = DestCount;
		rerr->Type = 3;
		rerr->UnreachableDestinationIPAddress = DestinationList;
		rerr->UnreachableDestinationSequenceNumber = DestinationSequence;

		//Add Network out event to transmit packet
		memcpy(&pevent,pstruEventDetails,sizeof pevent);
		pevent.dPacketSize = packet->pstruNetworkData->dPacketSize;
		pevent.nApplicationId = 0;
		pevent.nEventType = NETWORK_OUT_EVENT;
		pevent.nPacketId = 0;
		pevent.nProtocolId = NW_PROTOCOL_AODV;
		pevent.nSegmentId = 0;
		pevent.nSubEventType = 0;
		pevent.pPacket = packet;
		pevent.szOtherDetails = NULL;
		pevent.pPacket->pstruNetworkData->szNextHopIp=NULL;
		fnpAddEvent(&pevent);
		//update the metrics
		AODV_METRICS_VAR(pevent.nDeviceId).rerrSent++;
		AODV_METRICS_VAR(pevent.nDeviceId).routeBreak++;
	}
	return 1;
}
/**
This function Processess a route error. It deletes the entry to the target in the Route Table
and forwards the REER packet to previous HOP if any.
*/
int fn_NetSim_AODV_ProcessRERR(NetSim_EVENTDETAILS* pstruEventDetails)
{
	bool flag = false;
	NETSIM_IPAddress nextHop = pstruEventDetails->pPacket->pstruNetworkData->szSourceIP;
	AODV_RERR* rerr = (AODV_RERR*)pstruEventDetails->pPacket->pstruNetworkData->Packet_RoutingProtocol;
	AODV_ROUTETABLE* routeTable = AODV_DEV_VAR(pstruEventDetails->nDeviceId)->routeTable;
	while(routeTable)
	{
		unsigned int loop;
		for(loop=0;loop<rerr->DestCount;loop++)
		{
			if(!IP_COMPARE(rerr->UnreachableDestinationIPAddress[loop],routeTable->DestinationIPAddress) &&
				!IP_COMPARE(nextHop,routeTable->NextHop))
			{
				flag = true; //Forward the rerr packet
				routeTable->routingFlags = AODV_RoutingFlag_InValid;
				routeTable->Lifetime = pstruEventDetails->dEventTime + AODV_DELETE_PERIOD;
				break;
			}

		}
		routeTable = LIST_NEXT(routeTable);
	}
	if(flag)
	{
		IP_FREE(pstruEventDetails->pPacket->pstruNetworkData->szSourceIP);
		pstruEventDetails->pPacket->pstruNetworkData->szSourceIP = aodv_get_curr_ip();
		pstruEventDetails->nEventType = NETWORK_OUT_EVENT;
		fnpAddEvent(pstruEventDetails);
		AODV_METRICS_VAR(pstruEventDetails->nDeviceId).rerrForwarded++;
	}
	return 1;
}