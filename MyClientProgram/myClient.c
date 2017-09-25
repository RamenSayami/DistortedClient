#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void checkArgumentsPassedCorrectly(int argc, char* command){
     if (argc < 3) {  
       fprintf(stderr,"usage %s hostname port\n", command);
       exit(0);
    }
    return;
}

void checkOpeningSocket(int sockfd){
     if (sockfd < 0) 
        error("ERROR opening socket");
    return;
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char bufferStdin[10001];
    char bufferSocket[10001];
    char* messageRecieved;
    int recieverCount =0;

    checkArgumentsPassedCorrectly(argc, argv[0]); //Exit program if not correctly entered arguments
  
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    checkOpeningSocket(sockfd); //Check if port is free
   

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
    bzero(bufferStdin,10001);
 //    for (int i=0; i < 10000; i++){
    // bufferStdin[i] = 'a';
 //    }
    fgets(bufferStdin,10000,stdin);
        printf("waiting for input/output");

    while(1){
        printf("waiting for input/output");
        if (!feof(stdin)){ // Check if data in stdin? 
            if(write(sockfd,bufferStdin,strlen(bufferStdin))<0){
                         error("ERROR writing to socket ");
                         printf("%s", bufferStdin);
            }
        } 

        if(read(sockfd,bufferSocket,10000)<0){
            error("ERROR reading from socket");
        }else{
            for(int i=0 ; i< sizeof(bufferSocket) ; i++){
                messageRecieved[recieverCount++]= bufferSocket[i];
            }
            printf("%s\n", messageRecieved);
        }
    }
    messageRecieved[recieverCount] = '\n';  

    // n = write(sockfd,bufferStdin,strlen(bufferStdin));
    // if (n < 0) 
    //      error("ERROR writing to socket");
    // bzero(buffer,10001);
    // n = read(sockfd,buffer,10000);
    // if (n < 0) 
    //      error("ERROR reading from socket");
    printf("Message recieved: %s\n",messageRecieved);
    close(sockfd);
    return 0;
}
