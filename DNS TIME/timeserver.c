#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<time.h>
#define PORT 5100
int main(int argc,char* argv[])
{
    int sd,newsd;
    struct sockaddr_in serv,cli;
    char buff[100],msg[200];
    int len;
    sd=socket(AF_INET,SOCK_DGRAM,0);
    if(sd<0)
    {
        printf("\nError in creation");
    }
    else
    {
        printf("\nSocket created");
    }
    bzero((char *)&serv,sizeof(serv));
    serv.sin_family=AF_INET;
    serv.sin_port=htons(PORT);
    serv.sin_addr.s_addr=inet_addr("127.0.0.1");
    if(bind(sd,(struct sockaddr*)&serv,sizeof(serv))<0)
    {
        printf("\nBinding Error");
        exit(1);
    }
    else
    {
        printf("\nBinded");
    }
    int n;
    len=sizeof(cli);
    n=recvfrom(sd,buff,sizeof(buff),0,(struct sockaddr*)&cli,&len);
    buff[n]='\0';

    printf("%s",buff);
    time_t tick;
    tick=time(NULL);
    sprintf(buff,"time at server:%s",ctime(&tick));

    if((n=sendto(sd,buff,sizeof(buff),0,(struct sockaddr*)&cli,sizeof(cli)))<0)
    {
        printf("\nError in reading");
        exit(1);
    }
	
    recvfrom(sd,buff,sizeof(buff),0,(struct sockaddr*)&cli,&len);
	sprintf(msg,"nslookup %s",buff);
	sendto(sd,msg,sizeof(msg),0,(struct sockaddr*)&cli,sizeof(cli));
    close(sd);
    return 0;
}