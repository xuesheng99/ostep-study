//生产者-消费者问题
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include "../include/thread.h"

#define LOOPS 101
#define MAX 10

int buffer[MAX];
int fill_ptr = 0;
int use_ptr = 0;
int count = 0; //默认表示buffer中为空。
int loops = LOOPS;
pthread_cond_t empty, fill;
pthread_mutex_t mutex;

void init();
void put(int value);
int get();
void *producer(void *arg);
void *consumer(void *arg);

int main(int argc, char const *argv[])
{
    init();
    pthread_t t1, t2, t3;
    Pthread_create(&t1, NULL, producer, "producer 0");
    Pthread_create(&t2, NULL, consumer, "consumer 0");
    Pthread_create(&t3, NULL, consumer, "consumer 1");
    Pthread_join(t1, NULL);
    Pthread_join(t2, NULL);
    Pthread_join(t3, NULL);
    return 0;
}

void init() {
    Pthread_mutex_init(&mutex, NULL);
    Pthread_cond_init(&empty, NULL);
    Pthread_cond_init(&fill, NULL);
}

void put(int value) {
    buffer[fill_ptr] = value;
    fill_ptr = (fill_ptr + 1) % MAX;
    count++;
}

int get() {
    int tmp = buffer[use_ptr];
    use_ptr = (use_ptr + 1) % MAX;
    count--;
    return tmp;
}

void *producer(void *arg) {
    for (int i = 0; i < loops; i++)
    {
        Pthread_mutex_lock(&mutex);
        while (count == MAX)
            Pthread_cond_wait(&empty, &mutex);

        put(i);
        Pthread_cond_signal(&fill);
        Pthread_mutex_unlock(&mutex);
        printf("%s %d\n", arg, i);
    }
    return NULL;
}

void *consumer(void *arg) {
    for (int i = 0; i < loops; i++)
    {
        Pthread_mutex_lock(&mutex);
        while (count == 0)
            Pthread_cond_wait(&fill, &mutex);

        int tmp = get();
        Pthread_cond_signal(&empty);
        Pthread_mutex_unlock(&mutex);
        printf("%s %d\n", arg, tmp);
    }
    return NULL;
}

