#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_MESSAGE_LEN 1024

void *listen_for_updates(void *arg) {
    int client_sd = *((int *)arg);
    char buffer[MAX_MESSAGE_LEN];
    while (1) {
        bzero(&buffer, sizeof(buffer));
        ssize_t bytes_received = recv(client_sd, buffer, sizeof(buffer), 0);
        if (bytes_received == 0) {
            printf("Server disconnected\n");
            break;
        } else if (bytes_received < 0) {
            perror("recv error");
            break;
        } else {
            buffer[bytes_received] = '\0'; // Null-terminate the received message
            printf("\nServer: %s\n", buffer); // Print the received message
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "usage:./client IPofserver port\n");
        exit(1);
    }
    // create a socket
    int client_sd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sd < 0)
    {
        perror("socket creation failed!");
        exit(1);
    }

    // set socket address
    struct sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &server.sin_addr);

    // connect socket
    int connect_status = connect(client_sd, (struct sockaddr *)&server, sizeof(server));
    if (connect_status < 0)
    {
        perror("connection failed!");
        exit(1);
    }
    else
    {
        printf("connection sucessful!");
    }

    char *name = NULL;
    size_t nameSize= 0;
    printf("please enter your name?\n");
    ssize_t  nameCount = getline(&name,&nameSize,stdin);
    name[nameCount-1]=0;

    // Create a thread to listen for updates
    pthread_t update_thread;
    pthread_create(&update_thread, NULL, listen_for_updates, &client_sd);

    // input from user pass to server read output from server then output to user
    char message[MAX_MESSAGE_LEN];


    char *line = NULL,response[MAX_MESSAGE_LEN];
    size_t lineSize= 0;
    printf("type and we will send(type :exit)...\n");

        recv(client_sd,response,sizeof(response),0);
        printf("Server:%s",response);

    while (1)
    {

        printf("You: ");
        ssize_t  charCount = getline(&line,&lineSize,stdin);
        line[charCount-1]=0;
        if (strcmp(line, ":exit\n") == 0)
        {
            break;
        }

        sprintf(message,"%s:%s",name,line);

        send(client_sd, message, sizeof(message), 0);

        bzero(response,sizeof(response));
        recv(client_sd,response,sizeof(response),0);
        printf("Server:%s\n",response);

        if (strcmp(line, ":exit\n") == 0)
        {
            break;
        }

    }
    pthread_cancel(update_thread); // Cancel the update listening thread
    close(client_sd);
    return 0;
}