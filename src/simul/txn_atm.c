
#include "iso_listener.h"
#include <openssl/des.h>
unsigned char		buffer[1048576];
unsigned char		ucpBootUpMsg[]="12000B0000";
unsigned char		ucpReadyMsg[]="22000123456789";
unsigned char		ucpNDCScreenHeader[]={'3','\x1c','0','0','0','\x1c','\x1c','1','1','\x1c'};
unsigned char		ucpNDCStateTblHeader[]={'3','\x1c','0','0','0','\x1c','\x1c','1','2','\x1c'};
unsigned char		ucpNDCEnhancedConfig[]={'3','\x1c','0','0','0','\x1c','\x1c','1','A','\x1c'};
unsigned char		ucpNDCFITData[]={'3','\x1c','0','0','0','\x1c','\x1c','1','5','\x1c'};
unsigned char		ucpNDCDateTime[]={'3','\x1c','0','0','0','\x1c','\x1c','1','C','\x1c'};
unsigned char		ucpNDCConfigIdLoad[]={'3','\x1c','0','0','0','\x1c','\x1c','1','6','\x1c'};
unsigned char		ucpNDCCassetteMapping[]={'3','\x1c','0','0','0','\x1c','\x1c','1','E','\x1c'};

unsigned char		ucpTransactionHeader[]={'1','1','\x1c','0','0','6','\x1c','\x1c','\x1c','1','5','\0'};

signed char Timeout_Recv(int mysocfd)
{
	fd_set rfds;
	struct timeval tv;
	unsigned int timeout=0,retval=0;
	PGresult *res;
	unsigned char log_string[100];
	
	timeout=1000;
	
	FD_ZERO(&rfds);
	FD_SET(mysocfd, &rfds);
 
    tv.tv_sec = timeout;
	tv.tv_usec = 0;

    retval = select(mysocfd+1, &rfds, NULL, NULL, &tv);
    
	if (retval)
	{
		fprintf(stderr,"Data is available now.\n\n");
		return 0;
	}
	else {
      		fprintf(stderr,"Connection Created but no data in  %d seconds.\n",timeout);
      		sprintf(log_string,"Connection Created but no data in  %d seconds disconnecting\n",timeout);
      		close(mysocfd);
		return -1;
	}	
}


void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
   
	int 			sockfd, portno, n,retval=0;
	struct sockaddr_in 	serv_addr;
	struct hostent 		*server;
	FILE			*filp,*filp1;
	
	unsigned char		ucpFileName[32]={0},ucpSendBuffer[256]={0};


	unsigned char           block[20]={0},uclsbmask=0xF0,ucmsbmask=0x0F,uctemp=0;
        unsigned char           printkey[16]={0},ucFlag=0;
        unsigned char           count=0,count2=0;
        unsigned char           output[16]={0};
        unsigned char           ival[8]={0},key3[8]={0};
        DES_key_schedule        schedule,schedule2;
        DES_cblock              ivec,key;







	
	
	if (argc < 4)
       	{
		fprintf(stderr,"usage %s hostname port track2 amount(8 bytes)\n", argv[0]);
		exit(0);
	}
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
        error("ERROR opening socket");
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
   	
   	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0) 
	error("ERROR connecting");
    
	strcat(ucpSendBuffer,ucpTransactionHeader);
	strcat(ucpSendBuffer,argv[3]);		
	strncat(ucpSendBuffer,"\x1c\x1c\x1c",3);
	strcat(ucpSendBuffer,argv[4]);
	strncat(ucpSendBuffer,"\x1c",1);
	filp=fopen("keyfile","r");
	if(!filp)perror("fopen keyfile");
	fread(key,1,8,filp);
	fclose(filp);	
	for(count=0;count < 8;count++,count2++)
	{
		uctemp=key[count] & uclsbmask;
		uctemp>>=4;
		printkey[count2++]=uctemp+0x30;
		uctemp=key[count] & ucmsbmask;
		printkey[count2]=uctemp+0x30;
	}
	strncat(ucpSendBuffer,printkey,8);
	strncat(ucpSendBuffer,"\x1c\x1c\x1c\x1c",4);

	retval=send(sockfd,ucpSendBuffer,strlen(ucpSendBuffer),0);
	if(retval ==-1) perror("Send:");
	fprintf(stderr,"\nMsg Sent :%s\n",ucpBootUpMsg);	
	
	for(n=0;;)
	{
		filp=fopen("logfile.log","a");
		if(!filp)
		{
			perror("fopen:");
			error("Error FIle");
		}
		(void)bzero(buffer,sizeof(buffer));	
		Timeout_Recv(sockfd);
		retval=recv(sockfd,buffer,sizeof(buffer),0);
		if(retval ==-1) perror("recv:");
		if(memcmp(ucpNDCScreenHeader,buffer,10)==0)
		{
			printf("\nGot Screen data header==%d\n",retval);
			fprintf(filp,"\n=========================================\n");
			fprintf(filp,"\n===============Screen data header============\n");
			fwrite(buffer,1,14,filp);

			n++;
			bzero(ucpFileName,sizeof(ucpFileName));
			printf("\nGot Screen Data\n");
			sprintf(ucpFileName,"screen%d.pcx",n);
			filp1=fopen(ucpFileName,"w+");
			fwrite(buffer+13,1,retval-13,filp1);
			fclose(filp1);
		}
		else if(memcmp(ucpNDCStateTblHeader,buffer,10)==0)
		{
			printf("\nGot State table data==%d\n",retval);
			fprintf(filp,"\n=========================================\n");
			fprintf(filp,"\n===============State Table data============\n");
			fwrite(buffer,1,retval,filp);
		}
		else if (memcmp(ucpNDCEnhancedConfig,buffer,10)==0)
		{
			printf("\nGot Enhanced Config data\n");
			fprintf(filp,"\n=========================================\n");
			fprintf(filp,"\n===============Enhanced Config data============\n");
			fwrite(buffer,1,retval,filp);
		}
		else if (memcmp(ucpNDCFITData,buffer,10)==0)
		{
			printf("\nGot FIT data\n");
			fprintf(filp,"\n=========================================\n");
			fprintf(filp,"\n===============FIT Table data============\n");
			fwrite(buffer,1,retval,filp);
		}
		else if (memcmp(ucpNDCConfigIdLoad,buffer,10)==0)
		{
			printf("\nGot Config ID load data\n");
			fprintf(filp,"\n=========================================\n");
			fprintf(filp,"\n===============Config Id load============\n");
			fwrite(buffer,1,retval,filp);
			fflush(filp);
		}
		else if (memcmp(ucpNDCDateTime,buffer,strlen(ucpNDCDateTime))==0)
		{
			printf("\nGot Date Time data\n");
			fprintf(filp,"\n=========================================\n");
			fprintf(filp,"\n===============Date and TIme Data============\n");
			fwrite(buffer,1,retval,filp);
		}
		else if (memcmp(ucpNDCCassetteMapping,buffer,strlen(ucpNDCCassetteMapping))==0)
		{
			printf("\nGot Cassette mapping data\n");
			fprintf(filp,"\n=========================================\n");
			fprintf(filp,"\n===============Cassette Mapping Data============\n");
			fwrite(buffer,1,retval,filp);
		}
		else 
		{
			fprintf(filp,"\n=========================================\n");
			fprintf(filp,"\n===============Unknown data============\n");
			fwrite(buffer,1,retval,filp);		
		}
		fprintf(stderr,"\nMsg Recv :%s\n",buffer);
		retval=send(sockfd,ucpReadyMsg,sizeof(ucpReadyMsg),0);
		if(retval ==-1) perror("Send:");
		fprintf(stderr,"\nMsg Sent :%s\n",ucpReadyMsg);
		fclose(filp);
	}
	
}
