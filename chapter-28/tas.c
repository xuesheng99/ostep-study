#include <stdio.h>

typedef struct lock_t {
    int flag;
} lock_t;


int TestAndSet(int *old_ptr, int new);
void init(lock_t *lock);

int main(int argc, char const *argv[])
{
    return 0;
}

//xchg: test-and-set
//为了保证原子性，该代码段就不能被中断。
int TestAndSet(int *old_ptr, int new) {
    int old = *old_ptr;
    *old_ptr = new;
    return old;
}

void init(lock_t *lock) {
    lock->flag = 0;
}

void lock(lock_t *lock) {
    while (TestAndSet(&lock->flag, 1) == 1)
    ; //spin-wait (do nothing)
}

void unlock(lock_t *lock) {
    lock->flag = 0;
}