#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    int rc = fork();
    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
    }
    else if (rc == 0) //child
    {
        close(STDOUT_FILENO);
        printf("I am child, Can you see this?\n"); // dont output
        fprintf(stderr, "you can see error.\n");
    }
    else
    {
        printf("I am parent\n");
    }
    
    return 0;
}
