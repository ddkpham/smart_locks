#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "klock.h"
#include <pthread.h>

SmartLock glocks[3]; //this is a global array of all the smartlocks available to us

void *thread_0(void *arg) {

    printf("\t thread 0 started running\n");
    while (lock(&glocks[0]) == 0); // Force locking glocks[0]
    sleep(1);
    while (lock(&glocks[1]) == 0); // Force locking glocks[1]
    sleep(1);
    while (lock(&glocks[2]) == 0); // Force locking glocks[1]

    // Print after successfully acquiring lock
    printf("\t thread 0 is working on critical section for 1 second\n");
    sleep(1);

    unlock(&glocks[0]);
    unlock(&glocks[1]);
    unlock(&glocks[2]);

    return NULL;
}

void *thread_1(void *arg) {
    while (1) {
        printf("\t thread 1 started running\n");
        int lock1_res = lock(&glocks[1]);
        sleep(2);
        if (lock1_res) {
            int lock0_res = lock(&glocks[0]);  //This will never be granted with exisiting function //Cycle is detected
            if (lock0_res) {
                //
                sleep(1);
                int lock2_res = lock(&glocks[2]);
                if (lock2_res) {
                    printf("\t thread 1 is working on critical section for 1 second\n");
                    sleep(2);
                    unlock(&glocks[1]);
                    unlock(&glocks[0]);
                    unlock(&glocks[2]);
                    break;
                }
                else {
                    unlock(&glocks[1]);
                    unlock(&glocks[0]);
                    sleep(2);
                }
                
            } else {

            	// If thread_1 is not able to lock glocks[0] now, it will alsosudo
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
     while (1) {
        printf("\t thread 2 started running\n");
        int lock1_res = lock(&glocks[2]);
        sleep(1);
        if (lock1_res) {
            int lock0_res = lock(&glocks[1]);  //This will never be granted with exisiting function //Cycle is detected
            if (lock0_res) {
                
                sleep(3);
                int lock2_res = lock(&glocks[0]);
                if (lock2_res) {
                    printf("\t thread 2 is working on critical section for 1 second\n");
                    sleep(2);
                    unlock(&glocks[2]);
                    unlock(&glocks[1]);
                    unlock(&glocks[0]);
                    break;
                }
                else {
                    unlock(&glocks[2]);
                    unlock(&glocks[1]);
                    sleep(2);
                }
                
            } else {

            	// If thread_1 is not able to lock glocks[0] now, it will alsosudo
            	// unlock glocks[1] and sleep for 1 second before retry so that
            	// thread_0 can acquire glocks[1]
                unlock(&glocks[2]);
                sleep(3);
            }
        }
    }
    return NULL;
}

/*
 * This is a simple deadlock condition similar to the RAG showed in
 * assignment web page. ()
 * The critical sections of thread_0 and thread_1 should both be able
 * to invoke if the SmartLock is implemented correctly, and there should 
 * be no memory leak after finishing the main function.
 */

int main(int argc, char *argv[]) {

    init_lock(&glocks[0]);  
    init_lock(&glocks[1]);
    init_lock(&glocks[2]);

   // printf("LOCK ID %d \n" , glocks[1].unique_id);

    pthread_t tids[3];

    
    pthread_create(&tids[2], NULL, thread_2, NULL);
    pthread_create(&tids[1], NULL, thread_1, NULL);
    pthread_create(&tids[0], NULL, thread_0, NULL);
   // pthread_create(&tids[1], NULL, thread_1, NULL);
    
    pthread_join(tids[0], NULL);
    pthread_join(tids[1], NULL);
    pthread_join(tids[2], NULL);

    // You can assume that cleanup will always be the last function call
    // in main function of the test cases.
    cleanup();
    return 0;
}