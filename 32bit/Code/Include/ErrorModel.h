/************************************************************************************
* Copyright (C) 2015
* TETCOS, Bangalore. India															*

* Tetcos owns the intellectual property rights in the Product and its content.     *
* The copying, redistribution, reselling or publication of any or all of the       *
* Product or its content without express prior written consent of Tetcos is        *
* prohibited. Ownership and / or any other right relating to the software and all  *
* intellectual property rights therein shall remain at all times with Tetcos.      *
* Author:	Shashi Kant Suman														*
* ---------------------------------------------------------------------------------*/
#ifndef _NETSIM_ERROR_MODEL_H_
#define _NETSIM_ERROR_MODEL_H_

#include "Wireless.h"

typedef struct stru_ber
{
	double dSNR;
	double dBER;
}BER;

_declspec(dllexport) double calculate_ber(double snr,BER ber_table[],size_t table_len);
_declspec(dllexport) double calculate_snr(double dReceivedPower/*in dBm*/,double bandwidth/*In MHz*/);
_declspec(dllexport) double calculate_BER(PHY_MODULATION modulation,
										  double dReceivedPower,/*In dbm*/
										  double dBandwidth /*In MHz*/);
_declspec(dllexport) double propagation_calculateRXSensitivity(double PEP,
															   double refPacketSize /* In bytes*/,
															   PHY_MODULATION modulation,
															   double bandwidth);
_declspec(dllexport) double add_power_in_dbm(double p1_dbm, double p2_dbm);
_declspec(dllexport) double substract_power_in_dbm(double p1_dbm, double p2_dbm);

#endif //_NETSIM_ERROR_MODEL_H_