//父线程等待子线程——基于自旋的方式
#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include "../include/thread.h"

void *child(void *arg);
//gcc 使用 -O0 优化级别（默认）时，不使用volatile也会得到正确结果；
//当使用-O1优化级别时，需要使用volatile修饰符，来保证可见性。
volatile int done = 0;

int main(int argc, char const *argv[])
{
    printf("parent: begin\n");
    pthread_t c;
    Pthread_create(&c, NULL, child, NULL);
    while (done == 0)
        ; //spin
    printf("parent: end\n");
    return 0;
}

void *child(void *arg) {
    printf("child\n");
    done = 1;
    return NULL;
}