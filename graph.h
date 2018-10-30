#include <stdio.h>
#include <stdlib.h>

#define MAX_NUM_IN_QUEUE 1000

struct queue{
    int items[MAX_NUM_IN_QUEUE];
    int front;
    int rear;
};


struct queue * createQueue();
void enqueue(struct queue* q, int value);
