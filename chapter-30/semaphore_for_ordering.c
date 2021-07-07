// 信号量
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <assert.h>
#include "../include/thread.h"

void *child(void *arg);
sem_t s;

int main(int argc, char const *argv[])
{
    sem_init(&s, 0, 0);
    printf("Parent: begin\n");
    pthread_t c;
    Pthread_create(&c, NULL, child, NULL);
    sem_wait(&s);
    printf("Parent: end\n");
    return 0;
}

void *child(void *arg) {
    printf("child\n");
    sem_post(&s);
    return NULL;
}
