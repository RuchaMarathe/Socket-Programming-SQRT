/* CmpE 207  Lab Assignment #1
/* Topic : Client and server software design using sockets
/* Author: Group 8
/* This TCP Server accepts an integer from the Client. Server takes this integer, computes the square root and send the result back to client.
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
#include <math.h>

int connectTCP(const char *service);
int connectsock(const char *service,const char *transport);

int main(char argc,char *argv[])
{

char message[100];
char *service="echo";
char errmsg[1000]="Entered number is negative.Please enter positive number";

int acceptsock,sock;
struct sockaddr_in sin;

sock=connectTCP(service);

/* Accepts the connection from the client. */

acceptsock=accept(sock,(struct sockaddr*)NULL,NULL);

if(acceptsock<0)
{
printf("error in accepting\n");
exit(0);
}

/* Reads the data from Client and the data is stored in message which has a length of 100. */

read(acceptsock,message,100);
double number;

/* Converting the data type to double */

number =atof(message);
double result;

/* Once the socket connection is accepted, the result is calculated and is sent back to the Client using write. */

if(number<0)
{
write(acceptsock,errmsg,1000);
}
else if(number>=0)
{
result=sqrt(number);
printf("Number recieved from client:%f\n",number);
printf("Square root of the number:%f\n",result);
sprintf(message,"%f",result);
write(acceptsock,message,strlen(message)+1);
}

/* Once the result is sent back, the acceptsocket is closed. */

close(acceptsock);
return 0;
}

int connectsock(const char *service,const char *transport)
{
struct sockaddr_in server;

int serversock,type,acceptsock,number;

memset(&server,0,sizeof(server));

server.sin_addr.s_addr=htons(INADDR_ANY);
server.sin_family=AF_INET;
server.sin_port=htons(8000);

/* Use protocol to choose a socket type */

if(strcmp(transport,"udp")==0)
{
type=SOCK_DGRAM;
}
else
{
type=SOCK_STREAM;
}

/* Allocate a socket */

serversock=socket(AF_INET,type,0);

if(serversock<0)
{
printf("Cant create socket\n");
exit(0);
}

number=1;

if(setsockopt(serversock,SOL_SOCKET,SO_REUSEPORT,(const char*)&number,sizeof(number))<0)
{
printf("setsockopt(SO_REUSEPORT) failed\n");
exit(0);
}

/* Bind Call : Adds details from serversock to socket descriptor */

int b;
b=bind(serversock,(struct sockaddr*)&server,sizeof(server));

if(b<0)
{
printf("Binding error\n");
exit(0);
}

/* Listen Call : Allows 10 connections at the socket to wait in queue  */

int listeningsock;
listeningsock=listen(serversock,10);
if(listeningsock<0)
{
printf("Error in listening\n");
exit(0);
}

return serversock;
}


int connectTCP(const char *service)
{
 return connectsock(service,"tcp");
}

