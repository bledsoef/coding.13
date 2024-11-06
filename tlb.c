/*
Questions:

1: It is precise down to the microsecond. It outputs the seconds and microseconds since the epoch. So the operation needs to take at least one microsecond for it to be timed, but much more than that to be timed precisely.

2. See below

3. See test.sh

5. I did this by just creating a new loop that iterates through the array we just messed with and adds all those values to another variable.

6. 

7.

*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <sched.h>

void *timed_function(void *arg) {
    int PAGESIZE = 40;
    long int NUMPAGES = (long int) atoi(((char **)arg)[1]);
    long int TRIALS = (long int) atoi(((char **)arg)[2]);
    int jump = PAGESIZE / sizeof(int);
    int a[NUMPAGES * jump];
    int seconds = 0;
    int counter = 0;
    int microseconds = 0;
    struct timeval start;
    struct timeval end;

    for (int j = 0; j < TRIALS; j += 1)
    {
        gettimeofday(&start, NULL);
        for (int i = 0; i < NUMPAGES * jump; i += jump)
        {
            a[i] += 1;
        }
        gettimeofday(&end, NULL);
        if (end.tv_sec - start.tv_sec == 0) {
            microseconds += (end.tv_usec - start.tv_usec);
            counter += 1;
        }
    }

    volatile int result = 0;
    for (int i = 0; i < NUMPAGES * jump; i += jump) {
        result += a[i];
    }

    int *var = a;
    printf("%d\n", (int) ((microseconds + result - result) / counter));
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t thread;
    cpu_set_t cpuset;
    int core_id = 0; 

    pthread_create(&thread, NULL, timed_function, argv);

    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset); 
    pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);

    pthread_join(thread, NULL);

    return 0;
}