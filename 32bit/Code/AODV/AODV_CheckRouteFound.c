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
This function checks if the route to target is present in Route Table
*/
bool fn_NetSim_AODV_CheckRouteFound(NETSIM_IPAddress ip, NetSim_EVENTDETAILS* pstruEventDetails)
{
	AODV_ROUTETABLE* table = AODV_DEV_VAR(pstruEventDetails->nDeviceId)->routeTable;
	while(table)
	{
		if(!IP_COMPARE(table->DestinationIPAddress,ip))
		{
			if(table->Lifetime > pstruEventDetails->dEventTime)
				return true;
		}
		table = (AODV_ROUTETABLE*)LIST_NEXT(table);
	}
	return false;
}