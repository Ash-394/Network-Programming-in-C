#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX 200

void send_image(int socket, struct sockaddr_in *server) {
    FILE *fpimage;
    int size;
    int readsize;
    int status;
    int packet_index = 1;
    char send_buffer[10239];
    char f_name[100];

    printf("\nEnter Image Path/Image file name : ");
    scanf("%s", f_name);

    fpimage = fopen(f_name, "r");
    if (fpimage == NULL) {
        printf("Error occurred while opening Image File!\n");
        return;
    }

    fseek(fpimage, 0, SEEK_END);
    size = ftell(fpimage);
    fseek(fpimage, 0, SEEK_SET);
    printf("Total Image size: %i\n", size);

    printf("Sending Image Size\n");
    sendto(socket, (void *)&size, sizeof(int), 0, (struct sockaddr *)server, sizeof(struct sockaddr_in));

    while (!feof(fpimage)) {
        readsize = fread(send_buffer, 1, sizeof(send_buffer) - 1, fpimage);

        do {
            status = sendto(socket, send_buffer, readsize, 0, (struct sockaddr *)server, sizeof(struct sockaddr_in));
        } while (status < 0);

        packet_index++;
        bzero(send_buffer, sizeof(send_buffer));
    }

    fclose(fpimage);
    printf("Image sent successfully\n");
}

int main(int argc, char *argv[])
{
    if(argc!=3)
    {
        fprintf(stderr,"usage:./client IPofserver port\n");
        exit(1);
    }

    //creating socket
    int client_sd=socket(AF_INET,SOCK_DGRAM,0);
    if(client_sd<0)
    {
        perror("socketcreation failed!");
        exit(1);
    }

    //initializing socket
    struct sockaddr_in server;
    bzero(&server,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET,argv[1],&server.sin_addr);

    char buf[MAX+1];
    sprintf(buf,"SMTP REQUEST FROM CLIENT\n");
    int status = sendto(client_sd,buf,strlen(buf),0,(struct sockaddr*)&server,sizeof(server));
    if(status<0)
    {
        perror("send error");
        exit(1);
    }
    
    bzero(&buf,sizeof(buf));
    status = recvfrom(client_sd,buf,MAX,0, NULL, NULL);
    if(status<0)
    {
        perror("receive error");
        exit(1);
    } 

    buf[status] ='\0';
    printf("Server:%s",buf);

    bzero(&buf,sizeof(buf));
    sprintf(buf,"Hello client_mail_server\n");
    status = sendto(client_sd,buf,strlen(buf),0,(struct sockaddr*)&server,sizeof(server));
    if(status<0)
    {
        perror("send error");
        exit(1);
    }

    bzero(&buf,sizeof(buf));
    status = recvfrom(client_sd,buf,MAX,0, NULL, NULL);
    if(status<0)
    {
        perror("receive error");
        exit(1);
    } 

    buf[status]='\0';
    printf("Server:%s",buf);

    char address[MAX];
    printf("Please enter the sender email:");
    fgets(address,sizeof(address),stdin);
    address[strlen(address)-1]='\0';

    bzero(&buf,sizeof(buf));
    sprintf(buf,"MAIL FROM: <%.90s>",address);
    status = sendto(client_sd,buf,strlen(buf),0,(struct sockaddr*)&server,sizeof(server));
    if(status<0)
    {
        perror("send error");
        exit(1);
    }
    bzero(&buf,sizeof(buf));
    status = recvfrom(client_sd,buf,MAX,0, NULL, NULL);
    if(status<0)
    {
        perror("receive error");
        exit(1);
    } 

    buf[status]='\0';
    printf("Server:%s",buf);

    bzero(&address,sizeof(address));
    printf("Please enter the receipient email:");
    fgets(address,sizeof(address),stdin);
    address[strlen(address)-1]='\0';

    bzero(&buf,sizeof(buf));
    sprintf(buf,"RECIPIENT TO: <%.90s>",address);
    status = sendto(client_sd,buf,strlen(buf),0,(struct sockaddr*)&server,sizeof(server));
    if(status<0)
    {
        perror("send error");
        exit(1);
    } 

    bzero(&buf,sizeof(buf));
    status = recvfrom(client_sd,buf,MAX,0, NULL, NULL);
    if(status<0)
    {
        perror("receive error");
        exit(1);
    } 

    buf[status]='\0';
    printf("Server:%s",buf);

    bzero(&buf,sizeof(buf));
    sprintf(buf,"DATA\n");
    status = sendto(client_sd,buf,strlen(buf),0,(struct sockaddr*)&server,sizeof(server));
    if(status<0)
    {
        perror("send error");
        exit(1);
    } 

    bzero(&buf,sizeof(buf));
    status = recvfrom(client_sd,buf,MAX,0, NULL, NULL);
    if(status<0)
    {
        perror("receive error");
        exit(1);
    }

    buf[status]='\0';
    printf("Server:%s",buf);

    char message[MAX];
    do
    {

        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = '\0'; // Remove newline character
        sendto(client_sd, message, sizeof(message), 0, (struct sockaddr *)&server, sizeof(server));

        if (strcmp(message, ".") == 0)
            break;

    } while (1);

    bzero(&buf,sizeof(buf));
    sprintf(buf,"ATTACHMENT:");
    status = sendto(client_sd,buf,strlen(buf),0,(struct sockaddr*)&server,sizeof(server));
    if(status<0)
    {
        perror("send error");
        exit(1);
    } 
    bzero(&buf,sizeof(buf));
    status = recvfrom(client_sd,buf,MAX,0, NULL, NULL);
    if(status<0)
    {
        perror("receive error");
        exit(1);
    }

    buf[status]='\0';
    printf("Server:%s",buf);
    send_image(client_sd,&server);
    bzero(&buf,sizeof(buf));
    status = recvfrom(client_sd,buf,MAX,0, NULL, NULL);
    if(status<0)
    {
        perror("receive error");
        exit(1);
    }


    bzero(&buf,sizeof(buf));
    sprintf(buf, "QUIT\n");
    sendto(client_sd, buf, strlen(buf), 0, (struct sockaddr *)&server, sizeof(server));


    bzero(&buf,sizeof(buf));
    status = recvfrom(client_sd,buf,MAX,0, NULL, NULL);
    if(status<0)
    {
        perror("receive error");
        exit(1);
    }
    buf[status]='\0';
    printf("Server:%s",buf);

    

    bzero(&buf,sizeof(buf));
    status = recvfrom(client_sd,buf,MAX,0, NULL, NULL);
    if(status<0)
    {
        perror("receive error");
        exit(1);
    }
    buf[status]='\0';
    printf("Server:%s",buf);


}