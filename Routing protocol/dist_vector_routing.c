#include<stdio.h>


struct node{
    unsigned dist[20];
    unsigned from[20];
} rt[10];

int main()
{
    int cost_matrix[20][20];
    int i,j,k,n,count;

    printf("\nEnter the number of nodes : ");
    scanf("%d",&n);

    printf("\nEnter the cost matrix :\n");
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            scanf("%d",&cost_matrix[i][j]);
            cost_matrix[i][i]=0;
            rt[i].dist[j]=cost_matrix[i][j];//initialise the distance equal to cost matrix
            rt[i].from[j]=j;
        }
    }

    do{

        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                for (k = 0; k < n; k++)
                    if(rt[i].dist[j]>cost_matrix[i][k]+rt[k].dist[j])
                    {
                        rt[i].dist[j]=rt[i].dist[k]+rt[k].dist[j];
                        rt[i].from[j]=k;
                        count++;
                    }
    }while(count!=0);

    for(i=0;i<n;i++)
    {
        printf("\n\n For router %d\n",i+1);
            for(j=0;j<n;j++)
            {
                printf("\t\n node %d via %d Distance %d ",j+1,rt[i].from[j]+1,rt[i].dist[j]);
            }
    }

}