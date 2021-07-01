//并发链表主函数
//gcc -o cll_main cll_main.c concurrent_linked_list.c -Wall -pthread && ./cll_main

#include "./concurrent_linked_list.h"

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


