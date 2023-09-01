#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>

#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define MAX_CLIENTS 5

int activeClients[MAX_CLIENTS] = {0};

void handle_client(int client_sd,int client_count)
{
    char msg[200];
    sprintf(msg,"Welcome to the chat server! No. of active clients:%d\n",client_count);
    printf("%s",msg);
    send(client_sd,msg,sizeof(msg),0);

    printf("Client %d connected\n", client_count);


    while(1)
    {
        char buffer[1024];
        bzero(&buffer,sizeof(buffer));

       // Receive msg from client
        ssize_t bytes_received = recv(client_sd, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            buffer[bytes_received]='\0';
            if (strcmp(buffer, ":exit") == 0) {
                printf("Client %d disconnected\n", client_count);
            } else {
                perror("recv error");
            }
            break;
        }
        else
        {
            printf("Client: %s\n", buffer);
            for (int i = 0; i < MAX_CLIENTS; i++)
            {
                printf("handle : checking activeclient %d:%d\n",i,activeClients[i]);
                if ( activeClients[i]!=client_sd && activeClients[i] != 0)
                {
                    send(activeClients[i], buffer, strlen(buffer), 0);
                }
            }
        }


    }
    // Close client socket and remove from active clients
    close(client_sd);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (activeClients[i] == client_sd) {
                activeClients[i] = 0;
                break;
            }
    }   
    
}

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
    printf("Server listening on port %d...\n", server.sin_port);
    int client_count=0;
   // Accept and fork for each clien 
    while(1)
    {
        int client_len;
        client_sd = accept(server_sd,(struct sockaddr*)&client,&client_len);
        if(client_sd<0)
        {
            perror("accept failed");
            exit(1);
        }
        else{
            printf("Connection accepted from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
            // Find an available slot in the activeClients 
            for(int i=0;i<MAX_CLIENTS;i++)
            {
                if(activeClients[i]==0)
                {
                    activeClients[i]=client_sd;
                    printf("checking activeclient %d:%d\n",i,activeClients[i]);
                    client_count++;
                    break;
                    
                }
            }
            // Fork a new process to handle the client
            if(fork()==0)
            {
                close(server_sd);
                handle_client(client_sd,client_count);
                exit(0);
                
            }
            
            else{
                close(client_sd);
            }
        }
    }

    //close sds
    close(client_sd);
    close(server_sd);
}