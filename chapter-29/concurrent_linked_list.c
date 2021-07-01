//并发链表
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "./concurrent_linked_list.h"

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
