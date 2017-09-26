#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <poll.h>

void error(const char*);
void checkArgumentsPassedCorrectly(int, char*);
void checkOpeningSocket(int);
void checkForHost(struct hostent*);


int main(int argc, char *argv[])
{
    int sockfd, portno, n, recieverCount =0;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char bufferStdin[10001], bufferSocket[10001];
    char* messageRecieved;
    struct pollfd pollInputs[2];
    {
        /* data */
    };

    checkArgumentsPassedCorrectly(argc, argv[0]); //Exit program if not correctly entered arguments
  
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    pollInputs[0].fd = sockfd;
    pollInputs[0].events = POLLIN;
    checkOpeningSocket(sockfd); //Check if port is free

    server = gethostbyname(argv[1]);
    checkForHost(server);

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    n = connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr));
    if (n< 0) 
        error("ERROR connecting");
    
    printf("Please enter the message: ");
    bzero(bufferStdin,10001);
 //    for (int i=0; i < 10000; i++){
    // bufferStdin[i] = 'a';
 //    }
    pollInputs[1].fd = fileno(stdin);
    pollInputs[1].events = POLLIN;

    // fgets(bufferStdin,10000,stdin);
    //     printf("waiting for input/output");
    printf("Waiting for input/socket");

    while(1){
        if (pollInputs[1].revents & POLLIN){ // Check if data in stdin? 
            if(write(sockfd,bufferStdin,strlen(bufferStdin))<0){
                    error("ERROR writing to socket ");
                    printf("%s", bufferStdin);
                    memset(bufferStdin, '\0', 10001);
            }
        } 
        if(pollInputs[0].revents & POLLIN){
            if(read(sockfd,bufferSocket,10000)<0){
                error("ERROR reading from socket");
            }else{
                n =0;
                while(bufferSocket[n] != '\0'){
                    messageRecieved[recieverCount++]= bufferSocket[n];
                    n++;
                }
                    memset(bufferSocket, '\0', 10001);
            }   
        }
    }
    messageRecieved[recieverCount] = '\n';  
    printf("%s\n", messageRecieved);

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

void checkArgumentsPassedCorrectly(int argc, char* command){
     if (argc != 3) {  
       fprintf(stderr,"usage %s hostname port\n", command);
       exit(0);
    }
    return;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void checkOpeningSocket(int sockfd){
     if (sockfd < 0) 
        error("ERROR opening socket");
    return;
}

void checkForHost(struct hostent *server){
    if (server == NULL)
        error("ERROR, no such host");
    return;
}
