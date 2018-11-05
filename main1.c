#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "klock.h"
#include <stdbool.h>
#include <pthread.h>

SmartLock glocks[5];
// SmartLock glocks[2];

void *thread_0(void *arg) {
    printf("thread 0\n");
    while (lock(&glocks[0]) == 0); // Force locking glocks[0]
    // while (lock(&glocks[0]) == 0); // Force locking glocks[0]

    sleep(1);
    while (lock(&glocks[1]) == 0); // Force locking glocks[1]

    printf("thread 0 is working on critical section for 1 second\n");
    sleep(1);

    unlock(&glocks[0]);
    unlock(&glocks[1]);

    return NULL;
}

void *thread_1(void *arg) {
    printf("thread 1\n");
    while (1) {
        int lock1_res = lock(&glocks[1]);
        sleep(2);
        if (lock1_res) {
            int lock0_res = lock(&glocks[0]);
            if (lock0_res) {
                printf("thread 1 is working on critical section for 1 second\n");
                sleep(1);
                unlock(&glocks[1]);
                unlock(&glocks[0]);
                break;
            } else {
            	// If thread_1 is not able to lock glocks[0] now, it will also
            	// unlock glocks[1] and sleep for 1 second before retry so that
            	// thread_0 can acquire glocks[1]
                unlock(&glocks[1]);
                sleep(1);
            }
        }
    }
    return NULL;
}

void *thread_2(void *arg) {
    while (lock(&glocks[0]) == 0); // Force locking glocks[0]
    // while (lock(&glocks[0]) == 0); // Force locking glocks[0]

    sleep(1);
    while (lock(&glocks[1]) == 0); // Force locking glocks[1]
    // while (lock(&glocks[2]) == 0); // Force locking glocks[1]

    printf("thread 2 is working on critical section for 1 second\n");
    sleep(1);

    unlock(&glocks[0]);
    unlock(&glocks[1]);

    return NULL;
}

void *thread_3(void *arg) {
    while (1) {
        int lock1_res = lock(&glocks[1]);
        sleep(2);
        if (lock1_res) {
            int lock0_res = lock(&glocks[0]);
            if (lock0_res) {
                printf("thread 3 is working on critical section for 1 second\n");
                sleep(1);
                unlock(&glocks[1]);
                unlock(&glocks[0]);
                break;
            } else {
            	// If thread_1 is not able to lock glocks[0] now, it will also
            	// unlock glocks[1] and sleep for 1 second before retry so that
            	// thread_0 can acquire glocks[1]
                unlock(&glocks[1]);
                sleep(1);
            }
        }
    }
    return NULL;
}

void *thread_4(void *arg) {
    while (1) {
        int lock1_res = lock(&glocks[1]);
        sleep(2);
        if (lock1_res) {
            int lock0_res = lock(&glocks[0]);
            if (lock0_res) {
                printf("thread 4 is working on critical section for 1 second\n");
                sleep(1);
                unlock(&glocks[1]);
                unlock(&glocks[0]);
                break;
            } else {
            	// If thread_1 is not able to lock glocks[0] now, it will also
            	// unlock glocks[1] and sleep for 1 second before retry so that
            	// thread_0 can acquire glocks[1]
                unlock(&glocks[1]);
                sleep(1);
            }
        }
    }
    return NULL;
}

void *thread_5(void *arg) {
    while (1) {
        int lock1_res = lock(&glocks[1]);
        sleep(2);
        if (lock1_res) {
            int lock0_res = lock(&glocks[0]);
            if (lock0_res) {
                printf("thread 5 is working on critical section for 1 second\n");
                sleep(1);
                unlock(&glocks[1]);
                unlock(&glocks[0]);
                break;
            } else {
            	// If thread_1 is not able to lock glocks[0] now, it will also
            	// unlock glocks[1] and sleep for 1 second before retry so that
            	// thread_0 can acquire glocks[1]
                unlock(&glocks[1]);
                sleep(1);
            }
        }
    }
    return NULL;
}
void *thread_6(void *arg) {
      while (lock(&glocks[0]) == 0); // Force locking glocks[0]
    // while (lock(&glocks[0]) == 0); // Force locking glocks[0]

    sleep(1);
    while (lock(&glocks[1]) == 0); // Force locking glocks[1]
    // while (lock(&glocks[2]) == 0); // Force locking glocks[1]

    printf("thread 6 is working on critical section for 1 second\n");
    sleep(1);

    unlock(&glocks[0]);
    unlock(&glocks[1]);

    return NULL;
}

/*
 * This is a simple deadlock condition similar to the RAG showed in
 * assignment web page.
 * The critical sections of thread_0 and thread_1 should both be able
 * to invoke if the SmartLock is implemented correctly, and there should 
 * be no memory leak after finishing the main function.
 */
int main(int argc, char *argv[]) {
    printf("starting\n");

    init_lock(&glocks[0]);
    init_lock(&glocks[1]);

    pthread_t tids[7];

    pthread_create(&tids[0], NULL, thread_0, NULL);
    pthread_create(&tids[1], NULL, thread_1, NULL);
    pthread_create(&tids[2], NULL, thread_2, NULL);
    pthread_create(&tids[3], NULL, thread_3, NULL);
    pthread_create(&tids[4], NULL, thread_4, NULL);
    pthread_create(&tids[5], NULL, thread_5, NULL);
    pthread_create(&tids[6], NULL, thread_6, NULL);

    pthread_join(tids[0], NULL);
    pthread_join(tids[1], NULL);
    pthread_join(tids[2], NULL);
    pthread_join(tids[3], NULL);
    pthread_join(tids[4], NULL);
 	pthread_join(tids[5], NULL);
	pthread_join(tids[6], NULL);


    // You can assume that cleanup will always be the last function call
    // in main function of the test cases.
    cleanup();
    return 0;
}