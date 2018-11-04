#include "klock.h"
#include <stdio.h>
#include <stdbool.h>
#include "graph.h"
#include<pthread.h>
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
	
	pthread_mutex_lock(&count_lock);
	pthread_mutex_lock(&index_lock);
	lock->lock_number = vertex_count;
	vertex_index[vertex_count] = lock->lock_number;
	vertex_count++;
	pthread_mutex_unlock(&index_lock);
	pthread_mutex_unlock(&count_lock);
	if(resource_graph_initiated==false){
		//make sure to change this later!
		graph = createGraph(6);
	}
	
}

int lock(SmartLock* lock) {
	//check if this thread is a new thread trying to acquire a lock for the first time
	int thread_number = (int) pthread_self();
	_Bool new_thread_check = is_new_thread(thread_number);
	//if it is new, add it to index list and add an edge to the graph
	if(new_thread_check){
		//updating num of vertices and vertex indexing, setting vertex number
		pthread_mutex_lock(&count_lock);
		pthread_mutex_lock(&index_lock);
		vertex_index[vertex_count] = thread_number;
		int vertex_number = vertex_count;
		printf("thread number %d, is now vertex number %d and vi[%d] = %d\n", thread_number, vertex_count, vertex_count, thread_number);
		vertex_count++;
		pthread_mutex_unlock(&index_lock);
		pthread_mutex_unlock(&count_lock);

		printf("\nthis is %d printing here!\n", vertex_number);
		//addEdge(graph, vertex_number, lock->lock_number);


		//handle logic for cycle detection here
		//create edge for resource request
		pthread_mutex_lock(&graph_lock);
		printf("\n\tadding edge\n");
		addEdge(graph, vertex_number, lock->lock_number);
		printGraph(graph);
		_Bool c_test = bfs_cycle_detect(graph, vertex_number);
		//if there is a cycle, delete edge and return 0
		if(c_test){
			deleteEdge(graph, vertex_number, lock->lock_number);
			pthread_mutex_unlock(&graph_lock);
			//printf("\nthis is %d exiting here!\n", vertex_number);
			return 0;
		}
		printf("\nthis is %d exiting here!\n", vertex_number);

		//no cycle, keep else and obtain lock 
		pthread_mutex_unlock(&graph_lock);

		//check if there is a cycle, if there is delete edge and deny resource, if there isn't attempt to acquire lock
	}
	else{//not a new thread, find vertex number, add edge and check for cycle
		int vertex_number = find_vertex_number(vertex_index, thread_number,vertex_count);
		printf("\nthis is %d printing here!\n", vertex_number);
		
		//add edge to graph and see if it causes a cycle 
		pthread_mutex_lock(&graph_lock);
		printf("\n\tadding edge...\n");
		addEdge(graph, vertex_number, lock->lock_number);

		printGraph(graph);
		_Bool c_test = bfs_cycle_detect(graph, lock->lock_number);
		//if there is a cycle, delete edge and return 0
		if(c_test==true){
			deleteEdge(graph, vertex_number, lock->lock_number);
			pthread_mutex_unlock(&graph_lock);
			printf("\n\tdeleting edge...\n");
			printGraph(graph);
			printf("\nthis is %d exiting here! This is where I am\n", vertex_number);
			sleep(1);
			return 0;
		}
		//no cycle, keep else and obtain lock 
		printf("\nthis is %d exiting here! and going to acquire lock!\n", vertex_number);
		pthread_mutex_unlock(&graph_lock);
		sleep(1);


	}
	pthread_mutex_lock(&(lock->mutex));
	

	return 1;
}

void unlock(SmartLock* lock) {
	pthread_mutex_unlock(&(lock->mutex));
	int thread_number = (int) pthread_self();
	int vertex_number = find_vertex_number(vertex_index,thread_number, vertex_count);
	pthread_mutex_lock(&graph_lock);
	deleteEdge(graph, lock->lock_number, vertex_number);
	pthread_mutex_unlock(&graph_lock);
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