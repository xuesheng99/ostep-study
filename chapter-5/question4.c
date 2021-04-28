#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    int rc = fork();
    if (rc < 0 )
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0) //child
    {
        char * const envp[] = {NULL};
        char * const argv[] = {"ls", NULL};

        execl("/bin/ls", "ls", NULL);
        // execlp("ls", "ls", NULL);
        
        // execle("/bin/ls", "ls", NULL, envp);
        // execv("/bin/ls", argv);
        // execvp("ls", argv);
    }
    else
    {
        wait(NULL);
    }
    
    return 0;
}
