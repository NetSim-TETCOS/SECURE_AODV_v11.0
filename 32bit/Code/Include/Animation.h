/************************************************************************************
* Copyright (C) 2017                                                               *
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
#pragma once
#ifndef _NETSIM_ANIMATION_H_
#define _NETSIM_ANIMATION_H_
#ifdef  __cplusplus
extern "C" {
#endif
#ifndef _NETSIM_ANIMATION_CODE_
#pragma comment (lib,"Animation.lib")
#endif

#ifndef NETSIM_ID
	typedef unsigned int NETSIM_ID;
#endif

	typedef void* ANIM_HANDLE;

	typedef enum enum_anim_flag
	{
		ANIMFLAG_DISABLE,
		ANIMFLAG_OFFLINE,
		ANIMFLAG_ONLINE,
	}ANIM_FLAG;

	typedef enum enum_anim_fileType
	{
		ANIMFILETYPE_GENERIC,
		ANIMFILETYPE_TABLE,
		ANIMFILETYPE_2DPLOT,
		ANIMFILETYPE_3DPLOT,
		ANIMFILETYPE_GRAPH,
		ANIMFILETYPE_BATTERY,
	}ANIMFILETYPE;
	static char animFileType[][50] = { "generic","table","2dplot","3dplot","graph","battery" };

	typedef enum enum_anim_type
	{
		ANIM_EXIT,
		ANIM_MOVENODE,
		ANIM_NODEJOIN,
		ANIM_NODELEAVE,
		ANIM_TRANSMITPACKET,
		ANIM_LABEL,
		ANIM_LINE,
		ANIM_CIRCLE,
		ANIM_REACTANGLE,
		ANIM_GRAPH,
		ANIM_PLOT2D,
		ANIM_PLOT3D,
		ANIM_TABLE,
		ANIM_BATTERY,
		ANIM_CUSTOM,
	}ANIM_TYPE;
	static char animType[][50] = { "EXIT",
		"MOVENODE",
		"NODEJOIN",
		"NODELEAVE",
		"TRANSMITPACKET",
		"LABEL",
		"LINE",
		"CIRCLE",
		"REACTANGLE",
		"GRAPH",
		"PLOT2D",
		"PLOT3D",
		"TABLE",
		"BATTERY",
		"" };

	typedef struct stru_move_node
	{
		NETSIM_ID d;
		double time;
		double x;
		double y;
		double z;
	}MOVENODE,*ptrMOVENODE;

	typedef struct stru_transmit_packet
	{
		NETSIM_ID tx;
		NETSIM_ID rx;
		double startTime;
		double endTime;
		char name[50];
		int type;
	}TRANSMITPACKET, *ptrTRANSMITPACKET;

	typedef struct stru_line
	{
		double time;
		double x1;
		double y1;
		double x2;
		double y2;
	}LINE, *ptrLINE;

	//Animation flag
	_declspec(dllexport) ANIM_FLAG anim_get_anim_flag();
	_declspec(dllexport) void anim_set_anim_flag(char* flag);

	//Animation menu
	/* handle: if not null, then new menu will be sub menu of handle.
	* name: name of the menu.
	* isThreadReq: if true, animation engine will create new thread.
	* isNewScreen: if true, GUI animator will open in new window
	* isFile: is true, animation engine will create default file to write
	* maxDatalen: expected max len for char written in file for each entry, default BUFSIZ
	* filetype: Type of file, Gui animator engine will call jar based on this
	*/
	_declspec(dllexport) ANIM_HANDLE anim_add_new_menu(ANIM_HANDLE handle,
													   char* name,
													   bool isThreadReq,
													   bool isNewScreen,
													   bool isFile,
													   UINT maxDataLen,
													   ANIMFILETYPE fileType);

	//Animation API
	_declspec(dllexport) void fn_NetSim_Anim_Init(char* ioPath);
	_declspec(dllexport) void fn_NetSim_Anim_Close();

	//Animation Table
	_declspec(dllexport) void animation_table_new(ANIM_HANDLE animHandle,
												  char* heading);
	_declspec(dllexport) void animation_table_time(ANIM_HANDLE animHandle,
												   double time);
	_declspec(dllexport) void animation_table_row(ANIM_HANDLE animHandle,
												  char* format, ...);
	
	//Add new animation entry
	_declspec(dllexport) void animation_add_new_entry(ANIM_HANDLE animHandle,
													  ANIM_TYPE type,
													  void* format,
													  ...);

	//Change the default file name. Animation won't write anything
	_declspec(dllexport) void anim_set_file_name(ANIM_HANDLE handle, char* name);

#ifdef  __cplusplus
}
#endif
#endif