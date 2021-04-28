#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#define LOOP_COUNT 1000000
int main(int argc, char const *argv[])
{
    struct timeval start, end;
    
    gettimeofday(&start, NULL);
    for (int i = 0; i < LOOP_COUNT; i++)
    {
        read(STDIN_FILENO, NULL, 0);
    }
    gettimeofday(&end, NULL);
    printf("The avg syscall time is %f us\n", (float)(end.tv_sec * 10E6 + end.tv_usec - start.tv_sec * 10E6 - start.tv_usec) / LOOP_COUNT);
    return 0;
}
