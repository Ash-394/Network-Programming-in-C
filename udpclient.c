#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>

#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main(int argc,char * argv[])
{
    if(argc!=3)
    {
        fprintf(stderr,"usage:./client IPofserver port\n");
        exit(1);
    }
    //create and initialize a server socket
    int client_sd;
    client_sd = socket(AF_INET,SOCK_DGRAM,0);
    if(client_sd<0)
    {
         perror("socket creation failed!");
         exit(1);
    }
    
    struct sockaddr_in server;
    bzero(&server,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET,argv[1],&server.sin_addr);

    
    // input matrices
    int i,j;
    int r1,c1,r2,c2; //r2=c1
    printf("matrix 1 row:");
    scanf("%d",&r1);
    printf("matrix 1 col:");
    scanf("%d",&c1);
    r2=c1;
    printf("matrix 2 row: %d\n",r2);
    printf("matrix 2 col:");
    scanf("%d",&c2);
    
    int mat1[r1][c1],mat2[r2][c2];

    printf("Matrix 1:\n");
    for(i=0;i<r1;i++)
        for(j=0;j<c1;j++)
            scanf("%d",&mat1[i][j]);

    printf("Matrix 2:\n");
    for(i=0;i<r2;i++)
        for(j=0;j<c2;j++)
            scanf("%d",&mat2[i][j]);
    
    int size[2][2];
    size[0][0]=r1; size[0][1]=c1;
    size[1][0]=r2;size[1][1]=c2;
    

//send size matrix to server
int status = sendto(client_sd,size,sizeof(size),0,(struct sockaddr*)&server,sizeof(server));
if(status<0)
{
    perror("sending size to server failed!");
    exit(1);
}

//send mat1 to server
status=sendto(client_sd,mat1,sizeof(mat1),0,(struct sockaddr*)&server,sizeof(server));
if(status<0)
{
    perror("sending mat1 to server failed!");
    exit(1);
}

//send mat2 to server
status=sendto(client_sd,mat2,sizeof(mat2),0,(struct sockaddr*)&server,sizeof(server));
if(status<0)
{
    perror("sending mat2 to server failed!");
    exit(1);
}

//receive product from server
int prod[r1][c2];
status = recvfrom(client_sd,prod,sizeof(prod),0,NULL,NULL);
if(status<0)
{
    perror("receiving product from server failed!");
    exit(1);
}

printf("Product matrix:\n");
for(i=0;i<r1;i++)
{   for(j=0;j<c2;j++)
    {
        printf("%d ",prod[i][j]);
    }
    printf("\n");
}
    
close(client_sd);
    
}