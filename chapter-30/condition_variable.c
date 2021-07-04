//父线程等待子线程——使用条件变量
#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include "../include/thread.h"

pthread_cond_t c = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void thr_exit();
void *child(void *arg);
void thr_join();

volatile int done = 0;

int main(int argc, char const *argv[])
{
    printf("parent: begin\n");
    pthread_t p;
    Pthread_create(&p, NULL, child, NULL);
    thr_join();
    printf("parent: end\n");
    return 0;
}

void thr_exit() {
    Pthread_mutex_lock(&m);
    done = 1;
    Pthread_cond_signal(&c);
    Pthread_mutex_unlock(&m);
}

void *child(void *arg) {
    printf("child\n");
    thr_exit();
    return NULL;
}

void thr_join() {
    Pthread_mutex_lock(&m);
    while (done == 0)
        Pthread_cond_wait(&c, &m); //wait会先释放锁，再使父线程进入休眠。当被唤醒时，再重新获得锁,再返回调用者。

    Pthread_mutex_unlock(&m);
}
