#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 5

int current_thread = 1;
pthread_mutex_t current_thread_mutex;
pthread_cond_t next_thread_cond[NUM_THREADS];

void* print_number(void* arg) {
    int thread_num = *(int*)arg;
    while (1) {
        pthread_mutex_lock(&current_thread_mutex);
        while (thread_num != current_thread) {
            pthread_cond_wait(&next_thread_cond[thread_num], &current_thread_mutex);
        }
        printf("%d\n", thread_num);
        current_thread = (current_thread % NUM_THREADS) + 1;
        pthread_cond_broadcast(&next_thread_cond[current_thread - 1]);
        pthread_mutex_unlock(&current_thread_mutex);
    }
    pthread_exit(NULL);
}

int main(int argc, char** argv) {
    pthread_t threads[NUM_THREADS];
    int rc, i, thread_nums[NUM_THREADS];

    // Initialize the mutex and condition variables
    pthread_mutex_init(&current_thread_mutex, NULL);
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_cond_init(&next_thread_cond[i], NULL);
        thread_nums[i] = i + 1;
    }

    // Spawn threads
    for (i = 0; i < NUM_THREADS; i++) {
        rc = pthread_create(&threads[i], NULL, print_number, &thread_nums[i]);
        if (rc) {
            printf("ERROR: return code from pthread_create() is %d\n", rc);
            return -1;
        }
    }

    // Wait for threads to finish (never happens)
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the mutex and condition variables
    pthread_mutex_destroy(&current_thread_mutex);
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_cond_destroy(&next_thread_cond[i]);
    }

    return 0;
}
