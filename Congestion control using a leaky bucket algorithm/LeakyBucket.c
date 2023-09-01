#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>

#define no_of_pckts 10

int Select(int a)
{
    srand(time(NULL));
    int rn = rand()%a;
    return rn==0 ? 1:rn;

}
int main()
{
    int buck_size,out_rate,packets[no_of_pckts];
    int i;
    
    //packets size set
    for(i=0;i<no_of_pckts;i++)
    {
        packets[i]=Select(6)*10;
    }

    printf("Packets to be send:\n");
    for(i=0;i<no_of_pckts;i++)
    {
        printf("Packet[%d]:%d bytes\n",i,packets[i]);
    }

    printf("Enter bucket size:");
    scanf("%d",&buck_size);

    printf("Enter output rate:");
    scanf("%d",&out_rate);

    int packs_size_remain=0;
    int p_time,clk,op;
    for(i=0;i<no_of_pckts;i++)
    {
        if((packets[i]+packs_size_remain)>buck_size)
        {
            if(packets[i]>buck_size)
            {
                printf("Incoming packet(%d bytes) exceeds bucket size(%d bytes)!-PACKET REJECTED\n",packets[i],buck_size);
            }
            else
            {
                printf("Bucket capacity exceeded-PACKET REJECTED\n");
            }
        }
        else{
            packs_size_remain +=packets[i];
            printf("\nIncoming Packet size: %d bytes\n", packets[i]);
            printf("\nBytes remaining to Transmit: %d\n", packs_size_remain);

            p_time = Select(4)*10;

            printf("\nTime left for transmission: %d units\n", p_time);
            for(clk=10;clk<=p_time;clk+=10)
            {
                sleep(1);
                if(packs_size_remain)
                {
                    if(packs_size_remain<=out_rate)
                    {
                        op=packs_size_remain;
                        packs_size_remain=0;
                    }
                    else
                    {
                        op = out_rate;
                        packs_size_remain -=out_rate;
                        printf("\nPacket of size %d Transmitted\n", op);
                        printf("----Bytes Remaining to Transmit: %d\n",packs_size_remain);
                    }
                    
                }
                else{
                    printf("\nTime left for transmission: %d units", p_time-clk);
                    printf("\nNo packets to transmit!!");
                }
            }

        }
    }

}