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
This function adds a packet to the FIFO Buffer
*/
bool fn_NetSim_AODV_AddToFIFOBuffer(NetSim_PACKET* packet,
	AODV_FIFO** fifoBuffer,
	double time)
{
	AODV_FIFO* fifo = *fifoBuffer;
	while(fifo)
	{
		if(!IP_COMPARE(fifo->destination,packet->pstruNetworkData->szDestIP))
		{
			NetSim_PACKET* packetList = fifo->packetList;
			if(!fifo->packetList)
			{
				fifo->packetList = packet;
				fifo->time = time;
				return false;
			}
			while(packetList->pstruNextPacket)
				packetList = packetList->pstruNextPacket;
			packetList->pstruNextPacket = packet;
			return true;
		}
		fifo = LIST_NEXT(fifo);
	}
	fifo = FIFO_ALLOC();
	fifo->destination = IP_COPY(packet->pstruNetworkData->szDestIP);
	fifo->packetList = packet;
	fifo->time = time;
	LIST_ADD_LAST(fifoBuffer,fifo);
	return false;
}
/**
This function empties the FIFO buffer
*/
int fnEmptyFIFOBuffer(AODV_DEVICE_VAR* devVar,NETSIM_IPAddress dest)
{
	AODV_FIFO* fifo = devVar->fifo;
	while(fifo)
	{
		if(!IP_COMPARE(fifo->destination,dest))
		{
			while(fifo->packetList)
			{
				NetSim_PACKET* packet = fifo->packetList;
				fifo->packetList = packet->pstruNextPacket;
				packet->pstruNextPacket = NULL;
				fn_NetSim_Packet_FreePacket(packet);
				//Update the metrics
				AODV_METRICS_VAR(pstruEventDetails->nDeviceId).packetDropped++;
			}
			IP_FREE(fifo->destination);
			LIST_FREE(&devVar->fifo,fifo);
			break;
		}
		fifo=LIST_NEXT(fifo);
	}
	return 1;
}
/**
This function Transmits the FIFO Buffer
*/
int fn_NetSim_AODV_TransmitFIFOBuffer(AODV_DEVICE_VAR* devVar,
	NetSim_EVENTDETAILS* pstruEventDetails)
{
	NetSim_EVENTDETAILS pevent;
	AODV_FIFO* fifo = devVar->fifo;
	memcpy(&pevent,pstruEventDetails,sizeof pevent);
	while(fifo)
	{
		if(AODV_CHECK_ROUTE_FOUND(fifo->destination))
		{
			while(fifo->packetList)
			{
				NetSim_PACKET* packet = fifo->packetList;
				fifo->packetList = fifo->packetList->pstruNextPacket;
				packet->pstruNextPacket = NULL;
				pevent.dPacketSize = fnGetPacketSize(packet);
				if(packet->pstruAppData)
				{
					pevent.nApplicationId = packet->pstruAppData->nApplicationId;
					pevent.nSegmentId = packet->pstruAppData->nSegmentId;
				}
				pevent.nInterfaceId = 0;
				pevent.nEventType = NETWORK_OUT_EVENT;
				pevent.nPacketId = packet->nPacketId;
				pevent.nProtocolId = fn_NetSim_Stack_GetNWProtocol(pstruEventDetails->nDeviceId);
				pevent.nSubEventType = 0;
				pevent.pPacket = packet;
				pevent.szOtherDetails = NULL;
				fnpAddEvent(&pevent);
			}
		}
		fifo = (AODV_FIFO*)LIST_NEXT(fifo);
	}
	return 1;
}
