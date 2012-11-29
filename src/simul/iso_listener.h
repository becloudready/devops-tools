
#ifndef ISO_LISTENER_H
#define ISO_LISTENER_H

## some changes

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "libpq-fe.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/un.h>
#include <netdb.h>
#include<assert.h>
/* for ntohl/htonl */
#include <netinet/in.h>
#include <arpa/inet.h>

#define FALSE		0
#define TRUE		1

#define	INT		22
#define	HEX		23
#define	CHAR		24
#define IN_FILE		25

#define PACKET_LIMIT	 512

#define	SEND		200
#define	RECV		201
#define	FILES_320	16
#define COMMS_DELAY 0
#define comms_delay 0
//#define comms_delay 1
#define INBETWEEN 1
//#define INBETWEEN 3
#define LAST_DELAY 0
//#define LAST_DELAY 3
#define COMMS_SLEEP(n) sleep(n)
#define END_DELAY 1
//#define END_DELAY 10
#define SEND_MAX 2500

#define OK 1
#define NOTOK 0
#define ISO_FILE  "Mti_Bits_File.ini"
#define TID_FILE  "valid_terminal_id"
#define RID_FILE  "valid_retailer_id"
#define CONFIG_FILE "host_config.ini"
#define LOGGER_SOCKET "logger_socket"
#define CHECKSUM_FILE "checksum"

void sig_chld(int);
void sig_chld_admin(int);

			/*  NDC funcs			*/

int build_terminal_command(unsigned char,unsigned char,unsigned char);
int build_customisation_data(unsigned char,unsigned char,unsigned char);

union sock
{
	struct sockaddr s;
	struct sockaddr_in i;
};

char conf_buffer[2048];
int packet_size;
unsigned char mti_bitmap_buffer[5000];
int conf_len;
int mti_bitmap_len;
int service_count; 
PGconn *conn;
unsigned char terminal_id[10];
int socfd;
int log_fd_child;
int ref_no;
#endif
