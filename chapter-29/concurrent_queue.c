#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/thread.h"

#define QUEUE_ITEMS 10 //入队列元素数
//队列节点结构
typedef struct __node_t
{
    int             value;
    struct __node_t *next;
} node_t;

//队列结构
typedef struct __queue_t
{
    node_t *head;
    node_t *tail;
    pthread_mutex_t head_lock, tail_lock;
} queue_t;

void queue_init(queue_t *q) {
    node_t *tmp = malloc(sizeof(node_t));
    assert(tmp != NULL);
    tmp->next = NULL;
    q->head = q->tail = tmp;
    Pthread_mutex_init(&q->head_lock, NULL);
    Pthread_mutex_init(&q->tail_lock, NULL);
}

int queue_enqueue(queue_t *q, int value) {
    node_t *tmp = malloc(sizeof(node_t));
    assert(tmp != NULL);
    tmp->next = NULL;
    tmp->value = value;
    Pthread_mutex_lock(&q->tail_lock);
    q->tail->next = tmp;
    q->tail = tmp;
    Pthread_mutex_unlock(&q->tail_lock);
    return 0;
}

//on success, return 0. 
int queue_dequeue(queue_t *q, int *value) {
    Pthread_mutex_lock(&q->head_lock);
    node_t *tmp = q->head;
    node_t *new_head = tmp->next;
    if (tmp->next == NULL) //queue was epmty
    {
        Pthread_mutex_unlock(&q->head_lock);
        return -1;
    }
    *value = new_head->value;
    q->head = new_head;
    Pthread_mutex_unlock(&q->head_lock);
    free(tmp);
    return 0;
}

void *enqueue_work(void *arg) {
    queue_t *q = arg;
    for (int i = 0; i < QUEUE_ITEMS; i++)
    {
        queue_enqueue(q, i);
        printf("enqueued item: %d\n", i);
    }
    return NULL;
}

void *dequeue_work(void *arg) {
    queue_t *q = arg;
    for (int i = 0; i < QUEUE_ITEMS; i++)
    {
        int value;
        queue_dequeue(q, &value);
        printf("dequeued item: %d\n", value);
    }
    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t t1,t2;
    queue_t q;
    queue_init(&q);
    Pthread_create(&t1, NULL, enqueue_work, (void *) &q);
    Pthread_create(&t2, NULL, dequeue_work, (void *) &q);
    Pthread_join(t1, NULL);
    Pthread_join(t2, NULL);
    return 0;
}