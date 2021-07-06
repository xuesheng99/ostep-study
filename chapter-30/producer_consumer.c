//生产者-消费者问题
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include "../include/thread.h"

int buffer;
int count = 0; //默认表示buffer中为空。
int loops;
pthread_cond_t cond;
pthread_mutex_t mutex;

void init();
void put(int value);
int get();
void *producer(void *arg);
void *consumer(void *arg);

int main(int argc, char const *argv[])
{
    pthread_t t1, t2;
    Pthread_create(&t1, NULL, producer, NULL);
    Pthread_create(&t2, NULL, consumer, NULL);
    Pthread_join(t1, NULL);
    Pthread_join(t2, NULL);
    return 0;
}

void init() {
    Pthread_mutex_init(&mutex, NULL);
    Pthread_cond_init(&cond, NULL);
    loops = 1000;
}

void put(int value) {
    assert(count == 0);
    count = 1;
    buffer = value;
}

int get() {
    assert(count == 1);
    count = 0;
    return buffer;
}

void *producer(void *arg) {
    for (int i = 0; i < loops; i++)
    {
        Pthread_mutex_lock(&mutex);
        if (count == 1)
            Pthread_cond_wait(&cond, &mutex);

        put(i);
        Pthread_cond_signal(&cond);
        Pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *consumer(void *arg) {
    for (int i = 0; i < loops; i++)
    {
        Pthread_mutex_lock(&mutex);
        if (count == 0)
            Pthread_cond_wait(&cond, &mutex);

        int tmp = get();    
        Pthread_cond_signal(&cond);
        Pthread_mutex_unlock(&mutex);
        printf("%d\n", tmp);
    }
    return NULL;
}

