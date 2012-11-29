
#include "iso_listener.h"
unsigned char		buffer[1048576];

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
   
	int 			sockfd,newsockfd, portno, n,retval=0;
	struct sockaddr_in 	serv_addr;
	struct hostent 		*server;
	FILE			*filp,*filp1;
	
	unsigned char		ucpFileName[32]={0},
				ucpRecvBuff[1024]={0},
				ucpSendBuff[]="ISO0260000700100623EE5A1A8E19118164293900000191006110000060217393701532512444206021111060306036011   0201000060645682206434677314293900000191006=111110154920005153188292310200035811159032035    TGI FRIDAY'S          EW DELHI        IN027111590320358100    00000000356A4DE9DA4B1FD3C75016CVISVISA-3300000019HDFB00000310000000006";
	

	sockfd=create_socket();
	bind_socket(sockfd,0);
	listen_socket(sockfd);
	for(;;)
	{
		newsockfd=accept_connection(sockfd);
		fprintf(stderr,"Got Connection\n");	
		Comms_Recv(ucpRecvBuff,newsockfd);
		Comms_Send(ucpSendBuff,newsockfd,strlen(ucpSendBuff));
		close(newsockfd);
	}
}

int 
Comms_Send(unsigned char *buffer,int sockfd,int len)
{
	unsigned char				ucpLog[2048]={0};

	printf("\n Data Sent==%d",len);
//	(void)print_array(buffer,HEX,len);
//	sprintf(ucpLog,"\n================Msg Sent===================\n%s",buffer);
//	write_log(NDC_LOG,LOG,ucpLog);
//	printf("\n");
	return send(sockfd,buffer,len,0);
}

int create_socket()
{
	int sockfd;
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	
	if(sockfd<0) {
		printf("server: can't open stream socket\n");
		exit(0);
	}
		
	return sockfd;
			
}
/*************************************************************************************************/
	
/*************************************************************************************************/
int bind_socket(int sockfd,int type)
{
	struct sockaddr_in serv_addr;
	unsigned char * ret;
	int tcp_port_no;
	int ret1;
		
	memset((char *) &serv_addr, 0, sizeof(serv_addr));

	
	if(type == 0){
//		ret=(char *)get_init_value("PORT");
		tcp_port_no=3000;
	}

	else {
//		ret=(char *)get_init_value("LOGGER_PORT");
		tcp_port_no=atoi(ret);
		
	}


	serv_addr.sin_family = AF_INET;
	//serv_addr.sin_addr.s_addr = inet_addr("192.168.127.250");
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(tcp_port_no);

	ret1=bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

		
	if(ret1<0){
		fprintf(stderr,"server: can't bind local address -- %d\n",tcp_port_no);
		kill(0,SIGKILL);
	}


}

int accept_connection(int sockfd)
{
	struct sockaddr_in cli_addr;
	struct sockaddr	peername;
	int newsockfd, clilen;
	struct linger l;
	unsigned char  		ucpPeerName[32]={0};	
	
	clilen = sizeof(cli_addr);
		
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,&clilen);

	if(newsockfd < 0) {
		fprintf(stderr,"server: accept error\n");
       		exit(0);
	}
	
#if 0
	clilen=sizeof(cli_addr);
	if(getpeername(newsockfd,(struct sockaddr* )&cli_addr,&clilen)!=0)
	{
		perror("getpeername");
	}
	
	if(strlen(cli_addr.sin_addr.s_addr )!=0)
	{
		strcpy(ucpPeerName,cli_addr.sin_addr.s_addr);
		fprintf(stderr,"\n\nPeerName is::%s\n\n",ucpPeerName);
	}
	else
	{
		fprintf(stderr,"No peer addr got");
	}
#endif  // if 0
	l.l_onoff = 1; l.l_linger = 10;
	setsockopt(newsockfd, SOL_SOCKET, SO_LINGER, (char *)&l, sizeof(struct linger));

	return newsockfd;
	
}
/*************************************************************************************************/
	
/*************************************************************************************************/
int listen_socket(int newsockfd)
{
	unsigned char * ret=(unsigned char *)calloc(1,10);
	int max_childs;
	
//	ret=(char *)get_init_value("PEAK_CHILDS");
	max_childs=10;
	listen(newsockfd, max_childs);
	
	return TRUE;
}

int Comms_Recv(unsigned char * buffer , int sockfd)
{
	int					retval=0,
						packet_len=0;
	unsigned char		ucpLog[2048]={0};
	retval=Timeout_Recv(sockfd);
	if(retval==-1)
	{
		printf("\nReturning E No data\n");
		return -1;
	}
	packet_len=1024;
	retval=recv(sockfd,buffer,packet_len,0);
	if(retval==-1  || retval ==0) 
	{
		perror("recv");
		close(sockfd);
		exit(0);
	}
//	sprintf(ucpLog,"\n================Msg Recv===================\n%s",buffer);
//	write_log(NDC_LOG,LOG,ucpLog);
//	fprintf(stderr,"No. Bytes Read are %d And The data is as Follows\n",retval);
//	(void)print_array(buffer,HEX,retval);printf("\n");
	return retval;
	
}


