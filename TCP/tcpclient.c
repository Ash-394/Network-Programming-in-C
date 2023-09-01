#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>

#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main(int argc,char* argv[])
{
      if(argc!=3)
    {
        fprintf(stderr,"usage:./client IPofserver port\n");
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
    int connect_status = connect(client_sd,(struct sockaddr*)&server,sizeof(server));
    if(connect_status<0)
     {
         perror("connection failed!");
         exit(1);
    }
    
    //input from user pass to server read output from server then output to user
    char buffer[50];
    fgets(buffer,sizeof(buffer),stdin);
    int len = strlen(buffer);
    buffer[len-1]='\0';
    send(client_sd,buffer,sizeof(buffer),0);
    recv(client_sd,buffer,sizeof(buffer),0);

    printf("%s\n",buffer);
    close(client_sd);

}