#include "main.h"
#include "AODV.h"

//SECTION KEY GENERATOR
static int prime1 = 11;
static int prime2 = 29;
static int ek_array[100];
static int dk_array[100];
static int counter = 0;
static int t;
static int multiplier;
FILE *fp1;

static int prime(long int pr) 
{ 
	int i, j = 0; 
	j=(int)sqrt(pr); 
	for(i=2;i<=j;i++) 
	{ 
		if(pr%i==0) 
		return 0; 
	} 
	return 1; 
} 

long int cd(long int x) 
{ 
	long int k=1; 
	while(1) 
	{ 
		k=k+t; 
		if(k%x==0) 
		return(k/x); 
	} 
} 

static void generate_rsa_key()
{
	
	int k=0; 
	int i;
	int flag;
	t=(prime1-1)*(prime2-1); 
	multiplier = prime1 * prime2;

	for(i=2;i<t;i++) 
	{ 
		if(t%i==0) 
			continue; 

		flag=prime(i); 

		if(flag==1 && i!=prime1 && i!=prime2) 
		{ 
			ek_array[k]=i;

			flag=cd(ek_array[k]); 
			
			if(flag>0) 
			{ 
				dk_array[k]=flag; 
				k++; 
			} 
			if(k==99) 
				break; 
		} 
	} 
}
/************/

//RSA
typedef struct stru_rsa_database
{
	int encryption_key;
	int decryption_key;
}RSA_DATABASE,*PRSA_DATABASE;

PRSA_DATABASE** rsa_database;

void init_rsa()
{
	NETSIM_ID i,j;
	PRSA_DATABASE r;
	generate_rsa_key();
	rsa_database = (PRSA_DATABASE **) calloc(NETWORK->nDeviceCount+1,sizeof* rsa_database);
	
	for(i=0;i<=NETWORK->nDeviceCount;i++)
	{
		rsa_database[i] =(PRSA_DATABASE *) calloc(NETWORK->nDeviceCount+1,sizeof* rsa_database[i]);
		for(j=0;j<=NETWORK->nDeviceCount;j++)
		{
			rsa_database[i][j] = (PRSA_DATABASE)calloc(1,sizeof* rsa_database[i][j]);
			r=rsa_database[i][j];
			if(!IsMaliciousNode(j) && !IsMaliciousNode(i))
			{
				r->encryption_key = ek_array[counter];
				r->decryption_key = dk_array[counter];
				counter++;
			}
		}
	}
}

void rsa_getkey(NETSIM_ID tx,NETSIM_ID rx,int* ek,int* dk)
{
	PRSA_DATABASE r;
	static int flag = 0;
	if(flag == 0)
	{
		init_rsa();
		flag = 1;
	}
	r = rsa_database[tx][rx];
	*ek = r->encryption_key;
	*dk = r->decryption_key;
}

void rsa_encrypt(char* msg,int len,int key,long* out) 
{ 
	long int pt,k;
	int i=0,j;	
	while(i!=len) 
	{ 
		pt=msg[i]; //original message
		pt=pt-96; 
		k=1; 
		for(j=0;j<key;j++) 
		{ 
			k=k*pt; 
			k=k%multiplier; 
		} 
		out[i]=k; //Public key
		i++; 
	}
}

void rsa_decrypt(long *msg, int len, int key, char *out) 
{ 
	long int pt,ct,k; 
	int i=0, j; 
	while(i<len) 
	{ 
		ct=msg[i]; 
		k=1; 
		for(j=0;j<key;j++) 
		{ 
			k=k*ct; 
			k=k%multiplier; 
		} 
		pt=k+96;
		out[i]=(char)pt; //decrypted data
		i++; 
	}
	out[i] = 0;
}
