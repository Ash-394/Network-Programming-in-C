#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX_BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s server_ip server_port\n", argv[0]);
        exit(1);
    }

    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUFFER_SIZE];

    // Create socket
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(1);
    }

    while (1) {
        printf("Enter a command (TIME or DNS): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

        // Send command to server
        sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

        if (strcmp(buffer, "TIME") == 0) {
            // Receive and display time from server
            memset(buffer, 0, sizeof(buffer));
            recvfrom(client_socket, buffer, sizeof(buffer), 0, NULL, NULL);
            printf("Time from server: %s\n", buffer);
        } else if (strcmp(buffer, "DNS") == 0) {
            printf("Enter hostname to look up: ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

            // Send DNS query to server
            sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

            // Receive and display DNS lookup result from server
            memset(buffer, 0, sizeof(buffer));
            recvfrom(client_socket, buffer, sizeof(buffer), 0, NULL, NULL);
            printf("DNS lookup result: %s\n", buffer);
        } else {
            printf("Invalid command\n");
        }
    }

    close(client_socket);
    return 0;
}
