/************************************************************************************
 * Copyright (C) 2015                                                               *
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



#ifndef _NETSIM_UTILITY_H_
#define _NETSIM_UTILITY_H_
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include "stdarg.h"
#include "Linux.h"
#endif
#include <time.h>
#include <stdint.h>


#ifdef  __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#ifndef _NETSIM_UTILITY_CODE_
#pragma comment(lib,"NetSim_utility.lib")
#define NETSIM_UTILITY_EXPORT _declspec(dllimport)
#else
#define NETSIM_UTILITY_EXPORT _declspec(dllexport)
#endif
#else
#ifndef _NETSIM_UTILITY_CODE_
#define NETSIM_UTILITY_EXPORT extern
#else
#define NETSIM_UTILITY_EXPORT 
#endif
#endif

	typedef struct stru_pcap_writer
	{
		FILE* pFile;
		time_t start_time;
		HANDLE hPipe;
	}PCAP_WRITER,*PPCAP_WRITER;

	typedef struct stru_wireshark_trace
	{
		PPCAP_WRITER** pcapWriterlist;
		void (*convert_sim_to_real_packet)(void* netsim_packet,void* pcap_handle,double dTime);
	}WIRESHARKTRACE,*PWIRESHARKTRACE;

	NETSIM_UTILITY_EXPORT WIRESHARKTRACE wireshark_trace;

	//Function prototype

	//Used to get path for any file
	_declspec(dllexport) void fnGetFilePath(char* name, char* path);

	//Used to get options index form command
	_declspec(dllexport) int fnParseOption(int count,char* arg[],char* opt);

	//Used to get current time in string
	_declspec(dllexport) void GetTimeStr(char* date, int len);

	//Used to print the log file with thread id and time
	_declspec(dllexport) FILE* open_log_file(char* filename,char* mode);
	_declspec(dllexport) int close_log_file(FILE* fp);
	_declspec(dllexport) int ReportLog(FILE* fp, const char* msg,...);

	/* Strip whitespace chars off end of given string, in place. Return s. */
	_declspec(dllexport) char* rstrip(char* s);

	/* Return pointer to first non-whitespace char in given string. */
	_declspec(dllexport) char* lskip(const char* s);

	/* Return pointer to first char c or ';' comment in given string, or pointer to
   null at end of string if neither found. ';' must be prefixed by a whitespace
   character to register as a comment. */
	_declspec(dllexport) char* find_char_or_comment(const char* s, char c);

	/* Version of strncpy that ensures dest (size bytes) is null-terminated. */
	_declspec(dllexport) char* strncpy0(char* dest, const char* src, size_t size);

	/* Strip the word out of string*/
	_declspec(dllexport) char* find_word(char** s);

	/* To check whether a string is start with specified string*/
	_declspec(dllexport) bool startsWith(const char *str, const char *pre);

	/* Used to init the start_time. Must be called once */
	_declspec(dllexport) void init_time();

	/* Used to get current time in microsecond */
	_declspec(dllexport) uint64_t em_current_time();

	/* Used to initialize the start time*/
	_declspec(dllexport) void em_init_time(void);

	/* Used to get start time */
	_declspec(dllexport) struct tm get_start_time();

	/* Sleep for microsec */
	_declspec(dllexport) void usleep(__int64 usec);

	/* used to get file name without extension */
	_declspec(dllexport) char* get_filename_without_extension(char* filename);

	/* Used to prepare filter string for wireshark */
	_declspec(dllexport) void create_wireshark_filter(__out char* filter,__in int len,__in char* src_ip,__in char* dst_ip);

	/* Call wireshark */
	_declspec(dllexport) void call_wireshark(int duration,char* filter_string);

	/* open pcap file for writing */
	_declspec(dllexport) void* open_pcap_file(const char *filename,char* pipename,uint32_t network);

	/* write packet to pcap file */
	_declspec(dllexport) void write_to_pcap(void* handle,unsigned char* packet,unsigned int len,double* dTime);

	/* close the pcap write */
	_declspec(dllexport) void close_pcap_file(void* pcap_handle);

	/*Bit wise Operation*/
#define SET_BIT(val, bitIndex) ((val) |= (1 << (bitIndex)))
#define SET_BIT_64(val, bitIndex) ((val) |= (1i64 << (bitIndex)))
#define CLEAR_BIT(val, bitIndex) ((val) &= ~(1 << (bitIndex)))
#define CLEAR_BIT_64(val, bitIndex) ((val) &= ~(1i64 << (bitIndex)))
#define TOGGLE_BIT(val, bitIndex) ((val) ^= (1 << (bitIndex)))
#define TOGGLE_BIT_64(val, bitIndex) ((val) ^= (1i64 << (bitIndex)))
#define BIT_IS_SET(val, bitIndex) ((val) & (1 << (bitIndex)))
#define BIT_IS_SET_64(val, bitIndex) ((val) & (1i64 << (bitIndex)))
#define LSHIFTCHAR(x,eposition) ((UINT8)(x << (8 - eposition)))
#define RSHIFTCHAR(x,eposition) ((UINT8)(x >> (8 - eposition)))
	
#define htonll(x) ((1==htonl(1)) ? (x) : ((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32))
	_declspec(dllexport) int isBitSet(unsigned int num, int index);
	_declspec(dllexport) unsigned int setBit(unsigned int num, int index);
	_declspec(dllexport) unsigned int unsetBit(unsigned int num, int index);
	_declspec(dllexport) UINT64 hex_to_decimal(char hex[]);
	_declspec(dllexport) char* hex_to_binary(char* hex, char* bin);
	_declspec(dllexport) void binary_to_hex(char* bin, char* hex);
	_declspec(dllexport) UINT64 binary_to_decimal(char bin[]);
	_declspec(dllexport) UINT8 maskChar(UINT8 sposition, UINT8 eposition);

	_declspec(dllexport) bool find_interaction(double arg1[2],
											   double arg2[2],
											   double intersection[2]);
#ifdef  __cplusplus
}
#endif
#endif //_NETSIM_UTILITY_H_
