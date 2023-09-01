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

int main(int argc, char* argv[])
{
    if(argc!=3)
    {
        fprintf(stderr,"usage: ./client ip_addr port");
        exit(1);
    }
    //create a socket 
    int client_sd = socket(AF_INET,SOCK_STREAM,0);
    if(client_sd<0)
    {
         perror("socket creation failed!");
         exit(1);
    }

    //set socket address
    struct sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET,argv[1],&server.sin_addr);

    //connect socket
    int status = connect(client_sd,(struct sockaddr*)&server,sizeof(server));
    if(status<0)
     {
         perror("connection failed!");
         exit(1);
    }

    char filename[MAX],line[MAX];
    printf("\nEnter the source file name : \n");
    scanf("%s",filename);

    send(client_sd,filename,MAX,0);
    while((status = recv(client_sd,line,MAX,0))!=0)
    {
        printf("%s",line);
    }

    close(client_sd);
    return 0;
}
