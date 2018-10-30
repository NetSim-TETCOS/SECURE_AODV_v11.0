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
_declspec(dllexport) int fn_NetSim_AODV_PacketDropNotification(NetSim_PACKET* packet);
/**
This function is called in the Network Out Event for routing of the packet.

If the Device route table has a route to the target, then the packet is added to sent to the
next hop ip

Whenever a packet is to be routed to a destination and the device route cache doesnt have
a route to the target, the Packet is added to the FIFO and a Route Discovery is 
iitiated if not already.

*/
int fn_NetSim_AODV_GeneralPacketProcessing(NetSim_EVENTDETAILS* pstruEventDetails)
{
	NETSIM_IPAddress destIP = pstruEventDetails->pPacket->pstruNetworkData->szDestIP;
	pstruEventDetails->pPacket->pstruNetworkData->szGatewayIP = IP_COPY(aodv_get_curr_ip());
	if(AODV_CHECK_ROUTE_FOUND(destIP))
	{
		NETSIM_ID port;
		//Route found
		NETSIM_IPAddress nextHop = IP_COPY(AODV_FIND_NEXT_HOP(AODV_DEV_VAR(pstruEventDetails->nDeviceId),destIP));
		pstruEventDetails->pPacket->nTransmitterId = pstruEventDetails->nDeviceId;
		pstruEventDetails->pPacket->nReceiverId = fn_NetSim_Stack_GetDeviceId_asIP(nextHop,&port);
		pstruEventDetails->pPacket->pstruNetworkData->szNextHopIp = nextHop;
		pstruEventDetails->pPacket->pstruNetworkData->nTTL = AODV_NET_DIAMETER;
		pstruEventDetails->pPacket->DropNotification = fn_NetSim_AODV_PacketDropNotification;

		///Update the life time
		AODV_UPDATE_ROUTE_TABLE(destIP,
			pstruEventDetails->dEventTime+AODV_ACTIVE_ROUTE_TIMEOUT);
		///Update the metrics
		if(pstruEventDetails->nDeviceId == pstruEventDetails->pPacket->nSourceId)
			AODV_METRICS_VAR(pstruEventDetails->nDeviceId).packetOrginated++;
		AODV_METRICS_VAR(pstruEventDetails->nDeviceId).packetTransmitted++;
	}
	else if(AODV_ADD_TO_FIFO(pstruEventDetails->pPacket,&AODV_DEV_VAR(pstruEventDetails->nDeviceId)->fifo,pstruEventDetails->dEventTime))
	{
		///No route found but RREQ is already sent
		pstruEventDetails->pPacket = NULL;
	}
	else
	{
		///No route found. generate RREQ
		pstruEventDetails->pPacket = AODV_GEN_RREQ();
	}
	return 1;
}
