#include <stdio.h>

typedef struct lock_t {
    int ticket;
    int turn;
} lock_t;

int main(int argc, char const *argv[])
{
    return 0;
}

//硬件原语 fetch-and-add
int FetchAndAdd(int *ptr) {
    int old = *ptr;
    *ptr = old + 1;
    return old;
}

void init(lock_t *lock) {
    lock->ticket  = 0;
    lock->turn = 0;
}

void lock(lock_t *lock) {
    int myturn = FetchAndAdd(&lock->ticket);
    while (myturn != lock->turn); // spin
}

void unlock(lock_t *lock) {
    FetchAndAdd(&lock->turn);
}