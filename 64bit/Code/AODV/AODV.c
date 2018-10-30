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

int fn_NetSim_AODV_Init_F();
char* fn_NetSim_AODV_Trace_F(NETSIM_ID);
int fn_NetSim_AODV_FreePacket_F();
int fn_NetSim_AODV_CopyPacket_F();
int fn_NetSim_AODV_Metrics_F(PMETRICSWRITER metricsWriter);
int fn_NetSim_AODV_Configure_F();
char* fn_NetSim_AODV_ConfigPacketTrace_F();
int fn_NetSim_AODV_Finish_F();
char* fn_NetSim_AODV_WritePacketTrace_F();





/**
AODV Init function initializes the AODV parameters.
*/
_declspec(dllexport) int fn_NetSim_AODV_Init()
{
	return fn_NetSim_AODV_Init_F();
}
/**
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
This is the AODV Run function which gets called by the IP layer for routing the data
by the DSR Network Routing Protocol. It includes the events NETWORK_OUT_EVENT, 
NETWORK_IN_EVENT and TIMER_EVENT.

NETWORK_OUT_EVENT - 
It process the Data Packets which arrive at the NetworkOut layer to route the packet.
NETWORK_IN_EVENT - 
It processes Data Packets, Route Request Packets, Route Reply Packets, Route Error 
Packets, Ack Packet
TIMER_EVENT -
It Process the AODV Route Request timeout and the AODV Maintenance Buffer timeout.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
_declspec(dllexport) int fn_NetSim_AODV_Run()
{
	set_aodv_curr();
	NETSIM_ID in = aodv_get_curr_if();
	if (!isAodvConfigured(pstruEventDetails->nDeviceId, in))
		return -1;

	switch(pstruEventDetails->nEventType)
	{
	case NETWORK_OUT_EVENT:
		{
			if(pstruEventDetails->pPacket->nPacketType != PacketType_Control ||
				pstruEventDetails->pPacket->nControlDataType/100 != NW_PROTOCOL_AODV)
			{
				NETSIM_IPAddress n1,n2;
				NETSIM_IPAddress dest = pstruEventDetails->pPacket->pstruNetworkData->szDestIP;
				NETSIM_IPAddress ip = aodv_get_curr_ip();
				if(ip->type != dest->type)
					return 0;
				n1=IP_NETWORK_ADDRESS(ip,
					DEVICE_INTERFACE(pstruEventDetails->nDeviceId,in)->szSubnetMask,
					DEVICE_INTERFACE(pstruEventDetails->nDeviceId,in)->prefix_len);
				n2=IP_NETWORK_ADDRESS(dest,
					DEVICE_INTERFACE(pstruEventDetails->nDeviceId,in)->szSubnetMask,
					DEVICE_INTERFACE(pstruEventDetails->nDeviceId,in)->prefix_len);

				if(!IP_COMPARE(n1,n2))
					AODV_PACKET_PROCESSING();
				else
					return 0; //Other network packet
			}
			else
			{
				//No processing required
				pstruEventDetails->pPacket->pstruNetworkData->szNextHopIp=aodv_get_dev_ip(pstruEventDetails->pPacket->nReceiverId);
			}
			if (pstruEventDetails->pPacket)
			{
				if (pstruEventDetails->pPacket->pstruNetworkData->nTTL > 1)
				{
					pstruEventDetails->nInterfaceId = in;
					if (!pstruEventDetails->pPacket->nReceiverId)
						AODV_DEV_VAR(pstruEventDetails->nDeviceId)->dLastBroadcastTime = pstruEventDetails->dEventTime;
				}
				else
				{
					fn_NetSim_Packet_FreePacket(pstruEventDetails->pPacket);
					pstruEventDetails->pPacket = NULL;
					pstruEventDetails->nInterfaceId = 0;
				}
			}
			else
				pstruEventDetails->nInterfaceId = 0;
		}
		break;
	case NETWORK_IN_EVENT:
#ifdef SAODV_ENABLE
		switch(pstruEventDetails->pPacket->nControlDataType)
		{
		case SAODV_RREQ:
		case SAODV_RREP:
		case SAODV_RERR:
			get_aodv_ctrl_packet(pstruEventDetails->pPacket);
			break;
		}
		if(pstruEventDetails->pPacket == NULL)
		{
			return -1; //Decryption fail.
		}
#endif
		switch(pstruEventDetails->pPacket->nControlDataType)
		{
		case AODVctrlPacket_RREQ:

			if(IsMaliciousNode(pstruEventDetails->nDeviceId))
			 fn_NetSim_AODV_MaliciousRouteAddToTable(pstruEventDetails);
						
			AODV_PROCESS_RREQ();
			pstruEventDetails->pPacket=NULL;
			break;
		case AODVctrlPacket_RREP:
			AODV_PROCESS_RREP();
			pstruEventDetails->pPacket=NULL;
			break;
		case AODVctrlPacket_RERR:
			AODV_PROCESS_RERR();
			pstruEventDetails->pPacket=NULL;
			break;
		default:
		{
			NetSim_PACKET* packet = pstruEventDetails->pPacket;
			if (isDestFoundinPacket(packet, pstruEventDetails->nDeviceId))
			{
				//Update the life time
				AODV_UPDATE_ROUTE_TABLE(packet->pstruNetworkData->szSourceIP,
										pstruEventDetails->dEventTime + AODV_ACTIVE_ROUTE_TIMEOUT);
			}		
	if(IsMaliciousNode(pstruEventDetails->nDeviceId))
				fn_NetSim_AODV_MaliciousProcessSourceRouteOption(pstruEventDetails);
		
		else
			{
				packet->pstruNetworkData->szNextHopIp = NULL;
				packet->DropNotification = NULL;
				//Update the life time of dest
				AODV_UPDATE_ROUTE_TABLE(packet->pstruNetworkData->szDestIP,
										pstruEventDetails->dEventTime + AODV_ACTIVE_ROUTE_TIMEOUT);
				//Update the life time of source
				AODV_UPDATE_ROUTE_TABLE(packet->pstruNetworkData->szSourceIP,
										pstruEventDetails->dEventTime + AODV_ACTIVE_ROUTE_TIMEOUT);
			}
		}
		break;
		}
		break;
	case TIMER_EVENT:
		switch(pstruEventDetails->nSubEventType)
		{
		case AODVsubevent_RREQ_TIMEOUT:
			AODV_RETRY_RREQ();
			break;
		case AODVsubevent_TRANSMIT_HELLO:
			AODV_TRANSMIT_HELLO();
			break;
		case AODVsubevent_ACTIVE_ROUTE_TIMEOUT:
			AODV_ACTIVE_ROUTE_TIMEOUT_EVENT();
			break;
		default:
			fnNetSimError("Unknown subevent for AODV");
			break;
		}
		break;
	default:
		fnNetSimError("Unknown event type for AODV");
		break;
	}
	return 1;
}
/**
This function is called by NetworkStack.dll, while writing the evnt trace 
to get the sub event as a string.
*/
_declspec(dllexport) char* fn_NetSim_AODV_Trace(NETSIM_ID id)
{
	return fn_NetSim_AODV_Trace_F(id);
}
/**
This function is called by NetworkStack.dll, to free the AODV protocol parameters.
*/
_declspec(dllexport) int fn_NetSim_AODV_FreePacket(NetSim_PACKET* packet)
{
#ifdef SAODV_ENABLE
	switch(packet->nControlDataType)
	{
	case SAODV_RERR:
	case SAODV_RREQ:
	case SAODV_RREP:
		saodv_free_packet(packet);
		return 0;
		break;
	default:
		remove_from_mapper(packet->pstruNetworkData->Packet_RoutingProtocol, true);
		return 0;
		break;
	}
#endif
	return fn_NetSim_AODV_FreePacket_F(packet);
}
/**
This function is called by NetworkStack.dll, to copy the AODV protocol parameters
from source packet to destination.
*/
_declspec(dllexport) int fn_NetSim_AODV_CopyPacket(const NetSim_PACKET* destPacket,const NetSim_PACKET* srcPacket)
{
#ifdef SAODV_ENABLE
	switch(srcPacket->nControlDataType)
	{
	case SAODV_RERR:
	case SAODV_RREQ:
	case SAODV_RREP:
		saodv_copy_packet(destPacket,srcPacket);
		return 0;
		break;
	default:
#endif
	return fn_NetSim_AODV_CopyPacket_F(destPacket,srcPacket);
#ifdef SAODV_ENABLE
	break;
	}
#endif
}
/**
This function call WLAN Metrics function in lib file to write the Metrics in Metrics.txt	
*/
_declspec(dllexport) int fn_NetSim_AODV_Metrics(PMETRICSWRITER metricsWriter)
{
	return fn_NetSim_AODV_Metrics_F(metricsWriter);
}
/**
This function is called by NetworkStack.dll, while configuring the device 
NETWORK layer for AODV protocol.	
*/
_declspec(dllexport) int fn_NetSim_AODV_Configure(void** var)
{
	return fn_NetSim_AODV_Configure_F(var);
}
/**
This function will return the string to write packet trace heading.
*/
_declspec(dllexport) char* fn_NetSim_AODV_ConfigPacketTrace()
{
	return fn_NetSim_AODV_ConfigPacketTrace_F();
}
/**
This function is called by NetworkStack.dll, once simulation end to free the 
allocated memory for the network.	
*/
_declspec(dllexport) int fn_NetSim_AODV_Finish()
{
	return fn_NetSim_AODV_Finish_F();
}
/**
This function will return the string to write packet trace.																									
*/
_declspec(dllexport) char* fn_NetSim_AODV_WritePacketTrace()
{
	return fn_NetSim_AODV_WritePacketTrace_F();
}
