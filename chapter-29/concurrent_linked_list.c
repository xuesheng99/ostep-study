//并发链表
#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/thread.h"

//基本节点结构
typedef struct __node_t
{
    int     key;
    struct __node_t *next;
} node_t;

//基本链表结构
typedef struct __list_t
{
    node_t *head;
    pthread_mutex_t lock; //对整个list加锁
} list_t;

//线程的参数
typedef struct __thread_args {
    list_t *l;
    int key;
    int thread_no; //线程编号
} thread_args;

void list_init(list_t *l);
int list_insert(list_t *l, int key);
int list_lookup(list_t *l, int key);
void *insert_work(void *arg);
void *lookup_work(void *arg);

int main(int argc, char const *argv[])
{
    list_t l;
    list_init(&l);
    thread_args targ1 = { .l = &l, .key = 20, .thread_no = 1}; //用于 insert node
    thread_args targ2 = { .l = &l, .key = 30, .thread_no = 2}; //用于 insert node 
    thread_args targ3 = { .l = &l, .key = 3, .thread_no = 3}; //用于 lookup node 

    pthread_t t1,t2,t3;

    Pthread_create(&t1, NULL, insert_work, (void *)&targ1);
    Pthread_create(&t2, NULL, insert_work, (void *)&targ2);
    Pthread_create(&t3, NULL, lookup_work, (void *)&targ3);
    Pthread_join(t1, NULL);
    Pthread_join(t2, NULL);
    Pthread_join(t3, NULL);
    
    return 0;
}

void list_init(list_t *l) {
    l->head = NULL;
    Pthread_mutex_init(&l->lock, NULL);
}

// On success, return 0.
int list_insert(list_t *l, int key) {
    node_t *new = malloc(sizeof(node_t));
    if (new == NULL) {
        perror("malloc");
        return -1;
    }
    new->key = key;
    // just lock critical section
    Pthread_mutex_lock(&l->lock);
    new->next = l->head;
    l->head = new; //head pointer始终指向新插入的节点。
    Pthread_mutex_unlock(&l->lock);
    return 0;
}

// On success, return 0. 
int list_lookup(list_t *l, int key) {
    int rv = -1;
    Pthread_mutex_lock(&l->lock);
    node_t *curr = l->head;
    while (curr)
    {
        if (curr->key == key)
        {
            rv = 0;
            break;
        }
        curr = curr->next;
    }
    
    Pthread_mutex_unlock(&l->lock);
    return rv; //now both success and failure.
}

void *insert_work(void *arg) {
    thread_args *targ = (thread_args *) arg;
    int rc = list_insert(targ->l, targ->key);
    if (rc == 0)
    {
        printf("Thread: %d key %d inserted.\n", targ->thread_no, targ->key);
    } else {
        printf("Thread: %d key %d insert failed.\n", targ->thread_no, targ->key);
    }
    return NULL;
}

void *lookup_work(void *arg) {
    thread_args *targ = (thread_args *) arg;
    int rc = list_lookup(targ->l, targ->key);
    if (rc == 0)
    { 
        printf("Thread: %d key %d node found.\n", targ->thread_no, targ->key);
    } else {
        printf("Thread: %d key %d node not found.\n", targ->thread_no, targ->key);
    }
    
    return NULL;
}
