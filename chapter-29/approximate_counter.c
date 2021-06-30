//近似并发计数器
#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include "../include/thread.h"

#define CPU_CORES 8 //模拟CPU核数
#define THREAD_NUM 8
#define LOOP_COUNT 1E6
#define THRESHOLD 5 * 200
#define COUNTER_STEP 1

typedef struct __counter_t {
    int             global;             //全局计数器
    pthread_mutex_t glock;              //全局锁
    int             local[CPU_CORES];   //每个CPU的局部计数器
    pthread_mutex_t llock[CPU_CORES];   //每个CPU的局部锁
    int             threshold;          //局部向全局更新的频率
} counter_t;

void init(counter_t *c, int threshold);
void update(counter_t *c, int threadID, int amount);
int get(counter_t *c);
void *work(void * arg);

int main(int argc, char const *argv[])
{
    counter_t c;
    init(&c, THRESHOLD);
    pthread_t pa[THREAD_NUM];
    for (int i = 0; i < THREAD_NUM; i++)
        Pthread_create(&pa[i], NULL, work, (void *) &c);
    
    for (int i = 0; i < THREAD_NUM; i++)
        Pthread_join(pa[i], NULL);

    printf("CPU cores: %d\n", CPU_CORES);
    printf("Thread num: %d\n", THREAD_NUM);
    printf("Expected counter = %d\n", (int) (THREAD_NUM * LOOP_COUNT));
    printf("Approximate counter = %d\n", get(&c));
    return 0;
}

void init(counter_t *c, int threshold) {
    c->threshold = threshold;
    c->global = 0;
    Pthread_mutex_init(&c->glock, NULL);
    for (int i = 0; i < CPU_CORES; i++)
    {
        c->local[i] = 0;
        Pthread_mutex_init(&c->llock[i], NULL);
    }
}

void update(counter_t *c, int threadID, int amount) {
    //may be more than one thread per cpu.
    int cpu = threadID % CPU_CORES;
    Pthread_mutex_lock(&c->llock[cpu]);
    c->local[cpu] += amount;
    if (c->local[cpu] >= c->threshold)
    {
        //transfer local to global (assume amount > 0)
        Pthread_mutex_lock(&c->glock);
        c->global += c->local[cpu];
        Pthread_mutex_unlock(&c->glock);
        c->local[cpu] = 0;
    }
    
    Pthread_mutex_unlock(&c->llock[cpu]);
}
//get: just return global amount (approximate)
int get(counter_t *c) {
    Pthread_mutex_lock(&c->glock);
    int val = c->global;
    Pthread_mutex_unlock(&c->glock);
    return val;
}

void *work(void * arg) {
    pid_t tid = gettid();
    for (int i = 0; i < LOOP_COUNT; i++)
        update((counter_t *) arg, tid, COUNTER_STEP);
    
    return NULL;
}