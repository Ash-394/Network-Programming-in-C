#include <stdio.h>
#define INFINITY 9999
#define MAX 10
void dijkstra(int G[MAX][MAX], int n, int startnode);
int main()
{
    int cost_matrix[MAX][MAX], i, j, n, u;
    printf("Enter no. of routers:");
    scanf("%d", &n);
    printf("\nEnter the adjacency cost_matrix:\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
        {   printf("\n%d->%d:", i, j);    
            scanf("%d", &cost_matrix[i][j]);
            if (cost_matrix[i][j] < 0)
                cost_matrix[i][j] = 1000;
        }
    printf("\nEnter the starting node:");
    scanf("%d", &u);
    dijkstra(cost_matrix, n, u);
    return 0;
}
void dijkstra(int G[MAX][MAX], int n, int startnode)
{
    int  distance[MAX], pred[MAX];
    int visited[MAX], count, mindistance, nextnode, i, j;

    // initialize pred[],distance[] and visited[]
    for (i = 0; i < n; i++)
    {
        distance[i] = G[startnode][i];
        pred[i] = startnode;
        visited[i] = 0;
    }
    
   
    visited[startnode] = 1;
    count = 0;
    while (count < n )
    {
        mindistance = INFINITY;
        // nextnode gives the node at minimum distance
        for (i = 0; i < n; i++)
            if (distance[i] < mindistance && !visited[i])
            {
                mindistance = distance[i];
                nextnode = i;
            }
        // check if a better path exists through nextnode
        visited[nextnode] = 1;
        for (i = 0; i < n; i++)
            if (!visited[i])
                if (mindistance + G[nextnode][i] < distance[i])
                {
                    distance[i] = mindistance + G[nextnode][i];
                    pred[i] = nextnode;
                }
        count++;
    }
    // print the path and distance of each node
    for (i = 0; i < n; i++)
        
        {
            printf("\n%d==>%d:Path taken:%d ",startnode, i,i);
            j = i;
            do
            {
                j = pred[j];
                printf("<-%d", j);
            } while (j != startnode);
             printf("\n Shortest path cost:%d", distance[i]);
        }
}