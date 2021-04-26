#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
    int fd = open("./dist/wait", O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);

    int rc = fork();
    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(0);
    }
    else if (rc == 0) // child
    {
        printf("hello\n");
        char wait = '1';
        write(fd, &wait, sizeof(char));
        close(fd);
    }
    else
    {
        char wait = 0;

        while (1)
        {
            read(fd, &wait, sizeof(char));
            lseek(fd, 0, SEEK_SET);
            if (wait == '1')
            {
                break;
            }
        }

        printf("goodbye\n");
        close(fd);
        
    }
    
    return 0;
}
