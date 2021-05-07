#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sched.h>

#define LOOPSIZE 10e3
int main(int argc, char const *argv[])
{
    //allow cpu #0
    cpu_set_t set;
    int ret;

    CPU_ZERO(&set);
    CPU_SET(0, &set);
    ret = sched_setaffinity(0, sizeof(cpu_set_t), &set);
    if (ret == -1)
    {
        perror("sched_getaffinity");
    }

    //create two pipes
    int pipefd1[2];
    int pipefd2[2];

    if (pipe(pipefd1)) {
        fprintf(stderr, "pipe1 create failed\n");
        exit(1);
    }

    if (pipe(pipefd2)) {
        fprintf(stderr, "pipe2 create failed\n");
        exit(1);
    }
    
    int rc1 = fork();
    if (rc1 < 0)
    {
        fprintf(stderr, "Child process fork failed.\n");
        exit(1);
    } else if(rc1 == 0) {
        // Child process
        char buf[1];
        struct timeval start, end;
        gettimeofday(&start, NULL);
        for (int i = 0; i < LOOPSIZE; i++)
        {
            write(pipefd1[1], "a", 1); //pipe1 write end
            read(pipefd2[0], buf, 1); //pipe2 read end
        }
        gettimeofday(&end, NULL);
        printf("The avg context switch time is %f us\n", (float)(end.tv_sec * 10E6 + end.tv_usec - start.tv_sec * 10E6 - start.tv_usec) / (LOOPSIZE * 2));

    } else {
        //parent process
        char buf[1];
        for (int i = 0; i < LOOPSIZE; i++)
        {
            read(pipefd1[0], buf, 1); //pipe1 read end
            write(pipefd2[1], "a", 1); //pipe2 write end
        }
        
    }
    
    return 0;
}
