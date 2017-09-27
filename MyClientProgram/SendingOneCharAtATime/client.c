#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define SEND 49
#define ACK1 50
#define SEND_AGAIN 51
#define ACK2 52
#define TERMINATE 53

#define THIS_CLIENT 0 // Change number for different instances please!

char packet[10];

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
void transmitData(int sockfd, char *buffer){
    int n;
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0)
        error("ERROR writing to socket");
        
}
void buildPacket(int clientId, int seqNum, int comSignal, char msg){
        // char packet[10];
        bzero(packet, 10);
        int i=0;
        packet[i++] = clientId;
        packet[i++] = ':';
        while(seqNum%10){
            int n = seqNum % 10;
            packet[i++] = 48+n;
            seqNum = seqNum/10;
        }
        packet[i++] = ':';
        packet[i++] = comSignal +48;
        packet[i++] = ':';
        packet[i++] = msg;
        packet[i++] = '\n';
}  

struct MessageDetails
{
    int seqNo;
    char correctChar;
    char recievedMessage1[10];
    char recievedMessage2[10];
    int commSignal;

} messageArray[1000];
char bufferRecv[10];

char actualMessageDecoded[1000];


// MessageDetails messageArray[1000];

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[10001];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
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
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    printf("Please enter the message: ");
    bzero(buffer,10001);
    for (int i=0; i < 10000; i++){
	buffer[i] = 'a';
    }
    fgets(buffer,10000,stdin);
    // Sliding window apply garnu parcha..
    // window size 5 jati..
    // dont send 6th character unless 1st is double acked
    // keep resending after some milisecond.
    // use struct? as a class? garo huncha hola ra?

    int sendingCounter =0;
    int flag = 1;
    int i = 0;
    while(flag){
        // for(int i=0+sendingCounter; i<5+sendingCounter; i++){
            // bzero(toSendBuffer, 10);
            if(buffer[i] == '\n')
                flag = -1;
            else{
                buildPacket(THIS_CLIENT, i, SEND, buffer[i]);
                transmitData(sockfd, packet);    
            }
            
        // }

        // while(read(sockfd,bufferRecv,10000)){
        //     if(bufferRecv[0] == THIS_CLIENT & bufferRecv[2]>48){
        //         if(bufferRecv[4] == ACK1){
        //             bzero(toSendBuffer, 10);
        //                 toSendBuffer[0] = 48+THIS_CLIENT;
        //                 toSendBuffer[1] = ":";
        //                 toSendBuffer[2] = 48+bufferRecv[];
        //                 toSendBuffer[3] = ':';
        //                 toSendBuffer[4] = SEND;
        //                 toSendBuffer[5] = ':';
        //                 toSendBuffer[6] = buffer[i];
        //                 toSendBuffer[7] = '\n';
        //         }
        //     }
        // }
            
             
            
        // }
    }
    while(1){
        bzero(buffer,10001);
        n = read(sockfd,buffer,10000);
        printf("%s",buffer);    
    }
    
    close(sockfd);
    return 0;
}


                