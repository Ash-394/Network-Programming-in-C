#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include<netdb.h>

#define MAX_BUFFER_SIZE 1024
#define TIME_SERVICE_COMMAND "TIME"
#define DNS_SERVICE_COMMAND "DNS"

void handle_time_service(int client_socket,struct sockaddr_in* client,int client_len) {
    printf("In TIME func\n");
    time_t current_time;
    char time_str[MAX_BUFFER_SIZE];
    current_time = time(NULL);
    sprintf(time_str, "Current time: %s", ctime(&current_time));
    sendto(client_socket, time_str, strlen(time_str), 0, (struct sockaddr*)client, client_len);
}

void handle_dns_service(int client_socket, char *query,struct sockaddr_in* client,int client_len) {
    printf("In DNS func\n");

    char result[MAX_BUFFER_SIZE];
    char ip[INET_ADDRSTRLEN];

    struct hostent *hent;
    struct in_addr **addr_list;
    int i;
    if ((hent = gethostbyname(query)) == NULL)
    {
        herror("gethostbyname error");
        exit(1);
    }
    addr_list = (struct in_addr **)hent->h_addr_list;
    for (i = 0; addr_list[i] != NULL; i++)
    {
        strcpy(ip, inet_ntoa(*addr_list[i]));
        break;
    }
    printf("%s\n",ip);

    sprintf(result,"IP address is %s",ip);
    printf("%s\n",result);
    sendto(client_socket, result, strlen(result), 0, (struct sockaddr*)client, client_len);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(1);
    }

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[MAX_BUFFER_SIZE];

    // Create socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(atoi(argv[1]));

    // Bind the socket to the server address
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(1);
    }

    printf("UDP Server listening on port %s...\n", argv[1]);

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        // Receive data from client
        recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_addr_len);
        printf("Received request from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        if (strcmp(buffer, TIME_SERVICE_COMMAND) == 0) {
            handle_time_service(server_socket,&client_addr,client_addr_len);
        } else if (strcmp(buffer, DNS_SERVICE_COMMAND) == 0) {
            // Receive DNS query from client
            recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_addr_len);
            printf("%s\n",buffer);
            handle_dns_service(server_socket, buffer,&client_addr,client_addr_len);
        } else {
            char error_message[] = "Invalid command";
            sendto(server_socket, error_message, strlen(error_message), 0, (struct sockaddr *)&client_addr, client_addr_len);
        }
    }

    close(server_socket);
    return 0;
}
