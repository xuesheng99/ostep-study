#include <stdio.h>

static int unmodified = 0; //表示未修改

typedef struct lock_t {
    int flag;
} lock_t;

int main(int argc, char const *argv[])
{
    return 0;
}

//链接加载和条件存储，MIPS中提供了这么一对指令。

int LoadLinked(int *ptr) {
    return *ptr;
}

int StoreConditional(int *ptr, int value) {
    if (unmodified == 0) { //表示自链接式载入*ptr后，ptr指向的地址空间没有被更新过。
        *ptr = value;
        return 1;
    }
    return 0;
}

void lock(lock_t *lock) {
    while (1)
    {
        while(LoadLinked(&lock->flag) == 1); // spin until it's zero
        if (StoreConditional(&lock->flag, 1) == 1) {
            return;
        }
    }
}

void unlock(lock_t *lock) {
    lock->flag = 0;
}