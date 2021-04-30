#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
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
        fprintf(stderr, "Child #1 fork failed.\n");
        exit(1);
    } else if(rc1 == 0) {
        // Child #1
        int counter = 0; //msg counter
        while (1)
        {
            sleep(2);
            printf("Child #1 Round #%d\n", ++counter);
            size_t ret;
            ret = write(pipefd1[1], "Child #1 send msg\n", 512); //pipe1 write end
            printf("Child #1 has send to pipe1 %ld bytes on success\n", ret);
            char buffer[512];
            ret = read(pipefd2[0], buffer, 512); //pipe2 read end
            printf("Child #1 has read %ld bytes from pipe2 on success\n", ret);
            printf("    ---msg: %s", buffer);
        }

    } else {
        //parent process
        int rc2 = fork();
        if (rc2 < 0) 
        {
            fprintf(stderr, "Child #2 fork failed.\n");
            exit(1);
        }else if(rc2 == 0){
            // Child #2
            int counter = 0; //msg counter
            while (1)
            {
                sleep(1);
                printf("Child #2 Round #%d\n", ++counter);
                size_t ret;
                char buffer[512];
                read(pipefd1[0], buffer, 512); //pipe1 read end
                printf("Child #2 has read %ld bytes from pipe1 on success\n", ret);
                printf("    ---msg: %s", buffer);
                ret = write(pipefd2[1], "Child #2 send msg\n", 512); //pipe2 write end
                printf("Child #2 has send to pipe2 %ld bytes on success\n", ret);
            }


        }else{
            //parent process
            waitpid(rc2, NULL, 0);
            printf("finished\n");

        }
        
    }
    
    return 0;
}
