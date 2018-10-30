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
 * Author: Shashi kant suman                                                        *
 *                                                                                  *
 * ---------------------------------------------------------------------------------*/
#ifndef _PACKET_HEADER_H_
#define _PACKET_HEADER_H_
#ifdef  __cplusplus
extern "C" {
#endif

#ifndef _WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#define ERROR_INVALID_PARAMETER	EINVAL
#else

#define INT8    char
#define UINT8   unsigned char
#define INT16   short
#define UINT16  unsigned short
#define INT32   int
#define UINT32  unsigned int
#define INT64   long long int
#define UINT64  unsigned long long int
#define UINT	unsigned int

#endif



/*
 * Flags for DivertHelperCalcChecksums()
 */
#define WINDIVERT_HELPER_NO_IP_CHECKSUM                     1
#define WINDIVERT_HELPER_NO_ICMP_CHECKSUM                   2
#define WINDIVERT_HELPER_NO_ICMPV6_CHECKSUM                 4
#define WINDIVERT_HELPER_NO_TCP_CHECKSUM                    8
#define WINDIVERT_HELPER_NO_UDP_CHECKSUM                    16


#ifndef _WIN32
//****** From ipmib.h from windows ******/
// ICMP4_TYPE
//
// There are no RFC-specified defines for ICMPv4 message types, so we try to
// use the ICMP6 values from RFC 2292 modified to be prefixed with ICMP4.
//
typedef enum {
	ICMP4_ECHO_REPLY        =  0, // Echo Reply.
	ICMP4_DST_UNREACH       =  3, // Destination Unreachable.
	ICMP4_SOURCE_QUENCH     =  4, // Source Quench.
	ICMP4_REDIRECT          =  5, // Redirect.
	ICMP4_ECHO_REQUEST      =  8, // Echo Request.
	ICMP4_ROUTER_ADVERT     =  9, // Router Advertisement.
	ICMP4_ROUTER_SOLICIT    = 10, // Router Solicitation.
	ICMP4_TIME_EXCEEDED     = 11, // Time Exceeded.
	ICMP4_PARAM_PROB        = 12, // Parameter Problem.
	ICMP4_TIMESTAMP_REQUEST = 13, // Timestamp Request.
	ICMP4_TIMESTAMP_REPLY   = 14, // Timestamp Reply.
	ICMP4_MASK_REQUEST      = 17, // Address Mask Request.
	ICMP4_MASK_REPLY        = 18, // Address Mask Reply.
} ICMP4_TYPE, *PICMP4_TYPE;

#endif
/*
 * IPv4/IPv6/ICMP/ICMPv6/TCP/UDP header definitions.
 */
typedef struct
{
	UINT8  HdrLength:4;
	UINT8  Version:4;
	UINT8  TOS;
	UINT16 Length;
	UINT16 Id;
	UINT16 FragOff0;
	UINT8  TTL;
	UINT8  Protocol;
	UINT16 Checksum;
	UINT32 SrcAddr;
	UINT32 DstAddr;
} WINDIVERT_IPHDR, *PWINDIVERT_IPHDR;



typedef struct
{
	UINT8  TrafficClass0:4;
	UINT8  Version:4;
	UINT8  FlowLabel0:4;
	UINT8  TrafficClass1:4;
	UINT16 FlowLabel1;
	UINT16 Length;
	UINT8  NextHdr;
	UINT8  HopLimit;
	UINT32 SrcAddr[4];
	UINT32 DstAddr[4];
} WINDIVERT_IPV6HDR, *PWINDIVERT_IPV6HDR;

#define IPV6_HDR_SIZE	40 //Bytes

typedef struct
{
	UINT8  Type;
	UINT8  Code;
	UINT16 Checksum;
	UINT32 Body;
} WINDIVERT_ICMPHDR, *PWINDIVERT_ICMPHDR;

typedef struct
{
	UINT8  Type;
	UINT8  Code;
	UINT16 Checksum;
	UINT32 Body;
} WINDIVERT_ICMPV6HDR, *PWINDIVERT_ICMPV6HDR;

typedef struct
{
	UINT16 SrcPort;
	UINT16 DstPort;
	UINT32 SeqNum;
	UINT32 AckNum;
	UINT16 Reserved1:4;
	UINT16 HdrLength:4;
	UINT16 Fin:1;
	UINT16 Syn:1;
	UINT16 Rst:1;
	UINT16 Psh:1;
	UINT16 Ack:1;
	UINT16 Urg:1;
	UINT16 Reserved2:2;
	UINT16 Window;
	UINT16 Checksum;
	UINT16 UrgPtr;
} WINDIVERT_TCPHDR, *PWINDIVERT_TCPHDR;

typedef struct
{
	UINT16 SrcPort;
	UINT16 DstPort;
	UINT16 Length;
	UINT16 Checksum;
} WINDIVERT_UDPHDR, *PWINDIVERT_UDPHDR;


BOOL WinDivertHelperParsePacket(PVOID pPacket, UINT packetLen,
	PWINDIVERT_IPHDR *ppIpHdr, PWINDIVERT_IPV6HDR *ppIpv6Hdr,
	PWINDIVERT_ICMPHDR *ppIcmpHdr, PWINDIVERT_ICMPV6HDR *ppIcmpv6Hdr,
	PWINDIVERT_TCPHDR *ppTcpHdr, PWINDIVERT_UDPHDR *ppUdpHdr, PVOID *ppData,
	UINT *pDataLen);

UINT WinDivertHelperCalcChecksums(PVOID pPacket, UINT packetLen,
	UINT64 flags);
BOOL WinDivertHelperParseIPv4Address(const char *str, UINT32 *addr_ptr);
BOOL WinDivertHelperParseIPv6Address(const char *str, UINT32 *addr_ptr);

#ifdef  __cplusplus
}
#endif
#endif //_PACKET_HEADER_H_

