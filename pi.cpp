#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <pthread.h>

using namespace std;
long long number_of_tosses, number_in_circle;
long long part_toss;
pthread_mutex_t mutex;

int NUMBER_OF_THREAD = 1;

inline double fRand(double fMin, double fMax) {
    double f = (double)rand()/ RAND_MAX;
    return fMin + f * (fMax - fMin);
}

void* threadSum(void* tid) {
    long long my_toss = part_toss;
    long my_tid = (long) tid;
    if (my_tid == NUMBER_OF_THREAD-1) my_toss = number_of_tosses - part_toss*(NUMBER_OF_THREAD-1);
    long long my_number_in_circle = 0;
    for (long long toss = 0; toss < my_toss; ++toss) {
        double x = fRand(-1, 1);
        double y = fRand(-1, 1);
        double distance_squared = x*x + y*y;
        if (distance_squared <= 1) {
            ++my_number_in_circle;
        }
    }
    /* add local to global */
    pthread_mutex_lock(&mutex);
    number_in_circle += my_number_in_circle;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main (int argc, char* argv[]) {
    if (argc != 2 && argc !=3) {
        printf("Usage: ./hw1_pthread <number of tosses>\n");
        return 0;
    }

    if (argc == 3) {
        NUMBER_OF_THREAD = atoi(argv[2]);
    }

    srand(time(NULL));

    /* total toss number */
    number_of_tosses = atoll(argv[1]);
    part_toss = number_of_tosses/NUMBER_OF_THREAD;
    /* init of thread's id */
    pthread_t* thread_handles;
    thread_handles = (pthread_t*) malloc (NUMBER_OF_THREAD * sizeof(pthread_t));

    /* init of mutex */
    pthread_mutex_init(&mutex, NULL);
    /* init of number in circle */
    number_in_circle = 0;

    /* create thread */
    long i;
    for (i = 0; i < NUMBER_OF_THREAD; ++i) {
        pthread_create(&thread_handles[i], NULL, threadSum, (void*)i);
    }

    /* join threads */
    for (i = 0; i < NUMBER_OF_THREAD; ++i) {
        pthread_join(thread_handles[i], NULL);
    }
    
    /* calculate answer */
    double pi_estimate = 4*number_in_circle/((double) number_of_tosses);
    printf("%f\n", pi_estimate);

    /* free */
    pthread_mutex_destroy(&mutex);
    free(thread_handles);
    
    pthread_exit(NULL);
}
