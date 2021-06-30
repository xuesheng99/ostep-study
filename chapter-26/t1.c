#include <stdio.h>
#include <pthread.h>

#include "./include/thread.h"

static volatile int counter = 0;

void *mythread(void *arg) {
    printf("%s:begin\n", (char *) arg);
    for (int i = 0; i < 10e7; i++) {
        counter = counter + 1;
    }
    printf("%s: done\n", (char *)arg);
    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t p1, p2;
    printf("main: begin (counter = %d)\n", counter);
    Pthread_create(&p1, NULL, mythread, "A");
    Pthread_create(&p2, NULL, mythread, "B");
    Pthread_join(p1, NULL);
    Pthread_join(p2, NULL);
    printf("main: down (counter = %d)\n",counter);
    return 0;
}

