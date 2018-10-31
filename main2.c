#include <stdlib.h>
#include <stdio.h>
#include "graph.h"
#include "klock.h"
#include <stdbool.h>

_Bool inQueue = false;

int main(void){
    struct Graph * graph = createGraph(8);

    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    //addEdge(graph, 0, 3);
    addEdge(graph, 3, 4);
    //addEdge(graph, 1, 4);
    addEdge(graph, 1, 5);
    addEdge(graph, 3, 5);
    addEdge(graph, 3, 6);
    addEdge(graph, 5, 7);
    //addEdge(graph, 6, 7);
    //addEdge(graph, 4, 7);


    bfs(graph, 0);

    /* struct queue * queue = createQueue();
    for(int i = 0; i< 15; i++){
        enqueue(queue, i);
    }
    
    for(int i = -10; i< 20; i++){
        if(isInQueue(queue, i)){
            printf("%d is in the queue!\n", i);
        }
        else{
            printf("%d is not in the queue!\n", i);
        }

    }*/
    

    
    


    return 0;
}
