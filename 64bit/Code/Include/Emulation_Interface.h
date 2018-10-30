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
 * Author:		,~~.																*
				(  6 )-_,	
			(\___ )=='-'
			\ .   ) )
			\ `-' /
			~'`~'`~'`~'`~     Shashi kant suman                                                 
 *																					*
 * ---------------------------------------------------------------------------------*/
#ifndef _EMULATION_INTERFACE_H_
#define _EMULATION_INTERFACE_H_
#ifdef  __cplusplus
extern "C" {
#endif

#ifdef _WIN32
_declspec(dllexport) int flag_emulation;				// INPUT FROM USER FOR EMULATION
#else
	int flag_emulation;	// INPUT FROM USER FOR EMULATION
#endif


	HINSTANCE hEmulationLib;

	typedef struct stru_emu_interface
	{
		int (*app_map_init)();
		int (*main_emulation)();
		int (*emulation_send_packet)(NetSim_PACKET*);
		int (*emulation_finish)();
		int pcap_logfile_status[4];
	}EMU_INTERFACE;
	EMU_INTERFACE *emu_interface;


	//function prototype
	void set_emulation_status(void);
	void fn_Load_Emulation(char * Path);
	void em_init_time(void);
#ifdef  __cplusplus
}
#endif
#endif
