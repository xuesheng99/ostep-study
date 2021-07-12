//哲学家就餐问题--存在死锁的解决方案 TODO
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include "../include/thread.h"

void think(void);
void getforks(void);
void eat(void);
void putforks(void);

int main(int argc, char const *argv[])
{

    return 0;
}

//保证没有死锁——哲学家饿死
//尽可能的并发高——让更多的哲学家同时吃东西
void *philosopher(void *arg) {
    while(1) {
        think();
        getforks();
        eat();
        putforks();
    }

    return NULL;    
}