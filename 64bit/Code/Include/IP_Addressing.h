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
 * Author:		,~~.																*
				(  6 )-_,	
			(\___ )=='-'
			\ .   ) )
			\ `-' /
			~'`~'`~'`~'`~     Shashi kant suman                                                 
 *																					*
 * ---------------------------------------------------------------------------------*/
#ifndef _NETSIM_IPADDRESSING_H_
#define _NETSIM_IPADDRESSING_H_
#ifdef  __cplusplus
extern "C" {
#endif
#ifdef _WIN32
#else
	#include  "Linux.h"
// #define _declspec(dllexport) extern
#endif
#define _NETSIM_IP_LEN 50
#define _NETSIM_BIN_IP_LEN 130 //128+1(\0)+1(precaution) for IPv6
#define MAX_TTL 255
	typedef struct stru_ip* NETSIM_IPAddress;

struct stru_ip
{
	int type;
	union
	{
		struct ipv4
		{
			unsigned int byte1;
			unsigned int byte2;
			unsigned int byte3;
			unsigned int byte4;
		}IPV4;
		struct ipv6
		{
			unsigned int byte[8];
		}IPV6;
	}IP;
	unsigned int int_ip[4];

	//The following are stored to do fast calculation
	char str_ip[_NETSIM_IP_LEN];
	char bin_ip[130];
	void *nets;
	void* macAddress;
};

#define IP_COMPARE6(ip1,ip2) (ip1->int_ip[0]==ip2->int_ip[0] \
	?(ip1->int_ip[1]==ip2->int_ip[1] \
	?(ip1->int_ip[2]==ip2->int_ip[2] \
	?(ip1->int_ip[3]==ip2->int_ip[3]?0:1) :1) :1) :1)

#define IP_COMPARE4(ip1,ip2) (ip1->int_ip[0]==ip2->int_ip[0]?0:1)

#define IP_COMPARE(ip1,ip2) ((ip1)->type==(ip2)->type \
		? ((ip1)->type==4?IP_COMPARE4((ip1),(ip2)):IP_COMPARE6((ip1),(ip2))) \
		:1)

#define IP_TO_STR(ip,ipstr) ((ip)?(strcpy((ipstr),((ip)->str_ip))):strcpy(ipstr,"0.0.0.0"))


_declspec(dllexport) NETSIM_IPAddress STR_TO_IP(char* ipStr,int type);

_declspec(dllexport) NETSIM_IPAddress IP_COPY(NETSIM_IPAddress ip);
_declspec(dllexport) void IP_FREE(NETSIM_IPAddress ip);
_declspec(dllexport) NETSIM_IPAddress IP_COPY_FORCE(NETSIM_IPAddress ip);
_declspec(dllexport) void IP_FREE_FORCE(NETSIM_IPAddress ip);
_declspec(dllexport) void IP_TO_BINARY(NETSIM_IPAddress ip,char* binary);
_declspec(dllexport) unsigned int IP_ADD_TO_LIST(NETSIM_IPAddress** pipList,unsigned int* count,const NETSIM_IPAddress ip);
_declspec(dllexport) unsigned int IP_REMOVE_FROM_LIST(NETSIM_IPAddress* pipList,unsigned int* count,const NETSIM_IPAddress ip);
_declspec(dllexport) int IP_CHECK_IN_LIST(const NETSIM_IPAddress* ipList,unsigned int count,const NETSIM_IPAddress ip);
_declspec(dllexport) int IP_IS_IN_SAME_NETWORK(NETSIM_IPAddress ip1,NETSIM_IPAddress ip2,NETSIM_IPAddress subnet,unsigned int prefix_len);
_declspec(dllexport) NETSIM_IPAddress IP_NETWORK_ADDRESS(NETSIM_IPAddress ip,NETSIM_IPAddress subnet,unsigned int prefix_len);
_declspec(dllexport) NETSIM_IPAddress GET_BROADCAST_IP(int type);

_declspec(dllexport) int IP_FREE_ALL(); //Free all IP. Used only by NetworkStack.dll after simulation
//Validate all configured ip. return 1 if all ip is valid else return 0
/** Very costly function. Use only for debugging **/
_declspec(dllexport) int IP_VALIDATE_ALL_IP();

_declspec(dllexport) int STR_GET_IP_TYPE(char* ip);
#define STR_TO_IP4(ipstr) STR_TO_IP(ipstr,4)
#define STR_TO_IP6(ipstr) STR_TO_IP(ipstr,6)

#define IP_IS_IN_SAME_NETWORK_IPV4(ip1,ip2,subnet)	IP_IS_IN_SAME_NETWORK(ip1,ip2,subnet,0)
#define IP_IS_IN_SAME_NETWORK_IPV6(ip1,ip2,prefix)	IP_IS_IN_SAME_NETWORK(ip1,ip2,NULL,prefix)

#define IP_NETWORK_ADDRESS_IPV4(ip,subnet)	IP_NETWORK_ADDRESS(ip,subnet,0)
#define IP_NETWORK_ADDRESS_IPV6(ip,prefix)	IP_NETWORK_ADDRESS(ip,NULL,prefix)

_declspec(dllexport) bool isBroadcastIP(NETSIM_IPAddress ip);
_declspec(dllexport) bool isMulticastIP(NETSIM_IPAddress ip);
_declspec(dllexport) bool isValidIPAddress(char* ip);
_declspec(dllexport) int IP_find_match_length(NETSIM_IPAddress ip1, NETSIM_IPAddress ip2);

#ifdef  __cplusplus
}
#endif
#endif
