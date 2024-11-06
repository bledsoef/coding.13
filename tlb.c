/*
Questions:

1: It is precise down to the microsecond. It outputs the seconds and microseconds since the epoch. So the operation needs to take at least one microsecond for it to be timed, but much more than that to be timed precisely.

2. See below

3. See test.sh

5. I did this by just creating a new loop that iterates through the array we just messed with and adds all those values to another variable.

6. By tying it to one core it makes sure there is cache affinity and that the process is not getting bounced from core to core and needing to start with a fresh core. I attempted to implement it and had no such luck. You can include a package to specify the core and make sure the process is only running on one CPU with using things like pthread_setaffinity_np. I tried to implement this but the resources online either did not work or were very complicated.

7. I inititally was initializing the array just by doing int a[NUMPAGES * jump]; but I shifted to using calloc that way it is already cleared and allocated.

*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    long int NUMPAGES = (long int)atoi(argv[1]);
    long int TRIALS = (long int)atoi(argv[2]);
    long PAGESIZE = sysconf(_SC_PAGESIZE); // 163840 is the default page size for MACOS
    long jump = PAGESIZE / sizeof(int); 
    int *a = calloc(NUMPAGES, PAGESIZE);
    int counter = 0;
    int microseconds = 0;
    struct timeval start;
    struct timeval end;
    gettimeofday(&start, NULL);
    for (int j = 0; j < TRIALS; j += 1)
    {
        for (int i = 0; i < NUMPAGES * jump; i += jump)
        {
            a[i] += 1;
        }
    }
    gettimeofday(&end, NULL);

    int result = 0; // trying to maybe prevent compiler optimization?
    for (int i = 0; i < NUMPAGES * jump; i += jump) {
        result += a[i];
    }

    int *var = a; 
    printf("%ld, %f\n", NUMPAGES,
           ((end.tv_sec - start.tv_sec) * 1E6 // convert seconds to micro
            + end.tv_usec - start.tv_usec) /
               (TRIALS * NUMPAGES)); // calculte per access

    free(a);
    return 0;
}