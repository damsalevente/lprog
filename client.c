
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define port 4444

int main()
{
    int clientSocket,ret;
    struct  sockaddr_in serverAddr;
    char buffer[512];

    clientSocket = socket(AF_INET,SOCK_STREAM,0);
    if(clientSocket<0){
        printf("Error in connection\n");
        exit(1);
    }
    printf("Socket created\n");

    memset(&serverAddr,'\0',sizeof(serverAddr));
    serverAddr.sin_addr.s_addr= inet_addr("127.0.0.1");
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    ret = connect(clientSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
    if(ret<0){
        printf("Error during connection\n");
        exit(1);
    }
    printf("Connected to server\n");
    while(1){
        printf("Client:\t");
        fgets(buffer,512,stdin);
        send(clientSocket,buffer,strlen(buffer),0);
        if(strcmp(buffer,":exit")==0){
            printf("Disconnected from the server\n");
            exit(1);
        }
        if(recv(clientSocket,buffer,512,0)<0){
            printf("error receiving data\n");
        }
        else{
            printf("Server:\t%s\n",buffer);
        }
    }
}