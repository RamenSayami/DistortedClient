#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <math.h>

#define SEND 49
#define ACK1 50
#define SEND_AGAIN 51
#define ACK2 52
#define TERMINATE 53

#define THIS_CLIENT 0 // Change number for different instances please!
char packet[10];
char number[4];

// char * convertNumberIntoArray(unsigned int number) {
//     unsigned int length = (int)(log10((float)number)) + 1;
//     char * arr = (char *) malloc(length * sizeof(char)), * curr = arr;
//     do {
//         *curr++ = number % 10;
//         number /= 10;
//     } while (number != 0);
//     return arr;
// }

void intToString(int num){
    bzero(number,4);
    if(num == 0){
        number[0] = '0';
        number[1] = '\n';
        return;
    }

    if(num > 0 & num <=9){
        number[0] = num+'0';
        number[1] = '\n';
        return;
    }

    int n;
    int i=0;
    do{
        n = num %10;
        number[i++] = n + '0';
        num = num /10;
    }while(num>0);
    number[i] = '\n';
    return;
}

void buildPacket(int clientId, int seqNum, int comSignal, char msg){
        // char packet[10];
        bzero(packet, 10);
        int i=0;
        packet[i++] = clientId+48;
        packet[i++] = ':';
        intToString(seqNum);
        for(int j = strlen(number)-2; j>=0 ; j--){
            packet[i++] = number[j];

        }

        packet[i++] = ':';
        packet[i++] = comSignal;
        packet[i++] = ':';
        packet[i++] = msg;
        packet[i++] = '\n';
        fprintf(stderr,"%s", packet);
}  


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
    int i =0;
    // Sliding window apply garnu parcha..
    // window size 5 jati..
    // dont send 6th character unless 1st is double acked
    // keep resending after some milisecond.
    // use struct? as a class? garo huncha hola ra?

    while(buffer[i] != '\n'){
        buildPacket(THIS_CLIENT, i, SEND, buffer[i]);
        // char toSendBuffer[10];
        // bzero(toSendBuffer, 10);
        // toSendBuffer[0] = 48+i;
        // toSendBuffer[1] = ':';
        // toSendBuffer[2] = SENDING;
        // toSendBuffer[3] = ':';
        // toSendBuffer[4] = buffer[i];
        // toSendBuffer[5] = '\n';
            transmitData(sockfd, packet);
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
