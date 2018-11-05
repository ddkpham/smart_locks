#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_NUM_IN_QUEUE 10000
struct node{
    int vertex_number;
    struct node* next;
};

struct queue{
    int items[MAX_NUM_IN_QUEUE];
    int front;
    int rear;
};

struct Graph{
    int num_of_vertices;
    struct node** adjLists;
    int* visited;
};

//queue functions for BFS
struct queue* createQueue();
void enqueue(struct queue* queue, int value);
int dequeue(struct queue* queue);
int isEmpty(struct queue* queue);
void printQueue(struct queue* queue);
_Bool isInQueue(struct queue* queue, int value);
//Graph functions
struct node* createNode(int value);
struct Graph* createGraph(int vertices);
void addEdge(struct Graph *, int src, int dest);
void printGraph(struct Graph* graph);
_Bool bfs_cycle_detect(struct Graph* graph, int startVertex);
void deleteEdge(struct Graph* graph, int src, int dest);


