

/****************************************************
         This file contains code for generation of Malicious Node(SinkHole) for networks running AODV in Layer3.
		 This works only for UDP and not for TCP.
		 
		 
		 The function bool IsMaliciousNode(NETSIM_ID devId) 
		 return true when the deviceID is the malicious node which is mentioned in the if statement in the function definition.

		 
		 The function  fn_NetSim_AODV_MaliciousRouteAddToTable(NetSim_EVENTDETAILS*)
		 adds the target address of the AODV RREQ it receives to its route cache so as to create a false route from 
		 the Source node to target node


		 The function fn_NetSim_AODV_MaliciousProcessSourceRouteOption(NetSim_EVENTDETAILS*)
		 Process the Data Packet received by the Malicious Node. It does not call the NetworkOut Event and destroys 
		 the packet, thus giving false acknowledge replies.

		 Code Flow - 
		 If The Node is a Malicious Node, Then when a Route Request is Received, the Function adds the route from itself 
		 to the target in the route cache and sends a false route reply.
		 When a malicious node receives a data packet, it gives acknowledge reply and frees the packet.
		  
		 


*****************************************************/


	/* Malicious Node */


#include "main.h"
#include "AODV.h"
#include "List.h"

//Set up malicious node
static int malicious_node_count = 1;
static NETSIM_ID malicious_node[] = {4}; //Array of malicious node


//Function prototype
bool IsMaliciousNode(NETSIM_ID devId);
int  fn_NetSim_AODV_MaliciousRouteAddToTable(NetSim_EVENTDETAILS*);
int fn_NetSim_AODV_MaliciousProcessSourceRouteOption(NetSim_EVENTDETAILS*);


bool IsMaliciousNode(NETSIM_ID devId)
{
#ifdef MALICIOUS_ENABLE
	int i;
	for(i=0;i<malicious_node_count;i++)
		if(malicious_node[i] == devId)
			return true;
#endif
	return false;
}


int  fn_NetSim_AODV_MaliciousRouteAddToTable(NetSim_EVENTDETAILS* pstruEventDetails)
{

	AODV_RREQ* rreq = (AODV_RREQ*)pstruEventDetails->pPacket->pstruNetworkData->Packet_RoutingProtocol;
	
	AODV_INSERT_ROUTE_TABLE(rreq->DestinationIPAddress,
		rreq->DestinationSequenceNumber,
		0,
		rreq->DestinationIPAddress,
		pstruEventDetails->dEventTime+AODV_ACTIVE_ROUTE_TIMEOUT);

	return 1;

}

int fn_NetSim_AODV_MaliciousProcessSourceRouteOption(NetSim_EVENTDETAILS* pstruEventDetails)
{
	NetSim_PACKET* packet = pstruEventDetails->pPacket;
	
	
		//update the metrics
		AODV_DEV_VAR(pstruEventDetails->nDeviceId)->aodvMetrics.packetReceived++;
		
		
	fn_NetSim_Packet_FreePacket(pstruEventDetails->pPacket);
	return 0;
}
