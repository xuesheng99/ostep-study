/*
* 读者-写者锁
* 需要处理三种场景
* 读写互斥
* 写写互斥
* 读读不互斥
*/
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include "../include/thread.h"

#define READ_LOOPS 100
#define WRITE_LOOPS 100

typedef struct _rwlock_t {
    sem_t lock;         //binary semaphore (basic lock)
    sem_t writelock;    //allow one write/many readers
    int   readers;      //#readers in citical section 
} rwlock_t;

void rwlock_init(rwlock_t *rw);
void rwlock_acquire_readlock(rwlock_t *rw);
void rwlock_release_readlock(rwlock_t *rw);
void rwlock_acquire_writelock(rwlock_t *rw);
void *reader(void *arg);
void *writer(void *arg);

rwlock_t mutex;
int counter; //计数器-写者

int main(int argc, char const *argv[])
{

    rwlock_init(&mutex);
    pthread_t r, w;
    Pthread_create(&r, NULL, reader, NULL);
    Pthread_create(&w, NULL, writer, NULL);
    Pthread_join(r, NULL);
    Pthread_join(w, NULL);
    return 0;
}

void rwlock_init(rwlock_t *rw) {
    rw->readers = 0;
    sem_init(&rw->lock, 0, 1);
    sem_init(&rw->writelock, 0, 1);
}

//获得读锁
void rwlock_acquire_readlock(rwlock_t *rw) {
    sem_wait(&rw->lock);
    rw->readers++;
    if (rw->readers == 1) sem_wait(&rw->writelock); //first reader gets writelock.
    sem_post(&rw->lock);
}

//释放读锁
void rwlock_release_readlock(rwlock_t *rw) {
    sem_wait(&rw->lock);
    rw->readers--;
    if (rw->readers == 0) sem_post(&rw->writelock); //last reader lets it go
    sem_post(&rw->lock);
}

//获得写锁
void rwlock_acquire_writelock(rwlock_t *rw) {
    sem_wait(&rw->writelock);
}

//释放写锁
void rwlock_release_writelock(rwlock_t *rw) {
    sem_post(&rw->writelock);
}

void *reader(void *arg) {
    int local = 0;
    for (int i = 0; i < READ_LOOPS; i++)
    {
        rwlock_acquire_readlock(&mutex);
        local = counter;
        printf("[%d] read %d\n", i, local);
        rwlock_release_readlock(&mutex);
    }
    printf("read done: %d\n", local);
    return NULL;
}

void *writer(void *arg){
    int local = 0;
    for (int i = 0; i < WRITE_LOOPS; i++)
    {
        rwlock_acquire_writelock(&mutex);
        local = ++counter;
        printf("[%d] write %d\n", i, local);
        rwlock_release_writelock(&mutex);
    }
    printf("write done\n");
    return NULL;
}