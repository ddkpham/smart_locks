#include "klock.h"
#include <stdio.h>
#include <stdbool.h>
#include "graph.h"
#include <pthread.h>
#include <unistd.h>


#define MAX_NUM_VERTICES 10000

pthread_mutex_t count_lock;
pthread_mutex_t index_lock;
pthread_mutex_t graph_lock;

int vertex_count = 0;
_Bool resource_graph_initiated = false; 
int vertex_index[MAX_NUM_VERTICES];

struct Graph * graph;

void init_lock(SmartLock* lock) {
	pthread_mutex_init(&(lock->mutex), NULL);
	//initialize locks
	pthread_mutex_lock(&count_lock);
	pthread_mutex_lock(&index_lock);
	lock->lock_number = vertex_count;
	vertex_index[vertex_count] = lock->lock_number;
	vertex_count++;
	pthread_mutex_unlock(&index_lock);
	pthread_mutex_unlock(&count_lock);
	//check to see if it is first lock initiated
	if(resource_graph_initiated==false){
		//Initialize mutexes for our shared resources
		graph = createGraph(2000);
		if (pthread_mutex_init(&count_lock, NULL) != 0)
    	{
        	printf("\n mutex init failed\n");
        	return;
    	}
		if (pthread_mutex_init(&graph_lock, NULL) != 0)
    	{
        	printf("\n mutex init failed\n");
        	return;
    	}
		if (pthread_mutex_init(&index_lock, NULL) != 0)
    	{
        	printf("\n mutex init failed\n");
        	return;
    	}
	resource_graph_initiated= true;
	}
}

int lock(SmartLock* lock) {
	//check if this thread is a new thread trying to acquire a lock for the first time
	int thread_number = (int) pthread_self();
	pthread_mutex_lock(&index_lock);
	_Bool new_thread_check = is_new_thread(thread_number);
	pthread_mutex_unlock(&index_lock);
	//if it is new
	if(new_thread_check){
		//New thread, give it a vertex number and mark it in index
		pthread_mutex_lock(&count_lock);
		pthread_mutex_lock(&index_lock);
		vertex_index[vertex_count] = thread_number;
		int vertex_number = vertex_count;
		vertex_count++;
		pthread_mutex_unlock(&index_lock);
		pthread_mutex_unlock(&count_lock);

		//create edge for resource request
		pthread_mutex_lock(&graph_lock);
		addEdge(graph, vertex_number, lock->lock_number);
		_Bool c_test = bfs_cycle_detect(graph, lock->lock_number);
		//if there is a cycle, delete edge and return 0
		if(c_test){
			deleteEdge(graph, vertex_number, lock->lock_number);
			pthread_mutex_unlock(&graph_lock);
			return 0;
		}
		pthread_mutex_unlock(&graph_lock);
		pthread_mutex_lock(&(lock->mutex));
		return 1;
	}
	else{
		//not a new thread, find vertex number, add edge and check for cycle
		int vertex_number = find_vertex_number(vertex_index, thread_number,vertex_count);
	
		//add edge to graph and see if it causes a cycle 
		pthread_mutex_lock(&graph_lock);
		addEdge(graph, vertex_number, lock->lock_number);
		_Bool c_test = bfs_cycle_detect(graph, lock->lock_number);
		//if there is a cycle, delete edge and return 0
		if(c_test==true){
			deleteEdge(graph, vertex_number, lock->lock_number);
			pthread_mutex_unlock(&graph_lock);
			return 0;
		}
		//there is no cycle, keep edge and attempt to lock 
		pthread_mutex_unlock(&graph_lock);
		pthread_mutex_lock(&(lock->mutex));
		return 1;

	}
	
}

void unlock(SmartLock* lock) {
	//find which vertex number is unlocking
	int thread_number = (int) pthread_self();
	int vertex_number = find_vertex_number(vertex_index,thread_number, vertex_count);

	//delete edge from graph and free lock
	pthread_mutex_lock(&graph_lock);
	deleteEdge(graph, vertex_number, lock->lock_number);
	pthread_mutex_unlock(&graph_lock);
	pthread_mutex_unlock(&(lock->mutex));	
}


/*
 * Cleanup any dynamic allocated memory for SmartLock to avoid memory leak
 * You can assume that cleanup will always be the last function call
 * in main function of the test cases.
 */
void cleanup() {
	destroyGraph(graph);
	pthread_mutex_destroy(&count_lock);
	pthread_mutex_destroy(&index_lock);
	pthread_mutex_destroy(&graph_lock);
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