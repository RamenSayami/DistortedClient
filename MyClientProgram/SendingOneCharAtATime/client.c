#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define SENDING 49
#define ACK1 50
#define SEND_AGAIN 51
#define ACK2 52
#define SENDING_AGAIN 53
#define TERMINATE 54

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

char* intToString(int number){
    char string[5];
    int flag =1;
    bzero(string, 5);
    while(flag){
        int toConvert = number %10;
        char charNum[1];
        charNum[0] = 48+toConvert;
        strcat(string, charNum);
        if(number/10 == 0){
            flag = -1;
        }else{
            number = number/10;
        }
    }
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char *helpingString[100];
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
    int i =0;
    // Sliding window apply garnu parcha..
    // window size 5 jati..
    // dont send 6th character unless 1st is double acked
    // keep resending after some milisecond.
    // use struct? as a class? garo huncha hola ra?

    while(buffer[i] != '\n'){
        char toSendBuffer[10];
        bzero(toSendBuffer, 10);
        bzero(helpingString, 100);
        // strcpy(helpingString, intToString(48+i));
            strcpy(toSendBuffer, intToString(i));
            strcat(toSendBuffer, ":");
        bzero(helpingString, 100);
        // helpingString = itoa(SENDING);
            strcat(toSendBuffer, intToString(SENDING));
            strcat(toSendBuffer, ":");
        char ch[1];
        ch[0] = buffer[i];
            strcat(toSendBuffer, ch);
            // strcat(toSendBuffer, '\n');

            // toSendBuffer[1] = ':';
            // toSendBuffer[2] = SENDING;
            // toSendBuffer[3] = ':';
            // toSendBuffer[4] = buffer[i];
            // toSendBuffer[5] = '\n';
            transmitData(sockfd, toSendBuffer);
        i++;
    }
        // char toSendBuffer[10];
        // bzero(toSendBuffer, 10);
        // toSendBuffer[0] = '\n';
        //     transmitData(sockfd, toSendBuffer);

    // n = write(sockfd,buffer,strlen(buffer));
    // if (n < 0) 
    //      error("ERROR writing to socket");
    while(1){
        bzero(buffer,10001);
        n = read(sockfd,buffer,10000);
        if (n < 0) 
            error("ERROR reading from socket");
        printf("%s",buffer);    
    }
    
    close(sockfd);
    return 0;
}

