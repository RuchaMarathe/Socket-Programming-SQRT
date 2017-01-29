/* CmpE 207  Lab Assignment #1
/* Topic : Client and server software design using sockets
/* Author: Group 8
/* This TCP Client passes an integer to the server. Server takes this integer, computes the square root and send the result back to client.
Client is reading the result and prints the initial value and the square root value. */

#include <stdio.h>
#define _USE_BSD 1
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#define BUFSIZE 64

#ifndef INADDR_NONE
#define INADDR_NONE 0xffffffff
#endif

extern int errno;

int errexit(const char *format,...);
int connectTCP(const char *service,const char *host);
int connectsock(const char *service,const char *host,const char *transport);

int main(int argc,char *argv[])
{
char *host;
char recvmsg[100];
char *number;
double result;
char *service="echo";

int sock,n;

switch(argc)
{
case 1:
host="localhost";
break;

case 2:
host="localhost";
break;

case 3:
number=argv[2];
host=argv[1];
break;

default:printf("Error in reading arguments\n");
exit(1);
}

result=atof(number);

/* Establishes connectivity with the server by choosing TCP protocol. Returns a positive Integer on connecting. */

sock=connectTCP(service,host);

/* On establishing the connection, the integer stored in number is sent to the server who's length is the length of number+1 */

write(sock,number,strlen(number)+1);

/* The client receives the square root result back from the server and is stored in recvmsg and has a length of 100 */

n=read(sock,recvmsg,100);

/* Printing the entered number */

if(n<0)
{
printf("reading failed\n");
}

printf("Entered number is : %s\n",number);

/* Printing the result received from server. */

if(result>=0)
{
 printf("Square root of given number : %s\n",recvmsg);
}
else if(result<0)
{
  printf("%s\n",recvmsg);
}
exit(0);

}

/* connectsock - allocate & connect a socket using TCP or UDP */

int connectTCP(const char *service,const char *host)
{
return connectsock(service,host,"tcp");
}

/*
* Arguments:
* host - name of host to which connection is desired
* service - service associated with the desired port
* transport - name of transport protocol to use ("tcp" or "udp")
*/

int connectsock(const char *service,const char *host,const char *transport)
{
struct sockaddr_in sin;
int sock,type;

memset(&sin,0,sizeof(sin));
sin.sin_family=AF_INET;

sin.sin_port=htons(8000);

inet_pton(AF_INET,host,&(sin.sin_addr));

/* Use protocol to choose a socket type */

if(strcmp(transport,"udp")==0)
type=SOCK_DGRAM;
else
type=SOCK_STREAM;

 /* Allocate a socket */

sock=socket(AF_INET,type,0);

if(sock<0)
errexit("Error in creating socket : %s\n",strerror(errno));

/* Connect the socket */

if((connect(sock,(struct sockaddr *) &sin,sizeof(sin)))<0)
errexit("Error in connection %s.%s: %s\n",host,service,strerror(errno));

return sock;
}

int errexit(const char* format,...)
{
va_list args;

va_start(args,format);
vfprintf(stderr,format,args);
va_end(args);
exit(1);
}

