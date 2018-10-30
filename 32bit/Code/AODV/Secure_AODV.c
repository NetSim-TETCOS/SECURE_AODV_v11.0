/************************************************************************************
* Copyright (C) 2017                                                               *
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
FILE *fpSecureAodvLog = NULL;

typedef struct stru_saodv_ctrl_packet
{
	NETSIM_ID tx;
	NETSIM_ID rx;
	AODV_CONTROL_PACKET ctrlPacketType;
	void* ctrlPacket;
	char orgData[BUFSIZ];
	long publickey[BUFSIZ];
	char DecryptedData[BUFSIZ];
	size_t len;
}SAODV_CTRL_PACKET,*PSAODV_CTRL_PACKET;

typedef struct stru_mapper
{
	void* ptr;
	AODV_CONTROL_PACKET type;
	UINT count;
	_ele* ele;
}MAPPER,*PMAPPER;
#define MAPPER_ALLOC() (struct stru_mapper*)list_alloc(sizeof(struct stru_mapper),offsetof(struct stru_mapper,ele))

PMAPPER mapper = NULL;

//Function prototype
static void saodv_encrypt_packet(PSAODV_CTRL_PACKET ctrl);
static bool saodv_decrypt_packet(PSAODV_CTRL_PACKET ctrl);
void rsa_getkey( NETSIM_ID tx, NETSIM_ID rx, int *ek, int *dk);
void rsa_encrypt(char* msg,int len,int key,long* out); 
void rsa_decrypt(long *msg, int len, int key, char *out);
static void get_saodv_ctrl_packet_type(NetSim_PACKET* packet);
void get_saodv_ctrl_packet(NetSim_PACKET* packet);

static void open_log()
{
	fpSecureAodvLog = fopen("Secure_AODV.log","w");
}

static void write_log(PSAODV_CTRL_PACKET ctrl)
{
	int i;
	if(!fpSecureAodvLog)
		open_log();

	fprintf(fpSecureAodvLog,"Packet Type = %d\n",ctrl->ctrlPacketType);
	if(!ctrl->len)
	{
		fprintf(fpSecureAodvLog,"Encryption and decryption fails. This could be a malicious node\n");
		fprintf(fpSecureAodvLog,".........................................\n");
		fflush(fpSecureAodvLog);
		return;
	}
	fprintf(fpSecureAodvLog,"Org Data = ");
	for(i=0;i<ctrl->len;i++)
	{
		fprintf(fpSecureAodvLog,"%c",ctrl->orgData[i]);
	}
	fprintf(fpSecureAodvLog,"\n");

	fprintf(fpSecureAodvLog,"Encrypted Data = ");
	for(i=0;i<ctrl->len;i++)
	{
		fprintf(fpSecureAodvLog,"%c",ctrl->publickey[i]);
	}
	fprintf(fpSecureAodvLog,"\n");

	fprintf(fpSecureAodvLog,"Decrypted Data = ");
	for(i=0;i<ctrl->len;i++)
	{
		fprintf(fpSecureAodvLog,"%c",ctrl->DecryptedData[i]);
	}
	fprintf(fpSecureAodvLog,"\n");

	fprintf(fpSecureAodvLog,".........................................\n");
	fflush(fpSecureAodvLog);
}

PMAPPER find_mapper(void* ptr)
{
	PMAPPER m = mapper;
	while (m)
	{
		if (m->ptr == ptr)
			return m;
		m = LIST_NEXT(m);
	}
	return NULL;
}

void add_to_mapper(void* ptr,AODV_CONTROL_PACKET type)
{
	PMAPPER m = find_mapper(ptr);
	if(m)
	{
		m->count++;
	}
	else
	{
		m = MAPPER_ALLOC();
		m->count = 1;
		m->ptr = ptr;
		m->type = type;
		LIST_ADD_LAST(&mapper, m);
	}
}


void remove_from_mapper(void* ptr,bool isfree)
{
	PMAPPER m = find_mapper(ptr);
	if(!m)
	{
		//Incorrect place to come
		//fnNetSimError("Why i am called if no mapper for this ptr %u\n",ptr);
		return;
	}
	m->count--;
	if (m->count == 0)
	{
		if (isfree)
		{
			NetSim_PACKET* p = fn_NetSim_Packet_CreatePacket(NETWORK_LAYER);
			p->nControlDataType = m->type;
			p->pstruNetworkData->Packet_RoutingProtocol = ptr;
			fn_NetSim_Packet_FreePacket(p);
		}
		LIST_FREE(&mapper, m);
	}
}

static void get_saodv_ctrl_packet_type(NetSim_PACKET* packet)
{
	//fnNetSimError("write this function %s", __FUNCTION__);
	switch(packet->nControlDataType)
	{
		case AODVctrlPacket_RREQ:
			packet->nControlDataType = SAODV_RREQ;
			strcpy(packet->szPacketType,"SAODV_RREQ");
			break;
		case AODVctrlPacket_RREP:
			packet->nControlDataType = SAODV_RREP;
			strcpy(packet->szPacketType,"SAODV_RREP");
			break;
		case AODVctrlPacket_RERR:
			packet->nControlDataType = SAODV_RERR;
			strcpy(packet->szPacketType,"SAODV_RERR");
			break;
		default:
			break;
	}
}

void get_saodv_ctrl_packet(NetSim_PACKET* packet)
{
	PSAODV_CTRL_PACKET ctrl = calloc(1, sizeof* ctrl);
	if(packet->nControlDataType == AODVctrlPacket_RERR)
	{
		//NOT IMPLEMENTING RERR
		free(ctrl);
		return;
	}
	ctrl->ctrlPacket = packet->pstruNetworkData->Packet_RoutingProtocol;
	ctrl->ctrlPacketType = packet->nControlDataType;
	ctrl->tx = packet->nSourceId;
	ctrl->rx = aodv_get_curr_if();

	add_to_mapper(ctrl->ctrlPacket,ctrl->ctrlPacketType);

	if(!IsMaliciousNode(ctrl->tx)) //Malicious node can't encrypt.
		saodv_encrypt_packet(ctrl);

	get_saodv_ctrl_packet_type(packet);
	packet->pstruNetworkData->Packet_RoutingProtocol = ctrl;
}

void get_aodv_ctrl_packet(NetSim_PACKET* packet)
{
	PSAODV_CTRL_PACKET ctrl = packet->pstruNetworkData->Packet_RoutingProtocol;

	if(packet->nControlDataType == AODVctrlPacket_RERR)
	{
		//NOT IMPLEMENTING RERR
		return;
	}
	assert(_msize(ctrl) == sizeof(SAODV_CTRL_PACKET));
	

	if(!saodv_decrypt_packet(ctrl))
		pstruEventDetails->pPacket = NULL; //No further processing

	remove_from_mapper(ctrl->ctrlPacket,false);

	packet->pstruNetworkData->Packet_RoutingProtocol = ctrl->ctrlPacket;
	packet->nControlDataType = ctrl->ctrlPacketType;
	packet->szPacketType[0] = 0;
	free(ctrl);
}

void saodv_copy_packet(NetSim_PACKET* dest, NetSim_PACKET* src)
{
	PSAODV_CTRL_PACKET d = calloc(1, sizeof* d);
	PSAODV_CTRL_PACKET s = src->pstruNetworkData->Packet_RoutingProtocol;
	memcpy(d, s, sizeof* d);
	add_to_mapper(d->ctrlPacket,d->ctrlPacketType);
	dest->pstruNetworkData->Packet_RoutingProtocol = d;
}

void saodv_free_packet(NetSim_PACKET* packet)
{
	PSAODV_CTRL_PACKET ctrl = packet->pstruNetworkData->Packet_RoutingProtocol;

	saodv_decrypt_packet(ctrl);

	remove_from_mapper(ctrl->ctrlPacket, true);

	packet->pstruNetworkData->Packet_RoutingProtocol = NULL;
	free(ctrl);
}

static void get_rreq_str_data(PSAODV_CTRL_PACKET ctrl)
{
	AODV_RREQ* req = ctrl->ctrlPacket;
	sprintf(ctrl->orgData,"%c,%d,%d,%s,%d,%s,%d",
		req->JRGDU[4],
		req->HopCount,
		req->RREQID,
		req->DestinationIPAddress->str_ip,
		req->DestinationSequenceNumber,
		req->OriginatorIPAddress->str_ip,
		req->OriginatorSequenceNumber);
	ctrl->len = strlen(ctrl->orgData);
}

static void get_rrep_str_data(PSAODV_CTRL_PACKET ctrl)
{
	AODV_RREP* rep = ctrl->ctrlPacket;
	sprintf(ctrl->orgData,"%c,%d,%s,%d,%s,%d",
		rep->RA,
		rep->HopCount,
		rep->DestinationIPaddress->str_ip,
		rep->DestinationSequenceNumber,
		rep->OriginatorIPaddress->str_ip,
		rep->Lifetime);
	ctrl->len = strlen(ctrl->orgData);
}

static void get_rerr_str_data(PSAODV_CTRL_PACKET ctrl)
{
	AODV_RERR* rer = ctrl->ctrlPacket;
	sprintf(ctrl->orgData,"%c,%d,%s,%d",
		rer->N,
		rer->DestCount,
		rer->UnreachableDestinationIPAddress,
		rer->UnreachableDestinationSequenceNumber);
	ctrl->len = strlen(ctrl->orgData);
}

	
static void saodv_encrypt_packet(PSAODV_CTRL_PACKET ctrl)
{
	int ek;
	int dk;
	//fnNetSimError("Implement this function %s\n",__FUNCTION__);
	switch(ctrl->ctrlPacketType)
	{
		case AODVctrlPacket_RREQ:
			get_rreq_str_data(ctrl);
			break;
		case AODVctrlPacket_RREP:
			get_rrep_str_data(ctrl);
			break;
		case AODVctrlPacket_RERR:
			get_rerr_str_data(ctrl);
			break;
		default:
			break;
	}
	rsa_getkey(ctrl->tx,
		ctrl->rx,
		&ek,&dk);
	//Call RSA to encrypt
	rsa_encrypt(ctrl->orgData,ctrl->len,ek,ctrl->publickey);
	return;
}

static bool saodv_decrypt_packet(PSAODV_CTRL_PACKET ctrl)
{
	//fnNetSimError("Implement this function %s\n", __FUNCTION__);
	int dk, ek;
	rsa_getkey(ctrl->tx,
		ctrl->rx,
		&ek,&dk);
	//Call RSA to decrypt
	rsa_decrypt(ctrl->publickey,ctrl->len,dk, ctrl->DecryptedData);

	write_log(ctrl);

	if(!memcmp(ctrl->orgData,ctrl->DecryptedData,ctrl->len) && ctrl->len != 0)
	{
		return true;
	}
	else
	{
		return false; //Decryption fail.
	}
}