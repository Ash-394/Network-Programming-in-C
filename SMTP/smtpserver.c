#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#define MAX 200
void receive_image(int socket, struct sockaddr_in *client) {
    FILE *fpimage;
    int size;
    int readsize;
    char receive_buffer[10239];
    char f_name[100];

    printf("\nEnter desired Image name to save: ");
    scanf("%s", f_name);

    fpimage = fopen(f_name, "w");
    if (fpimage == NULL) {
        printf("Error occured while opening Image File!\n");
        return;
    }

    socklen_t client_len = sizeof(struct sockaddr_in);
    printf("Receiving Image Size...\n");
    recvfrom(socket, (void *)&size, sizeof(int), 0, (struct sockaddr *)client, &client_len);
    printf("Received Image Size: %i\n", size);

    printf("Receiving Image as a byte array...\n");
   
   do {
        printf("\nRemaining size before decrease: %d\n", size);
        readsize = recvfrom(socket, &receive_buffer, sizeof(receive_buffer), 0, (struct sockaddr *)client, &client_len);
        if (readsize > 0)
        {
            fwrite(receive_buffer, 1, readsize, fpimage);
            size -= readsize; // Decrease the remaining image size
        }
        
        printf("\nRemaining size %d\n",size);
    } while (size!=101673);

    printf("Closing the file...\n");
    fclose(fpimage);
    printf("Image received and saved as '%s'\n", f_name);
}


int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Usage: ./server port\n");
        exit(1);
    }

    //create socket
    int server_sd = socket(AF_INET,SOCK_DGRAM,0);
    if(server_sd<0)
    {
        perror("socket creation failed");
        exit(1);
    }

    //initialize socket
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
    else{
        printf("Server socket created and bind succesful!\n Waiting for client message....\n");
    }
    int client_len;
    char msg[MAX+1];

    int status = recvfrom(server_sd,msg,sizeof(msg),0,(struct sockaddr*)&client,&client_len);
    if(status<0)
    {
        perror("receive error");
        exit(1);
    }
    msg[status]='\0';
    printf("Message:%s",msg);

    bzero(&msg,sizeof(msg));
    sprintf(msg,"220 Server_mail_server\n");
    sendto(server_sd,msg,MAX,0,(struct sockaddr*)&client,sizeof(client));

    bzero(&msg,sizeof(msg));
    status = recvfrom(server_sd,msg,MAX,0, (struct sockaddr *)&client, &client_len);
    if(status<0)
    {
        perror("receive error");
        exit(1);
    }
    msg[status] = '\0';
    printf("Client:%s\n", msg);

    char *buf_ptr = msg + 5; // Skip the command code;
    char response[MAX]; // New variable to hold the response message
    sprintf(response, "250 Hello %.90s ", buf_ptr);
    // Send the response
    sendto(server_sd, response, strlen(response), 0, (struct sockaddr*)&client, sizeof(client));
;

    bzero(&msg,sizeof(msg));
    status = recvfrom(server_sd,msg,MAX,0, (struct sockaddr *)&client, &client_len);
    if(status<0)
    {
        perror("receive error");
        exit(1);
    }
    msg[status] = '\0';
    printf("Client:%s\n", msg);

    
    buf_ptr = strchr(msg, ':') + 2; // Skip the command code;
    
    sprintf(response,  "250 Hello %.90s .......sender OK\n", buf_ptr);

    // Send the response
    sendto(server_sd, response, strlen(response), 0, (struct sockaddr*)&client, sizeof(client));

    

    bzero(&msg,sizeof(msg));
    status = recvfrom(server_sd,msg,MAX,0, (struct sockaddr *)&client, &client_len);
    if(status<0)
    {
        perror("receive error");
        exit(1);
    }
    msg[status] = '\0';
    printf("Client:%s\n", msg);

    
    buf_ptr = strchr(msg, ':') + 2; // Skip the command code;

    sprintf(response,  "250 Hello %.90s .......receiver OK\n", buf_ptr);

    // Send the response
    sendto(server_sd, response, strlen(response), 0, (struct sockaddr*)&client, sizeof(client));

    
    bzero(&msg,sizeof(msg));
    status = recvfrom(server_sd,msg,MAX,0, (struct sockaddr *)&client, &client_len);
    if(status<0)
    {
        perror("receive error");
        exit(1);
    }
    msg[status] = '\0';
    printf("Client:%s\n", msg);

    sprintf(msg,"354 Enter mail,end with \".\" on a line by itself \n");
    sendto(server_sd,msg,MAX,0,(struct sockaddr*)&client,sizeof(client));

    while(1)
    {
        bzero(&msg,sizeof(msg));
        status = recvfrom(server_sd,msg,MAX,0, (struct sockaddr *)&client, &client_len);
        if(status<0)
        {
            perror("receive error");
            exit(1);
        }

        msg[status] = '\0';
        printf("Client:%s\n", msg);     
        if (strcmp(msg, ".") == 0)
            break;   

    }

    bzero(&msg,sizeof(msg));
    status = recvfrom(server_sd,msg,MAX,0, (struct sockaddr *)&client, &client_len);
    if(status<0)
    {
        perror("receive error");
        exit(1);
    }
    msg[status] = '\0';
    printf("Client:%s\n", msg);
    sprintf(msg, "354 Send attachment\n");
    sendto(server_sd,msg,MAX,0,(struct sockaddr*)&client,sizeof(client));
    
    receive_image(server_sd,&client);
    sprintf(msg, "250 Image received successfully\n");
    sendto(server_sd,msg,MAX,0,(struct sockaddr*)&client,sizeof(client));


    bzero(&msg,sizeof(msg));
    status = recvfrom(server_sd,msg,MAX,0, (struct sockaddr *)&client, &client_len);
    if(status<0)
    {
        perror("receive error");
        exit(1);
    }
    msg[status] = '\0';
    printf("Client:%s\n", msg);

    sprintf(msg, "QUIT\n");
    sendto(server_sd,msg,MAX,0,(struct sockaddr*)&client,sizeof(client));

    sprintf(msg, "221 servers mail server closing connection\n");
    sendto(server_sd,msg,MAX,0,(struct sockaddr*)&client,sizeof(client));

    
}