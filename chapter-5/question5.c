#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    int rc = fork();
    
    if (rc < 0)
    {
        fprintf(stderr, "fork failed.\n");
        exit(1);
    }
    else if (rc == 0)
    {
        int wc = wait(NULL); // return -1
        printf("I am child\n");
        printf("return value from wait() is %d\n", wc);
    }
    else
    {
        wait(NULL); //on success, return terminated child process ID.on error, return -1.
        printf("I am parent\n");
    }
    
    return 0;
}
