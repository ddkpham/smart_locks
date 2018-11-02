#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include <stdbool.h>

#define MAX_NUM_IN_QUEUE 10000
//queue functions for BFS
struct queue* createQueue(){
    struct queue* queue = malloc(sizeof(struct queue));
    queue->front = -1;
    queue->rear = -1;
    return queue;
}

void enqueue(struct queue* queue, int value){
    if(queue->rear == MAX_NUM_IN_QUEUE -1 ){
        printf("Queue is full\n");
        return;
    }
    if(queue->front ==-1){//empty queue
        queue->front=0;
    }
    queue->rear++;
    queue->items[queue->rear] = value;
}

int dequeue(struct queue* queue){
    if(queue->rear==-1 && queue->front==-1){
        printf("Queue is empty\n");
        return -1;
    }
    else{
        int temp = queue->items[queue->front];
        queue->front++;
        if(queue->front > queue->rear){//reset queue
            queue->front = -1;
            queue->rear = -1;
        }
        return temp;
    }
}

void printQueue(struct queue* queue){
    if(queue->front ==-1 && queue->rear == -1){
        printf("Queue is empty.\n");
    }
    else{
        printf("Queue contains...\n");
        for(int i = queue->front; i < queue->rear +1; i++){
            printf("%d\t", queue->items[i]);
        }
        printf("\n");
    }
}

_Bool isInQueue(struct queue* queue, int value){
    for(int i = queue->front; i< queue->rear+1;i++){
        if(queue->items[i]== value){
            return true;
        }
    }
    return false;
}

int isEmpty(struct queue* queue){
    if(queue->front ==-1 && queue->rear ==-1){
        printf("Queue is empty\n");
        return 1;
    }
    else{
        return 0;
    }
}


//Graph functions

struct Graph* createGraph(int vertices){
    struct Graph* graph = malloc(sizeof(struct Graph));
    graph->num_of_vertices = vertices;

    graph->adjLists = malloc(sizeof(struct node*)* vertices);
    graph->visited = malloc(sizeof(int) * vertices);

    for(int i = 0; i< vertices; i++){
        graph->adjLists[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

struct node* createNode(int value){
    struct node* newNode = malloc(sizeof(struct node));
    newNode->vertex_number = value;
    newNode->next = NULL;
    return newNode;
}

void addEdge(struct Graph * graph , int src, int dest){
    //Add edge from src to dest
    struct node * newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;

    //Add edge from dest to src
    newNode = createNode(src);
    newNode->next = graph -> adjLists[dest];
    graph->adjLists[dest] = newNode;
}

void deleteSingleEdge(struct Graph* graph, int src, int dest){
    struct node * curr = graph->adjLists[src];
    struct node * prev = graph->adjLists[src];
    //if dest is first in list or only element
    if(curr->vertex_number== dest && prev->vertex_number == dest){
        graph->adjLists[src] = curr->next;
        free(curr);
        return;
    }
    curr= curr->next;
    while(curr){
        if(curr->vertex_number==dest){
            prev->next = curr->next;
            free(curr);
        }
        curr= curr->next;
    }
}

void deleteEdge(struct Graph* graph, int src, int dest){
    //delete from both adjacency lists
    deleteSingleEdge( graph, src, dest);
    deleteSingleEdge( graph, dest, src);
    return;
}

    

    
    //delete edge from dest to src




void addVertex(struct Graph *, int vertex_number);

void printGraph(struct Graph* graph){
    for(int i = 0; i<graph->num_of_vertices;i++){
        printf("The neighbours of vertex %d are:  ", i);
        struct node* temp = graph->adjLists[i];
        while(temp){
            printf("%d  ", temp->vertex_number);
            temp = temp->next;
        }
        printf("\n");
    }
}

//detects cycles now
//returns true if cycle is detected, false otherwise
_Bool bfs_cycle_detect(struct Graph* graph, int startVertex){
    int parent[graph->num_of_vertices];//parent of node u
    /*for(int i = 0; i<graph->num_of_vertices;i++){
        parent[i] = -1;
        printf("parent[%d] is: %d\n", i, parent[i]);
    }*/
    struct queue * q = createQueue();
    graph->visited[startVertex] = 1;
    enqueue(q, startVertex);
    while(!isEmpty(q)){
        printQueue(q);
        int currentVertex = dequeue(q);
        printf("Visited %d\n", currentVertex);
        struct node* temp = graph->adjLists[currentVertex];
        while(temp){
        int adjVertex = temp -> vertex_number;

            if(graph->visited[adjVertex] == 0){
                graph->visited[adjVertex] = 1;
                enqueue(q, adjVertex);
                parent[adjVertex] = currentVertex;
                //fix this to solve for cycles
            }
            if(graph->visited[adjVertex]== 1 && parent[adjVertex] != currentVertex 
                && parent[adjVertex]!=-1 && parent[currentVertex] != adjVertex){
                printf("Cycle detected!\n");
                return true;
            }
            temp = temp-> next;
       }
   }
   return false;
}
