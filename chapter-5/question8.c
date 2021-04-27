#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    int pipefd[2];
    int ret = pipe(pipefd);
    if (ret < 0)
    {
        fprintf(stderr, "pipe create failed\n");
        exit(1);
    }
    
    int rc1 = fork(); //child 1
    if (rc1 < 0)
    {
        fprintf(stderr, "fork child #1 failed\n");
        exit(1);
    }
    else if (rc1 == 0) //child #1
    {
        //write pipe
        printf("child #1");
        close(pipefd[0]); // close read.
        dup2(pipefd[1], STDOUT_FILENO); // redirect stdout to pipe write
        printf("the message send to pipe\n");

    }
    else
    {
        //parent process
        int rc2 =fork();
        if (rc2 < 0)
        {
            fprintf(stderr, "fork child #2 failed\n");
            exit(1);
        }
        else if (rc2 == 0) //child #2
        {
            //read pipe
            printf("child #1");
            close(pipefd[1]); //close write
            dup2(pipefd[0], STDIN_FILENO); // read end

            char buff[512]; // make a buffer
            read(STDIN_FILENO, buff, 512);
            printf("%s\n", buff);
            
        }
        else
        {
            //parent process
            waitpid(rc2, NULL, 0);
            printf("goodbye\n");
        }
        
    }
    
    return 0;
}
