//并发哈希表
//gcc -o ch concurrent_linked_list.c concurrent_hashtable.c -Wall -pthread && time ./ch

#include <stdio.h>
#include "./concurrent_linked_list.h"

#define BUCKETS 101

typedef struct _hash_t {
    list_t lists[BUCKETS];
} hash_t;

void hash_init(hash_t *h) {
    for (int i = 0; i < BUCKETS; i++)
        list_init(&h->lists[i]);
}

int  hash_insert(hash_t *h, int key) {
    return list_insert(&h->lists[key % BUCKETS], key);
}

int hash_lookup(hash_t *h, int key) {
    return list_lookup(&h->lists[key % BUCKETS], key);
}

void *hash_insert_work(void * arg) {
    hash_insert((hash_t *) arg, 9999);
    hash_insert((hash_t *) arg, 1111);
    hash_insert((hash_t *) arg, 8888);
    return NULL;
}

void *hash_lookup_work(void *arg) {
    int rc = hash_lookup((hash_t *) arg, 9999);
    if (rc == 0)
        printf("OK\n");
    return NULL;
}

int main(int argc, char const *argv[])
{
    hash_t h;
    pthread_t t1, t2;
    Pthread_create(&t1, NULL, hash_insert_work, &h);
    Pthread_create(&t2, NULL, hash_lookup_work, &h);
    Pthread_join(t1, NULL);
    Pthread_join(t2, NULL);
    return 0;
}
