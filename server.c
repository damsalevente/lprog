
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "player.h"
int get_command(char* line){
    if(strcmp(line,"START")==0){
        return 1;
    }
    if(strcmp(line,"BUY")==0){
        return 2;
    }
    if(strcmp(line,"GET")==0){
        return 3;
    }
}
#define port 4444

int main(){
    initStateMachine();
    Game game;
    createGame(&game);
    Player players[256];
    int pptr=0;
    int socketfd,ret;
    struct sockaddr_in serverAddr;

    int newSocket;
    struct sockaddr_in newAddr;

    socklen_t addr_size;

    char buffer[512];
    pid_t childpid;

    socketfd  = socket(AF_INET,SOCK_STREAM,0);
     if(socketfd<0){
        printf("Error in connection\n");
        exit(1);
    }
    printf("Server socket created\n");

    memset(&serverAddr,'\0',sizeof(serverAddr));
    serverAddr.sin_addr.s_addr= inet_addr("127.0.0.1");
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    ret = bind(socketfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
    if(ret<0){
        printf("Error in binding \n");
        exit(1);
    }
    printf("Bind to port %d\n",port);
    if(listen(socketfd,5) == 0){
        printf("Listening...\n");
    }
    else{
        printf("Error in binding\n");
        exit(1);
    }

    while(1){
        newSocket = accept(socketfd,(struct sockaddr*)&newAddr,&addr_size);
        if(newSocket <0){
            exit(1);
        }
        printf("Connection accepted from %s:%d\n",inet_ntoa(newAddr.sin_addr),ntohs(newAddr.sin_port));
        if(childpid=fork() == 0){
            close(socketfd);
            while(1){
                recv(newSocket,buffer,512,0);
                //Control logic goes here
                int cmd = get_command(buffer);
                if(cmd == 1){
                    Player p;
                    p.numMachines=0;
                    p.money=5000;
                    p.resource=0;
                    strncpy(p.name,"Karcsi",strlen("Karcsi"));
                    players[pptr]=p;
                    pptr++;
                    memcpy(buffer,"GAME STARTED\n",sizeof("GAME STARTED\n"));
                    send(newSocket,buffer,strlen(buffer),0);
                }
                if(cmd == 2){
                    for(int i=0;i<pptr;i++){
                        if(strcmp(players[i].name,"Karcsi")==0){
                            addMachine(&players[i],0);
                            memcpy(buffer,"MACHINE ADDED SUCCESFULLY\n",sizeof("MACHINE ADDED SUCCESFULLY\n"));
                            send(newSocket,buffer,strlen(buffer),0);
                        }
                    }
                }
                if(cmd == 3){
                    for(int i=0;i<pptr;i++){
                        if(strcmp(players[i].name,"Karcsi")==0){
                            memcpy(buffer,"GET INFORMATION\n",sizeof("GET INFORMATION\n"));
                            send(newSocket,buffer,strlen(buffer),0);
                        }
                    }
                }
                if(strcmp(buffer,":exit")==0){
                    printf("Disconnected from %s:%d\n",inet_ntoa(newAddr.sin_addr),ntohs(newAddr.sin_port));
                    break;
                }
                else{
                    sleep(5);
                    printf("Client:%s \n",buffer);
                    send(newSocket,buffer,strlen(buffer),0);
                    bzero(buffer,sizeof(buffer));
                }
            }
        }

    }
    close(newSocket);
    return 0;
}

