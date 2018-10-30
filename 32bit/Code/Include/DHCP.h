/************************************************************************************
 * Copyright (C) 2014                                                               *
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
#include "IP_Addressing.h"
#ifndef _NETSIM_DHCP_H_
#define _NETSIM_DHCP_H_
#ifdef  __cplusplus
extern "C" {
#endif

	typedef struct stru_IP_DHCP
	{
		NETSIM_IPAddress ip;
		NETSIM_IPAddress subnet;
		NETSIM_IPAddress gateway;
		NETSIM_IPAddress* iplist;
		unsigned int count;
	}IP_DHCP;
	
	_declspec(dllexport) IP_DHCP* DHCP_Init(NETSIM_IPAddress ip, NETSIM_IPAddress subnet,NETSIM_IPAddress gateway,struct stru_NetSim_Network* NETWORK);
	_declspec(dllexport) NETSIM_IPAddress DHCP_GET_NEW_IP(IP_DHCP* dhcp);
	_declspec(dllexport) unsigned int DHCP_DELETE_IP(IP_DHCP* dhcp,NETSIM_IPAddress ip);
#ifdef  __cplusplus
}
#endif
#endif