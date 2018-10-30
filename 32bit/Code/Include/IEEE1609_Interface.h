#pragma once
/************************************************************************************
* Copyright (C) 2016                                                               *
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
#ifndef _NETSIM_IEEE1609_INTERFACE_H_
#define _NETSIM_IEEE1609_INTERFACE_H_
#ifdef  __cplusplus
extern "C" {
#endif
	typedef enum enum_IEEE1609_ChannelType
	{
		IEEE1609_ChannelType_CCH,
		IEEE1609_ChannelType_SCH,
		IEEE1609_ChannelType_GUARD,
	}IEEE1609_CHANNEL_TYPE;

	typedef struct stru_IEEE1609_Buffer
	{
		UINT size;
		UINT max_size;
		NetSim_PACKET* head;
		NetSim_PACKET* tail;
	}IEEE1609_BUFFER,*PIEEE1609_BUFFER;

	typedef struct stru_IEEE1609_Mac_Var
	{
		MAC_LAYER_PROTOCOL secondary_protocol;
		void* secondary_protocol_var;

		double dSCH_Time;
		double dCCH_Time;
		double dGuard_Time;

		struct stru_Channel_Info
		{
			IEEE1609_CHANNEL_TYPE channel_type;
			IEEE1609_CHANNEL_TYPE prevChannel;
			double dStartTime;
			double dEndTime;
		}CHANNEL_INFO;

		IEEE1609_BUFFER buffer[2]; //For CCH and SCH
	}IEEE1609_MAC_VAR, *PIEEE1609_MAC_VAR;

#define GET_IEEE1609_MAC_VAR(nDeviceId,nInterfaceId) ((PIEEE1609_MAC_VAR)DEVICE_MACVAR(nDeviceId,nInterfaceId))
#define SET_IEEE1609_MAC_VAR(nDeviceId,nInterfaceId,mac) DEVICE_MACVAR(nDeviceId,nInterfaceId) = ((void*)(mac))
#define GET_IEEE1609_CURR_MAC_VAR GET_IEEE1609_MAC_VAR(pstruEventDetails->nDeviceId,pstruEventDetails->nInterfaceId)


	typedef struct stru_IEEE1609_Phy_Var
	{
		MAC_LAYER_PROTOCOL secondary_protocol;
		void* secondary_protocol_var;

		UINT16 nStandardChannel_SCH;
		double dFrequency_SCH;
		UINT16 nStandardChannel_CCH;
		double dFrequency_CCH;
	}IEEE1609_PHY_VAR, *PIEEE1609_PHY_VAR;

#define GET_IEEE1609_PHY_VAR(nDeviceId,nInterfaceId) ((PIEEE1609_PHY_VAR)DEVICE_PHYVAR(nDeviceId,nInterfaceId))
#define SET_IEEE1609_PHY_VAR(nDeviceId,nInterfaceId,phy) DEVICE_PHYVAR(nDeviceId,nInterfaceId) = ((void*)(phy))
#define GET_IEEE1609_CURR_PHY_VAR GET_IEEE1609_PHY_VAR(pstruEventDetails->nDeviceId,pstruEventDetails->nInterfaceId)


#ifdef  __cplusplus
}
#endif
#endif //_NETSIM_IEEE1609_INTERFACE_H_