#include "klock.h"
#include <stdio.h>

int lock_count = 0;

void init_lock(SmartLock* lock) {
	pthread_mutex_init(&(lock->mutex), NULL);
	lock->lock_number = lock_count;
	lock_count++;
}

int lock(SmartLock* lock) {
	printf("%lu attempting to acquire lock %d\n", pthread_self(), lock->lock_number);
	pthread_mutex_lock(&(lock->mutex));
	return 1;
}

void unlock(SmartLock* lock) {
	pthread_mutex_unlock(&(lock->mutex));
}

/*
 * Cleanup any dynamic allocated memory for SmartLock to avoid memory leak
 * You can assume that cleanup will always be the last function call
 * in main function of the test cases.
 */
void cleanup() {

}
