#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 10
#define NUM_INCREMENTS 1000

int counter = 0;
pthread_mutex_t counter_mutex;

void* increment_counter(void* arg) {
    int i;
    for (i = 0; i < NUM_INCREMENTS; i++) {
        pthread_mutex_lock(&counter_mutex);
        counter++;
        pthread_mutex_unlock(&counter_mutex);
    }
    pthread_exit(NULL);
}

int main(int argc, char** argv) {
    pthread_t threads[NUM_THREADS];
    int rc, i;

    // Initialize the mutex
    pthread_mutex_init(&counter_mutex, NULL);

    // Spawn threads
    for (i = 0; i < NUM_THREADS; i++) {
        rc = pthread_create(&threads[i], NULL, increment_counter, NULL);
        if (rc) {
            printf("ERROR: return code from pthread_create() is %d\n", rc);
            return -1;
        }
    }

    // Wait for threads to finish
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&counter_mutex);

    // Print final value of counter
    printf("Final value of counter: %d\n", counter);

    return 0;
}
