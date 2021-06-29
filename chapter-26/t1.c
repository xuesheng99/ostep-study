#include <stdio.h>
#include <pthread.h>
#include <assert.h>

#define Pthread_create(__newthread, __attr, __start_routine, __arg) \
assert((pthread_create(__newthread, __attr, __start_routine, __arg)) == 0) 

#define Pthread_join(__th, __thread_return) \
assert((pthread_join(__th, __thread_return)) == 0)

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

