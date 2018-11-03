#include <stdlib.h>
#include <stdio.h>
#include "graph.h"
#include "klock.h"
#include <stdbool.h>

_Bool inQueue = false;

int main(void){
    struct Graph * graph = createGraph(9);

    int index[100];

    for(int i = 0; i<20; i++){
        index[i] = i+10;
        printf("This is the value of index[%d] = %d\n", i, index[i]);
    }

    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 0, 3);
    addEdge(graph, 3, 4);
    addEdge(graph, 2, 5);
    addEdge(graph, 1, 6);
    
    addEdge(graph, 4, 8);
    addEdge(graph, 6, 7);
    addEdge(graph, 6, 5);
    // deleteEdge(graph, 6, 5);
    // deleteEdge(graph, 0, 1);
    // deleteEdge(graph, 0, 2);
    // deleteEdge(graph, 0, 3);
    // deleteEdge(graph, 3, 4);
    // deleteEdge(graph, 2, 5);
    // deleteEdge(graph, 1, 6);
    
    //addEdge(graph, 4, 8);
    //addEdge(graph, 6, 7);
    //addEdge(graph, 1,2);
    //addEdge(graph, 4, 7);


    _Bool test = bfs_cycle_detect(graph, 8);
    if(test){
        printf("There was a Cycle detected!\n");
    }
    else{
        printf("No cycle detected\n");
    }
    //printGraph(graph);
    int test_index = index[10];
    int vertex_num = find_vertex_number(index, test_index, 20);
    printf("the index of number 20 is : %d\n", vertex_num);
    return 0;
}
