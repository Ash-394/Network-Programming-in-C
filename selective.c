#include <stdio.h>
#include <stdlib.h>

int main()
{
    int frames[10], w, f;
    printf("frames ");
    scanf("%d", &f);

    printf("window");
    scanf("%d", &w);
    int nt=0,i,flag,k=0;
    int left[100]={-1},j;

    for(i=1;i<=f;i++)
    {
        flag=rand()%2;
        if(flag)
        {
            printf("Frame[%d] transmitted and acknowledged\n",i);
            nt++;
            
        }
        else{
            left[k++]=i;
            printf("Non acknowledgement/missing for %d\n",i);
            nt++;
        }
        if(i%w==0)
        {
            for(j=0;j<k;j++)
            {
                printf("Frame[%d] retransmitted \n ", left[j]);
                nt++;
                printf("Frame[%d] acknowledgement send \n ", left[j]);
            }
            k = 0;
        }
    }
    for (j = 0; j < k; j++)
    {
        printf("Frame[%d] retransmitted \n ", left[j]);
        nt++;
        printf("Frame[%d] acknowledgement send \n ", left[j]);
    }

    printf("No. of transmissions total %d  \n ", nt);
    return 0;
}
