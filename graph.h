#include <stdio.h>
#include <stdlib.h>

#define MAX_NUM_IN_QUEUE 1000

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

//queue for BFS
struct queue* createQueue();
void enqueue(struct queue* q, int value);
int dequeue(struct queue* q);
void display(struct queue* q);
int isEmpty(struct queue* q);
void printQueue(struct queue* q);

//Graph functions

struct Graph* createGraph(int vertices);
void addEdge(struct Graph *, int src, int dest);
void printGraph(struct Graph* graph);
void bfs(struct Graph* graph, int startVertex);

