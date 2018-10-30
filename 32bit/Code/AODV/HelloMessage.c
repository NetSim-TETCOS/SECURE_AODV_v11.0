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
This function generate the Hello Message Packets and transmit them.
*/
int fn_NetSim_AODV_TransmitHelloMessage(NetSim_EVENTDETAILS* pstruEventDetails)
{
	NetSim_EVENTDETAILS pevent;
	//Add an event to transmit next hello packet
	memcpy(&pevent,pstruEventDetails,sizeof pevent);
	pevent.dEventTime += AODV_HELLO_INTERVAL;
	fnpAddEvent(&pevent);
	if(AODV_DEV_VAR(pstruEventDetails->nDeviceId)->dLastBroadcastTime+AODV_HELLO_INTERVAL > pstruEventDetails->dEventTime)
		return 0; //Already broadcast packet sent
	if(AODV_DEV_VAR(pstruEventDetails->nDeviceId)->routeTable)
	{
		NetSim_PACKET* packet = fn_NetSim_AODV_GenerateCtrlPacket(pstruEventDetails->nDeviceId,
			0,
			0,
			pstruEventDetails->dEventTime,
			AODVctrlPacket_RREP);
		AODV_RREP* hello = calloc(1,sizeof* hello);
		packet->pstruNetworkData->Packet_RoutingProtocol = hello;
		packet->pstruNetworkData->nTTL = 2;
		hello->DestinationIPaddress = aodv_get_curr_ip();
		hello->DestinationSequenceNumber = AODV_DEV_VAR(pstruEventDetails->nDeviceId)->nSequenceNumber;
		hello->HopCount = 0;
		hello->LastAddress = aodv_get_curr_ip();
		hello->Lifetime = (unsigned int)pstruEventDetails->dEventTime +
			AODV_ALLOWED_HELLO_LOSS*AODV_HELLO_INTERVAL;
		hello->OriginatorIPaddress = aodv_get_curr_ip();
		//Broadcast hello message
		//Add network out event
		pevent.dEventTime = pstruEventDetails->dEventTime;
		pevent.dPacketSize = fnGetPacketSize(packet);
		pevent.nApplicationId = 0;
		pevent.nApplicationId = 0;
		pevent.nEventType = NETWORK_OUT_EVENT;
		pevent.nProtocolId = fn_NetSim_Stack_GetNWProtocol(pstruEventDetails->nDeviceId);
		pevent.nSubEventType = 0;
		pevent.pPacket = packet;
		pevent.szOtherDetails = NULL;
		fnpAddEvent(&pevent);
		packet->pstruNetworkData->szNextHopIp=NULL;
	}
	return 1;
}
