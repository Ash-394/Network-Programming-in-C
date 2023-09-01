#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define TIMEOUT 2
int main()
{
    int frames;
    printf("Enter the num of frames:");
    scanf("%d",&frames);

    int i;
    for(i=0;i<frames;)
    {
        printf("Sending frame %d\n",i);

        sleep(TIMEOUT);  // Wait for ACK or timeout

        int flag = rand() % 2; // Simulate random success or failure
        if (flag)
        {
            printf("Received ACK for frame %d\n",i);
            i++;
        }
        else
        {
            printf("ACK lost or NAK received for frame %d\n",i);
            
        }
    }
    printf("All frames sent and acknowledged.\n");
}