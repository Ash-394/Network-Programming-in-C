#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>

#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Usage: ./server port\n");
        exit(1);
    }
    //create sockets
    int server_sd = socket(AF_INET,SOCK_STREAM,0);
    if(server_sd<0)
    {
        perror("socket creation failed");
        exit(1);
    }
    int client_sd;
    
    //initialize sockets
    struct sockaddr_in server,client;
    bzero(&server,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1]));
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket
    int bind_status = bind(server_sd,(struct sockaddr*)&server,sizeof(server));
    if(bind_status<0)
    {
        perror("bind failed");
        exit(1);
    }
    
    listen(server_sd,5);

    // accept connection from client and store client details in struct client
    int client_len;
    client_sd = accept(server_sd,(struct sockaddr*)&client,&client_len);
    if(client_sd<0)
    {
        perror("accept failed");
        exit(1);
    }

    char buffer[50];
    recv(client_sd,buffer,sizeof(buffer),0);
    int len = strlen(buffer),i;
    for(i=0;i<=len/2;i++)
    {
        char ch = buffer[i];
        buffer[i] = buffer[len-1-i];
        buffer[len-1-i]=ch;

    }
    printf("%s\n",buffer);
    send(client_sd,buffer,sizeof(buffer),0);

    //close sds
    close(client_sd);
    close(server_sd);
}