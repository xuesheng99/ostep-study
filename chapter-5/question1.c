#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    int x;
    x = 100;
    int rc = fork();
    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0) //child process
    {
        printf("child process, x is : %d\n", x);
        x = 200;
        printf("child process, new x is : %d\n", x);
    }
    else
    {
        wait(NULL);
        printf("parent process, x is : %d\n", x);
        x = 300;
        printf("parent process, new x is : %d\n", x);
    }
    
    return 0;
}
