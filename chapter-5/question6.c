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
        printf("I am child\n");
        exit(0);
    }
    else
    {
        int stat;
        waitpid(rc, &stat, 0);
        // waitpid(rc, &stat, WNOHANG); //Don't block waiting.
        // waitpid(rc, &stat, WUNTRACED);
        printf("I am parent\n");
        printf("the stat value is %d\n", stat); 
    }
    
    return 0;
}
