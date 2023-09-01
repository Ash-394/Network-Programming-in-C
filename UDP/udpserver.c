#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>


int main(int argc,char* argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Usage: ./server port\n");
        exit(1);
    }

   //create and initialize a server socket
    int server_sd = socket(AF_INET,SOCK_DGRAM,0);
    if(server_sd<0)
    {
        perror("socket creation failed");
        exit(1);
    }

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

    int size[2][2];
    int client_len;
    //size matrix receive
    int status = recvfrom(server_sd,size,sizeof(size),0,(struct sockaddr*)&client,&client_len);
    if(status<0)
    {
        perror("receiving size from client failed");
        exit(1);
    }

    //mat 1 and 2 receive
    int r1 = size[0][0], c1 = size[0][1], r2 = size[1][0], c2 = size[1][1];
    int mat1[r1][c1], mat2[r2][c2], prod[r1][c2];
    printf("%d %d %d %d \n",r1,c1,r2,c2);

    status = recvfrom(server_sd,mat1,sizeof(mat1),0,(struct sockaddr*)&client,&client_len);
    if(status<0)
    {
        perror("receiving mat1 from client failed");
        exit(1);
    }

    status = recvfrom(server_sd,mat2,sizeof(mat2),0,(struct sockaddr*)&client,&client_len);
    if(status<0)
    {
        perror("receiving mat2 from client failed");
        exit(1);
    }

    //prod calculation
    int i,j,k;
    for (i=0; i < r1 ; i++)
        for (j=0; j < c2; j++)
        {
                prod[i][j]=0;
        }
        
    for(i=0;i<r1;i++)
    {
        for(j=0;j<c2;j++)
        {
            for(k=0;k<c1;k++)
            {
                prod[i][j] += mat1[i][k] + mat2[k][j];
            }
        }
    }

    

    printf("product:\n");
    for(i=0;i<r1;i++)
    {   for(j=0;j<c2;j++)
        {
            printf("%d ",prod[i][j]);
        }
        printf("\n");
    }
    status = sendto(server_sd,prod,sizeof(prod),0,(struct sockaddr*)&client,sizeof(client));
    if(status<0)
    {
        perror("sending prod from client failed");
        exit(1);
    }
    close(server_sd);
}