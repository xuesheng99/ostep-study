// 精确并发计数器
#define _GNU_SOURCE
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <sched.h>
#include "../include/thread.h"

#define THREAD_NUM 1
#define LOOP_COUNT 10E8
#define CPU_CORES 8  //用于绑核，默认8核。Intel(R) Core(TM) i7-8550U CPU @ 1.80GHz 2.00 GHz

typedef struct __counter_t
{
    int             value;
    pthread_mutex_t lock;
} counter_t;

void init(counter_t *c);
void increment(counter_t *c);
void decrement(counter_t *c);
int get(counter_t *c);
void *work(void *c);

int main(int argc, char const *argv[])
{
    cpu_set_t set;
    CPU_ZERO(&set);
    for (char i = 0; i < CPU_CORES; i++)
        CPU_SET(i, &set);

    assert(sched_setaffinity(0, sizeof(cpu_set_t), &set) == 0);
    pthread_t pa[THREAD_NUM];
    counter_t c;
    init(&c);
    for (int i = 0; i < THREAD_NUM; i++)
        Pthread_create(&pa[i], NULL, work, (void *) &c);
    
    for (int i = 0; i < THREAD_NUM; i++)
        Pthread_join(pa[i], NULL);
    
    printf("CPU cores: %d\n", CPU_CORES);
    printf("Thread num: %d\n", THREAD_NUM);
    printf("Expected counter = %d\n", (int) (THREAD_NUM * LOOP_COUNT));
    printf("Precise counter = %d\n", get(&c));

    return 0;
}

void init(counter_t *c) {
    c->value = 0;
    Pthread_mutex_init(&c->lock, NULL);
}

void increment(counter_t *c) {
    Pthread_mutex_lock(&c->lock);
    c->value ++;
    Pthread_mutex_unlock(&c->lock);
}

void decrement(counter_t *c) {
    Pthread_mutex_lock(&c->lock);
    c->value --;
    Pthread_mutex_unlock(&c->lock);
}

int get(counter_t *c) {
    Pthread_mutex_lock(&c->lock);
    int rc = c->value;
    Pthread_mutex_unlock(&c->lock);
    return rc;
}

/*
* 每个线程执行LOOP_COUNT次计数器
*/
void *work(void *c) {
    for (int i = 0; i < LOOP_COUNT; i++)
    {
        increment((counter_t *) c);
    }
    
    return NULL;
}