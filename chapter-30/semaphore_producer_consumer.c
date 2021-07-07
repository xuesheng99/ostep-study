#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <semaphore.h>
#include "../include/thread.h"

#define MAX 10
#define LOOPS 100

void put(int value);
int get();
void *producer(void *arg);
void *consumer(void *arg);

int buffer[MAX];
int fill = 0;
int use = 0;
sem_t empty;
sem_t full;
sem_t mutex; //binary semaphore

int main(int argc, char const *argv[])
{
    sem_init(&empty, 0, MAX);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1); //mutex = 1 because it is a  block.
    pthread_t pa, pb, c;
    Pthread_create(&pa, NULL, producer, NULL);
    Pthread_create(&pb, NULL, producer, NULL);
    Pthread_create(&c, NULL, consumer, NULL);
    Pthread_join(pa, NULL);
    Pthread_join(pb, NULL);
    Pthread_join(c, NULL);

    return 0;
}

void put(int value) {
    buffer[fill] = value;
    fill = (fill + 1) % MAX;
}

int get() {
    int tmp = buffer[use];
    use = (use + 1) % MAX;
    return tmp;
}

void *producer(void *arg) {
    for (int i = 0; i < LOOPS; i++)
    {
        sem_wait(&empty);
        sem_wait(&mutex);
        put(i);
        sem_post(&mutex);
        sem_post(&full);
        printf("producer %d\n", i);
    }
    return NULL;
}

void *consumer(void *arg) {
    int tmp = 0;
    while (tmp != -1)
    {
        sem_wait(&full);
        sem_wait(&mutex);
        int tmp = get();
        sem_post(&mutex);
        sem_post(&empty);
        printf("consumer %d\n", tmp);
    }
    return NULL;
}