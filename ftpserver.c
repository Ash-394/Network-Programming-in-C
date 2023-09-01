#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>

#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define MAX 100
FILE *f1;

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Usage: ./server port\n");
        exit(1);
    }

    //create and initialize socket
    int server_sd = socket(AF_INET,SOCK_STREAM,0);
    if(server_sd<0)
    {
        perror("socket creation failed");
        exit(1);
    }
    int client_sd;

    struct sockaddr_in server,client;
    bzero(&server,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1]));
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind
    int status = bind(server_sd,(struct sockaddr*)&server,sizeof(server));
    if(status<0)
    {
        perror("bind failed");
        exit(1);
    }
    printf("Socket created and bind succesfull!\n");
    
    printf("\nListening...");
    listen(server_sd,5);

    int client_len;
    client_sd = accept(server_sd,(struct sockaddr*)&client,&client_len);

    char str[MAX];
    recv(client_sd,&str,MAX,0);
    printf("\nClient message\n File Name : %s\n", str);

    f1 = fopen(str,"r");
    char line[MAX];
    if(f1==NULL)
    {
        sprintf(line,"File not found! Server PID:%d\n",getpid());
        send(client_sd,line,sizeof(line),0);

    }
    else{

        sprintf(line,"Server PID:%d",getpid());
        send(client_sd,line,sizeof(line),0);
        while((fgets(line,MAX,f1))!=0)
        {
                    
            send(client_sd,line,sizeof(line),0);
        }
    }
    close(server_sd);
    close(client_sd);
}