#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#define PORT 5100
int main(int argc,char* argv[])
{
    int sd;
    struct sockaddr_in serv;
    char buff[100],msg[200];
    sd=socket(AF_INET,SOCK_DGRAM,0);
    if(sd<0)
    {
        printf("\nError in creation");
    }
    else
    {
        printf("\nSocket created");
    }
    bzero(&serv,sizeof(serv));
    serv.sin_family=AF_INET;
    serv.sin_port=htons(PORT);
    serv.sin_addr.s_addr=inet_addr("127.0.0.1");
    sprintf(msg," Please send time.");
    sendto(sd,msg,sizeof(msg),0,(struct sockaddr *)&serv,sizeof(serv));
    recvfrom(sd,buff,sizeof(buff),0,NULL,NULL);
    printf("\nClient:%s",buff);
	printf("Enter domain name.");
	fgets(msg,sizeof(msg),stdin);
    sendto(sd,msg,sizeof(msg),0,(struct sockaddr *)&serv,sizeof(serv));

    recvfrom(sd,buff,sizeof(buff),0,NULL,NULL);
	printf("%s",buff);

    close(sd);
    return 0;
}