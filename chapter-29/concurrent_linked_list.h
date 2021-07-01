#include <assert.h>
#include <pthread.h>
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