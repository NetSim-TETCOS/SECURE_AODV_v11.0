#pragma once
/************************************************************************************
* Copyright (C) 2018                                                               *
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

#ifndef _NETSIM_MEDIUM_H_
#define _NETSIM_MEDIUM_H_
#ifdef  __cplusplus
extern "C" {
#endif

#ifndef _NETSIM_MDEIUM_CODE_
#pragma comment(lib,"Medium.lib")
#endif

	_declspec(dllexport) void medium_add_device(NETSIM_ID d,
												NETSIM_ID ifid,
												double dFrequency,
												double dBandwidth,
												double dRxSensitivity,
												double dEdThreshold,
												void(*medium_change_callback)(NETSIM_ID, NETSIM_ID, bool),
												bool(*isRadioIdle)(NETSIM_ID, NETSIM_ID),
												bool(*isTransmitterBusy)(NETSIM_ID, NETSIM_ID),
												double(*getRXPower_mw)(NETSIM_ID, NETSIM_ID, NETSIM_ID, NETSIM_ID, double));
	
	_declspec(dllexport) void medium_update_device(NETSIM_ID d,
												   NETSIM_ID ifid,
												   double dFrequency,
												   double dBandwidth,
												   double dRxSensitivity,
												   double dEdThreshold);
	
	_declspec(dllexport) void medium_notify_packet_send(NetSim_PACKET* packet,
														NETSIM_ID txId,
														NETSIM_ID txIf,
														NETSIM_ID rxId,
														NETSIM_ID rxIf);
	
	_declspec(dllexport) void medium_notify_packet_received(NetSim_PACKET* packet);

	_declspec(dllexport) bool medium_isIdle(NETSIM_ID d,
											NETSIM_ID in);

#ifdef  __cplusplus
}
#endif
#endif //_NETSIM_MEDIUM_H_