#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<signal.h>
#define ITER_COUNT 18 
#define THREAD_COUNT 4 
//       T1      T2                             T1       T2         
// e = 1/(0!) + 1/(1!) + 1/(1*2) + 1/(1*2*3) + 1/(4!) + 1/(5!) + 1/(6!) + .....
// THREAD0_SUM = 1/(0!) + 1/(4!) + 1/(8!) + ....
// THREAD1_SUM = 1/(1!) + 1/(5!) + 1/(9!) + ....
// THREAD2_SUM = 1/(2!) + 1/(6!) + 1/(10!) + ....
// THREAD3_SUM = 1/(3!) + 1/(7!) + 1/(11!) + ....
int c;
unsigned long factorial(unsigned long n) {
    unsigned long res = 1;
    while(n>0) {
        res*=n--;
    }
    return res;
}
typedef struct thread_arg {
    int index;
} thread_arg;

double sum = 0;
pthread_mutex_t lock1;
void* calc_partial_sum(void* argp) {
    int thread_index = ((thread_arg*)argp)->index;
    printf("STARTED THREAD_%d\n", thread_index);

    double partial_sum = 0;
    for(int i = thread_index; i < ITER_COUNT; i+=THREAD_COUNT) {
        partial_sum += 1./factorial(i);
    }
    pthread_mutex_lock(&lock1);
    sum += partial_sum; /// CRITICAL SECTION
    pthread_mutex_unlock(&lock1);
    printf("ENDED THREAD_%d\n", thread_index);
    return NULL;
}
int main() {

    signal(SIGINT, SIG_IGN);
    pthread_t threads[THREAD_COUNT];
    thread_arg thread_arguments[THREAD_COUNT];
    pthread_mutex_init(&lock1, NULL);
    for(int i = 0; i < THREAD_COUNT; i++) {
        thread_arguments[i].index = i;
        pthread_create(threads + i, NULL, calc_partial_sum, thread_arguments+i);
    }
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_join(threads[i], NULL);
    }
    printf("All threads have ended\n");
    printf("%.15lf\n", sum);
    pthread_mutex_destroy(&lock1);
    return 0;
}