#include "klock.h"
#include <stdio.h>
#include <stdbool.h>
#include "graph.h"
#include<pthread.h>

#define MAX_NUM_VERTICES 10000

pthread_mutex_t count_lock;

int vertex_count = 0;
_Bool resource_graph_initiated = false; 
int vertex_index[MAX_NUM_VERTICES];

struct Graph * graph;

void init_lock(SmartLock* lock) {
	pthread_mutex_init(&(lock->mutex), NULL);
	
	pthread_mutex_lock(&count_lock);
	lock->lock_number = vertex_count;
	vertex_count++;
	pthread_mutex_unlock(&count_lock);
	if(resource_graph_initiated==false){
		graph = createGraph(6);
	}
	vertex_index[vertex_count] = lock->lock_number;
}

int lock(SmartLock* lock) {
	printf("\n%d attempting to acquire lock %d\n", (int) pthread_self(), lock->lock_number);
	//check if this thread is a new thread trying to acquire a lock for the first time
	int thread_number = (int) pthread_self();
	_Bool new_thread_check = is_new_thread(thread_number);
	//if it is new, add it to index list and add an edge to the graph
	if(new_thread_check){
		pthread_mutex_lock(&count_lock);
		vertex_index[vertex_count] = thread_number;
		addEdge(graph, vertex_count, lock->lock_number);
		printf("\nthis is %d printing here!\n", vertex_count);
		printGraph(graph);
		vertex_count++;
		pthread_mutex_unlock(&count_lock);
		//while cycle is detected, do nothing
		//while(bfs_cycle_detect(graph, vertex_count)==true);
	}
	else{//not a new thread, find vertex number, add edge and check for cycle
		int vertex_number = find_vertex_number(vertex_index, thread_number,vertex_count);
		addEdge(graph, vertex_number, lock->lock_number);
		printf("\nthis is %d printing here!\n", vertex_number);
		//while cycle is detected do nothing
		printGraph(graph);
		//while(bfs_cycle_detect(graph, vertex_number)==true);
	}
	pthread_mutex_lock(&(lock->mutex));
	

	return 1;
}

void unlock(SmartLock* lock) {
	pthread_mutex_unlock(&(lock->mutex));
	int thread_number = (int) pthread_self();
	int vertex_number = find_vertex_number(vertex_index,thread_number, vertex_count);
	deleteEdge(graph, lock->lock_number, vertex_number);
	//delete edge from graph
}

/*
 * Cleanup any dynamic allocated memory for SmartLock to avoid memory leak
 * You can assume that cleanup will always be the last function call
 * in main function of the test cases.
 */
void cleanup() {

}

_Bool is_new_thread(int thread_number){
	for(int i = 0; i<vertex_count;i++){
		if(vertex_index[i] == thread_number){
			return false;
		}
	}
	return true;
}

int find_vertex_number(int* vertex_index, int thread_number, int vertex_count){
	for(int i = 0; i<vertex_count;i++){
		if(vertex_index[i] == thread_number){
			return i;
		}
	}
	printf("index not found!\n");
	return 0;
}