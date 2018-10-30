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
This function generates a route error.
*/
_declspec(dllexport) int fn_NetSim_AODV_PacketDropNotification(NetSim_PACKET* packet)
{
	set_aodv_curr();
	AODV_DEVICE_VAR* var = AODV_DEV_VAR(pstruEventDetails->nDeviceId);
	if(var->nRerrCount>10)
	{
		AODV_GENERATE_RERR(pstruEventDetails->nDeviceId,
			packet->pstruNetworkData->szNextHopIp);
		var->nRerrCount=1;
	}
	var->nRerrCount++;
	return 1;
}
