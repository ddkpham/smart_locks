#ifndef __KLOCK_H__
#define __KLOCK_H__
#include <stdbool.h>
#include <pthread.h>

typedef struct {
	int lock_number;
	pthread_mutex_t mutex;
} SmartLock;

void init_lock(SmartLock* lock);
int lock(SmartLock* lock);
void unlock(SmartLock* lock);
void cleanup();
_Bool is_new_thread(int thread_number);
int find_vertex_number(int* index, int thread_number, int vertex_count);

#endif
