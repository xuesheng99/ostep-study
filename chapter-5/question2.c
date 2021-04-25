#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    int fd = open("./dist/hello.txt", O_CREAT | O_RDWR, S_IRWXU);
    int rc = fork();

    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0) //child
    {
        const char *child_msg = "hello, I am child.\n";
        write(fd, child_msg, strlen(child_msg));
        exit(0);
    } else
    {
        wait(NULL);
        const char *parent_msg = "hello, I am parent.\n";
        write(fd, parent_msg, strlen(parent_msg));
    }
    sync();
    close(fd);
    
    return 0;
}
