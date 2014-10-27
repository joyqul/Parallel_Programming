#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <pthread.h>
#include <sys/sysinfo.h>

using namespace std;
long long number_of_tosses, number_in_circle;
pthread_mutex_t mutex;

void* threadSum(void* toss) {
    long long my_toss = (long long)toss;
    long long my_number_in_circle = 0;

    /* generate seed */
    unsigned int my_seed = rand();

    /* calculate */
    for (long long i = 0; i < my_toss; ++i) {
        double x = -1 + (double)rand_r(&my_seed) / RAND_MAX * 2;
        double y = -1 + (double)rand_r(&my_seed) / RAND_MAX * 2;
        double distance_squared = x*x + y*y;
        if (distance_squared <= 1) {
            ++my_number_in_circle;
        }
    }
    /* add local to global */
    pthread_mutex_lock(&mutex);
    number_in_circle += my_number_in_circle;
    pthread_mutex_unlock(&mutex);

    /* exit */
    pthread_exit(NULL);
}

int main (int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: ./hw1_pthread <number of tosses>\n");
        return 0;
    }

    int NUMBER_OF_THREAD = get_nprocs_conf();
    //printf("NUMBER_OF_THREAD: %d\n", NUMBER_OF_THREAD);

    srand(time(NULL));

    /* total toss number */
    long long number_of_tosses = atoll(argv[1]);
    long long part_toss = number_of_tosses/NUMBER_OF_THREAD;
    /* init of thread's id */
    pthread_t* thread_handles;
    thread_handles = (pthread_t*) malloc (NUMBER_OF_THREAD * sizeof(pthread_t));

    /* init of mutex */
    pthread_mutex_init(&mutex, NULL);
    /* init of number in circle */
    number_in_circle = 0;

    /* create thread */
    for (long i = 0; i < NUMBER_OF_THREAD-1; ++i) {
        pthread_create(&thread_handles[i], NULL, threadSum, (void*)part_toss);
    }
    /* last one may not be part_toss */
    part_toss = number_of_tosses - part_toss*(NUMBER_OF_THREAD-1);
    pthread_create(&thread_handles[NUMBER_OF_THREAD-1], NULL, threadSum, (void*)part_toss);

    /* join threads */
    for (long i = 0; i < NUMBER_OF_THREAD; ++i) {
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
