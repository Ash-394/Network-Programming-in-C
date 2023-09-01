#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main()
{
    int frames,window;
    int i,j;
    int no_of_trans=0,success_ack_frames;

    printf("Enter the num of frames:");
    scanf("%d",&frames);

    printf("Enter the window size:");
    scanf("%d",&window);

    i=1; //starting from frame 1
    while(i<=frames)
    {
        success_ack_frames=0;
        for(j=i;j<i+window && j<=frames ; j++)
        {
            printf("Sending frame %d\n",j);
            no_of_trans++;
        }

        printf("Waiting for acknowledgements.....\n");

        for(j=i;j<i+window && j<=frames ; j++)
        {
            srand(time(NULL));
            int flag = rand()%2;
            if(flag)
            {
                printf("Acknowledgement received for frame %d\n",j);
                success_ack_frames++;
            }
            else{
                printf("Acknowledgement missing for frame %d\n",j);
                printf("Retransmitting....\n");
                break;
            }
        }

        i += success_ack_frames;
    }

    printf("All frames transmitted! \t No. of transmissions: %d\n",no_of_trans);
}