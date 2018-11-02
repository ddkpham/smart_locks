#include "klock.h"
#include <stdio.h>
#include <stdbool.h>

int lock_count = 0;
_Bool resource_graph_initiated = false; 

void init_lock(SmartLock* lock) {
	pthread_mutex_init(&(lock->mutex), NULL);
	lock->lock_number = lock_count;
	lock_count++;
	//if resource graph not initiated, initaite 

	//else add vertext to graph 
}

int lock(SmartLock* lock) {
	printf("%1lu attempting to acquire lock %d\n", (long) pthread_self(), lock->lock_number);
	//add edge to graph

	//check to see if it causes a cycle, if it does keep checking 
	pthread_mutex_lock(&(lock->mutex));
	

	return 1;
}

void unlock(SmartLock* lock) {
	pthread_mutex_unlock(&(lock->mutex));
	//delete edge from graph
	
}

/*
 * Cleanup any dynamic allocated memory for SmartLock to avoid memory leak
 * You can assume that cleanup will always be the last function call
 * in main function of the test cases.
 */
void cleanup() {

}
