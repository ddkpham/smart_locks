#include <stdlib.h>
#include <stdio.h>
#include "graph.h"
#include "klock.h"
#include <stdbool.h>

_Bool inQueue = false;

int main(void){
    struct Graph * graph = createGraph(9);

    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 0, 3);
    addEdge(graph, 3, 4);
    addEdge(graph, 2, 5);
    addEdge(graph, 1, 6);
    
    addEdge(graph, 4, 8);
    addEdge(graph, 6, 7);
    addEdge(graph, 6, 5);
    deleteEdge(graph, 6, 5);
    addEdge(graph, 5, 6);
    
    //addEdge(graph, 1,2);
    //addEdge(graph, 4, 7);


    _Bool test = bfs_cycle_detect(graph, 0);
    if(test){
        printf("There was a Cycle detected!\n");
    }
    else{
        printf("No cycle detected\n");
    }
    printGraph(graph);
    return 0;
}
