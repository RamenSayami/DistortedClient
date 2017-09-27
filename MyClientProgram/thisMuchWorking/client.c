#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <poll.h>

#define MAXSIZE 128 
void error(const char *msg){
	perror(msg);
	exit(0);
}
int openTCPSocketToServer(char *hostName, int port){
	int sockfd;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	/* Create a socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
	/* Do a DNS lookup to find the server's address.  The call returns a
	struct with information about the server */
	server = gethostbyname(hostName);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	/* Set up the address structure to tell the socket what to connect to. */
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, 
	(char *)&serv_addr.sin_addr.s_addr,
	server->h_length);
	/* htons() converts the port number to network byte order */
	serv_addr.sin_port = htons(port);

	/* This actually establishes the connection with the server.  Once this
	done we can send bytes back and forth. */
	if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) 
		error("ERROR connecting");

	return sockfd;
}
void transmitData(int sockfd, char *buffer){
	int n;
	bzero(buffer,MAXSIZE);
	fgets(buffer,MAXSIZE,stdin);
	n = write(sockfd,buffer,strlen(buffer));
	if (n < 0)
		error("ERROR writing to socket");
		
}
void recieveData(int sockfd, char *buf, int len){
	int n;
	n = read(sockfd,buf,len);
	if (n < 0){
		error("ERROR reading from socket");
	}else{
		// 	printf("%s", buf);
		fprintf(stderr, "%s",buf);
	} 	
}
int main(int argc, char *argv[]){
	char *transmitBuffer;
	char recieveBuffer[MAXSIZE];
	int portno, sockfd;
	if (argc < 3) {
		fprintf(stderr,"usage %s hostname port\n", argv[0]);
		exit(0);
	}			
	portno = atoi(argv[2]);	//for getting port number that we are going to use
	sockfd = openTCPSocketToServer(argv[1], portno);

	struct pollfd ufds[2];
	ufds[0].fd = 0; //stdin
	ufds[0].events = POLLIN; 
	ufds[1].fd = sockfd;
	ufds[1].events = POLLIN;
	printf("Enter your message:");
	while(1){
		poll(ufds, 2, 100);
		if(ufds[0].revents & POLLIN){
			transmitData(sockfd, transmitBuffer);

		}else if(ufds[1].revents & POLLIN){
			recieveData(sockfd, recieveBuffer, MAXSIZE);
		}else{
			exit(0);		
		}
	}
	return 0;
}