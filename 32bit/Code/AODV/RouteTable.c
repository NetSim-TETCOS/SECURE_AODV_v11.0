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
/**
This function adds the route in the Route table of the node. If the table is not there,
it creates one with a certain timeout after which the entries are deleted.
*/
int fn_NetSim_AODV_InsertInRouteTable(NETSIM_IPAddress ip,
	unsigned int seqNumber,
	unsigned int hopCount,
	NETSIM_IPAddress nextHop,
	double lifeTime,
	NetSim_EVENTDETAILS* pstruEventDetails)
{
	AODV_DEVICE_VAR* devVar = AODV_DEV_VAR(pstruEventDetails->nDeviceId);
	AODV_ROUTETABLE* table = devVar->routeTable;
	while(table)
	{
		if(!IP_COMPARE(table->DestinationIPAddress,ip))
		{
			if(table->ValidDestinationSequenceNumberflag == false)
				break;
			if(table->DestinationSequenceNumber < seqNumber)
				break;
			if(table->DestinationSequenceNumber > seqNumber)
				return 1;
			if(hopCount+1<table->HopCount || table->routingFlags==AODV_RoutingFlag_InValid)
				break;
			else
				return 1;
		}
		table = LIST_NEXT(table);
	}
	if(!table)
	{
		NetSim_EVENTDETAILS pevent;
		//Create new table
		table = ROUTETABLE_ALLOC();
		table->Lifetime = pstruEventDetails->dEventTime+AODV_ACTIVE_ROUTE_TIMEOUT;
		LIST_ADD_LAST(&AODV_DEV_VAR(pstruEventDetails->nDeviceId)->routeTable,table);
		//Add an event for active route time out
		pevent.dEventTime = pstruEventDetails->dEventTime+AODV_ACTIVE_ROUTE_TIMEOUT;
		pevent.dPacketSize = 0;
		pevent.nApplicationId = 0;
		pevent.nDeviceId = pstruEventDetails->nDeviceId;
		pevent.nDeviceType = pstruEventDetails->nDeviceType;
		pevent.nEventType = TIMER_EVENT;
		pevent.nInterfaceId = pstruEventDetails->nInterfaceId;
		pevent.nPacketId = 0;
		pevent.nPrevEventId = pstruEventDetails->nPrevEventId;
		pevent.nProtocolId = NW_PROTOCOL_AODV;
		pevent.nSegmentId = 0;
		pevent.nSubEventType = AODVsubevent_ACTIVE_ROUTE_TIMEOUT;
		pevent.pPacket = NULL;
		pevent.szOtherDetails = IP_COPY(ip);
		fnpAddEvent(&pevent);
	}
	if(!table->DestinationIPAddress)
		table->DestinationIPAddress = IP_COPY(ip);
	table->DestinationSequenceNumber = seqNumber;
	table->HopCount= hopCount;
	table->Lifetime = max(table->Lifetime,lifeTime);
	table->ListofPrecursors = devVar->precursorsList;
	table->NetworkInterface = 1;
	if(!table->NextHop)
		table->NextHop = IP_COPY(nextHop);
	table->routingFlags = AODV_RoutingFlag_Valid;
	if(table->DestinationSequenceNumber)
		table->ValidDestinationSequenceNumberflag = true;
	return 1;
}
/**
This function adds the given IP in the ADOV precursor list if not already present.
*/
int fn_NetSim_AODV_InsertInPrecursorsList(NETSIM_IPAddress ip,NetSim_EVENTDETAILS* pstruEventDetails)
{
	int loop;
	AODV_PRECURSORS_LIST* precursorsList = AODV_DEV_VAR(pstruEventDetails->nDeviceId)->precursorsList;
	if(!precursorsList)
	{
		precursorsList = calloc(1,sizeof* precursorsList);
		AODV_DEV_VAR(pstruEventDetails->nDeviceId)->precursorsList = precursorsList;
	}
	for(loop=0;loop<precursorsList->count;loop++)
	{
		if(!IP_COMPARE(precursorsList->list[loop],ip))
			return 0; //ip already present in list
	}
	if(!precursorsList->count)
		precursorsList->list = calloc(1,sizeof* precursorsList->list);
	else
		precursorsList->list = realloc(precursorsList->list,(precursorsList->count+1)*(sizeof* precursorsList->list));
	precursorsList->list[precursorsList->count] = IP_COPY(ip);
	precursorsList->count++;
	return 1;
}
/**
This function returns the next hop IP to the destination from the Route Table
*/
NETSIM_IPAddress fn_NetSim_AODV_FindNextHop(AODV_DEVICE_VAR* devVar,
	NETSIM_IPAddress dest,
	NetSim_EVENTDETAILS* pstruEventDetails)
{
	AODV_ROUTETABLE* table=devVar->routeTable;
	while(table)
	{
		if(!IP_COMPARE(table->DestinationIPAddress,dest))
			return table->NextHop;
		table = LIST_NEXT(table);
	}
	return NULL;
}
/**
This function returns the Route table with the mentioned destination IP
*/
AODV_ROUTETABLE* fnFindRouteTable(AODV_ROUTETABLE* table,NETSIM_IPAddress dest)
{
	while(table)
	{
		if(!IP_COMPARE(table->DestinationIPAddress,dest))
		{
			return table;
		}
		table = LIST_NEXT(table);
	}
	return NULL;
}
/**
This function updates the lifetime of the Route Table
*/
int fn_NetSim_AODV_UpdateRouteTable(NETSIM_IPAddress ip,
	double lifetime,
	NetSim_EVENTDETAILS* pstruEventDetails)
{
	AODV_ROUTETABLE* table = AODV_DEV_VAR(pstruEventDetails->nDeviceId)->routeTable;
	while(table)
	{
		if(!IP_COMPARE(table->NextHop,ip) || !IP_COMPARE(table->DestinationIPAddress,ip))
		{
			table->Lifetime = max(table->Lifetime,lifetime);
		}
		table=LIST_NEXT(table);
	}
	return 1;
}
/**
This function adds the timeout event of a Route Table which is equal to the table_LifeTime
*/
int fn_NetSim_AODV_ActiveRouteTimeout(NetSim_EVENTDETAILS* pstruEventDetails)
{
	int flag = 0;
	NETSIM_IPAddress dest = (NETSIM_IPAddress)pstruEventDetails->szOtherDetails;
	AODV_ROUTETABLE* table = AODV_DEV_VAR(pstruEventDetails->nDeviceId)->routeTable;
	while(table)
	{
		if(!IP_COMPARE(table->DestinationIPAddress,dest))
		{
			if(table->Lifetime <= pstruEventDetails->dEventTime)
			{
				AODV_ROUTETABLE* temp = LIST_NEXT(table);
				IP_FREE(table->DestinationIPAddress);
				IP_FREE(table->NextHop);
				LIST_FREE(&AODV_DEV_VAR(pstruEventDetails->nDeviceId)->routeTable,table);
				table = temp;
				continue;
			}
			else
			{
				//Add new time out event
				pstruEventDetails->dEventTime = table->Lifetime;
				fnpAddEvent(pstruEventDetails);
				flag = 1;
			}
		}
		table=(AODV_ROUTETABLE*)LIST_NEXT(table);
	}
	if(!flag)
		IP_FREE(dest);
	return 1;
}
