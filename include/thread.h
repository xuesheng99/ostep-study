//引用本header文件，需要先引用 pthread.h 和 assert.h 。
#define Pthread_create(__newthread, __attr, __start_routine, __arg) \
assert((pthread_create(__newthread, __attr, __start_routine, __arg)) == 0) 

#define Pthread_join(__th, __thread_return) \
assert((pthread_join(__th, __thread_return)) == 0)

#define Pthread_mutex_init(__mutex, __mutexattr) \
assert(pthread_mutex_init(__mutex, __mutexattr) == 0)

#define Pthread_mutex_lock(__mutex) \
assert(pthread_mutex_lock(__mutex) == 0)

#define Pthread_mutex_unlock(__mutex) \
assert(pthread_mutex_unlock(__mutex) == 0)

#define Pthread_cond_init(__cond, __condattr) \
assert(pthread_cond_init(__cond, __condattr) == 0)

#define Pthread_cond_wait(__cond, __mutex) \
assert(pthread_cond_wait(__cond, __mutex) == 0)

#define Pthread_cond_signal(__cond) \
assert(pthread_cond_signal(__cond) == 0)
